#include "BreakdownTools.h"
#include "DrawingStyle.h"
#include "DataClasses.h"

#include <string>
#include <iostream>

#include "TH1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLegend.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TList.h"
#include "TF1.h"
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

BreakdownTools::BreakdownTools(std::string filename, std::string treename, Topologies * topologies, std::string target_name) : 
    DrawingTools(filename, treename, ("BD" + treename).c_str()), m_printPOT(false), m_fullbreakdown(true) {

//    PDGInfo proton;
    m_pdglist.push_back( PDGInfo(2212, "proton",        "p"));
    m_pdglist.push_back( PDGInfo(211,  "piplus",        "#pi^{+}") );
    m_pdglist.push_back( PDGInfo(-211, "piminus",       "#pi^{-}") );
    m_pdglist.push_back( PDGInfo(2112, "neutron",       "n") );
    m_pdglist.push_back( PDGInfo(11,   "electronpm",    "e^{#pm}", true) );
    m_pdglist.push_back( PDGInfo(13,   "muon",          "#mu^{-}") );
    m_pdglist.push_back( PDGInfo(-13,  "amuon",         "#mu^{+}") );
    m_pdglist.push_back( PDGInfo(111,  "pizero",        "#pi^{0}") );
    m_pdglist.push_back( PDGInfo(321,  "kapm",          "K^{#pm}", true) );
    m_pdglist.push_back( PDGInfo(311,  "kazero",        "K^{0}") );
    //Miminum particles to define in breakdown:
    ResetPDGBDlist();

    m_topologies = new Topologies( topologies );

    if(!m_topologies->GetList2Draw().empty()){
        ClearTOPBDlist();
        m_toplist_minBD = m_topologies->GetList2Draw();
    }

    m_target = target_name;
    
    m_toplist = m_topologies->GetList();
    ResetTOPBDlist();

    m_statcounter = -1;

    m_signal = m_topologies->GetSignal();
    m_sig_top = m_topologies->GetTopology(m_signal);
}

BreakdownTools::~BreakdownTools(){
    m_pdglist.clear();
    m_toplist.clear();

    ClearPDGBDlist();
    ClearTOPBDlist();

    delete m_topologies;
}

void BreakdownTools::ResetPDGBDlist(){
    ClearPDGBDlist();
    SetMinPDGBDlist(2212);
    SetMinPDGBDlist(2112);
    SetMinPDGBDlist(211);
    SetMinPDGBDlist(-211);
    SetMinPDGBDlist(13);
}

void BreakdownTools::ResetTOPBDlist(){
    ClearTOPBDlist();
    SetMinTOPBDlist( Topology::CC1P1PiPlus        );
    SetMinTOPBDlist( Topology::CCNPNPiMinus       );
    SetMinTOPBDlist( Topology::CCNP               );
    SetMinTOPBDlist( Topology::CCNPiPlus          );
    SetMinTOPBDlist( Topology::CCNPNPiZero        );
    SetMinTOPBDlist( Topology::CCNPiZeroNPiPlus   );
    SetMinTOPBDlist( Topology::Other              );
}

BDCans BreakdownTools::PID(Variable var, Int_t nbins, Double_t * bins, std::string pdgvar, std::string cuts, bool check)
{
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

    std::string base_cuts = cuts;
    if(!cuts.empty()){
        base_cuts += " && ";
    }
    base_cuts += pdgvar;
    base_cuts += " != -999 ";

    return BaseBreakdown(var, nbins, bins, list, "PID", base_cuts, check);
}

BDCans BreakdownTools::PID(Variable var, Int_t nbins, Double_t low, Double_t high, std::string pdgvar, std::string cuts, bool check){
    return PID(var, nbins, SetBinning(nbins, low, high), pdgvar, cuts, check);
}

