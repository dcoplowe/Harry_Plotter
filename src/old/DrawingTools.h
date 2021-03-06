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
class TH1;
class THStack;
class TCanvas;
class TObject;

//using namespace PlotUtils;

class DrawingTools : public DrawingStyle {
public:
    
    struct KinMap {
        TH1D * recon;
        TH1D * truth;
        TH2D * smear;
        TH1D * ratio;

        KinMap& operator =(const KinMap& mp)
        {
            recon = mp.recon;
            truth = mp.truth;
            ratio = mp.ratio;
            smear = mp.smear;
            return *this;
        }
    };

//    KinMap KM;
    
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
    
    TH1D * GetHistFromStack(THStack * stack);
    TH1D * GetFirstHistFromStack(THStack * stack);

    TObject * GetObjectFromCanvas(TCanvas * can, std::string name);

    void Verbose(){ m_verbose = true; }
    void SetRatioInfo(Int_t nbins, Double_t range){ m_ratiobins = nbins; m_ratiorange = range; }
    
    static Double_t * SetBinning(int nbins, Double_t low, Double_t high);
    
    TLegend * Legend(Double_t x_size, Double_t y_size, Double_t x_start = 0.1, Double_t y_start = 0.1);
    
    TLegend * GetPOT(Double_t x_pos, Double_t y_pos);
    void SetPOT();
    
    static void SetColors(TH1D *&h1,   Int_t fill_color, Int_t line_color = kBlack, Int_t fill_style = 1001, Int_t line_style = 1);
    static void SetColors(KinMap &map, Int_t fill_color, Int_t line_color = kBlack, Int_t fill_style = 1001, Int_t line_style = 1);

    static std::vector<double> GetPercentage(std::vector<TH1D*> histos);
    static std::vector<double> GetPercentage(std::vector<DrawingTools::KinMap> histos, Int_t type);
    static std::vector<double> GetPercentage(std::vector<DrawingTools::KinMap> histos, Int_t type, KinMap other);
    
    TH1D * ToPDF(TH1D * hraw, TString hn = "");
    TH2D * NormalHist(TH2D * hraw, Double_t thres = 0, bool kmax=false);
    
    void SetWghtName(std::string var = "1"){ m_weight = var; }//Or 1 in initialisation -- weight for MINERvA and wgt for T2K?
 
    void DrawLine(TH1 * histo, double pos, int color = kGray + 2, int style = 1, int width = 2);
    void DrawLine(THStack * stack, double pos, int color = kGray + 2, int style = 1, int width = 2);
    void DrawLine(TCanvas * can, double pos, int color = kGray + 2, int style = 1, int width = 2);
    void DrawLine(double x_low, double y_low, double x_high, double y_high, int color = kGray + 2, int style = 1, int width = 2);
    void DrawBox(double * low, double * high, int color = kGray + 2, int style = 1, int width = 2);

    enum LineDirection
    {
        Left = 0,
        Right,
        Unknown
    };

    void DrawCutLine(double xpos, DrawingTools::LineDirection dir = DrawingTools::Unknown, double arrowpos = 0.5,
        int color = kBlack, int style = 1, int width = 3);
    
    void DrawCutArrow(double xmin, double ymin, double xmax, double ymax, int color = kBlack, int style = 1, int width = 3);

private:
    //File and directory info:
    std::string m_filename;
    std::string m_treename;

    std::string m_uniquename;
    std::string m_weight;
    
    TFile * m_file;
    TTree * m_tree;
    
    Int_t m_1Dcounter;
    Int_t m_2Dcounter;
    
    Double_t m_ratiorange;
    Int_t m_ratiobins;
    
    bool m_verbose;
    bool m_getPOT;
    
    bool m_bad_POT;
    double m_POT;
    bool m_linewarning;

    KinMap empty_kinmap;
    
};

#endif

