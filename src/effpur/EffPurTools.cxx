#ifndef __EFFPURTOOLS__CXX
#define __EFFPURTOOLS__CXX

#include "EffPurTools.hxx"

#include <cassert>

//ROOT Includes:
#include "TLeaf.h"

// #include "DataClasses.h"
#include "DrawingTools.hxx"
#include <ReadParam.hxx>

//Forward declarations;
#include "TFile.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"

EffPurTools::EffPurTools(std::string filename, std::string reconame, std::string truename) : m_debug(false) {
    
    if(m_debug){
        cout << "    Filename: " << filename << endl;
        cout << "Truth branch: " << truename << endl;
        cout << "Recon branch: " << reconame << endl;
    }
    //Include counter to make sure hists have unique names:
    // Counter = -1;

    m_ncuts = -999;
    
    m_file = new TFile(filename.c_str(), "READ");
    
    if(!m_file){
        cout << "Could not open file named: " << filename << endl;
        exit(0);
    }
    
   m_truth = static_cast<TTree*>(m_file->Get(truename.c_str()));
   m_recon = static_cast<TTree*>(m_file->Get(reconame.c_str()));

   assert(m_truth);
   assert(m_recon);

    // m_truth = (TTree*)m_file->Get(truename.c_str());
    // m_recon = (TTree*)m_file->Get(reconame.c_str());
    
    if(!m_truth || !m_recon){
        cout << "Could not access truth/recon tree(s)." << endl;
        exit(0);
    }

    string opts_file = string( getenv("HP_ROOT") );
    opts_file += "/effpur/parameters.txt";

    std::vector<std::string> tmp_cuts;  
    string line = ReadParam::GetParameterS( "cut 1", opts_file);
    int runner = 1;
    while( !line.empty() ){
        tmp_cuts.push_back( line );
        runner++;
        line = ReadParam::GetParameterS( string(Form("cut %d", runner)), opts_file);
    }

    SetCutNames(tmp_cuts);
}

EffPurTools::EffPurTools(std::string filename, std::vector<std::string> cut_names, std::string reconame, std::string truename) : m_debug(false) {
    
    if(m_debug){
        cout << "    Filename: " << filename << endl;
        cout << "Truth branch: " << truename << endl;
        cout << "Recon branch: " << reconame << endl;
    }
    
    //Include counter to make sure hists have unique names:

    m_ncuts = -999;
    
    m_file = new TFile(filename.c_str(), "READ");
    
    if(!m_file){
        cout << "Could not open file named: " << filename << endl;
        exit(0);
    }
    
    //    m_truth = static_cast<TTree*>(m_file->Get(truename.c_str()));
    //    m_recon = static_cast<TTree*>(m_file->Get(reconame.c_str()));
    
    m_truth = (TTree*)m_file->Get(truename.c_str());
    m_recon = (TTree*)m_file->Get(reconame.c_str());
    
    if(!m_truth || !m_recon){
        cout << "Could not access truth/recon tree(s)." << endl;
        exit(0);
    }
    
    SetCutNames(cut_names);
}

EffPurTools::~EffPurTools(){
    if(m_truth) delete m_truth;
    if(m_recon) delete m_recon;
    if(m_file->IsOpen()) m_file->Close();
    delete m_file;
}

//These may not be void functions:
TH1D * EffPurTools::EffVSCuts(std::string signal, int branch, std::string cuts){
    
    if(m_debug) cout << "EffPurTools::EffVSCuts()" << endl;
    
    if(signal.empty()){
        cout << "No signal defined -- Need for efficiency calculations" << endl;
        return 0x0;
    }
    else if(m_debug) cout << "    Signal: " << signal << endl;
    
    std::string full_signal = signal;
    
    if(!cuts.empty()){
        full_signal += " && ";
        full_signal += cuts;
        if(m_debug)  cout << "    Cut(s): " << cuts << endl;
    }
    else if(m_debug) cout << "    Cut(s): None" << endl;
    
    if(m_debug) cout << "Read tree " << m_truth->GetName() << endl;

    int ncuts = GetNCuts();

    if(m_debug) cout << "Found and Filled ncuts histogram " << endl;

    if(m_debug) cout << "Number of cuts found to be " << ncuts << endl;
    
    TH1D * num = EventsVSCuts(m_truth, full_signal, branch, ncuts);
    TH1D * den = new TH1D("den", "", num->GetNbinsX(), 0., (double)num->GetNbinsX());
    
    for(int i = 0; i < num->GetNbinsX(); i++){
        den->SetBinContent(i+1,num->GetBinContent(1));
        den->SetBinError(i+1,num->GetBinError(1));
        if(m_debug){
            cout << "num->GetBinContent(1) = " << num->GetBinContent(1) << " +/- " << num->GetBinError(1) << endl;
            cout << "den->GetBinContent("<< i+1 <<") = " << den->GetBinContent(i+1) << " +/- " << den->GetBinError(i+1) << endl;
        }
    }

    Counter++;
    TH1D * effcuts = DrawRatioVSCuts(num, den, "Efficiency (%)", Form("h_effic%.6d", Counter));
    effcuts->SetLineColor(Blue);
    
    delete num;
    delete den;
    
    return effcuts;
}

