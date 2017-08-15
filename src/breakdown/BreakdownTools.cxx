#include <BreakdownTools.hxx>
#include <DrawingStyle.hxx>
#include <ReadParam.hxx>

#include <PDGInfo.hxx>
#include <Topology.hxx>
#include <Variable.hxx>

#include <string>
#include <iostream>

#include <TH1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TLegend.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TList.h>
#include <TF1.h>
#include <TLine.h>

#ifndef __BREAKDOWN__CXX__
#define __BREAKDOWN__CXX__

void Breakdown::SetMap(DrawingTools::KinMap map)
{
    this->m_map = map; 
    DrawingTools::SetColors(m_map, m_fill_color, m_line_color, m_fill_style, m_line_style); 
}

void Breakdown::SetHist(TH1D * hist)
{ 
    m_hist = hist; 
    if(m_hist) DrawingTools::SetColors(m_hist, m_fill_color, m_line_color, m_fill_style, m_line_style); 
}

#endif

#ifndef __BREAKDOWNTOOLS__CXX__
#define __BREAKDOWNTOOLS__CXX__

BreakdownTools::BreakdownTools(std::string filename, std::string treename, bool check_pot) : 
    DrawingTools(filename, treename, (treename + "BD").c_str(), check_pot), m_check(false), m_target("")
{
    string opts_file = string( getenv("HP_ROOT") );
    opts_file += "/src/breakdown/BreakdownInputs.txt";

    std::vector<std::string> params = ReadParam::ReadFile(opts_file);

    if(params.empty()){
        cout << __FILE__ << ":" << __LINE__ << " : ERROR : Params needed in params file: " << opts_file << endl;
        exit(0);
    }

    std::vector<std::string> in_pars;
    in_pars.push_back( "Signal" ); 
    in_pars.push_back( "Target" );
    in_pars.push_back( "Check" ); 
    // params
    string tmp_sig = ReadParam::GetParameterS(in_pars[0], opts_file);
    m_signal = new Topology(Topology::GetEnum(tmp_sig), "");

    m_target = ReadParam::GetParameterS(in_pars[1], opts_file);

    m_check = ReadParam::GetParameterB(in_pars[2], opts_file);

    m_toplist.clear();
    m_pdglist.clear();
    m_tarlist.clear();

    cout << "Pre list: " << endl;
    for(size_t i = 0; i < params.size(); i++)
        cout << params[i] << endl;

    for(size_t i = 0; i < params.size(); i++){
        for(size_t ip = 0; ip < in_pars.size(); ip++){
            if(params[i].find(in_pars[ip]) != string::npos){
                params.erase(params.begin() + i);
            }
        }
    }

    cout << "Post list: " << endl;
    for(size_t i = 0; i < params.size(); i++)
        cout << params[i] << endl;

    for(size_t i = 0; i < params.size(); i++){
        string param = params[i];
        ReadParam::RemoveArrows(param);
        bool is_top = false;
        bool is_pdg = false;
        bool is_tar = false;
        if(param.find("TOP") != string::npos) is_top = true;
        else if(param.find("PDG") != string::npos) is_pdg = true;
        else if(param.find("TAR") != string::npos) is_tar = true;

        bool ch_list[3] = {is_top, is_pdg, is_tar};

        for(int ch1 = 0; ch1 < 3; ch1++){
            for(int ch2 = ch1; ch2 < 3; ch2++){
                if(ch1 == ch2) continue;
                else if(ch_list[ch1] == ch_list[ch2] && ch_list[ch1]){
                    cout << __FILE__ << ":" << __LINE__ << " : WARNING : Ill defined param: " << param << endl;
                    continue;
                }
            }
        }
        // if(is_top && is_pdg && is_tar){
        //     cout << __FILE__ << ":" << __LINE__ << " : WARNING : Ill defined param: " << param << endl;
        //     continue;
        // }

        // if( !is_top && !is_pdg){
        //     cout << __FILE__ << ":" << __LINE__ << " : WARNING : Ill defined param: " << param << endl;
        //     continue;
        // }

        Topology::Name top_name = Topology::Unknown;

        std::stringstream iss(param);
        string word;
        int counter = 0;
        while( iss >> word ){
            if(counter == 0){ 
                counter++; 
                continue;
            }
            else if(word.find(":") != string::npos) continue; 
            else if(is_top){
                if(counter == 1){ 
                    top_name = Topology::GetEnum(word);
                    if(top_name == Topology::Unknown){
                        cout << __FILE__ << ":" << __LINE__ << " : WARNING : Could not determine topology: " << word << endl;
                        break;
                    }
                    counter++;
                }
                else if(counter == 2){
                    m_toplist.push_back( Topology(top_name, word) );
                    if(top_name == m_signal->GetType()){ 
                        delete m_signal;
                        m_signal = new Topology(top_name, word);
                    }
                }
            }
            else if(is_pdg){
                string pdg_pm = "pm";
                if(ReadParam::IsNumber(word)) m_pdglist.push_back( PDGInfo( atoi(word.c_str()) ) );
                else if(word.find(pdg_pm) != string::npos){
                    word = word.erase(word.find(pdg_pm), pdg_pm.length());
                    if(ReadParam::IsNumber(word)) m_pdglist.push_back( PDGInfo( atoi(word.c_str()), true) );
                    else cout << __FILE__ << ":" << __LINE__ << " : WARNING : PDG code still not a number: " << word << endl;
                }
                else cout << __FILE__ << ":" << __LINE__ << " : WARNING : Could not interpret PDG code: " << word << endl;
            }
            else if(is_pdg){
                if(atoi(word.c_str()) == 1){
                    Breakdown hyd("Hydrogen", m_target + " == 1", DrawingStyle::HYDROGEN);
                    m_tarlist.push_back(hyd);
                }
                if(atoi(word.c_str()) == 6){
                    Breakdown car("Carbon", m_target + " == 6", DrawingStyle::CARBON);
                    m_tarlist.push_back(car);
                }
                if(atoi(word.c_str()) == 8){
                    Breakdown oxy("Oxygen", m_target + " == 8", DrawingStyle::OXYGEN);
                    m_tarlist.push_back(oxy);    
                }
                if(atoi(word.c_str()) == 18){
                    Breakdown arg("Oxygen", m_target + " == 18", DrawingStyle::ARGON);
                    m_tarlist.push_back(arg);
                }
            }
        }
    }

    if(m_toplist.empty() && m_pdglist.empty()){
        cout << __FILE__ << ":" << __LINE__ << " : ERROR : No topologies or particles defined " << endl;
        exit(0);
    }

    if(m_toplist.empty()) cout << __FILE__ << ":" << __LINE__ << " : WARNING : No topologies set." << endl;
    if(m_pdglist.empty()) cout << __FILE__ << ":" << __LINE__ << " : WARNING : No particles set." << endl;

    m_statcounter = -1;
}

