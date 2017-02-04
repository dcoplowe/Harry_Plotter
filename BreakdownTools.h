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

namespace TOPSTYPE {
    enum TOPSTYPE{
        CC1P1PiPlus,
        CCNPNPiMinus,
        CCNP,
        CCNPiPlus,
        CCNPNPiZero,
        CCNPiZeroNPiPlus,
        Other
    };
    
    inline std::string ToString(TOPSTYPE name, int form){
        std::string sname = "Unknown";
        if(name == CC1P1PiPlus && form == 0) sname = "CC1P1PiPlus";
        else if(name == CC1P1PiPlus && form == 1) sname = "CC1p1#pi^{+}";
        else if(name == CCNPNPiMinus && form == 0) sname = "CCNPNPiMinus";
        else if(name == CCNPNPiMinus && form == 1) sname = "CCNpN#pi^{-}";
        else if(name == CCNP && form == 0) sname = "CCNP";
        else if(name == CCNP && form == 1) sname = "CCNp";
        else if(name == CCNPiPlus && form == 0) sname = "CCNPiPlus";
        else if(name == CCNPiPlus && form == 1) sname = "CCN#pi^{+}";
        else if(name == CCNPNPiZero && form == 0) sname = "CCNPNPiZero";
        else if(name == CCNPNPiZero && form == 1) sname = "CCNPN#pi^{0}";
        else if(name == CCNPiZeroNPiPlus && form == 0) sname = "CCNPiZeroNPiPlus";
        else if(name == CCNPiZeroNPiPlus && form == 1) sname = "CCN#pi^{0}N#pi^{+}";
        else if(name == Other) sname = "Other";
        
        return sname;
    }
    
}

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

#ifndef _TOPOLOGY_
#define _TOPOLOGY_

class TOPS {
public:
    TOPS(TOPSTYPE::TOPSTYPE topo_type);
    ~TOPS(){};
    
    TOPSTYPE::TOPSTYPE type;
    std::string name;
    std::string symbol;
    std::string signal;
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

    BDCans TOPO(Variable var, Int_t nbins, Double_t * bins, std::string cuts = "");
    BDCans TOPO(Variable var, Int_t nbins, Double_t low, Double_t high, std::string cuts = "");
    
    BDCans TARGET(Variable var, Int_t nbins, Double_t * bins, std::string cuts = "");
    BDCans TARGET(Variable var, Int_t nbins, Double_t low, Double_t high, std::string cuts = "");

    void PrintPOT(){ m_printPOT = true; }
    
    void FullBreakDown(){ if(!m_fullbreakdown) m_fullbreakdown = true; else m_fullbreakdown = false;}//Switch on/off breakdown level.
    void SetMinPDGBDlist(Int_t pdg){ m_pdglist_minBD.push_back(pdg); }
    void ClearPDGBDlist(){ m_pdglist_minBD.clear(); }
    void ResetPDGBDlist();
    
    void SetMinTOPBDlist(TOPSTYPE::TOPSTYPE top){ m_toplist_minBD.push_back(top); }
    void ClearTOPBDlist(){ m_toplist_minBD.clear(); }
    void ResetTOPBDlist();
    
    //    void TopVar();
    //void MisPIDVar();
    
private:
    bool m_printPOT;
    bool m_fullbreakdown;
    
    std::vector<PDGs> m_pdglist;
    std::vector<Int_t> m_pdglist_minBD;

    std::vector<TOPS> m_toplist;
    std::vector<TOPSTYPE::TOPSTYPE> m_toplist_minBD;

};
#endif
