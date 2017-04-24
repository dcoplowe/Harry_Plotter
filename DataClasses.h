#include <string>
#include <sstream>
#include "TDatime.h"

using namespace std;

#ifndef _PARTICLE_
#define _PARTICLE_

class Particle {
public:
    Particle(EXP::EXP exp, std::string name, std::string symbol, std::string tag = "");//; : m_tag {};
    ~Particle(){};
    
    //Reco vars: Common:
    std::string P;
    std::string theta;
    std::string pT;
    std::string pTMag;
    std::string pTT;
    std::string startdir;
    std::string endpos;
    
    //True vars: Common:
    std::string trueP;
    std::string truetheta;
    std::string truepT;
    std::string truepTMag;
    std::string truepTT;
    std::string truestartdir;
    std::string trueendpos;
    std::string pdg;
    
    //MIN: Reco vars:
    std::string E;
    std::string P4;
    std::string T;//KE
    std::string startpos;
    std::string michel;
    std::string score;
    std::string phi;
    
    //MIN: True vars:
    std::string trueE;
    std::string trueP4;
    std::string trueT;//KE
    std::string truestartpos;
    std::string truephi;
    
    //T2K: Reco vars:
    std::string ctheta;
    std::string ctheta_nudir;
    
    //T2K: Reco vars:
    std::string truectheta;
    
    std::string GetSymbol(){ return m_symbol; }
    std::string GetName(){ return m_name; }
    std::string GetLabel(){ return m_label; }
    
private:
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
    KinematicVars(EXP::EXP exp);
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
    
};

#endif

#ifndef _TOPOLOGY_
#define _TOPOLOGY_

class Topology {
public:
    enum Name{
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
        Other,
        Unknown = -999;
    };

    Topology(Name name, std::string definition);
    ~Topology();

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

    inline std::string ToString(Name name, int form){
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
    Topologies(Topology::Name signal == Topology::HCC1P1PiPlus);//Contains all the topologies signal defs.
    ~Topologies(){};

    void AddTopology(Topology::Name name, std::string definition);
    void AddSignalToTopology(Topology::Name name, std::string add2def, bool reset = false);
    std::vector<Topology> GetList();

    Topology GetTopolgy(Topology::Name name);

    std::vector<Topology::Name> GetList2Draw(){ return m_list2draw; };
    void Add2DrawList(Topology::Name name){ m_list2draw.push_back(name); };
    void ResetList2Draw(){ m_list2draw.clear(); }

private:
    Topology::Name m_signal;

    Topology m_other; 

    std::vector<Topology> m_original;//This is the list prior to adding the other topology    
    std::vector<Topology::Name> m_list2draw;

};
#endif


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

#ifndef _EXP_
#define _EXP_

namespace EXP {
    enum EXP{
        T2K = 0,
        MIN,
        UNKNOWN
    };
    
    inline std::string ToString(EXP name){
        std::string sname = "Unknown";
        if(name == T2K) sname = "T2K";
        else if(name == MIN) sname = "MINERvA";
        return sname;
    }
}

#endif

#ifndef _EXPERIMENT_
#define _EXPERIMENT_

class Experiment
{
public:
    Experiment(EXP::EXP exp);
    ~Experiment();

    std::string GetName(){ return m_name; }
    std::string GetLogo(){ return m_logo; }

    std::string GetRecoName(){ return m_reconame; }
    std::string GetTrueName(){ return m_truename; }

    std::string GetSignal(){ return m_signal; }
    std::string GetSigDef(){ return m_signaldef; }

    std::string GetBaseCuts(int accum_level, int branch);

    Topologies * GetTopologies(){ m_topologies; }
    std::vector<std::string> GetIntModes(){ m_intmodes; }
    std::vector<std::string> GetCutNames(){ m_cuts; }
    std::string GetTag(){ return m_tag; }

    std::string GetTarVarName(){ return m_tarvarname; }

    //Add kinematic ranges for binning and N bins?

    // enum Name {
    //     T2K = 0,
    //     MIN,
    //     UNKNOWN
    // };

private: 

    std::string m_name;
    std::string m_logo;

    std::string m_reconame;
    std::string m_truename;

    std::string m_signal;
    std::string m_signaldef;

    Topologies * m_topologies;
    std::vector<std::string> m_intmodes;
    std::vector<std::string> m_cuts;
    
    std::string m_tag;
    std:: m_tarvarname;

    void SetTopologies(EXP::EXP exp);
    void SetIntModes(EXP::EXP exp);
    void SetCutNames(EXP::EXP exp);
};

#endif

