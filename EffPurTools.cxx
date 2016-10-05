#include "EffPurTools.h"

//ROOT Includes:

//Forward declarations;
#include "TFile.h"
#include "TH1I.h"
#include "TH1D.h"
#include "TTree.h"

using namespace std;

EffPurTools::EffPurTools(TString filename, bool debug, TString reconame, TString truename) {
    
    SetFileName(filename);
    SetRecoBranch(reconame);
    SetTrueBranch(truename);
    SetFile();
    
    cout << "    Filename: " << _filename.Data() << endl;
    cout << "Truth branch: " << _truename.Data() << endl;
    cout << "Recon branch: " << _reconame.Data() << endl;
    
    //Include counter to make sure hists have unique names:
    _purhcounter = -1;
    _effhcounter = -1;
    _ghcounter = -1;
    _effvarcounter = -1;
    _purvarcounter = -1;
    
    _DEBUG_ = debug;
    
    if(_DEBUG_) cout << "EffPurTools::EffPurTools(TString filename, TString reconame, TString truename)" << endl;
}

EffPurTools::EffPurTools(TString filename, std::vector<TString> cut_names, bool debug, TString reconame, TString truename){
    
    //EffPurTools(filename, reconame, truename);
    SetCutNames(cut_names);
    SetFileName(filename);
    SetRecoBranch(reconame);
    SetTrueBranch(truename);
    SetFile();
    
    _purhcounter = -1;
    _effhcounter = -1;
    _ghcounter = -1;
    _effvarcounter = -1;
    _purvarcounter = -1;
    
    _DEBUG_ = debug;
    
    if(_DEBUG_) cout << "EffPurTools::EffPurTools(TString filename, std::vector<TString> cut_names, TString reconame, TString truename)" << endl;

    cout << "    Filename: " << _filename.Data() << endl;
    cout << "Truth branch: " << _truename.Data() << endl;
    cout << "Recon branch: " << _reconame.Data() << endl;
}

//This initialisation probably wont work:
EffPurTools::EffPurTools() {
    if(_DEBUG_) cout << "EffPurTools::EffPurTools()" << endl;
    EffPurTools("UNKNOWN");
    //SetRecoBranch("CC1P1Pi");
    //SetTrueBranch("Truth");
}

//These may not be void functions:
TH1D * EffPurTools::EffVSCuts(const TString signal, int branch, const TString cuts){
    if(_DEBUG_) cout << "EffPurTools::EffVSCuts(TString, TString)" << endl;
    
    if(signal.EqualTo("",TString::kExact)){
        cout << "No signal defined -- Need for efficiency calculations" << endl;
        return 0x0;
    }
    else if(_DEBUG_) cout << "    Signal: " << signal.Data() << endl;
    
    TString full_signal = signal;
    
    if(!cuts.EqualTo("",TString::kExact)){
        full_signal.Append(" && ");
        full_signal.Append(cuts);
        if(_DEBUG_) cout << "    Cut(s): " << cuts.Data() << endl;
    }
    else if(_DEBUG_)cout << "    Cut(s): None" << endl;
    
    if(_DEBUG_) cout << "Starting to read tree " << endl;
    
    TTree * intree = (TTree*)_file->Get(_truename.Data());
    
    if(_DEBUG_) cout << "Read tree " << intree->GetName() << endl;
    
    TH1I * h_ncuts = new TH1I("h_ncuts", "",10, 0, 10);
    
    TString ncuts_name = "truth_ncuts";
    intree->Draw(ncuts_name + ">> h_ncuts");
    
    if(_DEBUG_) cout << "Found and Filled ncuts histogram " << endl;
    
    int ncuts = (int)h_ncuts->GetBinCenter(h_ncuts->GetMaximumBin());
    
    if(_DEBUG_) cout << "Number of cuts found to be " << ncuts << endl;
    
    TH1D * num = EventsVSCuts(intree, full_signal, branch, ncuts);
    TH1D * den = new TH1D("den", "", num->GetNbinsX(), 0., (double)num->GetNbinsX());

    for(int i = 0; i < num->GetNbinsX(); i++){
        den->SetBinContent(i+1,num->GetBinContent(1));
        den->SetBinError(i+1,num->GetBinError(1));
    }
    
    _effhcounter++;
    TH1D * effcuts = DrawRatioVSCuts(num, den, "Efficiency", Form("h_effic%.3d", _effhcounter));
    effcuts->SetLineColor(DSBlue);
    
    delete num;
    delete den;
    delete h_ncuts;
    
    return effcuts;
}

