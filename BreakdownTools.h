#ifndef _BREAKDOWNTOOL_
#define _BREAKDOWNTOOL_

#include <stdio.h>
#include <stdlib.h>

//Forward declarations:
class DrawingTools;
class TH1D;
class TH2D;
class TCanvas;

struct BDCans {
    TCanvas * recon;
    TCanvas * truth;
    TCanvas * smear;
    TCanvas * ratio;
};

struct Variable {
    std::string name;
    std::string print;
    std::string units;
};

class BreakdownTools {
public:
	
    BreakdownTools(std::string filename, std::string treename); //;
	~BreakdownTools();

	//Topology breakdown
	//Particle breakdown
	//Wrong PID
    BDCans PID(Variable var, const int var_nbins, const double var_low, const double var_high, std::string cuts = "");
    
//    BDCans TOPO(std::string var_name, const int var_nbins, const double mom_low, const double mom_high, std::string pname, std::string can_title, std::string mom_title = "", std::string cuts = "");

    void PrintPOT(){ m_printPOT = true; }
    void FullBreakDown(){ m_fullbreakdown = true; }
    
    //    void TopVar();
    //void MisPIDVar();
    
private:
    DrawingTools * m_plot;
    bool m_printPOT;
    bool m_fullbreakdown;
};
#endif