TH1D * EffPurTools::PurVSCuts(std::string signal, int branch, std::string cuts){
    if(m_debug) cout << "EffPurTools::PurVSCuts(TString, TString)" << endl;
    
    
    if(signal.empty()){
        cout << "No signal defined -- Need for purity calculations" << endl;
        return 0x0;
    }
    else if(m_debug) cout << "    Signal: " << signal << endl;
    
    
    std::string full_signal = signal;
    
    if(!cuts.empty()){
        full_signal += " && ";
        full_signal += cuts;
        if(m_debug)  cout << "    Cut(s): " << cuts << endl;
    }
    else if(m_debug) cout << "    Cut(s): None" << endl;
    
    if(m_debug) cout << "Read tree " << m_recon->GetName() << endl;
    
    int ncuts = GetNCuts();
    
    if(m_debug) cout << "Number of cuts found to be " << ncuts << endl;
    
    TH1D * num = EventsVSCuts(m_recon, full_signal, branch, ncuts, "pur_num");
    TH1D * den = EventsVSCuts(m_recon, cuts, branch, ncuts, "pur_den");
    
    if(m_debug){
        for(int i = 0; i < num->GetNbinsX(); i++ ){
            cout << "num->GetBinContent("<<i+1<<") = " << num->GetBinContent(i+1) << " +/- " << num->GetBinError(i+1) << endl;
            cout << "den->GetBinContent("<<i+1<<") = " << den->GetBinContent(i+1) << " +/- " << den->GetBinError(i+1) << endl;
        }
    }
    
    Counter++;
    TH1D * purcuts = DrawRatioVSCuts(num, den, "Purity (%)", Form("h_purity%.6d", Counter));
    purcuts->SetLineColor(Yellow);
    
    delete num;
    delete den;
    
    return purcuts;
}

TH1D * EffPurTools::EffVSVar(std::string var, int nbins, Double_t * xbins, std::string signal, std::string cuts, std::string x_title){
    if(m_debug) cout << "EffPurTools::EffVSVar()" << endl;
    
    TH1D * effvar = RatioVSVar(m_truth, var, nbins, xbins, signal, cuts, x_title);
    Counter++;
    effvar->SetName(Form("effvar%.6d", Counter));
    effvar->GetYaxis()->SetTitle("Efficiency (%)");
    return effvar;
}

TH1D * EffPurTools::EffVSVar(std::string var, int nbins, Double_t x_low, Double_t x_high, std::string signal, std::string cuts, std::string x_title){
    if(m_debug) cout << "EffPurTools::EffVSVar(const TString var, int nbins, const Double_t x_low, const Double_t x_high, const TString signal, const TString cuts, const TString x_title)" << endl;
    return EffVSVar(var, nbins, DrawingTools::SetBinning(nbins, x_low, x_high), signal, cuts, x_title);
}

TH2D * EffPurTools::EffVSVar(std::string var_yx, int x_nbins, Double_t * x_bins, int y_nbins, Double_t * y_bins, std::string signal, std::string cuts, std::string xy_title){
    if(m_debug) cout << "EffPurTools::EffVSVar()" << endl;
    
    TH2D * effvar = RatioVSVar(m_truth, var_yx, x_nbins, x_bins, y_nbins, y_bins, signal, cuts, xy_title);
    Counter++;
    effvar->SetName(Form("effvar%.6d", Counter));
    //effvar->Set
    return effvar;
}

