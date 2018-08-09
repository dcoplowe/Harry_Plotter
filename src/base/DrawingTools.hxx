#ifndef __DRAWINGTOOLS__HXX
#define __DRAWINGTOOLS__HXX

#include "DrawingStyle.hxx"

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
    
    static int s_1Dcounter;
    static int s_2Dcounter;

    static Long64_t MaxEntries;

    struct KinMap {
        TH1D * recon;
        TH1D * truth;
        TH2D * smear;
        TH2D * smearSN;
        TH1D * ratio;

        KinMap& operator =(const KinMap& mp)
        {
            recon = mp.recon;
            truth = mp.truth;
            ratio = mp.ratio;
            smear = mp.smear;
            smearSN = mp.smearSN;
            return *this;
        }
    };

//    KinMap KM;
    
    //Basic class for drawing plots and doesn't require filename, takes in tree and reads:
    DrawingTools(std::string filename, std::string treename, std::string uniquename = "", bool check_pot = false);
    ~DrawingTools();
    //This method takes in file, user sets truth and reco. tree names.
    
    TH1D * GetHisto(std::string var, int nbins, double * bins, std::string xy_title = "", std::string cuts = "");
    TH1D * GetHisto(std::string var, int nbins, double low, double high, std::string xy_title = "", std::string cuts = "");
    
    TH2D * GetHisto(std::string var_yx, int x_nbins, double * x_bins, int y_nbins, double * y_bins,
                    std::string xy_title = "", std::string cuts = "");
    
    TH2D * GetHisto(std::string var_yx, int x_nbins, double x_low, double x_high, int y_nbins, double y_low, double y_high,
                    std::string xy_title = "", std::string cuts = "");
    
    TH2D * GetSmear(std::string vars_yx, int nbins, double * bins, std::string xy_title = "", std::string cuts = "");
    TH2D * GetSmear(std::string vars_yx, int nbins, double low, double high, std::string xy_title = "", std::string cuts = "");
    
    TH2D * GetSmearSN(std::string vars_yx, int nbins, double * bins, std::string xy_title = "", std::string cuts = "",
        double thres = 0, bool kmax = true);
    TH2D * GetSmearSN(std::string vars_yx, int nbins, double low, double high, std::string xy_title = "", std::string cuts = "",
        double thres = 0, bool kmax = true);

    KinMap KinArray(std::string vars_tr, int nbins, double * bins, std::string rt_title = "", std::string cuts = "");
    KinMap KinArray(std::string vars_tr, int nbins, double low, double high, std::string rt_title = "", std::string cuts = "");
    
    TH1D * GetRTRatio(std::string vars_tr, std::string x_title, std::string cuts);
    
    TH1D * GetHistFromStack(THStack * stack);
    TH1D * GetFirstHistFromStack(THStack * stack);

    TObject * GetObjectFromCanvas(TCanvas * can, std::string name);

    void Verbose(){ m_verbose = true; }
    void SetRatioInfo(int nbins, double range){ m_ratiobins = nbins; m_ratiorange = range; }
    
    static double * SetBinning(int nbins, double low, double high);
    
    static TLegend * Legend(double x_size, double y_size, double x_start = 0.1, double y_start = 0.1);
    
    TLegend * GetPOT(double x_pos, double y_pos);
    void SetPOT();
    
    static void SetColors(TH1D *&h1,   int fill_color, int line_color = kBlack, int fill_style = 1001, int line_style = 1, int line_width = 2);
    static void SetColors(KinMap &map, int fill_color, int line_color = kBlack, int fill_style = 1001, int line_style = 1, int line_width = 2);

    static std::vector<double> GetPercentage(std::vector<TH1D*> histos);
    static std::vector<double> GetPercentage(std::vector<DrawingTools::KinMap> histos, int type);
    static std::vector<double> GetPercentage(std::vector<DrawingTools::KinMap> histos, int type, KinMap other);
    
    TH1D * ToPDF(TH1D * hraw, TString hn = "");
    TH2D * NormalHist(TH2D * hraw, double thres = 0., bool kmax = true);
        
    static void Normalise(TH1D *& hist);
    static void Normalise(TH2D *& hist);

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
    
    // If -999 is the presision use error as stat error.
    double * GetOptBinning(const std::string &var_name, const int x_nbins, const double x_min, const double x_max,
    const std::string &cuts = "", const double precision = -999.);

    int GetEntriesInRange(const std::string &var_name, const double x_min, const double x_max,  const std::string &cuts);

    // If -999 is the presision use error as stat error.
    static double * GetOptBinning(TTree * intree, const std::string &var_name, const int x_nbins, const double x_min, const double x_max,
    const std::string &cuts = "", const double precision = -999.);

    static int GetEntriesInRange(TTree * tree, const std::string &var_name, const double x_min, const double x_max,  const std::string &cuts);

private:
    //File and directory info:
    std::string m_filename;
    std::string m_treename;

    std::string m_uniquename;
    std::string m_weight;
    
    TFile * m_file;
    TTree * m_tree;
        
    double m_ratiorange;
    int m_ratiobins;
    
    bool m_verbose;
    bool m_checkPOT;
    
    bool m_bad_POT;
    double m_POT;
    bool m_linewarning;

    KinMap empty_kinmap;
    
};

#endif

