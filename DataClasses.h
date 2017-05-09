#include <string>
#include <sstream>
#include "TDatime.h"
#include "DrawingTools.h"

using namespace std;

#ifndef _PRINT_LEVEL_
#define _PRINT_LEVEL_

namespace print_level {
    extern bool debug;// = true;// = true;
    extern bool quiet;
    extern bool effpurtools;
    extern bool drawingtools;
    // extern bool debug;
}

#endif

#ifndef _T2KGEOM_
#define _T2KGEOM_

namespace t2kgeometry {
    extern double fgd1min[3];
    extern double fgd1max[3];
    extern double fgd2min[3];
    extern double fgd2max[3];

    extern double fgd1min_offset[3];
    extern double fgd1max_offset[3];
    extern double fgd2min_offset[3];
    extern double fgd2max_offset[3];

    extern double fgd1tpcmin[3];
    extern double fgd1tpcmax[3];
    extern double fgd2tpcmin[3];
    extern double fgd2tpcmax[3];

    extern double fgd1tpcmin_offset[3];
    extern double fgd1tpcmax_offset[3];
    extern double fgd2tpcmin_offset[3];
    extern double fgd2tpcmax_offset[3];

    extern double fgd1FV[3];
    extern double fgd2FV[3];

    extern double tpcXYmin[2];
    extern double tpcXYmax[2];

    extern double tpc1min[3];
    extern double tpc1max[3];

    extern double tpc2min[3];
    extern double tpc2max[3];

    extern double tpc3min[3];
    extern double tpc3max[3];

    extern double offset;

}

#endif

#ifndef _TOPOLOGY_
#define _TOPOLOGY_

class Topology {
public:
    
    enum Name{
        HCC1P1PiPlus = 0,
        HCC1P1PiPlusOOPS,
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
        Other,
        Unknown = -999
    };

    Topology();
    Topology(Name name, std::string definition);
    ~Topology(){;}

    void AddToSignal(std::string add2def, bool reset = false);

    void Reset();

    Name GetType(){ return m_type; }
    std::string GetName(){ return m_name; }
    std::string GetSymbol(){ return m_symbol; }
    std::string GetSignal(){ return m_signal; }
    Int_t GetFillColor(){ return m_fill_colour; }
    Int_t GetFillStyle(){ return m_fill_style; }
    Int_t GetLineColor(){ return m_line_colour; }
    Int_t GetLineStyle(){ return m_line_style; }

    static inline std::string ToString(Name name, int form = 0){
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

private:
    Name m_type;
    std::string m_name;
    std::string m_symbol;
    std::string m_signal;
    Int_t  m_fill_colour;
    Int_t  m_fill_style;
    Int_t  m_line_colour;
    Int_t  m_line_style;
};

#endif

#ifndef _TOPOLOGIES_
#define _TOPOLOGIES_

class Topologies {
public:
    Topologies(Topology::Name signal = Topology::HCC1P1PiPlus);//Contains all the topologies signal defs.
    ~Topologies();

    Topologies(Topologies * topologies);

    void AddTopology(Topology::Name name, std::string definition);
    void AddSignalToTopology(Topology::Name name, std::string add2def, bool reset = false);
    std::vector<Topology> GetList();

    Topology GetTopology(Topology::Name name);
    
    Topology::Name GetSignal(){ return m_signal; }    

    std::vector<Topology::Name> GetList2Draw(){ return m_list2draw; };
    void Add2DrawList(Topology::Name name){ m_list2draw.push_back(name); };
    void ResetList2Draw(){ m_list2draw.clear(); }

    std::vector<Topology> GetOriginal(){ return m_original; }

private:
    Topology::Name m_signal;

    Topology m_other; 

    std::vector<Topology> m_original;//This is the list prior to adding the other topology    
    std::vector<Topology::Name> m_list2draw;

};
#endif


#ifndef _PARTPDGInfo_
#define _PARTPDGInfo_

class PDGInfo {
public:
    PDGInfo() : m_pdg(-999), m_name(""), m_symbol(""), m_pdg_s(""), m_colour(1), m_line_colour(1), m_line_style(1) {;}
    PDGInfo(Int_t part_pdg, std::string part_name, std::string part_symbol);
    ~PDGInfo(){};