TH2D * EffPurTools::EffVSVar(std::string var_yx, int x_nbins, Double_t x_low, Double_t x_high, int y_nbins, Double_t y_low, Double_t y_high, std::string signal, std::string cuts, std::string xy_title){
    return EffVSVar(var_yx, x_nbins, DrawingTools::SetBinning(x_nbins, x_low, x_high), y_nbins, DrawingTools::SetBinning(y_nbins, y_low, y_high), signal, cuts, xy_title);
}

TH1D * EffPurTools::PurVSVar(std::string var, int nbins, Double_t * xbins, std::string signal, std::string cuts, std::string x_title){
    if(m_debug) cout << "EffPurTools::PurVSVar()" << endl;
    
    TH1D * purvar = RatioVSVar(m_recon, var, nbins, xbins, cuts, signal, x_title);
    Counter++;
    purvar->SetName(Form("purvar%.6d", Counter));
    purvar->GetYaxis()->SetTitle("Purity (%)");
    return purvar;
}

TH1D * EffPurTools::PurVSVar(std::string var, int nbins, Double_t x_low, Double_t x_high, std::string signal, std::string cuts, std::string x_title){
    if(m_debug) cout << "EffPurTools::PurVSVar()" << endl;
    return PurVSVar(var, nbins, DrawingTools::SetBinning(nbins, x_low, x_high), signal, cuts, x_title);
}

TH2D * EffPurTools::PurVSVar(std::string var_yx, int x_nbins, Double_t * x_bins, int y_nbins, Double_t * y_bins, std::string signal, std::string cuts, std::string  xy_title){
    if(m_debug) cout << "EffPurTools::PurVSVar()" << endl;
    
    TH2D * purvar = RatioVSVar(m_recon, var_yx, x_nbins, x_bins, y_nbins, y_bins, cuts, signal, xy_title);
    Counter++;
    purvar->SetName(Form("purvar%.6d", Counter));
//    purvar->GetYaxis()->SetTitle("Purity");
    return purvar;
}

TH2D * EffPurTools::PurVSVar(std::string var_yx, int x_nbins, Double_t x_low, Double_t x_high, int y_nbins, Double_t y_low, Double_t y_high, std::string signal, std::string cuts, std::string  xy_title){
    if(m_debug) cout << "EffPurTools::PurVSVar()" << endl;
    return PurVSVar(var_yx, x_nbins, DrawingTools::SetBinning(x_nbins, x_low, x_high), y_nbins, DrawingTools::SetBinning(y_nbins, y_low, y_high), signal, cuts, xy_title);
}

TH1D * EffPurTools::EventsVSCuts(TTree * intree, std::string cuts, int branch, int ncuts, std::string name){
    
    if(m_debug) cout << "EffPurTools::EventsVSCuts" << endl;
    
    std::string tmp_cuts;
    
    if(!cuts.empty()){
        tmp_cuts += cuts;
        tmp_cuts += " && ";
    }
    
    std::string tree_name = std::string( intree->GetName() );
    if(tree_name.find("Truth") != std::string::npos){
        tmp_cuts += "truth_";
    }

    std::stringstream sbranch;
    sbranch << branch;
    tmp_cuts += "accum_level[" + sbranch.str() + "]>";
    
    if(m_debug) cout << "Cut set as: " << tmp_cuts << endl;
    
    //This will complain: delete outside of function.
    TH1D * h_evntcuts = new TH1D(name.c_str(),"", ncuts + 1, 0., (double)(ncuts + 1));
    
    for(int i = 0; i < ncuts + 1; i++){
        TH1D * h_tmp = new TH1D("h_tmp","", 1, 0., 1.);
        std::string loop_cuts = Form("%s %d", tmp_cuts.c_str(), (i-1));
        if(m_debug) cout << "Loop cut: " << loop_cuts << endl;
        intree->Project("h_tmp","0.5", loop_cuts.c_str());
        double intergal = (double)h_tmp->Integral();
        h_evntcuts->SetBinContent(i+1, intergal);
        if(m_debug) cout << "Cut " << i << ": Events after cut " << intergal << endl;
        delete h_tmp;
    }
    return h_evntcuts;
}

