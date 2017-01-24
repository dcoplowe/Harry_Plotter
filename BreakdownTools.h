#ifndef _BREAKDOWNTOOL_
#define _BREAKDOWNTOOL_

#include <stdio.h>
#include <stdlib.h>
#include <string>

#inculde "DrawingTools.h"

//Forward declarations:
//class DrawingTools;
class TH1D;
class TH2D;
class TCanvas;
//
//class Particle;
//class KinematicVars;

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

#ifndef _PARTPDGS_
#define _PARTPDGS_
class PDGs{
public:
    PDGs(Int_t part_pdg, std::string part_name, std::string part_symbol);
    ~PGDs(){};
    
    Int_t pdg;
    std::string name;
    std::string symbol;
    std::string s_pdg;
};
#endif

PDGs::PDGs(Int_t part_pdg, std::string part_name, std::string part_symbol) : pdg(part_pdg), name(part_name), symbol(part_symbol){
    stringstream ss;
    ss << pdg;
    pdg_s = ss.str();
}

class BreakdownTools : public DrawingTools {
public:
	
    BreakdownTools(std::string filename, std::string treename); //;
	~BreakdownTools();

	//Topology breakdown
	//Particle breakdown
	//Wrong PID
    BDCans PID(Variable var, Int_t nbins, Double_t * bins, std::string pdgvar, std::string cuts = "");
    BDCans PID(Variable var, Int_t nbins, Double_t low, Double_t high, std::string pdgvar, std::string cuts = "");

//    BDCans TOPO(std::string var_name, const int var_nbins, const double mom_low, const double mom_high, std::string pname, std::string can_title, std::string mom_title = "", std::string cuts = "");

    void PrintPOT(){ m_printPOT = true; }
    void FullBreakDown(){ if(!m_fullbreakdown) m_fullbreakdown = true; else m_fullbreakdown = false;}//Switch on/off breakdown level.
    
    //    void TopVar();
    //void MisPIDVar();
    
private:
    bool m_printPOT;
    bool m_fullbreakdown;
    
    std::vector<PDGs> m_pdglist;
    
};
#endif
