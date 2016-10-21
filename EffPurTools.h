#ifndef EFFPURTOOLS_H
#define EFFPURTOOLS_H

#include "DrawingStyle.h"

#include "TString.h"
//#include "PlotUtils/MnvH1D.h"

#include <sstream>
//Forward declarations here:
class TFile;
class TTree;
class TH1D;

//using namespace PlotUtils;
using namespace std;

class EffPurTools : public DrawingStyle {
public:
    
    //Given that all eff. is based on truth info. and pur.
    //on the reco. these branch names will always remain
    //the same. We can therefore load the file into the tool
    //and run everything within the class.
    
    //For now have assume a file is read in.
    
    EffPurTools(TString filename, std::vector<TString> cut_names, bool debug = false, TString reconame = "sel", TString truename = "Truth");
    EffPurTools(TString filename, bool debug = false, TString reconame = "sel", TString truename = "Truth");
    EffPurTools();
    
    EffPurTools(std::string filename, std::vector<std::string> cut_names, bool debug = false, std::string reconame = "sel", std::string truename = "Truth");
    EffPurTools(std::string filename, bool debug = false, std::string reconame = "sel", std::string truename = "Truth");
    
    ~EffPurTools(){};
    
    void SetFileName(TString var){ _filename = var; }
    void SetRecoBranch(TString var){ _reconame = var; }
    void SetTrueBranch(TString var){ _truename = var; }
    void SetFile();
    
    void SetGlobalSignal(std::string var){ _glob_signal = TString(var); }
    void SetGlobalSignal(TString var){ _glob_signal = var; }
    
    //These may not be void functions:
    TH1D * EffVSCuts(const TString signal, int branch = 0, const TString cuts = "");
    TH1D * EffVSVar(const TString var, int nbins, const Double_t * xbins, const TString signal, const TString x_title = "", const TString cuts = "");
    TH1D * EffVSVar(const TString var, int nbins, const Double_t x_low, const Double_t x_high, const TString signal, const TString x_title = "", const TString cuts = "");
    
    TH1D * PurVSCuts(const TString signal, int branch = 0, const TString cuts = "");
    TH1D * PurVSVar(const TString var, int nbins, const Double_t * xbins, const TString signal, const TString x_title = "", const TString cuts = "");
    TH1D * PurVSVar(const TString var, int nbins, const Double_t x_low, const Double_t x_high, const TString signal, const TString x_title = "", const TString cuts = "");

    TH1D * EffVSCuts(std::string signal, int branch = 0, std::string cuts = "");
    TH1D * EffVSVar(std::string var, int nbins, const Double_t * xbins, std::string signal, std::string x_title = "", std::string cuts = "");
    TH1D * EffVSVar(std::string var, int nbins, const Double_t x_low, const Double_t x_high, std::string signal, std::string x_title = "", std::string cuts = "");
    
    TH1D * PurVSCuts(std::string signal, int branch = 0, std::string cuts = "");
    TH1D * PurVSVar(std::string var, int nbins, const Double_t * xbins, std::string signal, std::string x_title = "", std::string cuts = "");
    TH1D * PurVSVar(std::string var, int nbins, const Double_t x_low, const Double_t x_high, std::string signal, std::string x_title = "", std::string cuts = "");

    
    
    void SetCutNames(std::vector<TString> var);
    void SetCutName(TString var);//This must be written in order;
    void ResetCutNames();
    
private:
    //File and directory info:
    TString _filename;
    TString _truename;
    TString _reconame;
    
    TFile * _file;
    
    TTree * _truthtree;
    TTree * _recontree;
    
    //Global signal information
    TString _glob_signal;
    std::vector<TString> _cutnames;
    
    TH1D * EventsVSCuts(TTree * intree, const TString cuts, int branch, const int ncuts, TString name = "h_evntcuts");
    TH1D * DrawRatioVSCuts(TH1D * num, TH1D * den, TString y_title = "", TString h_name = "h_ratio");
    
    TH1D * RatioVSVar(TTree * intree, const TString var, int nbins, const Double_t * xbins, const TString signal, const TString x_title, const TString cuts);
    TH1D * RatioVSVar(TTree * intree, const TString var, int nbins, const Double_t x_low, const Double_t x_high, const TString signal, const TString x_title, const TString cuts);
    
    TH1D * GetHisto(TTree * intree, const TString var, int nbins, const Double_t * xbins, const TString cuts);
    TH1D * GetHisto(TTree * intree, const TString var, int nbins, const double x_low, const double x_high, const TString cuts);
    
    int _effhcounter;
    int _purhcounter;
    int _ghcounter;
    int _effvarcounter;
    int _purvarcounter;

    bool _DEBUG_;

};

#endif