TH1D * EffPurTools::DrawRatioVSCuts(TH1D * num, TH1D * den, std::string y_title, std::string h_name){
    if(m_debug) cout << "EffPurTools::DrawRatioVSCuts" << endl;
    TH1D * ratio = new TH1D(h_name.c_str(), (";;" + y_title).c_str(),den->GetNbinsX(),den->GetXaxis()->GetXmin(),den->GetXaxis()->GetXmax());
    ratio->GetYaxis()->SetRangeUser(0.,110.);
    ratio->Divide(num, den);
    ratio->Scale(100);
    
    if(m_debug){
        for(int i = 0; i < ratio->GetNbinsX(); i++){
            cout << "ratio->GetBinContent("<< i + 1 <<") = " << ratio->GetBinContent(i+1) << " +/- " << ratio->GetBinError(i+1) << endl;
        }
    }
    
    if(!((int)m_cutnames.size() > 0)){
        for(int i = 0; i < ratio->GetNbinsX(); i++){
            ratio->GetXaxis()->SetBinLabel(i+1, Form("%d.",(i+1)));
        }
        ratio->GetXaxis()->SetTitle("Cuts");
        return ratio;
    }
    
    int max_bins = ratio->GetNbinsX();
    //Stop trying to run over array size:
    if(max_bins > (int)(m_cutnames.size() + 1)){
        max_bins = (int)m_cutnames.size();
        if(m_debug)  cout << "Not all cuts have names. Running up to " << max_bins << endl;
    }
    else if(m_debug) cout << "Writing label names" << endl;
    
    //cout << "Size of m_cutnames array " << m_cutnames.size() << endl;
    
    for(int i = 0; i < ratio->GetNbinsX(); i++){
        std::string tmp_label;
        
        if(i == 0){
            tmp_label = "No Cuts";
        }
        else{
            if(max_bins > i ) tmp_label = Form("%d) %s", i, m_cutnames[i-1].c_str());
            else tmp_label = Form("%d)", i);
        }
        
        if(m_debug) cout << tmp_label << endl;
        ratio->GetXaxis()->SetBinLabel(i+1, tmp_label.c_str());
    }
    
    return ratio;
}

TH1D * EffPurTools::GetHisto(TTree * intree, std::string var, int nbins, Double_t * xbins, std::string cuts){
    Counter++;
    
    std::string host_name = Form("1Dvar%.6d", Counter);
    
    TH1D * hist = new TH1D(host_name.c_str(), "", nbins, xbins);
    
    intree->Project(host_name.c_str(), var.c_str(), cuts.c_str());
    
    if(m_debug) cout << "Histo: " << hist->GetName() << " : Entries = " << hist->Integral() << endl;
    
    return hist;
}

TH2D * EffPurTools::GetHisto(TTree * intree, std::string var_yx, int x_nbins, Double_t * xbins, int y_nbins, Double_t * ybins, std::string cuts){
    Counter++;
    std::string host_name = Form("2Dvar%.6d", Counter);
    
    TH2D * hist = new TH2D(host_name.c_str(), "", x_nbins, xbins, y_nbins, ybins);
    
    intree->Project(host_name.c_str(), var_yx.c_str(), cuts.c_str());
    
    if(m_debug) cout << "Histo: " << hist->GetName() << " : Entries = " << hist->Integral() << endl;
    
    return hist;
}

//-------------- Set cut names of x-axis: --------------//
void EffPurTools::SetCutName(std::string var){
    m_cutnames.push_back(var);
}

void EffPurTools::ResetCutNames(){
    m_cutnames.clear();
}

void EffPurTools::SetCutNames(std::vector<std::string> var){
    ResetCutNames();
    for(int i=0; i < (int)var.size(); i++){
        SetCutName(var[i]);
    }
}
//------------------------------------------------------//