    Int_t GetPDGNo(){ return m_pdg; }
    std::string GetName(){ return m_name;}
    std::string GetSymbol(){ return m_symbol;}
    std::string GetPDGStr(){ return m_pdg_s;}
    Int_t  GetColor(){ return m_colour; }//Fill colour}
    Int_t  GetLineColor(){ return m_line_colour;}
    Int_t  GetLineStyle(){ return m_line_style; }

private:    
    Int_t m_pdg;
    std::string m_name;
    std::string m_symbol;
    std::string m_pdg_s;
    Int_t  m_colour;//Fill colour
    Int_t  m_line_colour;
    Int_t  m_line_style;
};

#endif

// #ifndef _EXP_
// #define _EXP_

// namespace EXP {
//     enum EXP{
//         T2K = 0,
//         MIN,
//         UNKNOWN
//     };
    
//     inline std::string ToString(EXP name){
//         std::string sname = "Unknown";
//         if(name == T2K) sname = "T2K";
//         else if(name == MIN) sname = "MINERvA";
//         return sname;
//     }
// }
// 
// #endif

#ifndef _EXPERIMENT_
#define _EXPERIMENT_

class Experiment
{
public:
    enum Name {
        T2K = 0,
        MIN,
        UNKNOWN
    };

    Experiment(Name exp);
    ~Experiment();
    
    static inline std::string ToString(Experiment::Name name){
        std::string sname = "Unknown";
        if(name == T2K) sname = "T2K";
        else if(name == MIN) sname = "MINERvA";
        return sname;
    }

    Name GetType(){ return m_type; }
    std::string GetName(){ return m_name; }
    std::string GetLogo(){ return m_logo; }

    std::string GetRecoName(){ return m_reconame; }
    std::string GetTrueName(){ return m_truename; }

    std::string GetSignal(){ return m_signal.GetSignal(); }
    std::string GetSigDef(){ return m_signaldef; }

    std::string GetBaseCuts(int accum_level, int branch, std::string cuts = "");

    Topologies * GetTopologies(){ return m_topologies; }
    std::vector<std::string> GetIntModes(){ return m_intmodes; }
    std::vector<std::string> GetCutNames(){ return m_cuts; }
    std::string GetTag(){ return m_tag; }

    std::string GetTarVarName(){ return m_tarvarname; }

    std::string GetWgt(){ return m_weight; }

private: 

    Name m_type;

    std::string m_name;
    std::string m_logo;

    std::string m_reconame;
    std::string m_truename;

    Topology m_signal;
    std::string m_signaldef;

    Topologies * m_topologies;
    std::vector<std::string> m_intmodes;
    std::vector<std::string> m_cuts;
    
    std::string m_tag;
    std::string m_tarvarname;

    std::string m_weight;

    void SetTopologies(Name exp);
    void SetIntModes(Name exp);
    void SetCutNames(Name exp);
};

#endif

#ifndef _VARIABLE_
#define _VARIABLE_

class Variable {

public: 
    Variable() : m_name(""), m_symbol(""), m_units(""), m_savename(""), m_pdg(""){;}
    Variable(std::string name) : m_name(name), m_symbol(name), m_units(""), m_savename(name), m_pdg(""){;}
    Variable(std::string name, std::string symbol, std::string units) : m_name(name), m_symbol(symbol), m_units(units), m_savename(""), m_pdg(""){;}
    Variable(std::string name, int nbins, Double_t low, Double_t high, std::string symbol= "", std::string units = "", std::string savename = "", std::string pdg = "") :
        m_name(name), m_symbol(symbol), m_units(units), m_savename(savename), m_pdg(pdg), m_nbins(nbins), m_binning( DrawingTools::SetBinning(m_nbins, low, high) ) {;}

    Variable(Variable &input){ 
        m_name = input.GetName();
        m_symbol = input.GetSymbol();
        m_units = input.GetUnits();
        m_savename = input.GetSName();
        m_pdg = input.GetPDG();
        m_nbins = input.GetNBins();
        m_binning = input.GetBinnin();
    }
    
    ~Variable(){;}//delete m_binning;}

    void SetName(std::string var){ m_name = var; }
    void SetSymbol(std::string var){ m_symbol = var; }
    void SetUnits(std::string var){ m_units = var; }
    void SetPDG(std::string var){ m_pdg = var; }
    void SetSName(std::string var){ m_savename = var; }
    