TH1D * EffPurTools::EffVSVar(const TString var, int nbins, const Double_t * xbins, const TString signal, const TString x_title, const TString cuts){
    if(_DEBUG_) cout << "EffPurTools::EffVSVar()" << endl;
    TTree * intree = (TTree*)_file->Get(_truename.Data());
    TH1D * effvar = RatioVSVar(intree, var, nbins, xbins, signal, x_title, cuts);
    _effvarcounter++;
    effvar->SetName(Form("effvar%.3d", _effvarcounter));
    effvar->GetYaxis()->SetTitle("Efficiency");
    return effvar;
}

TH1D * EffPurTools::EffVSVar(const TString var, int nbins, const Double_t x_low, const Double_t x_high, const TString signal, const TString x_title, const TString cuts){
    if(_DEBUG_) cout << "EffPurTools::EffVSVar(TString, TString, TString)" << endl;
    TTree * intree = (TTree*)_file->Get(_truename.Data());
    TH1D * effvar = RatioVSVar(intree, var, nbins, x_low, x_high, signal, x_title, cuts);
    _effvarcounter++;
    effvar->SetName(Form("effvar%.3d", _effvarcounter));
    effvar->GetYaxis()->SetTitle("Efficiency");
    return effvar;
}

TH1D * EffPurTools::PurVSCuts(const TString signal, int branch, const TString cuts){
    if(_DEBUG_) cout << "EffPurTools::PurVSCuts(TString, TString)" << endl;
    
    if(signal.EqualTo("",TString::kExact)){
        cout << "No signal defined -- Need for purity calculations" << endl;
        return 0x0;
    }
    else if(_DEBUG_) cout << "    Signal: " << signal.Data() << endl;
    
    TString full_signal = signal;
    
    if(!cuts.EqualTo("",TString::kExact)){
        full_signal.Append(" && ");
        full_signal.Append(cuts);
        if(_DEBUG_) cout << "    Cut(s): " << cuts.Data() << endl;
    }
    else if(_DEBUG_) cout << "    Cut(s): None" << endl;
    
    if(_DEBUG_) cout << "Starting to read tree " << endl;
    
    TTree * intree = (TTree*)_file->Get(_reconame.Data());
    
    if(_DEBUG_) cout << "Opened Tree " << endl;
    
    if(!intree) return 0x0;
    
    if(_DEBUG_) cout << "Read tree " << intree->GetName() << endl;

    TH1I * h_ncuts = new TH1I("h_ncuts", "",10, 0, 10);
    intree->Draw("ncuts>> h_ncuts");
    int ncuts = (int)h_ncuts->GetBinCenter(h_ncuts->GetMaximumBin());
    
    if(_DEBUG_) cout << "Number of cuts found to be " << ncuts << endl;

    TH1D * num = EventsVSCuts(intree, full_signal, branch, ncuts, "pur_num");
    TH1D * den = EventsVSCuts(intree, cuts, branch, ncuts, "pur_den");
    
    _purhcounter++;
    TH1D * purcuts = DrawRatioVSCuts(num, den, "Purity", Form("h_purity%.3d", _purhcounter));

    delete num;
    delete den;
    
    return purcuts;
}


TH1D * EffPurTools::PurVSVar(const TString var, int nbins, const Double_t * xbins, const TString signal, const TString x_title, const TString cuts){
    if(_DEBUG_) cout << "EffPurTools::PurVSVar()" << endl;
    TTree * intree = (TTree*)_file->Get(_reconame.Data());
    TH1D * purvar = RatioVSVar(intree, var, nbins, xbins, signal, x_title, cuts);
    _purvarcounter++;
    purvar->SetName(Form("effvar%.3d", _purvarcounter));
    purvar->GetYaxis()->SetTitle("Purity");
    return purvar;
}

