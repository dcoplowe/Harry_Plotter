#ifndef EFFPURTOOLS_H
#define EFFPURTOOLS_H

//#include <PlotUtils/MnvH1D.h>

#include "TString.h"

//Forward declarations here:
class TFile;
class MnvH1D;

using namespace std;

class EffPurTools {
public:
    
    //Given that all eff. is based on truth info. and pur.
    //on the reco. these branch names will always remain
    //the same. We can therefore load the file into the tool
    //and run everything within the class.
    
    //For now have assume a file is read in.
    
    EffPurTools(TString filename, TString truename = "Truth", TString reconame = "CC1P1Pi");
    EffPurTools();
    
    ~EffPurTools(){};
    
    void SetFileName(TString var){ _filename = var; }
    void SetFile();
    void SetRecoBranch(TString var){ _reconame = var; }
    void SetTrueBranch(TString var){ _truename = var; }
    
    void SetGlobalSignal(TString var){ _glob_signal = var; }
    
    //These may not be void functions:
    void EffVSCuts(const TString signal, const TString cuts = "");
    void EffVSVar(TString var, TString signal, TString cuts = "");
    
    void PurVSCuts(TString signal, TString cuts = "");
    
    void SetCutName(TString var);
    
private:
    //File and directory info:
    TString _filename;
    TString _truename;
    TString _reconame;
    
    TFile * _file;
    
    //Global signal information
    TString _glob_signal;
    std::vector<TString> _cutnames;
    
    MnvH1D * EventsVSCuts(TTree * intree, const TString cuts, const int ncuts, TString name);
    
//    MnvH1D * CutsRatio(TString tree_name, TString var, TString, int nbins, double x_low, double x_high, TString signal, TString cuts, int min_x = 0);
//    MnvH1D * Ratio(TString tree_name, TString var, int nbins, double x_low, double x_high, TString signal, TString cuts);

};

#endif

