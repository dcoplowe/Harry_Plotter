#ifndef EFFPURTOOLS_H
#define EFFPURTOOLS_H

//#include <PlotUtils/MnvH1D.h>

//Forward declarations here:
class TString;

using namespace std;

class EffPurTools {
public:
    
    //Given that all eff. is based on truth info. and pur.
    //on the reco. these branch names will always remain
    //the same. We can therefore load the file into the tool
    //and run everything within the class.
    
    //For now have assume a file is read in.
    
    EffPurTools(TString filename, TString truename = "Truth", TString TString reconame = "CC1P1Pi");
    EffPurTools();
    
    ~EffPurTools(){};
    
    void SetFile(TString var){ _filename = var; }
    void SetRecoBranch(TString var){ _reconame = var; }
    void SetTrueBranch(TString var){ _truename = var; }
    
    void SetGlobalSignal(TString var){ _glob_signal = var; }
    
    //These may not be void functions:
    void EffVSCuts(TString signal, TString cuts = "");
    void EffVSVar(TString var, TString signal, TString cuts = "");
    
    void PurVSCuts(TString signal, TString cuts = "");
    
private:
    //File and directory info:
    TString _filename;
    TString _truename;
    TString _reconame;
    
    //Global signal information
    TString _glob_signal;
    
    
};

#endif

