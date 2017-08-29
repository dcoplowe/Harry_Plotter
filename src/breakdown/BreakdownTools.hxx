#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>

#include "DrawingTools.hxx"
//Forward declarations:
class TH1;
class TH1D;
class TH2D;
class TCanvas;
class TLegend;
class THStack;

struct BDCans {
    TCanvas * recon;
    TCanvas * truth;
    TCanvas * smear;
    TCanvas * smearSN;
    TCanvas * ratio;
};

#ifndef __BREAKDOWN__HXX__
#define __BREAKDOWN__HXX__

const int bd_fill_color = kBlack;
const int bd_fill_style = 1001;
const int bd_line_color = kBlack;
const int bd_line_style = 1;
const double bd_line_width = 2.5;
const std::string bd_legend_opt = "f";

class Breakdown
{
public:   
    Breakdown() : m_name("Unknown"), m_signal(m_name), m_fill_color(bd_fill_color), m_fill_style(bd_fill_style), m_line_color(bd_line_color),
        m_line_style(bd_line_style), m_line_width(bd_line_width), m_leg_opt(bd_legend_opt) {;}

    Breakdown(std::string name, std::string signal, int fill_color = bd_fill_color, int fill_style = bd_fill_style, int line_color = bd_line_color,
        int line_style = bd_line_style, double line_width = bd_line_width) : m_name(name), m_signal(signal), m_fill_color(fill_color), 
        m_fill_style(fill_style), m_line_color(line_color), m_line_style(line_style), m_line_width(line_width), m_leg_opt(bd_legend_opt) {;}
    
    ~Breakdown(){;}

    void SetMap(DrawingTools::KinMap map);
    void SetHist(TH1D * hist);

    DrawingTools::KinMap GetMap(){ return m_map; }
    TH1D * GetHist(){ return m_hist; } 

    void SetLegOpt(std::string opt){ m_leg_opt = opt;}

// Make consts?
    std::string GetSignal(){ return m_signal; }
    std::string GetName(){ return m_name; }
    int GetFillColor(){ return m_fill_color; }
    int GetFillStyle(){ return m_fill_style; }
    int GetLineColor(){ return m_line_color; }
    int GetLineStyle(){ return m_line_style; }
    double GetLineWidth(){ return m_line_width; }
    const char * GetLegOpt(){ return m_leg_opt.c_str(); }

    void Print();

private:
    std::string m_name; 
    std::string m_signal;
    bool m_check;

    int m_fill_color;
    int m_fill_style;
    int m_line_color;
    int m_line_style;
    double m_line_width;
    std::string m_leg_opt;

    DrawingTools::KinMap m_map;
    TH1D * m_hist;
};

#endif

#ifndef __BREAKDOWNTOOLS__HXX__
#define __BREAKDOWNTOOLS__HXX__

#include <Variable.hxx>

class PDGInfo;
class Topology;

class BreakdownTools : public DrawingTools {
public:

    BreakdownTools(std::string filename, std::string treename, bool check_pot = false);
	~BreakdownTools();

    BDCans PIDBD(Variable var, std::string pdgvar, std::string cuts = "");
    TCanvas * PIDC(Variable var, std::string pdgvar, std::string cuts = "");

    BDCans TOPOBD(Variable var, std::string cuts = "");
    TCanvas * TOPOC(Variable var, std::string cuts = "");

    BDCans TARBD(Variable var, std::string cuts = "");
    TCanvas * TARC(Variable var, std::string cuts = "");

    BDCans INTBD(Variable var, std::string cuts = "");
    TCanvas * INTC(Variable var, std::string cuts = "");

    void PrintPOT(){ m_printPOT = true; }
        
private:
    bool m_printPOT;
    bool m_check;

    std::string m_target;

    std::vector<PDGInfo> m_pdglist;
    std::vector<Topology> m_toplist;
    std::vector<Breakdown> m_tarlist;

    Topology * m_signal;

    std::vector<Breakdown> GetPIDs(std::string pdgvar, std::string cuts, std::string &final_cuts);
    std::vector<Breakdown> GetTOPs();
    std::vector<Breakdown> GetTARs();
    std::vector<Breakdown> GetINTs();

    int m_statcounter;
    TLegend * RatioStats(const THStack * ratio_tot);   
    void DrawZeroPointLine(TH1 * histo){ DrawLine(histo, 0.); } 
    void DrawZeroPointLine(THStack * histo); 

    DrawingTools::KinMap GetSignalMap(Variable var, std::string cuts);
    TH1D * GetSignalHist(Variable var, std::string cuts);

    BDCans BaseBreakdown(Variable var, std::vector<Breakdown> list, std::string basename, std::string cuts);
    TCanvas * SingleBase(Variable var, std::vector<Breakdown> list, std::string basename, std::string cuts);

    std::vector<double> GetPercentages(std::vector<Breakdown> list, int type = -999);
};

#endif
