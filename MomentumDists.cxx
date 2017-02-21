#include "TCanvas.h"
#include "TObject.h"

//#include "TDirectory.h"
//#include "TROOT.h"
#include "TClass.h"
#include <TCanvas.h>

#include <string>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <assert.h>

#include <stdio.h>

#include "EffPurTools.h"
#include "DrawingTools.h"
#include "BreakdownTools.h"

#include "TH1D.h"
#include "TH2D.h"
#include "THStack.h"
#include "TLegend.h"
#include "TFile.h"
#include "TDatime.h"
#include "TLatex.h"
#include "TDirectory.h"
#include "TMath.h"

//#include <TStyle.h>

using namespace std;

const string testing_mc("/pnfs/minerva/persistent/users/dcoplowe/CC1P1Pi_PL13C_080117_1/grid/central_value/minerva/ana/v10r8p9/00/01/32/00/SIM_minerva_00013200_Subruns_0001-0002-0003-0004_CC1P1PiAnalysis_Ana_Tuple_v10r8p9-dcoplowe.root");


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

#ifndef _PARTICLE_
#define _PARTICLE_

class Particle {
public:
    Particle(EXP::EXP exp, std::string name, std::string tag = "sel");//; : m_tag {};
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
    
private:
    std::string m_tag;
};

#endif

#ifndef _PARTICLE_CXX
#define _PARTICLE_CXX

