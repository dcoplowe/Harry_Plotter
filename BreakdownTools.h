#ifndef _BREAKDOWNTOOL_
#define _BREAKDOWNTOOL_

//Forward declarations:
#include "TString.h"
#include <stdio.h>

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

class BreakdownTools {
public:
	
    BreakdownTools(TString filename, bool debug = false, TString reconame = "sel", TString truename = "Truth");
	~BreakdownTools();

	//Topology breakdown
	//Particle breakdown
	//Wrong PID
    void PIDVar(const char * mom_name, const int mom_nbins, const double mom_low, const double mom_high, const char pname, const char * can_title, const char * mom_title = "", const char * cuts = "");
    void TopVar();
    void MisPIDVar();
    
private:
    DrawingTools * m_plot;
}

#endif