BreakdownTools::~BreakdownTools(){
    m_pdglist.clear();
    m_toplist.clear();
}

BDCans BreakdownTools::PIDBD(Variable var, std::string pdgvar, std::string cuts)
{
    std::string base_cuts = "";
    std::vector<Breakdown> list = GetPIDs(pdgvar, cuts, base_cuts);
    return BaseBreakdown(var, list, "PIDBD", base_cuts);
}

TCanvas * BreakdownTools::PIDC(Variable var, std::string pdgvar, std::string cuts)
{
    std::string base_cuts = "";
    std::vector<Breakdown> list = GetPIDs(pdgvar, cuts, base_cuts);
    return SingleBase(var, list, "PIDC", base_cuts);
}

BDCans BreakdownTools::TOPOBD(Variable var, std::string cuts)
{
    std::vector<Breakdown> list = GetTOPs();
    return BaseBreakdown(var, list, "TOPBD", cuts);
}

TCanvas * BreakdownTools::TOPOC(Variable var, std::string cuts)
{
    std::vector<Breakdown> list = GetTOPs();
    return SingleBase(var, list, "TOPC", cuts);
}

BDCans BreakdownTools::TAROBD(Variable var, std::string cuts)
{
    std::vector<Breakdown> list = GetTARs();
    return BaseBreakdown(var, list, "TARBD", cuts);
}