TH1D * EffPurTools::RatioVSVar(TTree * intree, std::string var, int nbins, Double_t * xbins, std::string common_cut, std::string num_only_cut, std::string x_title){
    
    if(m_debug){
        cout << "          Common Cut: " << common_cut << endl;
        cout << "    Cut to Numerator: " << num_only_cut << endl;
    }
    
    std::string num_cut = common_cut;
    
    if(!num_only_cut.empty()){
        num_cut += " && ";
        num_cut += num_only_cut;
        if(m_debug) cout << "    Numerator only cut(s): " << num_cut << endl;
    }
    else if(m_debug) cout << "    Numerator only cut(s): None" << endl;
    
    if(m_debug) cout << "Producing den/num histograms" << endl;
    TH1D * num = GetHisto(intree, var, nbins, xbins, num_cut);
    TH1D * den = GetHisto(intree, var, nbins, xbins, common_cut);

    Counter++;
    TH1D * ratio = new TH1D(Form("ratio_1D_%.6d", Counter), (";" + x_title + ";Ratio").c_str(), nbins, xbins);
    ratio->GetYaxis()->SetRangeUser(0., 110.);
    ratio->Divide(num, den);
    
    if(m_debug) cout << "Finished making ratio vs variable" << endl;

    if(m_debug){
        for(int i = 1; i < ratio->GetNbinsX() + 1; i++){
            cout << "Bin: " << i << "/" << ratio->GetNbinsX() << ": Ratio = " << ratio->GetBinContent(i);
            cout << " Num = " << num->GetBinContent(i) << " Den = " << den->GetBinContent(i) << endl;
        }
    }
    
    ratio->Scale(100);

    delete num;
    delete den;
    
    return ratio;
}

TH2D * EffPurTools::RatioVSVar(TTree * intree, std::string var_yx, int x_nbins, Double_t * x_bins, int y_nbins, Double_t * y_bins, std::string common_cut, std::string num_only_cut, std::string xy_title){
    
    if(m_debug){
        cout << "          Common Cut: " << common_cut << endl;
        cout << "    Cut to Numerator: " << num_only_cut << endl;
    }
    
    std::string num_cut = common_cut;

    if(!num_only_cut.empty()){
        num_cut += " && ";
        num_cut += num_only_cut;
        if(m_debug) cout << "    Numerator only cut(s): " << num_cut << endl;
    }
    else if(m_debug) cout << "    Numerator only cut(s): None" << endl;
    
    if(m_debug) cout << "Producing den/num histograms" << endl;
    TH2D * num = GetHisto(intree, var_yx, x_nbins, x_bins, y_nbins, y_bins, num_cut);
    TH2D * den = GetHisto(intree, var_yx, x_nbins, x_bins, y_nbins, y_bins, common_cut);
    
    if(m_debug) cout << "Dividing histograms" << endl;
    Counter++;
    TH2D * ratio = new TH2D(Form("ratio_2D_%.6d", Counter), (";" + xy_title).c_str(), x_nbins, x_bins, y_nbins, y_bins);
    ratio->Divide(num, den);
    if(m_debug) cout << "Finished making ratio vs variable" << endl;
    
    delete num;
    delete den;
    
    return ratio;
}

int EffPurTools::GetNCuts()
{
    // cout << "EffPurTools::GetNCuts() m_ncuts = " << m_ncuts << endl;

    if(m_ncuts == -999){
        // cout << "EffPurTools::GetNCuts() = " << m_ncuts << endl;
        assert( m_truth->GetEntries() > 1 );
        m_truth->GetEntry(0);
        TLeaf * lused = m_truth->GetLeaf("truth_ncuts");
        int ncuts = 0;
        if(lused) ncuts = lused->GetValue();
        else {
            ncuts = 0;
            TBranch * tmp_br = m_recon->GetBranch( Form("cut%d", ncuts) );
            while( tmp_br != 0x0 ){
                ncuts++;
                tmp_br = m_recon->GetBranch( Form("cut%d", ncuts) );
            }
        }
        m_ncuts = ncuts; // This will make the code run faster.
        // cout << "EffPurTools::GetNCuts() = " << ncuts << endl;
        return m_ncuts;
    }
    else return m_ncuts;
}