TH1D * EffPurTools::PurVSVar(const TString var, int nbins, const Double_t x_low, const Double_t x_high, const TString signal, const TString x_title, const TString cuts){
    if(_DEBUG_) cout << "EffPurTools::PurVSVar()" << endl;
    TTree * intree = (TTree*)_file->Get(_reconame.Data());
    TH1D * purvar = RatioVSVar(intree, var, nbins, x_low, x_high, signal, x_title, cuts);
    _purvarcounter++;
    purvar->SetName(Form("effvar%.3d", _purvarcounter));
    purvar->GetYaxis()->SetTitle("Purity");
    return purvar;
}

void EffPurTools::SetFile(){
    if(_DEBUG_) cout << "EffPurTools::SetFile()" << endl;
    _file = new TFile(_filename, "READ");
    
    if(!_file) exit(0);
}

TH1D * EffPurTools::EventsVSCuts(TTree * intree, const TString cuts, int branch, const int ncuts, TString name){
    
    if(_DEBUG_) cout << "EffPurTools::EventsVSCuts" << endl;
    
    std::stringstream sbranch;
    sbranch << branch;
    
    TString tmp_cuts = "accum_level[" + sbranch.str() + "]>";
    TString tree_name = intree->GetName();
    
    if(_DEBUG_) cout << "Reading tree named " << tree_name.Data() << endl;
    
    if(tree_name.EqualTo("Truth",TString::kExact)){
        tmp_cuts.Prepend("truth_");
    }
    
    if(!cuts.EqualTo("", TString::kExact)){
        tmp_cuts.Prepend(" && ");
        tmp_cuts.Prepend(cuts);
    }
    
    if(_DEBUG_) cout << "Cut set as: " << tmp_cuts.Data() << endl;
    
    //This will complain: delete outside of function.
    TH1D * h_evntcuts = new TH1D(name.Data(),"", ncuts + 1, 0., (double)(ncuts + 1));
    
    for(int i = 0; i < ncuts + 1; i++){
        TH1D * h_tmp = new TH1D("h_tmp","", 1, 0., 1.);
        TString loop_cuts = Form("%s %d", tmp_cuts.Data(), (i-1));
        if(_DEBUG_) cout << "Loop cut: " << loop_cuts.Data() << endl;
        intree->Project("h_tmp","0.5",loop_cuts);
        double intergal = (double)h_tmp->Integral();
        h_evntcuts->SetBinContent(i+1, intergal);
        if(_DEBUG_) cout << "Cut " << i << ": Events after cut " << intergal << endl;
        delete h_tmp;
    }
    return h_evntcuts;
}

TH1D * EffPurTools::DrawRatioVSCuts(TH1D * num, TH1D * den, TString y_title, TString h_name){
    if(_DEBUG_) cout << "EffPurTools::DrawRatioVSCuts" << endl;
    TH1D * ratio = new TH1D(Form("%s",h_name.Data()),Form("; ;%s", y_title.Data()),den->GetNbinsX(),den->GetXaxis()->GetXmin(),den->GetXaxis()->GetXmax());
    ratio->Divide(num, den);
    ratio->GetYaxis()->SetRangeUser(0,1.1);
    
    if(!((int)_cutnames.size() > 0)){
        for(int i = 0; i < ratio->GetNbinsX(); i++){
            ratio->GetXaxis()->SetBinLabel(i+1, Form("%d.",(i+1)));
        }
        ratio->GetXaxis()->SetTitle("Cuts");
        return ratio;
    }
    
    int max_bins = ratio->GetNbinsX();
    //Stop trying to run over array size:
    if(max_bins > (int)(_cutnames.size() + 1)){
        max_bins = (int)_cutnames.size();
        if(_DEBUG_)  cout << "Not all cuts have names. Running up to " << max_bins << endl;
    }
    else if(_DEBUG_) cout << "Writing label names" << endl;
    
    //cout << "Size of _cutnames array " << _cutnames.size() << endl;
    
    for(int i = 0; i < ratio->GetNbinsX(); i++){
        TString tmp_label;
        
        if(i == 0){
            tmp_label.Form("No Cuts");
        }
        else{
            if(max_bins > i ) tmp_label.Form("%d) %s", i, _cutnames[i-1].Data());
            else tmp_label.Form("%d)", i);
        }
        
        if(_DEBUG_) cout << tmp_label.Data() << endl;
        ratio->GetXaxis()->SetBinLabel(i+1, tmp_label.Data());
    }
    
    return ratio;
}