TCanvas * BreakdownTools::TAROC(Variable var, std::string cuts)
{
    std::vector<Breakdown> list = GetTARs();
    return SingleBase(var, list, "TARC", cuts);
}

std::vector<Breakdown> BreakdownTools::GetPIDs(std::string pdgvar, std::string cuts, std::string &final_cuts)
{
    if(m_pdglist.empty()){
        cout << __FILE__ << ":" << __LINE__ << " : ERROR : Called PID but no particles set" << endl; 
        exit(0);
    }

    std::vector<Breakdown> list;
    list.clear();

    for(int i = 0; i < (int)m_pdglist.size(); i++){
        PDGInfo particle = m_pdglist[i];
        std::string tmp_cuts;
        if(particle.IsBoth()) tmp_cuts += "TMath::Abs(";
        tmp_cuts += pdgvar;
        if(particle.IsBoth()) tmp_cuts += ")";
        tmp_cuts += " == ";
        tmp_cuts += particle.GetPDGStr();
        Breakdown tmp(particle.GetSymbol(), tmp_cuts, particle.GetColor());
        list.push_back(tmp);
    }
    
    std::string final_cuts = cuts;
    if(!cuts.empty()){
        final_cuts += " && ";
    }
    final_cuts += pdgvar;
    final_cuts += " != -999 ";

    return list;
}

std::vector<Breakdown> BreakdownTools::GetTOPs()
{
    if(m_toplist.empty()){
        cout << __FILE__ << ":" << __LINE__ << " : ERROR : Called TOPO but no topologies set" << endl; 
        exit(0);
    }

    std::vector<Breakdown> list;
    list.clear();

    for(int i = 0; i < (int)m_toplist.size(); i++){
        Topology topology = m_toplist[i];        
        if(topology.GetType() == m_signal->GetType()) continue;

        Breakdown tmp(topology.GetSymbol(), topology.GetSignal(), topology.GetFillColor(), topology.GetFillStyle(),
            topology.GetLineColor(), topology.GetLineStyle());
        list.push_back(tmp);
    }
    return list;
}

std::vector<Breakdown> BreakdownTools::GetTARs()
{
    if(m_target.empty()){
        cout << __FILE__ << ":" << __LINE__ << " : ERROR : Called TARGET but no target name set" << endl; 
        exit(0);
    }

    if(m_tarlist.empty()){
        cout << __FILE__ << ":" << __LINE__ << " : ERROR : Called TARGET but no targets set" << endl; 
        exit(0);
    }
    return m_tarlist;
}

void BreakdownTools::DrawZeroPointLine(THStack * histo){

    TList * rlist = histo->GetHists();
    TH1D * hfirst = (TH1D*)rlist->First()->Clone( (string(rlist->First()->GetName()) + "_clone").c_str() );
    DrawZeroPointLine(hfirst);
}
           
