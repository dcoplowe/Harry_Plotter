#include <string>
#include "TDatime.h"

using namespace std;

#ifndef _EXPERIMENT_
#define _EXPERIMENT_

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

#ifndef _PARTICLE_
#define _PARTICLE_

class Particle {
public:
    Particle(EXP::EXP exp, std::string name, std::string symbol, std::string tag = "sel");//; : m_tag {};
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
    
private:
    std::string m_tag;
    std::string m_symbol;
};

#endif

#ifndef _PARTICLE_CXX
#define _PARTICLE_CXX

Particle::Particle(EXP::EXP exp, std::string name, std::string symbol, std::string tag) : m_symbol(symbol), m_tag(tag) {
    
    if(exp == EXP::T2K){
        //Reco vars: Common:
        P =         m_tag + name + "_mom";
        pT =        m_tag + name + "_pT";
        pTMag =     m_tag + name + "_pTMag";
        startdir =  m_tag + name + "_dir";
        endpos =    m_tag + name + "_endpos";
        
        //True vars: Common:
        trueP =         m_tag + name + "_truemom";
        truepT =        m_tag + name + "_truepT";
        truepTMag =     m_tag + name + "_truepTMag";
        truestartdir =  m_tag + name + "_truedir";
        trueendpos =    m_tag + name + "_endpos";
        pdg =           m_tag + name + "_pdg";
        
        //MIN: Reco vars:
        E = "";
        P4 = "";
        T = "";//KE
        pTT = "";
        theta = "";
        startpos = "";
        phi = "";
        
        michel = "";
        score = "";
        
        //MIN: True vars:
        trueE = "";
        trueP4 = "";
        trueT = "";//KE
        truepTT = "";
        truetheta = "";
        truestartpos = "";
        truephi = "";
        
        //T2K: Reco vars:
        ctheta =        m_tag + name + "_costheta";
        ctheta_nudir =  m_tag + name + "_nudir_costheta";
        
        //T2K: Reco vars:
        truectheta =    m_tag + name + "_truecostheta";
    }
    else if(exp == EXP::MIN){
        
        std::string tmpname = name;
        if(name.find("LL") != std::string::npos){
            std::string tmpstring = "_LL";
            tmpname.erase( tmpname.find(tmpstring), tmpname.length() );
        }
        else if(name.find("EX") != std::string::npos){
            std::string tmpstring = "_EX";
            tmpname.erase( tmpname.find(tmpstring), tmpname.length() );
        }
        
        //Reco vars: Common:
        P =         m_tag + name + "_mom";
        pT =        m_tag + name + "_pT";
        pTMag =     m_tag + name + "_pTMag";
        startdir =  m_tag + tmpname + "_startdir";
        endpos =    m_tag + tmpname + "_endpos";
        
        //True vars: Common:
        trueP =         m_tag + tmpname + "_truemom";
        truepT =        m_tag + tmpname + "_truepT";
        truepTMag =     m_tag + tmpname + "_truepTMag";
        truestartdir =  m_tag + tmpname + "_truestartdir";
        trueendpos =    m_tag + tmpname + "_endpos";
        pdg =           m_tag + tmpname + "_PDG";
        
        //MIN: Reco vars:
        E =     m_tag + name + "_E";
        P4 =    m_tag + name + "_4mom";
        T =     m_tag + name + "_KE";//KE
        pTT =   m_tag + name + "_pTT";
        theta = m_tag + name + "_Theta";
        phi =   m_tag + name + "_Phi";
        startpos = m_tag + tmpname + "_startpos";
        
        michel =    m_tag + name + "_michel";
        score =     m_tag + name + "_score";
        
        //MIN: True vars:
        trueE =         m_tag + tmpname + "_trueE";
        trueP4 =        m_tag + tmpname + "_true4mom";
        trueT =         m_tag + tmpname + "_trueKE";//KE
        truepTT =       m_tag + tmpname + "_truepTT";
        truetheta =     m_tag + tmpname + "_trueTheta";
        truephi =       m_tag + tmpname + "_truePhi";
        truestartpos =  m_tag + tmpname + "_truestartpos";
        
        //T2K: Reco vars:
        ctheta = "";
        ctheta_nudir = "";
        
        //T2K: Reco vars:
        truectheta = "";
    }
    else{
        //Reco vars: Common:
        P = "";
        pT = "";
        pTMag = "";
        startdir = "";
        endpos = "";
        
        //True vars: Common:
        trueP = "";
        truepT = "";
        truepTMag = "";
        truestartdir = "";
        trueendpos = "";
        pdg = "";
        
        //MIN: Reco vars:
        E = "";
        P4 = "";
        T = "";//KE
        pTT = "";
        theta = "";
        startpos = "";
        phi = "";
        
        michel = "";
        score = "";
        
        //MIN: True vars:
        trueE = "";
        trueP4 = "";
        trueT = "";//KE
        truepTT = "";
        truetheta = "";
        truestartpos = "";
        truephi = "";
        
        //T2K: Reco vars:
        ctheta = "";
        ctheta_nudir = "";
        
        //T2K: Reco vars:
        truectheta = "";
    }
    
}

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
    Particle * muon_alt;
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
    
