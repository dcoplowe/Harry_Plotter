#include "EffPurTools.h"

//ROOT Includes:

//Forward declarations;
#include "TFile.h"
#include "TH1I.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"

EffPurTools::EffPurTools(std::string filename, std::string reconame, std::string truename) : m_debug(true) {
    
    cout << "    Filename: " << filename << endl;
    cout << "Truth branch: " << truename << endl;
    cout << "Recon branch: " << reconame << endl;
    
    //Include counter to make sure hists have unique names:
    m_purhcounter = -1;
    m_effhcounter = -1;
    m_ghcounter1D = -1;
    m_ghcounter2D = -1;
    m_effvarcounter = -1;
    m_purvarcounter = -1;

    
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
}

EffPurTools::EffPurTools(std::string filename, std::vector<std::string> cut_names, std::string reconame, std::string truename) {
    EffPurTools(filename, reconame, truename);
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
    
    
    cout << "Is tree working :: " << endl;
    if(m_debug) cout << "Read tree " << m_truth->GetName() << endl;
    cout << "Is tree working :: " << endl;
    
    TH1I * h_ncuts = new TH1I("h_ncuts", "",10, 0, 10);
    
    TString ncuts_name = "truth_ncuts";
    m_truth->Project("h_ncuts", "truth_ncuts");//Draw(ncuts_name + ">> h_ncuts");
    
    if(m_debug) cout << "Found and Filled ncuts histogram " << endl;
    
    int ncuts = (int)h_ncuts->GetBinCenter(h_ncuts->GetMaximumBin());
    
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

    m_effhcounter++;
    TH1D * effcuts = DrawRatioVSCuts(num, den, "Efficiency", Form("h_effic%.3d", m_effhcounter));
    effcuts->SetLineColor(Blue);
    
    delete num;
    delete den;
    delete h_ncuts;
    
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
    
    TH1I * h_ncuts = new TH1I("h_ncuts", "",10, 0, 10);
    m_recon->Project("h_ncuts","ncuts"); //->Draw("ncuts>> h_ncuts");
    int ncuts = (int)h_ncuts->GetBinCenter(h_ncuts->GetMaximumBin());
    
    if(m_debug) cout << "Number of cuts found to be " << ncuts << endl;
    
    TH1D * num = EventsVSCuts(m_recon, full_signal, branch, ncuts, "pur_num");
    TH1D * den = EventsVSCuts(m_recon, cuts, branch, ncuts, "pur_den");
    
    if(m_debug){
        for(int i = 0; i < num->GetNbinsX(); i++ ){
            cout << "num->GetBinContent("<<i+1<<") = " << num->GetBinContent(i+1) << " +/- " << num->GetBinError(i+1) << endl;
            cout << "den->GetBinContent("<<i+1<<") = " << den->GetBinContent(i+1) << " +/- " << den->GetBinError(i+1) << endl;
        }
    }
    
    m_purhcounter++;
    TH1D * purcuts = DrawRatioVSCuts(num, den, "Purity", Form("h_purity%.3d", m_purhcounter));
    
    delete num;
    delete den;
    
    return purcuts;
}

TH1D * EffPurTools::EffVSVar(std::string var, int nbins, Double_t * xbins, std::string signal, std::string cuts, std::string x_title){
    if(m_debug) cout << "EffPurTools::EffVSVar()" << endl;
    
    TH1D * effvar = RatioVSVar(m_truth, var, nbins, xbins, signal, cuts, x_title);
    m_effvarcounter++;
    effvar->SetName(Form("effvar%.3d", m_effvarcounter));
    effvar->GetYaxis()->SetTitle("Efficiency");
    return effvar;
}

TH1D * EffPurTools::EffVSVar(std::string var, int nbins, Double_t x_low, Double_t x_high, std::string signal, std::string cuts, std::string x_title){
    if(m_debug) cout << "EffPurTools::EffVSVar(const TString var, int nbins, const Double_t x_low, const Double_t x_high, const TString signal, const TString cuts, const TString x_title)" << endl;
    return EffVSVar(var, nbins, EvenArray(nbins, x_low, x_high), signal, cuts, x_title);
}

TH2D * EffPurTools::EffVSVar(std::string var_yx, int x_nbins, Double_t * x_bins, int y_nbins, Double_t * y_bins, std::string signal, std::string cuts, std::string xy_title){
    if(m_debug) cout << "EffPurTools::EffVSVar()" << endl;
    
    TH2D * effvar = RatioVSVar(m_truth, var_yx, x_nbins, x_bins, y_nbins, y_bins, signal, cuts, xy_title);
    m_effvarcounter++;
    effvar->SetName(Form("effvar%.3d", m_effvarcounter));
    //effvar->Set
    return effvar;
}

