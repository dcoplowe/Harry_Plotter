#include "EffPurTools.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>

//ROOT Includes:
#include "TTree.h"


//Forward declarations;
#include "TFile.h"

using namespace std;


EffPurTools::EffPurTools(TString filename, TString truename, TString reconame){
    
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
    cout << "    Signal: " << signal.Data() << endl;
    cout << "    Cut(s): " << cuts.Data() << endl;
    
    TString full_signal = signal;
    
    if(!cuts.EqualTo("",TString::kExact)){
        full_signal.Append(" && ");
        full_signal.Append(cuts);
    }
    
    TTree * intree = (TTree*)_file->Get(_truename);
    
    MnvH1D * h_ncuts = new MnvH1D("h_ncuts", "",10, 0., 10.);
    
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
    
    MnvH1D * effcuts = new MnvH1D("effcuts","; ;Efficiency",den->GetNbinsX(),den->GetXaxis()->GetXmin(),den->GetXaxis()->GetXmax());
    effcuts->Divide(num, den);
    effcuts->GetYaxis()->SetRangeUser(0,1.1);
    
    for(int i = 0; i < effcuts->GetNbinsX(); i++){
        if(i < (int)_cutnames.size()){
            TString tmp_label = _cutnames[i];
            effcuts->GetXaxis()->SetBinLabel(i+1, tmp_label.Data());
        }
    }
    
    return effcuts;
}

void EffPurTools::EffVSVar(TString var, const TString signal, const TString cuts){
    cout << "EffPurTools::EffVSVar(TString, TString, TString)" << endl;
    cout << "    Signal: " << signal.Data() << endl;
    cout << "    Cut(s): " << cuts.Data() << endl;
}

/*MnvH1D * EffPurTools::PurVSCuts(TString signal, TString cuts){
    cout << "EffPurTools::PurVSCuts(TString, TString)" << endl;
    cout << "    Signal: " << signal.Data() << endl;
    cout << "    Cut(s): " << cuts.Data() << endl;
    
    TString tmp_cuts = cuts;
    TString tmp_signal = signal;
    if(!cuts.EqualTo(" ", TString::kExact)) tmp_cuts.Prepend(" && ");

    TTree * intree = (TTree*)_file->Get(_reconame);
    
    MnvH1D * h_ncuts = new MnvH1D("h_ncuts", "",10, 0, 10);
    
    TString ncuts_name = "ncuts";
    intree->Draw(ncuts_name + ">> h_ncuts");
    int ncuts = (int)h_ncuts->GetBinCenter(h_ncuts->GetMaximumBin());
    
    std::vector<MnvH1D*> num_bin;
    
    MnvH1D * num = EventsVSCuts(intree, cuts, ncuts);
    
    TString den_signal = "";
    TString num_signal = "";
    
    MnvH1D * den = new MnvH1D("den", "", nbins, x_low, x_high);
    
    //intree->Draw(var + ">> den", signal + " && " + tmp_cuts);

    
    
}*/

/*MnvH1D * EffPurTools::CutsRatio(TString tree_name, TString signal, TString cuts, int min_x){
    
    cout << "EffPurTools::CutsRatio(...)" << endl;
    //Will want to improve this to work with variable binning.
    TTree * intree = (TTree*)_file->Get(tree_name);
    
    //This is currently hard coded to have no more than 10 cuts, may change for the future.
    MnvH1D * h_ncuts = new MnvH1D("h_ncuts", "",10, 0, 10);
    
    TString ncuts_name = "ncuts";
    if(tree_name.Contains("truth", TString::kIgnoreCase)) ncuts_name.Prepend("truth");
    
    intree->Draw(ncuts_name + ">> h_ncuts");
    
    int ncuts = (int)h_ncuts->GetBinCenter(h_ncuts->GetMaximumBin());
    
    cout << "Number of cuts found to be " << ncuts << endl;
    
    TString den_signal = "";
    TString num_signal = ;
    
    MnvH1D * cutratio = Ratio(tree_name, ncuts_name, ncuts, min_x, ncuts, den_signal, num_signal, cuts);
    
    
}


MnvH1D * EffPurTools::Ratio(TString tree_name, TString var, int nbins, double x_low, double x_high, TString signal, TString cuts){
    //Will want to improve this to work with variable binning.
    TTree * intree = (TTree*)_file->Get(tree_name);
    
    MnvH1D * den = new MnvH1D("den", "", nbins, x_low, x_high);
    MnvH1D * num = new MnvH1D("num", "", nbins, x_low, x_high);
    
    intree->Draw(var + ">> den", signal + " && " + cuts);
    intree->Draw(var + ">> den", signal + " && " + cuts);
    
}

void EffPurTools::SetFile(){
    _file = new TFile(_filename, "RECREATE");
}*/

MnvH1D * EffPurTools::EventsVSCuts(TTree * intree, const TString cuts, const int ncuts, TString name){
    
    TString ampersand;
    if(!cuts.EqualTo("", TString::kExact)) ampersand = " && ";
    
    MnvH1D * h_evntcuts = new MnvH1D(name.Data(),"", ncuts, 0., (double)ncuts);
    
    for(int i = 0; i < ncuts; i++){
        MnvH1D * h_tmp = new MnvH1D("h_tmp","", 1, 0., 1.);
        TString tmp_cuts = Form("accum_level > %d%s%s ", (i - 1), ampersand.Data(), cuts.Data());
        intree->Project("h_tmp","0.5",tmp_cuts);
        double intergal = (double)h_tmp->Integral();
        h_evntcuts->SetBinContent(i+1, intergal);
        cout << "Cut " << (i+1) << "Events after cut " << intergal << endl;
        delete h_tmp;
    }
    return h_evntcuts;
}

void EffPurTools::SetCutName(TString var){
    _cutnames.push_back(var);
}


