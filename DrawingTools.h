#ifndef DRAWINGTOOLS_H
#define DRAWINGTOOLS_H

#include "DrawingStyle.h"

#include "TString.h"
#include "PlotUtils/MnvH1D.h"
#include "PlotUtils/MnvH2D.h"

//Forward declarations here:
class TFile;
class TTree;
class TLegend;
class TH1D;

using namespace PlotUtils;

class DrawingTools : public DrawingStyle {
public:
    
    //Basic class for drawing plots and doesn't require filename, takes in tree and reads:
    DrawingTools();
    
    MnvH1D * GetHisto(TTree * intree, const TString var, int nbins, const double x_low, const double x_high, const TString xy_title = "", const TString cuts = "");
    MnvH1D * GetHisto(TTree * intree, const TString var, int nbins, const Double_t * xbins, const TString xy_title = "", const TString cuts = "");
    
    MnvH2D * GetHisto(TTree * intree, const TString vars_yx, int x_nbins, const double x_low, const double x_high, int y_nbins, const double y_low, const double y_high, const TString xy_title = "", const TString cuts = "");
    MnvH2D * GetHisto(TTree * intree, const TString vars_yx, int x_nbins, const Double_t * xbins, int y_nbins, const Double_t * ybins, const TString xy_title = "", const TString cuts = "");
    
    MnvH2D * SmearMatrix(TTree * intree, const TString vars_yx, int nbins, const double low, const double high, const TString xy_title = "", const TString cuts = "");
    MnvH2D * SmearMatrix(TTree * intree, const TString vars_yx, int nbins, const Double_t * bins, const TString xy_title = "", const TString cuts = "");
    
    ~DrawingTools(){};//This method doesn't require any file;
    
    //This method takes in file, user sets truth and reco. tree names.
    DrawingTools(TString filename, TString reconame = "CC1P1Pi", TString truename = "Truth");

    MnvH1D * GetTruthHisto(const TString var, int nbins, const double x_low, const double x_high, const TString xy_title = "", const TString cuts = "");
    MnvH1D * GetTruthHisto(const TString var, int nbins, const Double_t * xbins, const TString xy_title = "", const TString cuts = "");

    MnvH1D * GetRecoHisto(const TString var, int nbins, const double x_low, const double x_high, const TString xy_title = "", const TString cuts = "");
    MnvH1D * GetRecoHisto(const TString var, int nbins, const Double_t * xbins, const TString xy_title = "", const TString cuts = "");
    
    MnvH2D * SmearMatrix(const TString vars_yx, int nbins, const double low, const double high, const TString xy_title = "", const TString cuts = "");
    MnvH2D * SmearMatrix(const TString vars_yx, int nbins, const Double_t * bins, const TString xy_title = "", const TString cuts = "");
    
    TLegend * GetPOT(double x_pos, double y_pos, TString filename = "");
    
    void ColFill(MnvH1D *&h1, int fill_color, int line_color = kBlack);
    void ColFill(TH1D *&h1, int fill_color, int line_color = kBlack);

    
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
    
};

#endif

