#ifndef _BREAKDOWNTOOL_
#define _BREAKDOWNTOOL_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>

#include "DrawingTools.h"

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
    TCanvas * smearSN;
    TCanvas * ratio;
};

struct Variable {
    std::string name;
    std::string symbol;
    std::string units;
    std::string savename;
};

#ifndef _PARTPDGS_
#define _PARTPDGS_

class PDGs {
public:
    PDGs(Int_t part_pdg, std::string part_name, std::string part_symbol);
    ~PDGs(){};
    
    Int_t pdg;
    std::string name;
    std::string symbol;
    std::string pdg_s;
    Int_t  colour;
};
#endif

class BreakdownTools : public DrawingTools {
public:
	
    BreakdownTools(std::string filename, std::string treename); //;
	~BreakdownTools();

	//Topology breakdown
	//Particle breakdown
	//Wrong PID
    BDCans PID(Variable var, Int_t nbins, Double_t * bins, std::string pdgvar, std::string cuts = "");
    BDCans PID(Variable var, Int_t nbins, Double_t low, Double_t high, std::string pdgvar, std::string cuts = "");

    BDCans TOPO(Variable var, Int_t nbins, Double_t * bins, std::string pdgvar, std::string cuts = "");
    BDCans TOPO(Variable var, Int_t nbins, Double_t low, Double_t high, std::string pdgvar, std::string cuts = "");

    void PrintPOT(){ m_printPOT = true; }
    
    void FullBreakDown(){ if(!m_fullbreakdown) m_fullbreakdown = true; else m_fullbreakdown = false;}//Switch on/off breakdown level.
    void SetMinBDlist(Int_t pdg){ m_MinBDlist.push_back(pdg); }
    void ClearBDlist(){ m_MinBDlist.clear(); }
    void ResetBDlist();
    
    //    void TopVar();
    //void MisPIDVar();
    
private:
    bool m_printPOT;
    bool m_fullbreakdown;
    
    std::vector<PDGs> m_pdglist;
    std::vector<Int_t> m_MinBDlist;

};
#endif
