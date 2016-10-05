#ifndef DRAWINGTOOLS_H
#define DRAWINGTOOLS_H

#include "DrawingStyle.h"

#include "TString.h"
//#include "PlotUtils/MnvH1D.h"
//#include "PlotUtils/MnvH2D_.h"

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
    DrawingTools();
    //This method takes in file, user sets truth and reco. tree names.
    DrawingTools(TString filename, bool debug = false, TString reconame = "sel", TString truename = "Truth");
    ~DrawingTools(){};//This method doesn't require any file;
    
    TH1D * GetHisto(TTree * intree, const TString var, int nbins, const double x_low, const double x_high, const TString xy_title = "", const TString cuts = "");
    TH1D * GetHisto(TTree * intree, const TString var, int nbins, const Double_t * xbins, const TString xy_title = "", const TString cuts = "");
    
    TH2D * GetHisto(TTree * intree, const TString vars_yx, int x_nbins, const double x_low, const double x_high, int y_nbins, const double y_low, const double y_high, const TString xy_title = "", const TString cuts = "");
    TH2D * GetHisto(TTree * intree, const TString vars_yx, int x_nbins, const Double_t * xbins, int y_nbins, const Double_t * ybins, const TString xy_title = "", const TString cuts = "");
    
    TH2D * SmearMatrix(TTree * intree, const TString vars_yx, int nbins, const double low, const double high, const TString xy_title = "", const TString cuts = "");
    TH2D * SmearMatrix(TTree * intree, const TString vars_yx, int nbins, const Double_t * bins, const TString xy_title = "", const TString cuts = "");
    
    TH2D * SmearMatrix(TTree * intree, const TString vars_yx, int re_nbins, const double re_low, const double re_high, int tr_nbins, const double tr_low, const double tr_high, const TString xy_title = "", const TString cuts = "");
    TH2D * SmearMatrix(TTree * intree, const TString vars_yx, int re_nbins, const Double_t * re_bins, int tr_nbins, const Double_t * tr_bins, const TString xy_title = "", const TString cuts = "");
    
    void SetupRatio(double range, int nbins){ _ratiorange = range; _ratiobins = nbins; }
    
    TH1D * GetRTRatio(TTree * intree, const TString vars_tr, const TString x_title = "", const TString cuts = "");
    
    KinMap KinArray(TTree * intree, const TString vars_tr, int nbins, const double low, const double high, const TString rt_title = "", const TString cuts = "", bool cor = true);
    KinMap KinArray(TTree * intree, const TString vars_tr, int nbins, const Double_t * bins, const TString rt_title = "", const TString cuts = "", bool cor = true);
    KinMap KinArray(TTree * intree, const TString vars_tr, int re_nbins, const double re_low, const double re_high, int tr_nbins, const double tr_low, const double tr_high, const TString rt_title = "", const TString cuts = "", bool cor = true);
    KinMap KinArray(TTree * intree, const TString vars_tr, int re_nbins, const Double_t * re_bins, int tr_nbins, const Double_t * tr_bins, const TString rt_title = "", const TString cuts = "", bool cor = true);
    
    TH1D * GetRTRatio(const TString vars_tr, const TString x_title = "", const TString cuts = "");
    
    KinMap KinArray(const TString vars_tr, int nbins, const double low, const double high, const TString rt_title = "", const TString cuts = "", bool cor = true);
    KinMap KinArray(const TString vars_tr, int nbins, const Double_t * bins, const TString rt_title = "", const TString cuts = "", bool cor = true);
    KinMap KinArray(const TString vars_tr, int re_nbins, const double re_low, const double re_high, int tr_nbins, const double tr_low, const double tr_high, const TString rt_title = "", const TString cuts = "", bool cor = true);
    KinMap KinArray(const TString vars_tr, int re_nbins, const Double_t * re_bins, int tr_nbins, const Double_t * tr_bins, const TString rt_title = "", const TString cuts = "", bool cor = true);
    
    TH1D * GetTruthHisto(const TString var, int nbins, const double x_low, const double x_high, const TString xy_title = "", const TString cuts = "");
    TH1D * GetTruthHisto(const TString var, int nbins, const Double_t * xbins, const TString xy_title = "", const TString cuts = "");
    
    TH1D * GetRecoHisto(const TString var, int nbins, const double x_low, const double x_high, const TString xy_title = "", const TString cuts = "");
    TH1D * GetRecoHisto(const TString var, int nbins, const Double_t * xbins, const TString xy_title = "", const TString cuts = "");
    
    TH2D * SmearMatrix(const TString vars_yx, int nbins, const double low, const double high, const TString xy_title = "", const TString cuts = "");
    TH2D * SmearMatrix(const TString vars_yx, int nbins, const Double_t * bins, const TString xy_title = "", const TString cuts = "");
    
    TH2D * SmearMatrix(const TString vars_yx, int re_nbins, const double re_low, const double re_high, int tr_nbins, const double tr_low, const double tr_high, const TString xy_title = "", const TString cuts = "");
    TH2D * SmearMatrix(const TString vars_yx, int re_nbins, const Double_t * re_bins, int tr_nbins, const Double_t * tr_bins, const TString xy_title = "", const TString cuts = "");
    
    std::vector<double> GetPercentage(std::vector<TH1D*> histos);
    
    TLegend * GetPOT(double x_pos, double y_pos, TString filename = "");
    
    void ColFill(TH1D *&h1, int fill_color, int line_color = kBlack);
    
    TLegend * Legend(double x_size, double y_size, double x_start = 0.1, double y_start = 0.1);
    
    void SetFileName(TString var){ _filename = var; }
    void SetRecoBranch(TString var){ _reconame = var; }
    void SetTrueBranch(TString var){ _truename = var; }
    void SetFile();
    void SetTrees();
    
private:
    //File and directory info:
    TString _filename;
    TString _truename;
    TString _reconame;
    
    TFile * _file;
    
    TTree * _truetree;
    TTree * _recotree;
    TTree * _metatree;
    
    int _1Dcounter;
    int _2Dcounter;
    
    double _ratiorange;
    int _ratiobins;
    
    void SetPOT(TString filename = "");
    double _POT;
    
    bool _DEBUG_;
    
};

#endif