BDCans BreakdownTools::TOPO(Variable var, Int_t nbins, Double_t * bins, std::string cuts, bool check){
    // cout << "BreakdownTools::TOPO(Variable var, Int_t nbins, Double_t * bins, std::string cuts)" << endl;
    std::vector<Breakdown> list;
    list.clear();

    bool total_CC1p1pi = true;
    for(int i = 0; i < (int)m_toplist.size(); i++){
        if(m_toplist[i].GetType() == Topology::OCC1P1PiPlus){
           total_CC1p1pi = false;
           continue;
       }
    }

    for(int i = 0; i < (int)m_toplist.size(); i++){
        Topology topology = m_toplist[i];
        if(print_level::quiet){ 
            cout << "Topology = " << topology.GetName() << endl;
            cout << "           " << topology.GetSignal() << endl;
        }

        if(topology.GetType() == m_signal){ 
            if(print_level::quiet) cout << " (Is signal) -- Skipping." << endl;
            continue;
        }

        // if( (!total_CC1p1pi) && topology.GetType() == Topology::CC1P1PiPlus){
        //     cout << "Found Topology::CC1P1PiPlus -- Skipping" << endl;
        //     continue;
        // }

        Breakdown tmp(topology.GetSymbol(), topology.GetSignal(), topology.GetFillColor(), topology.GetFillStyle(),
            topology.GetLineColor(), topology.GetLineStyle());
        list.push_back(tmp);
    }
    
    return BaseBreakdown(var, nbins, bins, list, "TOP", cuts, check);
}

BDCans BreakdownTools::TOPO(Variable var, Int_t nbins, Double_t low, Double_t high, std::string cuts, bool check){
    return TOPO(var, nbins, SetBinning(nbins, low, high), cuts, check);
}

BDCans BreakdownTools::TARGET(Variable var, Int_t nbins, Double_t * bins, std::string cuts, bool check)
{
    std::vector<Breakdown> list;
    list.clear();

    Breakdown hyd("Hydrogen", m_target + " == 1", DrawingStyle::HYDROGEN);
    list.push_back(hyd);
    
    Breakdown car("Carbon", m_target + " == 6", DrawingStyle::CARBON);
    list.push_back(car);

    Breakdown oxy("Oxygen", m_target + " == 8", DrawingStyle::OXYGEN);
    list.push_back(oxy);
  
    return BaseBreakdown(var, nbins, bins, list, "TAR", cuts, check);
}

BDCans BreakdownTools::TARGET(Variable var, Int_t nbins, Double_t low, Double_t high, std::string cuts, bool check){
    return TARGET(var, nbins, SetBinning(nbins, low, high), cuts, check);
}

TCanvas * BreakdownTools::TARGETSingle(Variable var, Int_t nbins, Double_t * bins, std::string cuts, bool check){
    std::vector<Breakdown> list;
    list.clear();

    Breakdown hyd("Hydrogen", m_target + " == 1", DrawingStyle::HYDROGEN);
    list.push_back(hyd);
    
    Breakdown car("Carbon", m_target + " == 6", DrawingStyle::CARBON);
    list.push_back(car);

    Breakdown oxy("Oxygen", m_target + " == 8", DrawingStyle::OXYGEN);
    list.push_back(oxy);

    return SingleBase(var, nbins, bins, list, "TARONE", cuts, check);
}

TCanvas * BreakdownTools::TARGETSingle(Variable var, Int_t nbins, Double_t low, Double_t high, std::string cuts, bool check){
    return TARGETSingle(var, nbins, SetBinning(nbins, low, high), cuts, check);
}

TLegend * BreakdownTools::RatioStats(const THStack * ratio_tot)
{
//Get the ratio histograms, make them into one histogram and determine the rms and mean:
    TList * rlist = ratio_tot->GetHists();
    TH1D * hfirst = (TH1D*)rlist->First()->Clone( (string(rlist->First()->GetName()) + "_clone").c_str() );
    // cout << "FOund hfirst" << endl;
    Int_t ratio_nbins = hfirst->GetNbinsX();
    Double_t ratio_low = hfirst->GetXaxis()->GetXmin();
    Double_t ratio_high = hfirst->GetXaxis()->GetXmax();
    // cout << "Range : Bins = " << ratio_nbins << " low = " << ratio_low << " high = " << ratio_high << endl;
    
    TIter next(rlist);
    m_statcounter++;
    TH1D ratio_sum( Form("%s_ratio_sum%.3d", hfirst->GetName(), m_statcounter), "", ratio_nbins, ratio_low, ratio_high);
    TH1D * rhist_tmp;
    while ( (rhist_tmp = (TH1D*)next()) ) {
        ratio_sum.Add(rhist_tmp);
    }
    
    TLegend * ratio_stats = Legend(0.25, 0.8, 0.05, 0.1);
    ratio_stats->AddEntry((TObject*)0, Form("Mean = %.3f", (double)ratio_sum.GetMean()), "");
    ratio_stats->AddEntry((TObject*)0, Form(" RMS = %.3f", (double)ratio_sum.GetRMS() ), "");

    TF1 * cauchy = new TF1("cauchy","([2]*[1])/(TMath::Pi()*([1]*[1] + (x-[0])*(x-[0]) ) )", ratio_low, ratio_high);
    delete hfirst;

    cauchy->SetParameter(0, (double)ratio_sum.GetXaxis()->GetBinCenter(ratio_sum.GetMaximumBin() ) );
    cauchy->SetParameter(1, (double)ratio_sum.GetRMS()  );
    cauchy->SetParameter(2, (double)ratio_sum.Integral());
    TFitResultPtr r = ratio_sum.Fit(cauchy,"RLNQ");
    Int_t fitStatus = r;
    // cout << "(double)cauchy->GetParameter(0) = " << (double)cauchy->GetParameter(0) << endl;
    // cout << "(double)cauchy->GetParameter(1) = " << (double)cauchy->GetParameter(1) << endl;

    if(fitStatus == 0){//Fit status successful add the parameters:
        ratio_stats->AddEntry((TObject*)0, Form("Cauchy Mean = %.3f", (double)cauchy->GetParameter(0)), "");
        ratio_stats->AddEntry((TObject*)0, Form("Cauchy #sigma = %.3f", (double)cauchy->GetParameter(1)), "");
    }

    return ratio_stats;
}

