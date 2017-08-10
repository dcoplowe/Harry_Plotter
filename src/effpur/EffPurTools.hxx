#ifndef EFFPURTOOLS_H
#define EFFPURTOOLS_H

#include "DrawingStyle.hxx"
#include "TString.h"

#include <sstream>
//Forward declarations here:
class TFile;
class TTree;
class TH1D;
class TH2D;

//using namespace PlotUtils;
using namespace std;

class EffPurTools : public DrawingStyle {
public:
    
    static int Counter;
    //Given that all eff. is based on truth info. and pur.
    //on the reco. these branch names will always remain
    //the same. We can therefore load the file into the tool
    //and run everything within the class.
    
    //For now have assume a file is read in.
    
    EffPurTools(std::string filename, std::vector<std::string> cut_names, std::string reconame = "sel", std::string truename = "Truth");
    EffPurTools(std::string filename, std::string reconame = "sel", std::string truename = "Truth");
    
    ~EffPurTools();
  
    TH1D * EffVSCuts(std::string signal, int branch = 0, std::string cuts = "");
    TH1D * PurVSCuts(std::string signal, int branch = 0, std::string cuts = "");
    
    //Be careful here, given the efficiency is how many of the selected events are signal. The signal for eff. vs. var is common to both the numerator and denominator whereas the cut is made on the numerator only.
    TH1D * EffVSVar(std::string var, int nbins, double * xbins, std::string signal, std::string cuts, std::string x_title = "");
    TH1D * EffVSVar(std::string var, int nbins, double x_low, double x_high, std::string signal, std::string cuts, std::string x_title = "");
    
    TH2D * EffVSVar(std::string var_yx, int x_nbins, double * x_bins, int y_nbins, double * y_bins, std::string signal, std::string cuts, std::string xy_title = "");
    TH2D * EffVSVar(std::string var_yx, int x_nbins, double x_low, double x_high, int y_nbins, double y_low, double y_high, std::string signal, std::string cuts, std::string xy_title = "");
    
    TH1D * PurVSVar(std::string var, int nbins, double * xbins, std::string signal, std::string cuts, std::string x_title = "");
    TH1D * PurVSVar(std::string var, int nbins, double x_low, double x_high, std::string signal, std::string cuts, std::string x_title = "");
  
    TH2D * PurVSVar(std::string var_yx, int x_nbins, double * x_bins, int y_nbins, double * y_bins, std::string signal, std::string cuts, std::string xy_title = "");
    TH2D * PurVSVar(std::string var_yx, int x_nbins, double x_low, double x_high, int y_nbins, double y_low, double y_high, std::string signal,
     std::string cuts, std::string xy_title = "");
    
    TH1D * EffVSN1Cuts(std::string signal, int branch = 0, std::string cuts = "");
    TH1D * PurVSN1Cuts(std::string signal, int branch = 0, std::string cuts = "");

    void Debug(){ if(m_debug) m_debug = false; else m_debug = true; }
    void SetCutNames(std::vector<std::string> var);
    void SetCutName(std::string var);//This must be written in order;
    void ResetCutNames();
    void SetNCuts(int ncuts){ m_ncuts = ncuts;}
    
private:
    //File and directory info:
//    std::string m_filename;
//    std::string m_truename;
//    std::string m_reconame;
    bool m_debug;
    
    TFile * m_file;
    TTree * m_truth;
    TTree * m_recon;
    
    std::vector<std::string> m_cutnames;
    
    TH1D * EventsVSCuts(TTree * intree, std::string cuts, int branch, int ncuts, std::string name = "h_evntcuts");
    TH1D * DrawRatioVSCuts(TH1D * num, TH1D * den, std::string y_title = "", std::string h_name = "h_ratio");
    
    TH1D * RatioVSVar(TTree * intree, std::string var, int nbins, double * xbins, std::string common_cut, std::string num_only_cut, std::string x_title);
    TH2D * RatioVSVar(TTree * intree, std::string var_yx, int x_nbins, double * x_bins, int y_nbins, double * y_bins, std::string common_cut, std::string num_only_cut, std::string xy_title);
    
    TH1D * GetHisto(TTree * intree, std::string var, int nbins, double * xbins, std::string cuts);
    TH2D * GetHisto(TTree * intree, std::string var_yx, int x_nbins, double * xbins, int y_nbins, double * ybins, std::string cuts);
    
    int GetNCuts();
    int MaxCutsToDraw();

    double GetCutEntries(int ignore, std::string cuts, int branch);

    int m_purhcounter;
    int m_effhcounter;
    int m_ghcounter1D;
    int m_ghcounter2D;
    int m_effvarcounter;
    int m_purvarcounter;
    
    int m_ncuts;
    // double * EvenArray(int nbins, double x_low, double x_high);
};

#endif