BDCans BreakdownTools::BaseBreakdown(Variable var, std::vector<Breakdown> list, std::string basename, std::string cuts)
{   
    //Make outputs:
    if(list.empty()){ 
        BDCans bad;
        bad.recon = 0x0;
        bad.truth = 0x0;
        bad.smear = 0x0;
        bad.smearSN = 0x0;
        bad.ratio = 0x0;
        cout << __FILE__ << ":" << __LINE__ << " : List is empty, not building breakdown plot for " << basename << endl;
        return bad;
    }

    // Set Other:
    std::string other_cuts;    
    for(unsigned int i = 0; i < list.size(); i++){
        if(!list[i].GetSignal().empty()){
            if(!other_cuts.empty()) other_cuts += " && ";
            other_cuts += "!(";
            other_cuts += list[i].GetSignal();
            other_cuts += ")";
        }
    }
    Breakdown other("Other", other_cuts, DrawingStyle::Other);
    list.push_back(other);

    string base_cuts = cuts;
    if(!base_cuts.empty()) base_cuts += " && ";

    for(unsigned int i = 0; i < list.size(); i++){
        string tmp_cuts = base_cuts;
        tmp_cuts += list[i].GetSignal();
        list[i].SetMap( KinArray(var.GetName(), var.GetNBins(), var.GetBinning(), var.GetSymbol(), tmp_cuts) );
    }

    string units = "";
    if(!var.GetUnits().empty()){
        units += "(";
        units += var.GetUnits();
        units += ")";
    }
    // if(!list[0].GetMap().recon) cout << __FILE__ << ":" << __LINE__ << " : No Recon hist." << endl;
    // cout << "BreakdownTools::BaseBreakdown : Making Stacks" << endl;
    THStack * recon_tot = new THStack( (var.GetSName() + "_" + basename + "_recon").c_str(), (var.GetSymbol() + units + ";Count").c_str());
    
    THStack * truth_tot = new THStack( (var.GetSName() + "_" + basename + "_truth").c_str(), (var.GetSymbol() + units + ";Count").c_str());
    
    THStack * ratio_tot = new THStack( (var.GetSName() + "_" + basename + "_ratio").c_str(), 
        Form(";%s;%s", list[0].GetMap().ratio->GetXaxis()->GetTitle(), list[0].GetMap().ratio->GetYaxis()->GetTitle() ) );
    
    TH2D * smear_tot = (TH2D*)list[0].GetMap().smear->Clone( (var.GetSName() + "_" + basename + "_smear").c_str() );//Just add all of these histos.
    
    // cout << "BreakdownTools::BaseBreakdown : Making Legends" << endl;

    TLegend * recon_leg = Legend(0.25, 0.4, 0.551, 0.362);
    TLegend * truth_leg = Legend(0.25, 0.4, 0.551, 0.362);
    TLegend * ratio_leg = Legend(0.25, 0.4, 0.551, 0.362);
    
    // cout << "BreakdownTools::BaseBreakdown : Making Percentages" << endl;

    std::vector<double> recon_percent = GetPercentages(list, 0);
    std::vector<double> truth_percent = GetPercentages(list, 1);
    std::vector<double> ratio_percent = GetPercentages(list, 2);
    
    // cout << "BreakdownTools::BaseBreakdown : Getting signal dists." << endl;

    DrawingTools::KinMap signal_kinmap = GetSignalMap(var, cuts);

    DrawingTools::KinMap tmp_check;
    if(check){
        tmp_check = KinArray(var.GetName(), var.GetSymbol(), cuts);
        SetColors(tmp_check, 0, kPink-9, 0, 4);
        // TODO: Check first 0 - Fill color. 
    }

    BDCans cans;

    // cout << "BreakdownTools::BaseBreakdown : Drawing Plots" << endl;

    for(int type = 0; type < 3; type++){
        THStack * tmp_stack;
        TLegend * tmp_leg;
        TH1D * tmp_sig;
        std::vector<double> percent;
        switch(type){
            case 0: 
                cans.recon = new TCanvas( recon_tot->GetName(), "", 500, 500); 
                cans.recon->cd();
                tmp_sig = signal_kinmap.recon;
                tmp_stack = recon_tot; 
                tmp_leg = recon_leg; 
                percent = recon_percent;
                break;
            case 1: 
                cans.truth = new TCanvas( truth_tot->GetName(), "", 500, 500); 
                cans.truth->cd();
                tmp_sig = signal_kinmap.truth;
                tmp_stack = truth_tot; 
                tmp_leg = truth_leg; 
                percent = truth_percent;
                break;
            case 2: 
                cans.ratio = new TCanvas( ratio_tot->GetName(), "", 500, 500); 
                cans.ratio->cd();
                tmp_sig = signal_kinmap.ratio;
                tmp_stack = ratio_tot; 
                tmp_leg = ratio_leg; 
                percent = ratio_percent;
                break;
        }

        for(unsigned int i = 1; i < list.size() + 1; i++){
            Breakdown tmp = list[ list.size() - i ];
            TH1D * tmp_hist;
            switch(type){
                case 0: tmp_hist = tmp.GetMap().recon; break;
                case 1: tmp_hist = tmp.GetMap().truth; break;
                case 2: tmp_hist = tmp.GetMap().ratio; break;
            }
            // cout << "Adding to tmp_stack" << endl;
            tmp_stack->Add( tmp_hist );
        }

        tmp_leg->AddEntry(tmp_sig, m_signal->GetSymbol().c_str(), "l");

        for(unsigned int i = 0; i < list.size(); i++){
            Breakdown tmp = list[ i ];
            TH1D * tmp_hist;
            switch(type){
                case 0: tmp_hist = tmp.GetMap().recon; break;
                case 1: tmp_hist = tmp.GetMap().truth; break;
                case 2: tmp_hist = tmp.GetMap().ratio; break;
            }
            tmp_leg->AddEntry( tmp_hist, (tmp.GetName() + string(Form(" (%.2f)", percent[ i ] )) ).c_str(), tmp.GetLegOpt() );
        }

        tmp_stack->Draw();
        tmp_sig->Draw("SAME");
        tmp_leg->Draw();
        GetPOT(0.1,0.1)->Draw();
        percent.clear();

        if(type == 2){
            // cout << "Ratio Stack line" << endl;
            RatioStats(tmp_stack)->Draw();
            DrawZeroPointLine(tmp_stack);
            // cout << "Ratio Stack line done." << endl;
        }

        if(check){
            TH1D * check_hist;
            switch(type){
                case 0: check_hist = tmp_check.recon; break;
                case 1: check_hist = tmp_check.truth; break;
                case 2: check_hist = tmp_check.ratio; break;
            }
            check_hist->Draw("SAME");
            cout << "Checking if integral is the same:";
            double difference = GetHistFromStack(tmp_stack)->Integral() - check_hist->Integral();
            if(difference == 0.) cout << " Integrals consistent between stack and hist.";
            else cout << " Inconsistent integrals between stack and hist (Diff. = " << difference << ")";
            cout << endl;
        }
    }

    for(unsigned int i = 1; i < list.size(); i++) smear_tot->Add(list[i].GetMap().smear);

    cans.smear = new TCanvas( smear_tot->GetName(), "", 500, 500);
    cans.smear->cd();
    smear_tot->Draw("COLZ");
    GetPOT(0.1,0.1)->Draw();
    
    TH2D * smear_totSN = NormalHist(smear_tot, 0., true);
    cans.smearSN = new TCanvas( (std::string(smear_tot->GetName()) + "SN").c_str(), "", 500, 500);
    cans.smearSN->cd();
    smear_totSN->Draw("COLZ");
    GetPOT(0.1,0.1)->Draw();
    // cout << "BreakdownTools::BaseBreakdown : Finished." << endl;
    return cans;
}

