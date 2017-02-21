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
    std::string pdg;
};

namespace TOPSTYPE {
    enum TOPSTYPE{
        HCC1P1PiPlus = 0,
        CC1P1PiPlus,
        CCNPNPiMinus,
        CCNP,
        CCNPiPlus,
        CCNPNPiZero,
        CCNPiZeroNPiPlus,
        //Added topos (200217):
        CCKaonsOth,
        CCNN,//N Neutrons
        CCNPNN,//N Protons, N Neutrons
        CCNPiNN, //N pions, N Neutrons
        CCNPiNPNN, //N pions, N Protons N Neutrons
        //CCNucBreakUp, -- may be nice in the future
        Other
    };
    
    inline std::string ToString(TOPSTYPE name, int form){
        std::string sname = "Unknown";
        if(name == HCC1P1PiPlus)            sname = (form == 0) ? "HCC1P1PiPlus"        : "CC1p1#pi^{+} on H";
        else if(name == CC1P1PiPlus)        sname = (form == 0) ? "CC1P1PiPlus"         : "CC1p1#pi^{+}";
        else if(name == CCNPNPiMinus)       sname = (form == 0) ? "CCNPNPiMinus"        : "CCNpN#pi^{-}";
        else if(name == CCNP)               sname = (form == 0) ? "CCNP"                : "CCNp";
        else if(name == CCNPiPlus)          sname = (form == 0) ? "CCNPiPlus"           : "CCN#pi^{+}";
        else if(name == CCNPNPiZero)        sname = (form == 0) ? "CCNPNPiZero"         : "CCNPN#pi^{0}";
        else if(name == CCNPiZeroNPiPlus)   sname = (form == 0) ? "CCNPiZeroNPiPlus"    : "CCN#pi^{0}N#pi^{+}";
        else if(name == CCKaonsOth)         sname = (form == 0) ? "CCKaonsOth"          : "CC Kaons Inc.";
        else if(name == CCNN)               sname = (form == 0) ? "CCNN"                : "CCNn";
        else if(name == CCNPNN)             sname = (form == 0) ? "CCNPNN"              : "CCNpNn";
        else if(name == CCNPiNN)            sname = (form == 0) ? "CCNPiNN"             : "CCN#piNn";
        else if(name == CCNPiNPNN)          sname = (form == 0) ? "CCNPiPNNN"           : "CCN#piNpNn";
        else if(name == Other)              sname = "Other";
        
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
    Int_t  colour;//Fill colour
    Int_t  line_colour;
    Int_t  line_style;
};
#endif

#ifndef _TOPOLOGY_
#define _TOPOLOGY_

class TOPS {
public:
    TOPS(TOPSTYPE::TOPSTYPE topo_type);//Contains all the topologies signal defs.
    ~TOPS(){};
    
    TOPSTYPE::TOPSTYPE type;
    std::string name;
    std::string symbol;
    std::string signal;
    Int_t  fill_colour;
    Int_t  fill_style;
    Int_t  line_colour;
    Int_t  line_style;
    
    void AddToOther(TOPSTYPE::TOPSTYPE topo_type);
    
private:
    bool is_Other;
    
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
    
    TCanvas * TARGETSingle(Variable var, Int_t nbins, Double_t * bins, std::string cuts = "");
    TCanvas * TARGETSingle(Variable var, Int_t nbins, Double_t low, Double_t high, std::string cuts = "");

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