TH1D * EffPurTools::GetHisto(TTree * intree, const TString var, int nbins, const double x_low, const double x_high, const TString cuts){
    
    Double_t * xbins = new Double_t[ nbins + 1 ];
    
    Double_t range = x_high - x_low;
    Double_t binwidth = range/(Double_t)nbins;
    
    for (int i=0; i < nbins + 1; i++) {
        xbins[i] = x_low + binwidth*i;
        //cout << "Array Element: " << i << " : " << xbins[i] << endl;
    }
    
    return GetHisto(intree, var, nbins, xbins, cuts);
    
}

TH1D * EffPurTools::GetHisto(TTree * intree, const TString var, int nbins, const Double_t * xbins, const TString cuts){
    _ghcounter++;
    
    TString host_name = Form("var%.3d", _ghcounter);
    
    TH1D * hist = new TH1D(host_name.Data(), "", nbins, xbins);
    
    TString tmp_cuts = cuts.Data();
    if(!tmp_cuts.EqualTo("", TString::kExact)) tmp_cuts.Append(" && ");
    
    tmp_cuts.Append(Form("(%s != -999)", var.Data()));
    
    if(_DEBUG_) cout << tmp_cuts.Data() << endl;
    
    intree->Project(host_name.Data(), var.Data(), tmp_cuts.Data());
    
    if(_DEBUG_) cout << "Histo: " << hist->GetName() << " : Entries = " << hist->Integral() << endl;
    
    return hist;
}

//-------------- Set cut names of x-axis: --------------//
void EffPurTools::SetCutName(TString var){
    _cutnames.push_back(var);
}

void EffPurTools::ResetCutNames(){
    _cutnames.clear();
}

void EffPurTools::SetCutNames(std::vector<TString> var){
    ResetCutNames();
    for(int i=0; i < (int)var.size(); i++){
        TString tmp_string = var[i];
        SetCutName(tmp_string);
    }
}
//------------------------------------------------------//

TH1D * EffPurTools::RatioVSVar(TTree * intree, const TString var, int nbins, const Double_t * xbins, const TString signal, const TString x_title, const TString cuts){
    
    if(_DEBUG_){
        cout << "    Signal: " << signal.Data() << endl;
        cout << "    Cut(s): " << cuts.Data() << endl;
    }
    
    TString full_signal = signal;
    
    if(!cuts.EqualTo("",TString::kExact)){
        full_signal.Append(" && ");
        full_signal.Append(cuts);
        if(_DEBUG_) cout << "    Cut(s): " << cuts.Data() << endl;
    }
    else if(_DEBUG_) cout << "    Cut(s): None" << endl;
    
    TH1D * num = GetHisto(intree, var, nbins, xbins, full_signal);
    TH1D * den = GetHisto(intree, var, nbins, xbins, cuts);
    
    TH1D * ratio = new TH1D(Form("ratio_%s", var.Data()), Form(";%s;Ratio",x_title.Data()), nbins, xbins);
    ratio->Divide(num, den);
    
    delete num;
    delete den;
    
    return ratio;
}

TH1D * EffPurTools::RatioVSVar(TTree * intree, const TString var, int nbins, const Double_t x_low, const Double_t x_high, const TString signal, const TString x_title, const TString cuts){
    
    if(_DEBUG_){
        cout << "    Signal: " << signal.Data() << endl;
        cout << "    Cut(s): " << cuts.Data() << endl;
    }
    
    TString full_signal = signal;
    
    if(!cuts.EqualTo("",TString::kExact)){
        full_signal.Append(" && ");
        full_signal.Append(cuts);
        if(_DEBUG_) cout << "    Cut(s): " << cuts.Data() << endl;
    }
    else if(_DEBUG_) cout << "    Cut(s): None" << endl;
    
    TH1D * num = GetHisto(intree, var, nbins, x_low, x_high, full_signal);
    TH1D * den = GetHisto(intree, var, nbins, x_low, x_high, cuts);
    
    TH1D * ratio = new TH1D(Form("ratio_%s", var.Data()), Form(";%s;Ratio",x_title.Data()), nbins, x_low, x_high);
    ratio->Divide(num, den);
    ratio->GetYaxis()->SetRangeUser(0., 1.1);
    
    delete num;
    delete den;
    
    return ratio;
}