private:
    
};

#endif

#ifndef _KINEMATICVARS_CXX
#define _KINEMATICVARS_CXX

KinematicVars::KinematicVars(EXP::EXP exp){
    
    if(exp == EXP::T2K){
        muon = new Particle(exp, "mu", "#mu^{-}");
        proton = new Particle(exp, "pr", "p");
        pion = new Particle(exp, "pi", "#pi^{+}");
        
        muon_alt = 0x0;
        proton_alt = 0x0;
        pion_alt = 0x0;
        
        //Transverse Variables:
        dpTT = "sel_dpTT_EX";
        dpT = "sel_dpT_EX";
        dalphaT = "sel_dalphaT_EX";
        dphiT = "sel_dphiT_EX";
        
        truedpTT = "sel_truedpTT";
        truedpT = "sel_truedpT";
        truedalphaT = "sel_truedalphaT";
        truedphiT = "sel_truedphiT";
        
        dpTT_alt = "sel_dpTT_LL";
        dpT_alt = "sel_dpT_LL";
        dalphaT_alt = "sel_dalphaT_LL";
        dphiT_alt = "sel_dphiT_LL";
        
    }
    else if(exp == EXP::MIN){
        
        muon = new Particle(exp, "mu", "#mu^{-}", "sel_");
        proton = new Particle(exp, "pr_EX", "p", "sel_");
        pion = new Particle(exp, "pi_EX", "#pi^{+}", "sel_");
        
        muon_alt = new Particle(exp, "mu_LL", "#mu^{-}", "sel_");
        proton_alt = new Particle(exp, "pr_LL", "p", "sel_");
        pion_alt = new Particle(exp, "pi_LL", "#pi^{+}", "sel_");
        
        //Transverse Variables:
        dpTT = "sel_dpTT_EX";
        dpTT_tmumom = "sel_dpTT_EX_tmumom";
        dpTT_tpimom = "sel_dpTT_EX_tpimom";
        dpTT_tprmom = "sel_dpTT_EX_tprmom";
        dpTT_tnudir = "sel_dpTT_EX_tnudir";
        
        dpTT_pi = "sel_dpTT_pi_EX";
        dpTT_pr = "sel_dpTT_pr_EX";
        
        dpTT_pidir = "sel_dpTT_pi_dir_EX";
        dpTT_prdir = "sel_dpTT_pr_dir_EX";
        
        dpT = "sel_dpT_EX";
        dalphaT = "sel_dalphaT_EX";
        dphiT = "sel_dphiT_EX";
        
        truedpTT = "sel_truedpTT";
        truedpT = "sel_truedpT";
        truedalphaT = "sel_truedalphaT";
        truedphiT = "sel_truedphiT";
        
        dpTT_alt = "sel_dpTT_LL";
        dpTT_tmumom_alt = "sel_dpTT_LL_tmumom";
        dpTT_tpimom_alt = "sel_dpTT_LL_tpimom";
        dpTT_tprmom_alt = "sel_dpTT_LL_tprmom";
        dpTT_tnudir_alt = "sel_dpTT_LL_tnudir";
        
        dpTT_pi_alt = "sel_dpTT_pi_LL";
        dpTT_pr_alt = "sel_dpTT_pr_LL";
        
        dpTT_pidir_alt = "sel_dpTT_pi_dir_LL";
        dpTT_prdir_alt = "sel_dpTT_pr_dir_LL";
        
        dpT_alt = "sel_dpT_LL";
        dalphaT_alt = "sel_dalphaT_LL";
        dphiT_alt = "sel_dphiT_LL";
        
    }
    else{
        //Transverse Variables:
        dpTT = "";
        dpT = "";
        dalphaT = "";
        dphiT = "";
        
        truedpTT = "";
        truedpT = "";
        truedalphaT = "";
        truedphiT = "";
        
        dpTT_alt = "";
        dpT_alt = "";
        dalphaT_alt = "";
        dphiT_alt = "";
    }
}

KinematicVars::~KinematicVars(){
    delete muon;
    delete proton;
    delete pion;
    
    if(muon_alt)    delete muon_alt;
    if(proton_alt)  delete proton_alt;
    if(pion_alt)    delete pion_alt;
    
}

#endif