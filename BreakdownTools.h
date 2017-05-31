#ifndef _BREAKDOWNTOOL_
#define _BREAKDOWNTOOL_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>

#include "DrawingTools.h"
#include "DataClasses.h"

//Forward declarations:
class TH1;
class TH1D;
class TH2D;
class TCanvas;
class TLegend;
class THStack;
class Variable;

struct BDCans {
    TCanvas * recon;
    TCanvas * truth;
    TCanvas * smear;
    TCanvas * smearSN;
    TCanvas * ratio;
};

class Breakdown
{
public:   
    Breakdown(std::string name, std::string signal, int fill_color = kBlack, int fill_style = 1001, int line_color = kBlack, int line_style = 1, double line_width = 2.5) : 
        m_name(name), m_signal(signal), m_fill_color(fill_color), m_fill_style(fill_style), m_line_color(line_color),
        m_line_style(line_style), m_line_width(line_width) m_leg_opt("f") {;}
    
    ~Breakdown(){;}

    void SetMap(DrawingTools::KinMap map){
        m_map = map; 
        DrawingTools::SetColors(m_map, m_fill_color, m_line_color, m_fill_style, m_line_style); 
    }

    DrawingTools::KinMap GetMap(){ return m_map; }

    void SetLegOpt(std::string opt){ m_leg_opt = opt;}

    std::string GetSignal(){ return m_signal; }
    std::string GetName(){ return m_name; }
    int GetFillColor(){ return m_fill_color; }
    int GetFillStyle(){ return m_fill_style; }
    int GetLineColor(){ return m_line_color; }
    int GetLineStyle(){ return m_line_style; }
    double GetLineWidth(){ return m_line_width; }
    const char * GetLegOpt(){ return m_leg_opt.c_str(); }

private:
    std::string m_name; 
    std::string m_signal;

    int m_fill_color;
    int m_fill_style;
    int m_line_color;
    int m_line_style;
    double m_line_width;
    std::string m_leg_opt;

    DrawingTools::KinMap m_map;
};

// struct Variable {
//     std::string name;
//     std::string symbol;
//     std::string units;
//     std::string savename;
//     std::string pdg;
// };

class BreakdownTools : public DrawingTools {
public:
	
    BreakdownTools(std::string filename, std::string treename, Topologies * topologies, std::string target_vname = "mc_targetZ"); //;
	~BreakdownTools();

	//Topology breakdown
	//Particle breakdown
	//Wrong PID
    BDCans PID(Variable var, Int_t nbins, Double_t * bins, std::string pdgvar, std::string cuts = "", bool check = false);
    BDCans PID(Variable var, Int_t nbins, Double_t low, Double_t high, std::string pdgvar, std::string cuts = "", bool check = false);
    TCanvas * PID(Variable var, std::string pdgvar, std::string cuts, std::vector<PDGInfo> pdglist, bool check = false);

    BDCans TOPO(Variable var, Int_t nbins, Double_t * bins, std::string cuts = "", bool check = false);
    BDCans TOPO(Variable var, Int_t nbins, Double_t low, Double_t high, std::string cuts = "", bool check = false);
    
    BDCans TARGET(Variable var, Int_t nbins, Double_t * bins, std::string cuts = "", bool check = false);
    BDCans TARGET(Variable var, Int_t nbins, Double_t low, Double_t high, std::string cuts = "", bool check = false);
    
    TCanvas * TARGETSingle(Variable var, Int_t nbins, Double_t * bins, std::string cuts = "");
    TCanvas * TARGETSingle(Variable var, Int_t nbins, Double_t low, Double_t high, std::string cuts = "");

    void PrintPOT(){ m_printPOT = true; }
    
    void FullBreakDown(){ if(!m_fullbreakdown) m_fullbreakdown = true; else m_fullbreakdown = false;}//Switch on/off breakdown level.
    void SetMinPDGBDlist(Int_t pdg){ m_pdglist_minBD.push_back(pdg); }
    void ClearPDGBDlist(){ m_pdglist_minBD.clear(); }
    void ResetPDGBDlist();
    
    void SetMinTOPBDlist(Topology::Name top){ m_toplist_minBD.push_back(top); }
    void ClearTOPBDlist(){ m_toplist_minBD.clear(); }
    void ResetTOPBDlist();
    
    void SetTargetVarName(std::string var = "mc_targetZ"){ m_target = var; }
    void SetnFSPartVarName(std::string var = "mc_nFSPart"){ m_nFSParts = var; }

    void SetSignal(Topology::Name name = Topology::HCC1P1PiPlusPS){ m_signal = name; }

    //    void TopVar();
    //void MisPIDVar();
    
private:
    bool m_printPOT;
    bool m_fullbreakdown;
    
    Topologies * m_topologies;

    std::string m_target;
    std::string m_nFSParts;

    std::vector<PDGInfo> m_pdglist;
    std::vector<Int_t> m_pdglist_minBD;

    std::vector<Topology> m_toplist;
    std::vector<Topology::Name> m_toplist_minBD;

    Topology::Name m_signal;
    Topology m_sig_top;

    Int_t m_statcounter;
    TLegend * RatioStats(const THStack * ratio_tot);   
    void DrawZeroPointLine(TH1 * histo){ DrawLine(histo, 0.); } 
    void DrawZeroPointLine(THStack * histo); 

    DrawingTools::KinMap GetSignalMap(Variable var, Int_t nbins, Double_t * bins, std::string cuts);
    BDCans BaseBreakdown(Variable var, Int_t nbins, Double_t * bins, std::vector<Breakdown> list, std::string basename, std::string cuts, bool check);

    std::vector<double> GetPerentages(std::vector<Breakdown> list, int type);
};
#endif