Particle::Particle(EXP::EXP exp, std::string name, std::string tag) : m_tag(tag) {
    
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
        muon = new Particle(exp, "mu");
        proton = new Particle(exp, "pr");
        pion = new Particle(exp, "pi");
        
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
        
        muon = new Particle(exp, "mu", "sel_");
        proton = new Particle(exp, "pr_EX", "sel_");
        pion = new Particle(exp, "pi_EX", "sel_");
        
        muon_alt = new Particle(exp, "mu_LL", "sel_");
        proton_alt = new Particle(exp, "pr_LL", "sel_");
        pion_alt = new Particle(exp, "pi_LL", "sel_");
        
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

class MomentumDists {

public:
    MomentumDists(EXP::EXP exp, std::string filename, bool debug);
    ~MomentumDists();
    
    void SetSaveName(std::string var){ m_savename = var; }
    void SetTrueTreeName(std::string var){ m_truename = var; }
    void SetRecoTreeName(std::string var){ m_reconame = var; }
    
//    void ProduceGroup();

    void MakePlots();
    
    TLatex * GetLogo();
    void PrintLogo(TCanvas *& can);
    
    void ProduceGroup(Variable var, Int_t nbins, Double_t * bins, std::string cuts);
    void ProduceGroup(Variable var, Int_t nbins, Double_t low, Double_t high, std::string cuts);
    
    void MakeDir(std::string name);
    void cdDir(std::string name = ""){  m_outfile->cd((m_savename + ":/" + name).c_str()); }//Default is the root dir.
    
private:
    EXP::EXP m_experiment;
    std::string m_infilename;
    
    KinematicVars * m_recovars;
    
    std::string m_savename;
    
    TFile * m_outfile;// = new TFile(m_savename.c_str(), "RECREATE");

    std::string m_truename;
    std::string m_reconame;
    std::string m_exper_logo;
    
    std::string GetDate();
    
    //tmp POT bool (off for t2k):
    bool m_getPOT;
    
//    TFile * m_outfile;
    
    Particle * m_muon;
    Particle * m_proton;
    Particle * m_pion;
    
    Particle * m_muon_alt;
    Particle * m_proton_alt;
    Particle * m_pion_alt;
    
    BreakdownTools * m_run;
};

MomentumDists::MomentumDists(EXP::EXP exp, std::string filename, bool debug) : m_experiment(exp), m_infilename(filename) {
    
    cout << "Experiment: " <<  EXP::ToString(exp) << endl;
    
    m_exper_logo = EXP::ToString(exp);
    
    if(exp == EXP::T2K){
        m_truename = "truth";
        m_reconame = "default";
        m_getPOT = false;
        m_exper_logo += " Work In Progress";
    }
    else if(exp == EXP::MIN){
        
        m_truename = "Truth";
        m_reconame = "sel";
        m_getPOT = true;
        m_exper_logo += " Preliminary";
    }
//    else{
//        
//    }
    
    m_recovars = new KinematicVars(exp);//Setup reco var names
    //For truth tree seems like all we need is the following form: 'truth_pi_E'
    
    m_muon = m_recovars->muon;
    m_proton = m_recovars->proton;
    m_pion = m_recovars->pion;
    
    if(exp == EXP::MIN){
        m_muon_alt = m_recovars->muon_alt;
        m_proton_alt = m_recovars->proton_alt;
        m_pion_alt = m_recovars->pion_alt;
    }
    
    m_savename = EXP::ToString(exp) + "_CC1P1PiPlus_Plots_" + GetDate() + ".root";
    
    cout << "Saving file as " << m_savename << endl;
    
    m_run = new BreakdownTools(m_infilename, m_reconame);
    
    m_outfile = new TFile(m_savename.c_str(), "RECREATE");
}

MomentumDists::~MomentumDists(){
    
    if(m_outfile){
        if(m_outfile->IsOpen()) m_outfile->Close();
        delete m_outfile;
    }
    
    delete m_recovars;
    delete m_run;
}

void MomentumDists::PrintLogo(TCanvas *& can){
    can->cd();
    TLatex * logo = GetLogo();
    logo->Draw();
}

void MomentumDists::ProduceGroup(Variable var, Int_t nbins, Double_t low, Double_t high, std::string cuts){
    ProduceGroup(var, nbins, m_run->SetBinning(nbins, low, high), cuts);
}

void MomentumDists::ProduceGroup(Variable var, Int_t nbins, Double_t * bins, std::string cuts){//, Int_t setsave = 11111??

    if(m_outfile->IsOpen()){
        
        BDCans var_top = m_run->TOPO(var, nbins, bins, cuts);
        BDCans var_tar = m_run->TARGET(var, nbins, bins, cuts);
        
        BDCans var_pid;
        if(!var.pdg.empty()) var_pid = m_run->PID(var, nbins, bins, var.pdg, cuts);
        
        //Recon Vars:
        PrintLogo(var_top.recon);
        PrintLogo(var_tar.recon);
        if(!var.pdg.empty()) PrintLogo(var_pid.recon);
        
        var_top.recon->Write();
        var_tar.recon->Write();
        if(!var.pdg.empty()) var_pid.recon->Write();
        
        //Truth Vars:
        PrintLogo(var_top.truth);
        PrintLogo(var_tar.truth);
        if(!var.pdg.empty()) PrintLogo(var_pid.truth);
        
        var_top.truth->Write();
        var_tar.truth->Write();
        if(!var.pdg.empty()) var_pid.truth->Write();
        
        //Ratio Vars:
        PrintLogo(var_top.ratio);
        PrintLogo(var_tar.ratio);
        if(!var.pdg.empty()) PrintLogo(var_pid.ratio);
        
        var_top.ratio->Write();
//        var_tar.ratio->Write();
//        if(!var.pdg.empty()) var_pid.ratio->Write();
        
        //Smear Vars:
        PrintLogo(var_top.smear);
//        PrintLogo(var_tar.smear);
//        if(!var.pdg.empty()) PrintLogo(var_pid.smear);
        
        PrintLogo(var_top.smearSN);
//        PrintLogo(var_tar.smearSN);
//        if(!var.pdg.empty()) PrintLogo(var_pid.smearSN);
        
        var_top.smear->Write();
//        var_tar.smear->Write();
//        if(!var.pdg.empty()) var_pid.smear->Write();
        
        var_top.smearSN->Write();
//        var_tar.smearSN->Write();
//        if(!var.pdg.empty()) var_pid.smearSN->Write();
        
    }
    else std::cout << "MomentumDists::ProduceGroup : ERROR : File is not open..." << std::endl;
    
}

void MomentumDists::MakeDir(std::string name){
    if(m_outfile->IsOpen()){
        cdDir();
        TDirectory * tmp_dir = m_outfile->GetDirectory(name.c_str());
        if (!tmp_dir) {
            m_outfile->mkdir(name.c_str());
            cdDir(name);
//            m_outfile->cd((m_savename + ":/" + name).c_str());
        }
        else{
            cout << "MomentumDists::MakeDir : Directory exists, entering : " << name << endl;
            cdDir(name);
        }
    }
    else cout << "MomentumDists::MakeDir : File is closed..." << endl;
}

void MomentumDists::MakePlots(){
    
    //dEdX (0) and LL mom. (1) reco plots:
    //W/WO michel tag.
    
    //Pass all cuts check://Only consider dEdX ( accum_level[0]) recon for now:
//    string base_dir = "dEdX";

    string EX_base_cut = "accum_level[0] > 5 && " + m_pion->michel + " == 1";
    string LL_base_cut = "accum_level[1] > 5 && " + m_pion_alt->michel + " == 1";//Alt michel too
    //**************************************** Mom START ****************************************//
    
    Variable mom;
    mom.units = "MeV/#it{c}";

    //Proton:
    MakeDir("Mom/dEdX/Proton");
    mom.name = m_proton->trueP + ":" + m_proton->P;
    mom.symbol = "#it{p}_{p}";
    mom.savename = m_proton->P;
    mom.pdg = m_proton->pdg;
    ProduceGroup(mom, 40, 0, 2000, EX_base_cut);
    
    //Pion:
    MakeDir("Mom/dEdX/Pion");
    mom.name = m_pion->trueP + ":" + m_pion->P;
    mom.symbol = "#it{p}_{#pi}";
    mom.savename = m_pion->P;
    mom.pdg = m_pion->pdg;
    ProduceGroup(mom, 40, 0, 2000, EX_base_cut);
    
    //Muon:
    MakeDir("Mom/dEdX/Muon");
    mom.name = m_muon->trueP + ":" + m_muon->P;
    mom.symbol = "#it{p}_{#mu}";
    mom.savename = m_muon->P;
    mom.pdg = m_muon->pdg;
    ProduceGroup(mom, 40, 0, 2000, EX_base_cut);
    
    //Proton:
    MakeDir("Mom/LL/Proton");
    mom.name = m_proton_alt->trueP + ":" + m_proton_alt->P;
    mom.symbol = "#it{p}_{p}";
    mom.savename = m_proton_alt->P;
    mom.pdg = m_proton_alt->pdg;
    ProduceGroup(mom, 40, 0, 2000, LL_base_cut);
    
    //Pion:
    MakeDir("Mom/LL/Pion");
    mom.name = m_pion_alt->trueP + ":" + m_pion_alt->P;
    mom.symbol = "#it{p}_{#pi}";
    mom.savename = m_pion_alt->P;
    mom.pdg = m_pion_alt->pdg;
    ProduceGroup(mom, 40, 0, 2000, LL_base_cut);
    
//    //Muon:
//    MakeDir("Mom/LL/Muon");
//    mom.name = m_muon_alt->trueP + ":" + m_muon_alt->P;
//    mom.symbol = "#it{p}_{#mu}";
//    mom.savename = m_muon_alt->P;
//    mom.pdg = m_muon_alt->pdg;
//    ProduceGroup(mom, 40, 0, 2000, LL_base_cut);
    
    //**************************************** Mom END ****************************************//
    //*****************************************************************************************//
    //*****************************************************************************************//
    //*****************************************************************************************//
    //**************************************** Theta START ************************************//
    
    Variable theta;
//    theta.units = "degrees";
    theta.units = "rad";

    //Proton:
    MakeDir("Theta/dEdX/Proton");
//    theta.name = m_proton->trueP + "*TMath::RadToDeg():" + m_proton->P + "*TMath::RadToDeg()";//This probably wont work as the code looks for :: to make a split... Add fix.
    theta.name = m_proton->trueP + ":" + m_proton->P;//This probably wont work as the code looks for :: to make a split... Add fix.
    theta.symbol = "#theta_{p}";
    theta.savename = m_proton->P;
    theta.pdg = m_proton->pdg;
    ProduceGroup(theta, 40, 0, 2.*TMath::Pi(), EX_base_cut);
    
    //Pion:
    MakeDir("Theta/dEdX/Pion");
//    theta.name = m_pion->trueP + "*TMath::RadToDeg():" + m_pion->P + "*TMath::RadToDeg()";
    theta.name = m_pion->trueP + ":" + m_pion->P;//This probably wont work as the code looks for :: to make a split... Add fix.
    theta.symbol = "#theta_{#pi}";
    theta.savename = m_pion->P;
    theta.pdg = m_pion->pdg;
    ProduceGroup(theta, 40, 0, 2.*TMath::Pi(), EX_base_cut);
    
    //Muon:
    MakeDir("Theta/dEdX/Muon");
//    theta.name = m_muon->trueP + "*TMath::RadToDeg():" + m_muon->P + "*TMath::RadToDeg()";
    theta.name = m_muon->trueP + ":" + m_muon->P;//This probably wont work as the code looks for :: to make a split... Add fix.
    theta.symbol = "#theta_{#mu}";
    theta.savename = m_muon->P;
    theta.pdg = m_muon->pdg;
    ProduceGroup(theta, 40, 0, 2.*TMath::Pi(), EX_base_cut);
    
    //Proton:
    MakeDir("Theta/LL/Proton");
    //    theta.name = m_proton->trueP + "*TMath::RadToDeg():" + m_proton->P + "*TMath::RadToDeg()";//This probably wont work as the code looks for :: to make a split... Add fix.
    theta.name = m_proton_alt->trueP + ":" + m_proton_alt->P;//This probably wont work as the code looks for :: to make a split... Add fix.
    theta.symbol = "#theta_{p}";
    theta.savename = m_proton_alt->P;
    theta.pdg = m_proton_alt->pdg;
    ProduceGroup(theta, 40, 0, 2.*TMath::Pi(), LL_base_cut);
    
    //Pion:
    MakeDir("Theta/LL/Pion");
    //    theta.name = m_pion->trueP + "*TMath::RadToDeg():" + m_pion->P + "*TMath::RadToDeg()";
    theta.name = m_pion_alt->trueP + ":" + m_pion_alt->P;//This probably wont work as the code looks for :: to make a split... Add fix.
    theta.symbol = "#theta_{#pi}";
    theta.savename = m_pion_alt->P;
    theta.pdg = m_pion_alt->pdg;
    ProduceGroup(theta, 40, 0, 2.*TMath::Pi(), LL_base_cut);
    
//    //Muon:
//    MakeDir("Theta/LLMuon");
//    //    theta.name = m_muon->trueP + "*TMath::RadToDeg():" + m_muon->P + "*TMath::RadToDeg()";
//    theta.name = m_muon_alt->trueP + ":" + m_muon_alt->P;//This probably wont work as the code looks for :: to make a split... Add fix.
//    theta.symbol = "#theta_{#mu}";
//    theta.savename = m_muon_alt->P;
//    theta.pdg = m_muon_alt->pdg;
//    ProduceGroup(theta, 40, 0, 2.*TMath::Pi(), LL_base_cut);
    
    //**************************************** Theta END **************************************//
    //*****************************************************************************************//
    //*****************************************************************************************//
    //*****************************************************************************************//
    //**************************************** Phi START ************************************//
//    
    Variable phi;
    //    phi.units = "degrees";
    phi.units = "rad";
    
    //Proton:
    MakeDir("Phi/dEdX/Proton");
    //    phi.name = m_proton->trueP + "*TMath::RadToDeg():" + m_proton->P + "*TMath::RadToDeg()";//This probably wont work as the code looks for :: to make a split... Add fix.
    phi.name = m_proton->truephi + ":" + m_proton->phi;//This probably wont work as the code looks for :: to make a split... Add fix.
    phi.symbol = "#phi_{p}";
    phi.savename = m_proton->phi;
    phi.pdg = m_proton->pdg;
    ProduceGroup(mom, 40, 0, 2.*TMath::Pi(), EX_base_cut);
    
    //Pion:
    MakeDir("Phi/dEdX/Pion");
//    mom.name = m_pion->truephi + "*TMath::RadToDeg():" + m_pion->phi + "*TMath::RadToDeg()";
    phi.name = m_pion->truephi + ":" + m_pion->phi;//This probably wont work as the code looks for :: to make a split... Add fix.
    phi.symbol = "#phi_{#pi}";
    phi.savename = m_pion->phi;
    phi.pdg = m_pion->pdg;
    ProduceGroup(phi, 40, 0, 2.*TMath::Pi(), EX_base_cut);
    
    //Muon:
//    MakeDir("Phi/dEdX/Muon");
////    phi.name = m_muon->truephi + "*TMath::RadToDeg():" + m_muon->phi + "*TMath::RadToDeg()";
//    phi.name = m_muon->truephi + ":" + m_muon->phi;//This probably wont work as the code looks for :: to make a split... Add fix.
//    phi.symbol = "#theta_{#mu}";
//    phi.savename = m_muon->phi;
//    phi.pdg = m_muon->pdg;
//    ProduceGroup(phi, 40, 0, 2.*TMath::Pi(), EX_base_cut);
    
    //Proton:
    MakeDir("Phi/LL/Proton");
    //    phi.name = m_proton->trueP + "*TMath::RadToDeg():" + m_proton->P + "*TMath::RadToDeg()";//This probably wont work as the code looks for :: to make a split... Add fix.
    phi.name = m_proton_alt->truephi + ":" + m_proton_alt->phi;//This probably wont work as the code looks for :: to make a split... Add fix.
    phi.symbol = "#phi_{p}";
    phi.savename = m_proton_alt->phi;
    phi.pdg = m_proton_alt->pdg;
    ProduceGroup(mom, 40, 0, 2.*TMath::Pi(), LL_base_cut);
    
    //Pion:
    MakeDir("Phi/LL/Pion");
    //    mom.name = m_pion->truephi + "*TMath::RadToDeg():" + m_pion->phi + "*TMath::RadToDeg()";
    phi.name = m_pion_alt->truephi + ":" + m_pion_alt->phi;//This probably wont work as the code looks for :: to make a split... Add fix.
    phi.symbol = "#phi_{#pi}";
    phi.savename = m_pion_alt->phi;
    phi.pdg = m_pion_alt->pdg;
    ProduceGroup(phi, 40, 0, 2.*TMath::Pi(), LL_base_cut);
    
    //Muon:
//    MakeDir("Phi/LL/Muon");
//    //    phi.name = m_muon->truephi + "*TMath::RadToDeg():" + m_muon->phi + "*TMath::RadToDeg()";
//    phi.name = m_muon_alt->truephi + ":" + m_muon_alt->phi;//This probably wont work as the code looks for :: to make a split... Add fix.
//    phi.symbol = "#theta_{#mu}";
//    phi.savename = m_muon_alt->phi;
//    phi.pdg = m_muon_alt->pdg;
//    ProduceGroup(phi, 40, 0, 2.*TMath::Pi(), LL_base_cut);
//
//    //**************************************** Theta END **************************************//
//    //*****************************************************************************************//
//    //*****************************************************************************************//
//    //*****************************************************************************************//
//    //**************************************** DPTT START ************************************//
//    
    Variable dpTT;
    dpTT.units = "MeV/#it{c}";
    dpTT.symbol = "#deltap_{TT}";
    dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT;
    
    MakeDir("dpTT/dEdX/normal");
    dpTT.savename = m_recovars->dpTT;// + "300";
    ProduceGroup(dpTT, 39, -300, 300, EX_base_cut);
   
    MakeDir("dpTT/dEdX/pm1000");
    dpTT.savename = m_recovars->dpTT + "1000";
    ProduceGroup(dpTT, 99, -1000, 1000, (EX_base_cut + " && " + m_recovars->dpTT + " != -999."));
    
    MakeDir("dpTT/dEdX/pm7000");
    dpTT.savename = m_recovars->dpTT + "7000";
    ProduceGroup(dpTT, 199, -7000, 7000, (EX_base_cut + " && " + m_recovars->dpTT + " != -999."));

    MakeDir("dpTT/dEdX/tmumom");
    dpTT.symbol = "#deltap_{TT}";
    dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_tmumom;
    dpTT.savename = m_recovars->dpTT_tmumom;
    ProduceGroup(dpTT, 39, -300, 300, EX_base_cut);
    
    MakeDir("dpTT/dEdX/tmumom");
    dpTT.symbol = "#deltap_{TT}";
    dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_tpimom;
    dpTT.savename = m_recovars->dpTT_tpimom;
    ProduceGroup(dpTT, 39, -300, 300, EX_base_cut);
    
    MakeDir("dpTT/dEdX/tprmom");
    dpTT.symbol = "#deltap_{TT}";
    dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_tprmom;
    dpTT.savename = m_recovars->dpTT_tprmom;
    ProduceGroup(dpTT, 39, -300, 300, EX_base_cut);
    
    MakeDir("dpTT/dEdX/tnudir");
    dpTT.symbol = "#deltap_{TT}";
    dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_tnudir;
    dpTT.savename = m_recovars->dpTT_tnudir;
    ProduceGroup(dpTT, 39, -300, 300, EX_base_cut);
    
    MakeDir("dpTT/dEdX/pimom");
    dpTT.symbol = "#deltap_{TT}";
    dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_pi;
    dpTT.savename = m_recovars->dpTT_pi;
    ProduceGroup(dpTT, 39, -300, 300, EX_base_cut);
    
    MakeDir("dpTT/dEdX/prmom");
    dpTT.symbol = "#deltap_{TT}";
    dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_pr;
    dpTT.savename = m_recovars->dpTT_pr;
    ProduceGroup(dpTT, 39, -300, 300, EX_base_cut);
    
    MakeDir("dpTT/dEdX/pidir");
    dpTT.symbol = "#deltap_{TT}";
    dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_pidir;
    dpTT.savename = m_recovars->dpTT_pidir;
    ProduceGroup(dpTT, 39, -300, 300, EX_base_cut);
    
    MakeDir("dpTT/dEdX/prdir");
    dpTT.symbol = "#deltap_{TT}";
    dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_prdir;
    dpTT.savename = m_recovars->dpTT_prdir;
    ProduceGroup(dpTT, 39, -300, 300, EX_base_cut);
    
    //LL Method
    MakeDir("dpTT/LL");
    dpTT.savename = m_recovars->dpTT_alt;// + "300";
    ProduceGroup(dpTT, 39, -300, 300, EX_base_cut);
    
    MakeDir("dpTT/LL/pm1000");
    dpTT.savename = m_recovars->dpTT_alt + "1000";
    ProduceGroup(dpTT, 99, -1000, 1000, (LL_base_cut + " && " + m_recovars->dpTT_alt + " != -999."));
    
    MakeDir("dpTT/LL/pm7000");
    dpTT.savename = m_recovars->dpTT + "7000";
    ProduceGroup(dpTT, 199, -7000, 7000, (LL_base_cut + " && " + m_recovars->dpTT_alt + " != -999."));
    
    MakeDir("dpTT/LL/tmumom");
    dpTT.symbol = "#deltap_{TT}";
    dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_tmumom_alt;
    dpTT.savename = m_recovars->dpTT_tmumom_alt;
    ProduceGroup(dpTT, 39, -300, 300, EX_base_cut);
    
    MakeDir("dpTT/LL/tmumom");
    dpTT.symbol = "#deltap_{TT}";
    dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_tpimom_alt;
    dpTT.savename = m_recovars->dpTT_tpimom_alt;
    ProduceGroup(dpTT, 39, -300, 300, LL_base_cut);
    
    MakeDir("dpTT/LL/tprmom");
    dpTT.symbol = "#deltap_{TT}";
    dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_tprmom_alt;
    dpTT.savename = m_recovars->dpTT_tprmom_alt;
    ProduceGroup(dpTT, 39, -300, 300, LL_base_cut);
    
    MakeDir("dpTT/LL/tnudir");
    dpTT.symbol = "#deltap_{TT}";
    dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_tnudir_alt;
    dpTT.savename = m_recovars->dpTT_tnudir_alt;
    ProduceGroup(dpTT, 39, -300, 300, LL_base_cut);
    
    MakeDir("dpTT/LL/pimom");
    dpTT.symbol = "#deltap_{TT}";
    dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_pi_alt;
    dpTT.savename = m_recovars->dpTT_pi_alt;
    ProduceGroup(dpTT, 39, -300, 300, LL_base_cut);
    
    MakeDir("dpTT/LL/prmom");
    dpTT.symbol = "#deltap_{TT}";
    dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_pr_alt;
    dpTT.savename = m_recovars->dpTT_pr_alt;
    ProduceGroup(dpTT, 39, -300, 300, LL_base_cut);
    
    MakeDir("dpTT/LL/pidir");
    dpTT.symbol = "#deltap_{TT}";
    dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_pidir_alt;
    dpTT.savename = m_recovars->dpTT_pidir_alt;
    ProduceGroup(dpTT, 39, -300, 300, LL_base_cut);
    
    MakeDir("dpTT/LL/prdir");
    dpTT.symbol = "#deltap_{TT}";
    dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_prdir_alt;
    dpTT.savename = m_recovars->dpTT_prdir_alt;
    ProduceGroup(dpTT, 39, -300, 300, LL_base_cut);
//
//    //**************************************** DPTT END ***************************************//
//    //*****************************************************************************************//
//    //*****************************************************************************************//
//    //*****************************************************************************************//
//
//    MakeDir("W");
//
//    Variable W_mass;
//    W_mass.units = "MeV";
//    W_mass.symbol = "W";
//    W_mass.name = "mc_w";
//    W_mass.savename = "W_EX";
//
//    TCanvas * w_dist_EL = m_run->TARGETSingle(W_mass, 200, 0, 3000, EX_base_cut);
//    PrintLogo(w_dist_EL);
//    w_dist_EL->Write();
//    
//    W_mass.savename = "W_EX";
//    TCanvas * w_dist_LL = m_run->TARGETSingle(W_mass, 200, 0, 3000, LL_base_cut);
//    PrintLogo(w_dist_LL);
//    w_dist_LL->Write();
    
    //CLOSE THE DIRECTORY
    if(m_outfile->IsOpen()) m_outfile->Close();
    if(m_outfile) delete m_outfile;
    //DONE

    
    
    
}


std::string MomentumDists::GetDate(){
    
    TDatime today;
    int day = today.GetDay();
    int mon = today.GetMonth();
    int year = today.GetYear();
    year -= 2000;
    
//    stringstream sday, smon, syear;
//    
//    sday << day;
//    smon << mon;
//    syear << year;
//    return (sday.str() + smon.str() + syear.str());
    
    return std::string(Form("%.2d%.2d%.2d",day, mon, year));
}

TLatex * MomentumDists::GetLogo(){
    TLatex * logo = new TLatex(0.0, 0.1, ("#font[62]{" + m_exper_logo + "}").c_str() );
    return logo;
}

int main(int argc, char *argv[])
{
    
    string filename = testing_mc;
    string savename = "";// = "CC1P1PiP_Plots_" + sday.str() + smon.str() + syear.str() + ".root";
    bool debug = false;
    
    EXP::EXP experiment = EXP::UNKNOWN;
    
    
    char cc;
    while((cc = getopt(argc, argv, "i:o:d::t::m::")) != -1){
        switch (cc){
            case 'i': filename = std::string(optarg); break;
            case 'o': savename = optarg; break;
            case 'd': debug = true; break;
            case 't': experiment = EXP::T2K; break;
            case 'm': experiment = EXP::MIN; break;
            default: return 1;
        }
    }
    
    if(experiment == EXP::UNKNOWN){
        cout << "**** Experiment not defined ****" << endl;
        cout << "For T2K : -t" << endl;
        cout << "For T2K : -m" << endl;
        return 0;
    }

    MomentumDists * plots = new MomentumDists(experiment, filename, debug);
    
    if(!savename.empty()) plots->SetSaveName(savename);
    
    plots->MakePlots();

//    delete plots; -- TODO: Fix this
    
    return 0;
}