TCanvas * BreakdownTools::PID(Variable var, std::string pdgvar, std::string cuts, std::vector<PDGInfo> pdglist, bool check)
{
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

    std::string base_cuts = cuts;
    if(!cuts.empty()){
        base_cuts += " && ";
    }
    base_cuts += pdgvar;
    base_cuts += " != -999 ";

    return SingleBase(var, var.GetNBins(), var.GetBinning(), list, "PID", base_cuts, check);
}

// void BreakdownTools::DrawZeroPointLine(TH1 * histo){

//     TLine * z_line = new TLine(0.0, histo->GetMinimum(), 0.0, histo->GetMaximum());
//     z_line->SetLineColor(1);
//     z_line->SetLineStyle(2);
//     z_line->Draw(); 
// }

void BreakdownTools::DrawZeroPointLine(THStack * histo){

    TList * rlist = histo->GetHists();
    TH1D * hfirst = (TH1D*)rlist->First()->Clone( (string(rlist->First()->GetName()) + "_clone").c_str() );
    DrawZeroPointLine(hfirst);
}
           
BDCans BreakdownTools::BaseBreakdown(Variable var, Int_t nbins, Double_t * bins, std::vector<Breakdown> list, 
    std::string basename, std::string cuts, bool check)
{
    //Make outputs:
    if(list.empty()){ 
        BDCans bad;
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
    // cout << "BreakdownTools::BaseBreakdown : Making Kin Array" << endl;    

    string base_cuts = cuts;
    if(!base_cuts.empty()) base_cuts += " && ";

    for(unsigned int i = 0; i < list.size(); i++){
        string tmp_cuts = base_cuts;
        tmp_cuts += list[i].GetSignal();
        list[i].SetMap( KinArray(var.GetName(), nbins, bins, var.GetSymbol(), tmp_cuts) );
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
    
    TH2D * smear_tot = (TH2D*)list[0].GetMap().smear->Clone( (var.GetSName() + "_PID_smear").c_str() );//Just add all of these histos.
    
    // cout << "BreakdownTools::BaseBreakdown : Making Legends" << endl;

    TLegend * recon_leg = Legend(0.25, 0.4, 0.551, 0.362);
    TLegend * truth_leg = Legend(0.25, 0.4, 0.551, 0.362);
    TLegend * ratio_leg = Legend(0.25, 0.4, 0.551, 0.362);
    
    // cout << "BreakdownTools::BaseBreakdown : Making Percentages" << endl;

    std::vector<double> recon_percent = GetPercentages(list, 0);
    std::vector<double> truth_percent = GetPercentages(list, 1);
    std::vector<double> ratio_percent = GetPercentages(list, 2);
    
    // cout << "BreakdownTools::BaseBreakdown : Getting signal dists." << endl;

    DrawingTools::KinMap signal_kinmap = GetSignalMap(var, nbins, bins, cuts);

    DrawingTools::KinMap tmp_check;
    if(check){
        tmp_check = KinArray(var.GetName(), nbins, bins, var.GetSymbol(), cuts);
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
            tmp_stack->Add( tmp_hist );
        }

        tmp_leg->AddEntry(tmp_sig, m_sig_top.GetSymbol().c_str(), "l");

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
            RatioStats(tmp_stack)->Draw();
            DrawZeroPointLine(tmp_stack);
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
    
    cout << "BreakdownTools::BaseBreakdown : Finished." << endl;

    return cans;
}

TCanvas * BreakdownTools::SingleBase(Variable var, Int_t nbins, Double_t * bins, std::vector<Breakdown> list, std::string basename, 
    std::string cuts, bool check)
{
//Make outputs:
    if(list.empty()){ 
        TCanvas * bad;
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
    // cout << "BreakdownTools::BaseBreakdown : Making Kin Array" << endl;    

    string base_cuts = cuts;
    if(!base_cuts.empty()) base_cuts += " && ";

    for(unsigned int i = 0; i < list.size(); i++){
        string tmp_cuts = base_cuts;
        tmp_cuts += list[i].GetSignal();
        list[i].SetHist( GetHisto(var.GetName(), nbins, bins, var.GetAxisTitle(), tmp_cuts) );
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
    
    std::vector<double> hist_percent = GetPercentages(list, 0);
    
    TH1D * signal_hist = GetSignalHist(var, nbins, bins, cuts);

    TH1D * tmp_check;
    if(check){
        tmp_check = GetHisto(var.GetName(), nbins, bins, var.GetSymbol(), cuts);
        SetColors(tmp_check, 0, kPink-9, 0, 4);
        // TODO: Check first 0 - Fill color. 
    }

    for(unsigned int i = 1; i < list.size() + 1; i++) hist_tot->Add( list[ list.size() - i ].GetHist() );

    hist_leg->AddEntry(signal_hist, m_sig_top.GetSymbol().c_str(), "l");

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
    percent.clear();

    if(check){
        tmp_check->Draw("SAME");
        cout << "Checking if integral is the same:";
        double difference = GetHistFromStack(hist_tot)->Integral() - tmp_check->Integral();
        if(difference == 0.) cout << " Integrals consistent between stack and hist.";
        else cout << " Inconsistent integrals between stack and hist (Diff. = " << difference << ")";
        cout << endl;
    }

    cout << "BreakdownTools::BaseBreakdown : Finished." << endl;

    return cans;
}


DrawingTools::KinMap BreakdownTools::GetSignalMap(Variable var, Int_t nbins, Double_t * bins, std::string cuts){
    std::string hsignal = cuts;
    if(!hsignal.empty()){
        hsignal += " && ";
    }
    hsignal += m_sig_top.GetSignal();
    DrawingTools::KinMap signal_kinmap = KinArray(var.GetName(), nbins, bins, var.GetSymbol(), hsignal);
    SetColors(signal_kinmap, m_sig_top.GetFillColor(), m_sig_top.GetLineColor(), m_sig_top.GetFillStyle(), m_sig_top.GetLineStyle());
    signal_kinmap.recon->SetLineWidth(2);
    signal_kinmap.truth->SetLineWidth(2);
    signal_kinmap.ratio->SetLineWidth(2);
    return signal_kinmap;
}

TH1D * BreakdownTools::GetSignalHist(Variable var, Int_t nbins, Double_t * bins, std::string cuts)
{
    std::string hsignal = cuts;
    if(!hsignal.empty()){
        hsignal += " && ";
    }
    hsignal += m_sig_top.GetSignal();
    TH1D * signal_hist = GetHisto(var.GetName(), nbins, bins, var.GetSymbol(), hsignal);
    SetColors(signal_hist, m_sig_top.GetFillColor(), m_sig_top.GetLineColor(), m_sig_top.GetFillStyle(), m_sig_top.GetLineStyle());
    signal_hist->SetLineWidth(2);
    return signal_hist;
}

std::vector<double> BreakdownTools::GetPercentages(std::vector<Breakdown> list, int type)
{
    std::vector<TH1D*> histos;
    bool is_hist = false;
    if(list[0].GetHist()) is_hist = true;;
    for(unsigned int i = 0; i < list.size(); i++){
        if(is_hist) histos.push_back( list[i].GetHist() );
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

// BreakdownTools::SetPerentages(std::vector<Breakdown> list, int type)
// {
//     std::vector<double> pers = DrawingTools::GetPercentage(histos, type);

//     for(unsigned int i = 0; i < list.size(); i++){
//         switch(type) {
//             case 0 : list[i].GetMap().recon ); break;
//             case 1 : list[i].GetMap().truth ); break;
//             case 2 : list[i].GetMap().ratio ); break;
//             default: cout << __FILE__ << ":" << __LINE__ << ": Warning could not determine type" << endl; break;
//         }
//     }
// }

#endif








