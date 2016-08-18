#include "EffPurTools.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>

//ROOT Includes:
#include "TTree.h"


//Forward declarations;
#include "TFile.h"
#include "TH1I.h"

using namespace std;


EffPurTools::EffPurTools(TString filename, std::vector<TString> cut_names, TString reconame, TString truename){
    
    cout << "EffPurTools::EffPurTools(TString, TString, TString)" << endl;
    SetFileName(filename);
    SetRecoBranch(reconame);
    SetTrueBranch(truename);
    SetFile();
    SetCutNames(cut_names);
    
    cout << "    Filename: " << _filename.Data() << endl;
    cout << "Truth branch: " << _truename.Data() << endl;
    cout << "Recon branch: " << _reconame.Data() << endl;
}

EffPurTools::EffPurTools(TString filename, TString reconame, TString truename){
    
    cout << "EffPurTools::EffPurTools(TString, TString, TString)" << endl;
    SetFileName(filename);
    SetRecoBranch(reconame);
    SetTrueBranch(truename);
    SetFile();
    
    cout << "    Filename: " << _filename.Data() << endl;
    cout << "Truth branch: " << _truename.Data() << endl;
    cout << "Recon branch: " << _reconame.Data() << endl;
}

EffPurTools::EffPurTools() {
    cout << "EffPurTools::EffPurTools()" << endl;
    EffPurTools("UNKNOWN");
    //SetRecoBranch("CC1P1Pi");
    //SetTrueBranch("Truth");
}

//These may not be void functions:
MnvH1D * EffPurTools::EffVSCuts(const TString signal, const TString cuts){
    cout << "EffPurTools::EffVSCuts(TString, TString)" << endl;
    
    if(signal.EqualTo("",TString::kExact)){
        cout << "No signal defined -- Need for efficiency calculations" << endl;
        return 0x0;
    }
    else cout << "    Signal: " << signal.Data() << endl;

    TString full_signal = signal;
    
    if(!cuts.EqualTo("",TString::kExact)){
        full_signal.Append(" && ");
        full_signal.Append(cuts);
        cout << "    Cut(s): " << cuts.Data() << endl;
    }
    else cout << "    Cut(s): None" << endl;
    
    TTree * intree = (TTree*)_file->Get(_truename);
    
    TH1I * h_ncuts = new TH1I("h_ncuts", "",10, 0, 10);
    
    TString ncuts_name = "ncuts";
    intree->Draw(ncuts_name + ">> h_ncuts");
    int ncuts = (int)h_ncuts->GetBinCenter(h_ncuts->GetMaximumBin());
    
    cout << "Number of cuts found to be " << ncuts << endl;
    
    MnvH1D * num = EventsVSCuts(intree, full_signal, ncuts);
    MnvH1D * den = new MnvH1D("den", "", ncuts, 0., (double)ncuts);

    for(int i = 0; i < num->GetNbinsX(); i++){
        den->SetBinContent(i+1,num->GetBinContent(1));
        den->SetBinError(i+1,num->GetBinError(1));
    }
    
    MnvH1D * effcuts = DrawRatioVSCuts(num, den, "Efficiency", "h_effic");
    
    delete num;
    delete den;
    
    return effcuts;
}

void EffPurTools::EffVSVar(TString var, const TString signal, const TString cuts){
    cout << "EffPurTools::EffVSVar(TString, TString, TString)" << endl;
    cout << "    Signal: " << signal.Data() << endl;
    cout << "    Cut(s): " << cuts.Data() << endl;
}

MnvH1D * EffPurTools::PurVSCuts(TString signal, TString cuts){
    cout << "EffPurTools::PurVSCuts(TString, TString)" << endl;
    
    if(signal.EqualTo("",TString::kExact)){
        cout << "No signal defined -- Need for purity calculations" << endl;
        return 0x0;
    }
    else cout << "    Signal: " << signal.Data() << endl;
    
    TString full_signal = signal;
    
    if(!cuts.EqualTo("",TString::kExact)){
        full_signal.Append(" && ");
        full_signal.Append(cuts);
        cout << "    Cut(s): " << cuts.Data() << endl;
    }
    else cout << "    Cut(s): None" << endl;
    
    TTree * intree = (TTree*)_file->Get(_reconame);
    
    TH1I * h_ncuts = new TH1I("h_ncuts", "",10, 0, 10);
    
    TString ncuts_name = "ncuts";
    intree->Draw(ncuts_name + ">> h_ncuts");
    int ncuts = (int)h_ncuts->GetBinCenter(h_ncuts->GetMaximumBin());
    
    MnvH1D * num = EventsVSCuts(intree, full_signal, ncuts);
    MnvH1D * den = EventsVSCuts(intree, cuts, ncuts);
    
    MnvH1D * purcuts = DrawRatioVSCuts(num, den, "Purity", "h_purity");

    delete num;
    delete den;
    
    return purcuts;
}

void EffPurTools::SetFile(){
    _file = new TFile(_filename, "RECREATE");
}

MnvH1D * EffPurTools::EventsVSCuts(TTree * intree, const TString cuts, const int ncuts, TString name){
    
    TString tmp_cuts = "accum_level >";
    TString tree_name = intree->GetName();
    
    if(tree_name.EqualTo("Truth",TString::kExact)){
        tmp_cuts.Prepend("truth");
    }
    
    if(!cuts.EqualTo("", TString::kExact)){
        tmp_cuts.Prepend(" && ");
        tmp_cuts.Prepend(cuts);
    }
    
    //This will complain: delete outside of function.
    MnvH1D * h_evntcuts = new MnvH1D(name.Data(),"", ncuts, 0., (double)ncuts);
    
    for(int i = 0; i < ncuts; i++){
        MnvH1D * h_tmp = new MnvH1D("h_tmp","", 1, 0., 1.);
        TString tmp_cuts = Form("%s %d", tmp_cuts.Data(), (i - 1));
        intree->Project("h_tmp","0.5",tmp_cuts);
        double intergal = (double)h_tmp->Integral();
        h_evntcuts->SetBinContent(i+1, intergal);
        cout << "Cut " << (i+1) << "Events after cut " << intergal << endl;
        delete h_tmp;
    }
    return h_evntcuts;
}

MnvH1D * EffPurTools::DrawRatioVSCuts(MnvH1D * num, MnvH1D * den, TString y_title, TString h_name){
    
    MnvH1D * ratio = new MnvH1D(Form("%s",h_name.Data()),Form("; ;%s", y_title.Data()),den->GetNbinsX(),den->GetXaxis()->GetXmin(),den->GetXaxis()->GetXmax());
    ratio->Divide(num, den);
    ratio->GetYaxis()->SetRangeUser(0,1.1);
    
    if(!((int)_cutnames.size() > 0)) return ratio;
    
    int max_bins = ratio->GetNbinsX();
    
    //Stop trying to run over array size:
    if(max_bins > (int)_cutnames.size()){
        max_bins = (int)_cutnames.size();
        cout << "Not all cuts have names. Running up to " << max_bins << endl;
    }
    
    for(int i = 0; i < max_bins; i++){
        TString tmp_label = _cutnames[i];
        ratio->GetXaxis()->SetBinLabel(i+1, tmp_label.Data());
    }

    return ratio;
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


