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
    std::string pdg;
};

class BreakdownTools : public DrawingTools {
public:
	
    BreakdownTools(std::string filename, std::string treename, Topologies * topologies, std::string target_vname = "mc_targetZ"); //;
	~BreakdownTools();

	//Topology breakdown
	//Particle breakdown
	//Wrong PID
    BDCans PID(Variable var, Int_t nbins, Double_t * bins, std::string pdgvar, std::string cuts = "");
    BDCans PID(Variable var, Int_t nbins, Double_t low, Double_t high, std::string pdgvar, std::string cuts = "");

    BDCans TOPO(Variable var, Int_t nbins, Double_t * bins, std::string cuts = "");
    BDCans TOPO(Variable var, Int_t nbins, Double_t low, Double_t high, std::string cuts = "");
    
    BDCans TARGET(Variable var, Int_t nbins, Double_t * bins, std::string cuts = "");
    BDCans TARGET(Variable var, Int_t nbins, Double_t low, Double_t high, std::string cuts = "");
    
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

    void SetSignal(Topology::Name name = Topology::HCC1P1PiPlus){ m_signal = name; }

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

    Int_t m_statcounter;
    TLegend * RatioStats(const THStack * ratio_tot);   
    void DrawZeroPointLine(TH1 * histo); 
    void DrawZeroPointLine(THStack * histo); 
};
#endif