TH1D * EffPurTools::EffVSN1Cuts(std::string signal, int branch, std::string cuts)
{
// Want to do ncuts - 1: 
    // For each cut turn all other cuts on
    string joint_cut = signal;
    if(!joint_cut.empty() && !cuts.empty()) joint_cut += " && ";
    joint_cut += cuts;

    TH1D * histo = new TH1D(Form("effN1cuts%.6d", Counter++), "", GetNCuts() + 1, 0, GetNCuts() + 1);

    int max_bins = MaxCutsToDraw();

    TH1D * denom_h = new TH1D("denom_h","", 1, 0., 1.);

    m_truth->Project("denom_h", "0.5", joint_cut.c_str());

    double denom = (double)denom_h->Integral();
    delete denom_h;

    if(m_debug) cout << " denom = " << denom << endl;

    for(int ignore = 0; ignore < GetNCuts() + 1; ignore++){

        if(ignore < max_bins){
            histo->GetXaxis()->SetBinLabel(ignore + 1, m_cutnames[ignore].c_str());
        }
        else histo->GetXaxis()->SetBinLabel(ignore + 1, Form("Cut %d", ignore));

        double ratio = 0.;
        if(ignore == GetNCuts()){
            histo->GetXaxis()->SetBinLabel(ignore + 1, "All Cuts Passed");   
            ratio = GetCutEntries(-1, joint_cut, branch);
        }
        else ratio = GetCutEntries(ignore, joint_cut, branch);
        if(m_debug) cout << "cut" << ignore << " : R = " << ratio << "/" << denom << endl; 
        ratio *= 1./denom;
        if(m_debug) cout << "EffPurTools::EffVSN1Cuts : cut" << ignore << " : R = " << ratio << endl;

        histo->SetBinContent(ignore + 1, ratio);
    }
    histo->Scale(100.00);
    histo->GetYaxis()->SetRangeUser(0,110.);
    histo->SetLineColor(Blue);
    histo->GetYaxis()->SetTitle("Efficiency");
    return histo;
}

TH1D * EffPurTools::PurVSN1Cuts(std::string signal, int branch, std::string cuts)
{
    // Want to do ncuts - 1: 
    // For each cut turn all other cuts on
    string numer_cut = signal;
    if(!numer_cut.empty() && !cuts.empty()) numer_cut += " && ";
    numer_cut += cuts;

    TH1D * histo = new TH1D(Form("purN1cuts%.6d", Counter++), "", GetNCuts() + 1, 0, GetNCuts() + 1);

    int max_bins = MaxCutsToDraw();

    for(int ignore = 0; ignore < GetNCuts() + 1; ignore++){

        if(ignore < max_bins){
            histo->GetXaxis()->SetBinLabel(ignore + 1, m_cutnames[ignore].c_str());
        }
        else histo->GetXaxis()->SetBinLabel(ignore + 1, Form("Cut %d", ignore));

        double ratio = 0.;
        if(ignore == GetNCuts()){
            histo->GetXaxis()->SetBinLabel(ignore + 1, "All Cuts Passed");   
            ratio = GetCutEntries(-1, numer_cut, branch)/GetCutEntries(-1, cuts, branch);
        }
        else ratio = GetCutEntries(ignore, numer_cut, branch)/GetCutEntries(ignore, cuts, branch);
        if(m_debug) cout << "EffPurTools::PurVSN1Cuts : cut" << ignore << " : R = " << ratio << endl;
        histo->SetBinContent(ignore + 1, ratio);
    }
    histo->Scale(100.00);
    histo->GetYaxis()->SetRangeUser(0,110.);
    histo->SetLineColor(Yellow);
    histo->GetYaxis()->SetTitle("Purity");
    return histo;
}

double EffPurTools::GetCutEntries(int ignore, std::string cuts, int branch)
{
    //For T2K this info is save in the recon tree.
    string tmp_cuts = cuts;
    for(int i = 0; i < GetNCuts(); i++){
        if(i == ignore) continue;
        if(!tmp_cuts.empty()) tmp_cuts += " && ";
        tmp_cuts += Form("cut%d[%d] == 1", i, branch);
    }
    // cout << __FILE__ << " : " << __LINE__ << endl;//<--- This is a sweet means of getting the file name and line number.
    if(m_debug) cout << " cut" << ignore << " Cut = " << tmp_cuts << endl;
    TH1D * histo = new TH1D(Form("cut%dh", ignore), "", 1, 0, 1);
    m_recon->Project( Form("cut%dh", ignore), "0.5", tmp_cuts.c_str() );
    double integral = (double)histo->Integral();
    delete histo;
    return integral;
}

int EffPurTools::MaxCutsToDraw(){
    int max_bins = GetNCuts(); 
    if(max_bins > (int)m_cutnames.size()){
        max_bins = (int)m_cutnames.size();
    }
    else if(max_bins != (int)m_cutnames.size()){
        cout << __FILE__ << ":" << __LINE__ << " : " << "WARNING : Number of cuts (" << GetNCuts() << ") is less than dimension (" << (int)m_cutnames.size() << ") of cut name list." << endl;
    }
    return max_bins;
}

#endif