TCanvas * BreakdownTools::SingleBase(Variable var, std::vector<Breakdown> list, std::string basename, std::string cuts)
{
//Make outputs:
    if(list.empty()){ 
        TCanvas * bad = 0x0;
        cout << __FILE__ << ":" << __LINE__ << " : List is empty, not building breakdown plot for " << basename << endl;
        return bad;
    }

    // Set Other:
    std::string other_cuts;    
    for(unsigned int i = 0; i < list.size(); i++){
        if(!list[i].GetSignal().empty()){
            if(!other_cuts.empty()) other_cuts += " && ";
            other_cuts += "!(";
            other_cuts += list[i].GetSignal();
            other_cuts += ")";
        }
    }

    Breakdown other("Other", other_cuts, DrawingStyle::Other);
    list.push_back(other);

    string base_cuts = cuts;
    if(!base_cuts.empty()) base_cuts += " && ";

    for(unsigned int i = 0; i < list.size(); i++){
        string tmp_cuts = base_cuts;
        tmp_cuts += list[i].GetSignal();
        list[i].SetHist( GetHisto(var.GetName(), var.GetNBins(), var.GetBinning(), var.GetAxisTitle(), tmp_cuts) );
    }

    string units = "";
    if(!var.GetUnits().empty()){
        units += "(";
        units += var.GetUnits();
        units += ")";
    }
    // if(!list[0].GetMap().recon) cout << __FILE__ << ":" << __LINE__ << " : No Recon hist." << endl;
    // cout << "BreakdownTools::BaseBreakdown : Making Stacks" << endl;
    THStack * hist_tot = new THStack( (var.GetSName() + "_" + basename).c_str(), (var.GetSymbol() + units + ";Counts").c_str());
    
    TLegend * hist_leg = Legend(0.25, 0.4, 0.551, 0.362);
    
    std::vector<double> hist_percent = GetPercentages(list);
    
    TH1D * signal_hist = GetSignalHist(var, cuts);

    TH1D * tmp_check;
    if(check){
        tmp_check = GetHisto(var.GetName(), var.GetNBins(), var.GetBinning(), var.GetSymbol(), cuts);
        SetColors(tmp_check, 0, kPink-9, 0, 4);
        // TODO: Check first 0 - Fill color. 
    }

    for(unsigned int i = 1; i < list.size() + 1; i++) hist_tot->Add( list[ list.size() - i ].GetHist() );

    hist_leg->AddEntry(signal_hist, m_signal->GetSymbol().c_str(), "l");

    for(unsigned int i = 0; i < list.size(); i++){
        Breakdown tmp = list[ i ];
        hist_leg->AddEntry( tmp.GetHist(), (tmp.GetName() + string(Form(" (%.2f)", hist_percent[ i ] )) ).c_str(), tmp.GetLegOpt() );
    }

    TCanvas * cans = new TCanvas( hist_tot->GetName(), "", 500, 500);
    cans = new TCanvas( hist_tot->GetName(), "", 500, 500); 
    cans->cd();

    hist_tot->Draw();
    signal_hist->Draw("SAME");
    hist_leg->Draw();
    GetPOT(0.1,0.1)->Draw();
    hist_percent.clear();

    if(check){
        tmp_check->Draw("SAME");
        cout << "Checking if integral is the same:";
        double difference = GetHistFromStack(hist_tot)->Integral() - tmp_check->Integral();
        if(difference == 0.) cout << " Integrals consistent between stack and hist.";
        else cout << " Inconsistent integrals between stack and hist (Diff. = " << difference << ")";
        cout << endl;
    }
    return cans;
}


