#ifndef DRAWINGTOOLS_H
#define DRAWINGTOOLS_H

#include "DrawingStyle.h"

//#include "std::string.h"
#include <string>

//Forward declarations here:
class TFile;
class TTree;
class TLegend;
class TH1D;
class TH2D;

//using namespace PlotUtils;
struct KinMap {
    TH1D * recon;
    TH1D * truth;
    TH2D * smear;
    TH1D * ratio;
};

class DrawingTools : public DrawingStyle {
public:
    
    //Basic class for drawing plots and doesn't require filename, takes in tree and reads:
    DrawingTools(std::string filename, std::string treename, std::string uniquename = "");
    ~DrawingTools();
    //This method takes in file, user sets truth and reco. tree names.
    
    TH1D * GetHisto(std::string var, Int_t nbins, Double_t * bins, std::string xy_title = "", std::string cuts = "");
    TH1D * GetHisto(std::string var, Int_t nbins, Double_t low, Double_t high, std::string xy_title = "", std::string cuts = "");
    
    TH2D * GetHisto(std::string var_yx, Int_t x_nbins, Double_t * x_bins, Int_t y_nbins, Double_t * y_bins,
                    std::string xy_title = "", std::string cuts = "");
    
    TH2D * GetHisto(std::string var_yx, Int_t x_nbins, Double_t x_low, Double_t x_high, Int_t y_nbins, Double_t y_low, Double_t y_high,
                    std::string xy_title = "", std::string cuts = "");
    
    TH2D * SmearMatrix(std::string vars_yx, Int_t nbins, Double_t * bins, std::string xy_title = "", std::string cuts = "");
    TH2D * SmearMatrix(std::string vars_yx, Int_t nbins, Double_t low, Double_t high, std::string xy_title = "", std::string cuts = "");
    
    KinMap KinArray(std::string vars_tr, Int_t nbins, Double_t * bins, std::string rt_title = "", std::string cuts = "");
    KinMap KinArray(std::string vars_tr, Int_t nbins, Double_t low, Double_t high, std::string rt_title = "", std::string cuts = "");
    
    TH1D * GetRTRatio(std::string vars_tr, std::string x_title, std::string cuts);
    
    KinMap KM;
    
    void Verbose(){ m_verbose = true; }
    void SetRatioInfo(Int_t nbins, Double_t range){ m_ratiobins = nbins; m_ratiorange = range; }
    
private:
    //File and directory info:
    std::string m_filename;
    std::string m_treename;

    std::string m_uniquename;
    
    TFile * m_file;
    TTree * m_tree;
    
    Int_t m_1Dcounter;
    Int_t m_2Dcounter;
    
    Double_t m_ratiorange;
    Int_t m_ratiobins;

    Double_t * SetBinning(int nbins, Double_t low, Double_t high);
    
    bool m_verbose;
};

#endif