TH2D * EffPurTools::EffVSVar(std::string var_yx, int x_nbins, Double_t x_low, Double_t x_high, int y_nbins, Double_t y_low, Double_t y_high, std::string signal, std::string cuts, std::string xy_title){
    return EffVSVar(var_yx, x_nbins, EvenArray(x_nbins, x_low, x_high), y_nbins, EvenArray(y_nbins, y_low, y_high), signal, cuts, xy_title);
}

TH1D * EffPurTools::PurVSVar(std::string var, int nbins, Double_t * xbins, std::string signal, std::string  x_title, std::string cuts){
    if(m_debug) cout << "EffPurTools::PurVSVar()" << endl;
    //TTree * intree = (TTree*)_file->Get(_reconame.Data());
    
    TH1D * purvar = RatioVSVar(m_recon, var, nbins, xbins, cuts, signal, x_title);
    m_purvarcounter++;
    purvar->SetName(Form("purvar%.3d", m_purvarcounter));
    purvar->GetYaxis()->SetTitle("Purity");
    return purvar;
}

TH1D * EffPurTools::PurVSVar(std::string var, int nbins, Double_t x_low, Double_t x_high, std::string signal, std::string x_title, std::string cuts){
    if(m_debug) cout << "EffPurTools::PurVSVar()" << endl;
    return PurVSVar(var, nbins, EvenArray(nbins, x_low, x_high), signal, x_title, cuts);
}

TH2D * EffPurTools::PurVSVar(std::string var_yx, int x_nbins, Double_t * x_bins, int y_nbins, Double_t * y_bins, std::string signal, std::string  xy_title, std::string cuts){
    if(m_debug) cout << "EffPurTools::PurVSVar()" << endl;
    //TTree * intree = (TTree*)_file->Get(_reconame.Data());
    
    TH2D * purvar = RatioVSVar(m_recon, var_yx, x_nbins, x_bins, y_nbins, y_bins, cuts, signal, xy_title);
    m_purvarcounter++;
    purvar->SetName(Form("purvar%.3d", m_purvarcounter));
//    purvar->GetYaxis()->SetTitle("Purity");
    return purvar;
}

TH2D * EffPurTools::PurVSVar(std::string var_yx, int x_nbins, Double_t x_low, Double_t x_high, int y_nbins, Double_t y_low, Double_t y_high, std::string signal, std::string  xy_title, std::string cuts){
    if(m_debug) cout << "EffPurTools::PurVSVar()" << endl;
    return PurVSVar(var_yx, x_nbins, EvenArray(x_nbins, x_low, x_high), y_nbins, EvenArray(y_nbins, y_low, y_high), signal, xy_title, cuts);
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
    ratio->Divide(num, den);
    ratio->GetYaxis()->SetRangeUser(0,1.1);
    
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
    m_ghcounter1D++;
    
    std::string host_name = Form("1Dvar%.3d", m_ghcounter1D);
    
    TH1D * hist = new TH1D(host_name.c_str(), "", nbins, xbins);
    
    intree->Project(host_name.c_str(), var.c_str(), cuts.c_str());
    
    if(m_debug) cout << "Histo: " << hist->GetName() << " : Entries = " << hist->Integral() << endl;
    
    return hist;
}

TH2D * EffPurTools::GetHisto(TTree * intree, std::string var_yx, int x_nbins, Double_t * xbins, int y_nbins, Double_t * ybins, std::string cuts){
    m_ghcounter2D++;
    std::string host_name = Form("2Dvar%.3d", m_ghcounter2D);
    
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
        TString tmp_string = var[i];
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
    
    TH1D * num = GetHisto(intree, var, nbins, xbins, num_cut);
    TH1D * den = GetHisto(intree, var, nbins, xbins, common_cut);
    
    m_ghcounter1D++;
    TH1D * ratio = new TH1D(Form("ratio_1D_%.3d", m_ghcounter1D), (";" + x_title + ";Ratio").c_str(), nbins, xbins);
    ratio->Divide(num, den);
    
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
    
    TH2D * num = GetHisto(intree, var_yx, x_nbins, x_bins, y_nbins, y_bins, num_cut);
    TH2D * den = GetHisto(intree, var_yx, x_nbins, x_bins, y_nbins, y_bins, common_cut);
    
    m_ghcounter2D++;
    TH2D * ratio = new TH2D(Form("ratio_2D_%.3d", m_ghcounter2D), (";" + xy_title).c_str(), x_nbins, x_bins, y_nbins, y_bins);
    ratio->Divide(num, den);
    
    delete num;
    delete den;
    
    return ratio;
}

Double_t * EffPurTools::EvenArray(int nbins, Double_t x_low, Double_t x_high){
    Double_t * xbins = new Double_t[ nbins + 1 ];
    
    Double_t range = x_high - x_low;
    Double_t binwidth = range/(Double_t)nbins;
    
    for (int i=0; i < nbins + 1; i++) {
        xbins[i] = x_low + binwidth*i;
        //cout << "Array Element: " << i << " : " << xbins[i] << endl;
    }
    return xbins;
}