DrawingTools::KinMap BreakdownTools::GetSignalMap(Variable var, std::string cuts){
    std::string hsignal = cuts;
    if(!hsignal.empty()){
        hsignal += " && ";
    }
    hsignal += m_signal->GetSignal();
    DrawingTools::KinMap signal_kinmap = KinArray(var.GetName(), var.GetNBins(), var.GetBinning(), var.GetSymbol(), hsignal);
    SetColors(signal_kinmap, m_signal->GetFillColor(), m_signal->GetLineColor(), m_signal->GetFillStyle(), m_signal->GetLineStyle());
    signal_kinmap.recon->SetLineWidth(2);
    signal_kinmap.truth->SetLineWidth(2);
    signal_kinmap.ratio->SetLineWidth(2);
    return signal_kinmap;
}

TH1D * BreakdownTools::GetSignalHist(Variable var, std::string cuts)
{
    std::string hsignal = cuts;
    if(!hsignal.empty()){
        hsignal += " && ";
    }
    hsignal += m_signal->GetSignal();
    TH1D * signal_hist = GetHisto(var.GetName(), var.GetNBins(), var.GetBinning(), var.GetSymbol(), hsignal);
    SetColors(signal_hist, m_signal->GetFillColor(), m_signal->GetLineColor(), m_signal->GetFillStyle(), m_signal->GetLineStyle());
    signal_hist->SetLineWidth(2);
    return signal_hist;
}

std::vector<double> BreakdownTools::GetPercentages(std::vector<Breakdown> list, int type)
{
    std::vector<TH1D*> histos;
    // cout<< (type == -999 ? "Running in single hist mode" : "") << endl;
    for(unsigned int i = 0; i < list.size(); i++){
        if(type == -999) histos.push_back( list[i].GetHist() );
        else{
            switch(type) {
                case 0 : histos.push_back( list[i].GetMap().recon ); break;
                case 1 : histos.push_back( list[i].GetMap().truth ); break;
                case 2 : histos.push_back( list[i].GetMap().ratio ); break;
                default: cout << __FILE__ << ":" << __LINE__ << ": Warning could not determine type" << endl; break;
            }
        }
    }
    return DrawingTools::GetPercentage(histos);
}

#endif