    std::string GetName(){ return m_name; }
    std::string GetSymbol(){ return m_symbol; }
    std::string GetUnits(){ return m_units; }
    std::string GetSName(){ return m_savename; }
    std::string GetPDG(){ return m_pdg; }
    std::string GetAxisTitle(){ return m_symbol + (m_units.empty() ? "" : " (" + m_units + ")"); }

    void SetNBins(int var){ m_nbins = var;}
    void SetBinning(int nbins, Double_t * binning){ m_nbins = nbins; m_binning = binning; }

    void SetBinning(int nbins, Double_t low, Double_t high){ m_nbins = nbins; m_binning = DrawingTools::SetBinning(m_nbins, low, high); }
    int GetNBins(){ return m_nbins; }
    Double_t * GetBinning(){ return m_binning; }

private:
    std::string m_name;
    std::string m_symbol;
    std::string m_units;
    std::string m_savename;
    std::string m_pdg;

    Int_t m_nbins;
    Double_t * m_binning;
};

#endif

#ifndef _PARTICLE_
#define _PARTICLE_

class Particle {
public:
    Particle(Experiment::Name exp, std::string name, std::string tag = "");//; : m_tag {};
    ~Particle(){};

    enum PDG {
        Proton = 2212,
        PionP = 211,
        MuonM = 13,
        Unknown = -999
    };

    PDGInfo info;

    //Reco vars: Common:
    Variable P;
    Variable theta;
    Variable pT;
    Variable pTMag;
    Variable pTT;
    Variable startdir;
    Variable startpos;
    Variable endpos;
    
    //True vars: Common:
    Variable trueP;
    Variable truetheta;
    Variable truepT;
    Variable truepTMag;
    Variable truepTT;
    Variable truestartdir;
    Variable trueendpos;
    Variable pdg;
    Variable truestartpos;
    
    //MIN: Reco vars:
    Variable E;
    Variable P4;
    Variable T;//KE
    Variable michel;
    Variable score;
    Variable phi;
    
    //MIN: True vars:
    Variable trueE;
    Variable trueP4;
    Variable trueT;//KE
    Variable truephi;
    
    //T2K: Reco vars:
    Variable ctheta;
    Variable ctheta_nudir;
    Variable cross_angle;
    Variable fgd_start;
    
    //T2K: True vars:
    Variable truectheta;
    // Variable truecrossing_angle;
    
    std::string GetSymbol(){ return m_symbol; }
    std::string GetName(){ return m_name; }
    std::string GetLabel(){ return m_label; }
    Particle::PDG GetPDG(){ return m_pdg; }
    bool IsPDG(Particle::PDG var){ return (var == m_pdg); }
    
private:
    Particle::PDG m_pdg; 

    std::string m_tag;
    std::string m_symbol;
    std::string m_name;
    std::string m_label;
};

#endif

#ifndef _KINEMATICVARS_
#define _KINEMATICVARS_

class KinematicVars {
public:
    KinematicVars(Experiment::Name exp);
    ~KinematicVars();
    
    Particle * muon;
    Particle * proton;
    Particle * pion;
    
    //For alt mom. reco:
    // Particle * muon_alt;
    Particle * proton_alt;
    Particle * pion_alt;
    
    //Transverse Variables:
    std::string dpTT;
    std::string dpTT_tmumom;
    std::string dpTT_tpimom;
    std::string dpTT_tprmom;
    std::string dpTT_tnudir;
    
    std::string dpTT_pi;
    std::string dpTT_pr;
    
    std::string dpTT_pidir;
    std::string dpTT_prdir;
    
    std::string dpT;
    std::string dalphaT;
    std::string dphiT;
    
    std::string truedpTT;
    std::string truedpT;
    std::string truedalphaT;
    std::string truedphiT;
    
    //Vars for LL/dEdX comp.
    
    std::string dpTT_alt;
    std::string dpTT_tmumom_alt;
    std::string dpTT_tpimom_alt;
    std::string dpTT_tprmom_alt;
    std::string dpTT_tnudir_alt;
    
    std::string dpTT_pi_alt;
    std::string dpTT_pr_alt;
    
    std::string dpTT_pidir_alt;
    std::string dpTT_prdir_alt;
    
    std::string dpT_alt;
    std::string dalphaT_alt;
    std::string dphiT_alt;
    
    Variable vtx_pos;
    Variable truevtx_pos;
    // Variable W_mass;

};

#endif
