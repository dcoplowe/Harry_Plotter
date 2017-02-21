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
    std::string dpT;
    std::string dalphaT;
    std::string dphiT;
    
    std::string truedpTT;
    std::string truedpT;
    std::string truedalphaT;
    std::string truedphiT;
    
    std::string dpTT_alt;
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
    
    void ProduceGroup(Variable var, Int_t nbins, Double_t * bins, std::string PDG_var, std::string cuts);
    void ProduceGroup(Variable var, Int_t nbins, Double_t low, Double_t high, std::string PDG_var, std::string cuts);
    
    void MakeDir(std::string name);
    
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
    
    m_savename = EXP::ToString(exp) + "_CC1P1PiPlus_Plots_" + GetDate() + ".root";
    
    cout << "Saving file as " << m_savename << endl;
    
    m_run = new BreakdownTools(m_infilename, m_reconame);
    
    m_outfile = new TFile(m_savename.c_str(), "RECREATE");
}

MomentumDists::~MomentumDists(){
    delete m_recovars;
    delete m_run;
    
    if(m_outfile){
        if(m_outfile->IsOpen()) m_outfile->Close();
        delete m_outfile;
    }
}

void MomentumDists::PrintLogo(TCanvas *& can){
    can->cd();
    TLatex * logo = GetLogo();
    logo->Draw();
}

void MomentumDists::ProduceGroup(Variable var, Int_t nbins, Double_t low, Double_t high, std::string PDG_var, std::string cuts){
    ProduceGroup(var, nbins, DrawingTools::SetBinning(nbins, low, high), PDG_var, cuts);
}

void MomentumDists::ProduceGroup(Variable var, Int_t nbins, Double_t * bins, std::string PDG_var, std::string cuts){//, Int_t setsave = 11111??

    if(m_outfile->IsOpen()){
        
        BDCans var_top = m_run->TOPO(mom, nbins, bins, base_cut);
        BDCans var_tar = run->TARGET(mom, nbins, bins, base_cut);
        BDCans var_pid = run->PID(mom, 40, 0, 2000, PDG_var, base_cut);
        
//        m_outfile->cd();
        
        //Recon Vars:
        PrintLogo(var_top.recon);
        PrintLogo(var_tar.recon);
        PrintLogo(var_pid.recon);
        
        var_top.recon->Write();
        var_tar.recon->Write();
        var_pid.recon->Write();
        
        //Truth Vars:
        PrintLogo(var_top.truth);
        PrintLogo(var_tar.truth);
        PrintLogo(var_pid.truth);
        
        var_top.truth->Write();
        var_tar.truth->Write();
        var_pid.truth->Write();
        
        //Ratio Vars:
        PrintLogo(var_top.ratio);
        PrintLogo(var_tar.ratio);
        PrintLogo(var_pid.ratio);
        
        var_top.ratio->Write();
        var_tar.ratio->Write();
        var_pid.ratio->Write();
        
        //Smear Vars:
        PrintLogo(var_top.smear);
        PrintLogo(var_tar.smear);
        PrintLogo(var_pid.smear);
        
        PrintLogo(var_top.smearSN);
        PrintLogo(var_tar.smearSN);
        PrintLogo(var_pid.smearSN);
        
        var_top.smear->Write();
        var_tar.smear->Write();
        var_pid.smear->Write();
        
        var_top.smearSN->Write();
        var_tar.smearSN->Write();
        var_pid.smearSN->Write();
        
    }
    else std::cout << "MomentumDists::ProduceGroup : ERROR : File is not open..." << std::endl;
    
}

void MomentumDists::MakeDir(std::string name){
    if(m_outfile->IsOpen()){
        TDirectory *tmp_dir = m_outfile->GetDirectory(name.c_str());
        if (!tmp_dir) {
            m_outfile->mkdir(name.c_str());
            m_outfile->cd(name.c_str());
            //                tmp_dir->cd();
        }
        else cout << "MomentumDists::MakeDir : Directory exists : " << name << endl;
    }
    else cout << "MomentumDists::MakeDir : File is closed..." << endl;
}

void MomentumDists::MakePlots(){
    
//    TFile * outfile = new TFile(m_savename.c_str(), "RECREATE");

//    BreakdownTools * run = new BreakdownTools(m_infilename, m_reconame);
//    run->FullBreakDown();//Off for now.
    
    //Pass all cuts check://Only consider dEdX ( accum_level[0]) recon for now:
    string base_cut = "accum_level[0] > 5 && " + m_pion->michel + " == 1";
    
    MakeDir("Mom");
    cout << "Current Dir : " << m_outfile->pwd();
    
    Variable mom;
    mom.name = m_proton->trueP + ":" + m_proton->P;
    mom.units = "MeV/#it{c}";
    mom.symbol = "#it{p}_{p}";
    mom.savename = m_proton->P;
    
    ProduceGroup(mom, 40, 0, 2000, m_proton->pdg, base_cut);
    
    Variable pi_mom;
    pi_mom.name = m_pion->trueP + ":" + m_pion->P;
    pi_mom.units = "MeV/#it{c}";
    pi_mom.symbol = "#it{p}_{p}";
    
    ProduceGroup(pi_mom, 40, 0, 2000, m_pion->pdg, base_cut);
    
    m_outfile->Close();
    delete m_outfile;

//
//    
//    //******************** MOMENTUM START ********************//
//    Variable mom;
//    mom.units = "MeV/#it{c}";
//    mom.symbol = "#it{p}_{p}";
//    
//    //**** Proton ****//
//    
//    mom.name = m_proton->trueP + ":" + m_proton->P;
//    mom.savename = m_proton->P;
//    
//    void ProduceGroup(){
//        
//        BDCans pr_mom_top = run->TOPO(mom, 40, 0, 2000, base_cut);
//        
//        outfile->cd();
//        
//        pr_mom_top.recon->cd();
//        TLatex * pr_mom_top_logo_recon = GetLogo();
//        pr_mom_top_logo_recon->Draw();
//        pr_mom_top.recon->Write();
//        
//        pr_mom_top.truth->cd();
//        TLatex * pr_mom_top_logo_truth = GetLogo();
//        pr_mom_top_logo_truth->Draw();
//        pr_mom_top.truth->Write();
//        
//        pr_mom_top.ratio->cd();
//        TLatex * pr_mom_top_logo_ratio = GetLogo();
//        pr_mom_top_logo_ratio->Draw();
//        pr_mom_top.ratio->Write();
//        
//        pr_mom_top.smearSN->cd();
//        TLatex * pr_mom_top_logo_smearSN = GetLogo();
//        pr_mom_top_logo_smearSN->Draw();
//        pr_mom_top.smearSN->Write();
//        
//        BDCans pr_mom_tar = run->TARGET(mom, 40, 0, 2000, base_cut);
//        
//        pr_mom_tar.recon->cd();
//        TLatex * pr_mom_tar_logo_recon = GetLogo();
//        pr_mom_tar_logo_recon->Draw();
//        pr_mom_tar.recon->Write();
//        
//        pr_mom_tar.truth->cd();
//        TLatex * pr_mom_tar_logo_truth = GetLogo();
//        pr_mom_tar_logo_truth->Draw();
//        pr_mom_tar.truth->Write();
//        
//        pr_mom_tar.ratio->cd();
//        TLatex * pr_mom_tar_logo_ratio = GetLogo();
//        pr_mom_tar_logo_ratio->Draw();
//        pr_mom_tar.ratio->Write();
//        
//        BDCans pr_mom_pid = run->PID(mom, 40, 0, 2000, m_proton->pdg, base_cut);
//        
//        pr_mom_pid.recon->cd();
//        TLatex * pr_mom_pid_logo_recon = GetLogo();
//        pr_mom_pid_logo_recon->Draw();
//        pr_mom_pid.recon->Write();
//        
//        pr_mom_pid.truth->cd();
//        TLatex * pr_mom_pid_logo_truth = GetLogo();
//        pr_mom_pid_logo_truth->Draw();
//        pr_mom_pid.truth->Write();
//        
//    }
//    
//    //**** Pion ****//
//    
//    mom.name = m_pion->trueP + ":" + m_pion->P;
//    mom.savename = m_pion->P;
//    mom.symbol = "#it{p}_{#pi^{#pm}}";
//    
//    BDCans pi_mom_top = run->TOPO(mom, 40, 0, 2000, base_cut);
//    
//    outfile->cd();
//    
//    pi_mom_top.recon->cd();
//    TLatex * pi_mom_top_logo_recon = GetLogo();
//    pi_mom_top_logo_recon->Draw();
//    pi_mom_top.recon->Write();
//    
//    pi_mom_top.truth->cd();
//    TLatex * pi_mom_top_logo_truth = GetLogo();
//    pi_mom_top_logo_truth->Draw();
//    pi_mom_top.truth->Write();
//    
//    pi_mom_top.ratio->cd();
//    TLatex * pi_mom_top_logo_ratio = GetLogo();
//    pi_mom_top_logo_ratio->Draw();
//    pi_mom_top.ratio->Write();
//    
//    pi_mom_top.smearSN->cd();
//    TLatex * pi_mom_top_logo_smearSN = GetLogo();
//    pi_mom_top_logo_smearSN->Draw();
//    pi_mom_top.smearSN->Write();
//    
//    BDCans pi_mom_tar = run->TARGET(mom, 40, 0, 2000, base_cut);
//    
//    pi_mom_tar.recon->cd();
//    TLatex * pi_mom_tar_logo_recon = GetLogo();
//    pi_mom_tar_logo_recon->Draw();
//    pi_mom_tar.recon->Write();
//    
//    pi_mom_tar.truth->cd();
//    TLatex * pi_mom_tar_logo_truth = GetLogo();
//    pi_mom_tar_logo_truth->Draw();
//    pi_mom_tar.truth->Write();
//    
//    pi_mom_tar.ratio->cd();
//    TLatex * pi_mom_tar_logo_ratio = GetLogo();
//    pi_mom_tar_logo_ratio->Draw();
//    pi_mom_tar.ratio->Write();
//    
//    BDCans pi_mom_pid = run->PID(mom, 40, 0, 2000, m_pion->pdg, base_cut);
//    
//    pi_mom_pid.recon->cd();
//    TLatex * pi_mom_pid_logo_recon = GetLogo();
//    pi_mom_pid_logo_recon->Draw();
//    pi_mom_pid.recon->Write();
//    
//    pi_mom_pid.truth->cd();
//    TLatex * pi_mom_pid_logo_truth = GetLogo();
//    pi_mom_pid_logo_truth->Draw();
//    pi_mom_pid.truth->Write();
//    
//    //**** Muon ****//
//    
//    mom.name = m_muon->trueP + ":" + m_muon->P;
//    mom.savename = m_muon->P;
//    mom.symbol = "#it{p}_{#mu^{#pm}}";
//    
//    BDCans mu_mom_top = run->TOPO(mom, 40, 0, 2000, base_cut);
//    
//    outfile->cd();
//    
//    mu_mom_top.recon->cd();
//    TLatex * mu_mom_top_logo_recon = GetLogo();
//    mu_mom_top_logo_recon->Draw();
//    mu_mom_top.recon->Write();
//    
//    mu_mom_top.truth->cd();
//    TLatex * mu_mom_top_logo_truth = GetLogo();
//    mu_mom_top_logo_truth->Draw();
//    mu_mom_top.truth->Write();
//    
//    mu_mom_top.ratio->cd();
//    TLatex * mu_mom_top_logo_ratio = GetLogo();
//    mu_mom_top_logo_ratio->Draw();
//    mu_mom_top.ratio->Write();
//    
//    mu_mom_top.smearSN->cd();
//    TLatex * mu_mom_top_logo_smearSN = GetLogo();
//    mu_mom_top_logo_smearSN->Draw();
//    mu_mom_top.smearSN->Write();
//    
//    BDCans mu_mom_tar = run->TARGET(mom, 40, 0, 2000, base_cut);
//    
//    mu_mom_tar.recon->cd();
//    TLatex * mu_mom_tar_logo_recon = GetLogo();
//    mu_mom_tar_logo_recon->Draw();
//    mu_mom_tar.recon->Write();
//    
//    mu_mom_tar.truth->cd();
//    TLatex * mu_mom_tar_logo_truth = GetLogo();
//    mu_mom_tar_logo_truth->Draw();
//    mu_mom_tar.truth->Write();
//    
//    mu_mom_tar.ratio->cd();
//    TLatex * mu_mom_tar_logo_ratio = GetLogo();
//    mu_mom_tar_logo_ratio->Draw();
//    mu_mom_tar.ratio->Write();
//    
//    BDCans mu_mom_pid = run->PID(mom, 40, 0, 2000, m_muon->pdg, base_cut);
//    
//    mu_mom_pid.recon->cd();
//    TLatex * mu_mom_pid_logo_recon = GetLogo();
//    mu_mom_pid_logo_recon->Draw();
//    mu_mom_pid.recon->Write();
//    
//    mu_mom_pid.truth->cd();
//    TLatex * mu_mom_pid_logo_truth = GetLogo();
//    mu_mom_pid_logo_truth->Draw();
//    mu_mom_pid.truth->Write();
//    
//    //******************** MOMENTUM END ********************//
//    
//    //******************** THETA START ********************//
//    Variable theta;
//    theta.units = "degrees";
//    
//    //**** Proton ****//
//    
//    theta.name = m_proton->truetheta + ":" + m_proton->theta;
//    theta.savename = m_proton->theta;
//    theta.symbol = "#theta_{p}";
//    
//    BDCans pr_theta_top = run->TOPO(theta, 40, 0, 3.14, base_cut);
//    
//    outfile->cd();
//    
//    pr_theta_top.recon->cd();
//    TLatex * pr_theta_top_logo_recon = GetLogo();
//    pr_theta_top_logo_recon->Draw();
//    pr_theta_top.recon->Write();
//    
//    pr_theta_top.truth->cd();
//    TLatex * pr_theta_top_logo_truth = GetLogo();
//    pr_theta_top_logo_truth->Draw();
//    pr_theta_top.truth->Write();
//    
//    pr_theta_top.ratio->cd();
//    TLatex * pr_theta_top_logo_ratio = GetLogo();
//    pr_theta_top_logo_ratio->Draw();
//    pr_theta_top.ratio->Write();
//    
//    pr_theta_top.smearSN->cd();
//    TLatex * pr_theta_top_logo_smearSN = GetLogo();
//    pr_theta_top_logo_smearSN->Draw();
//    pr_theta_top.smearSN->Write();
//    
//    BDCans pr_theta_tar = run->TARGET(theta, 40, 0, 3.14, base_cut);
//    
//    pr_theta_tar.recon->cd();
//    TLatex * pr_theta_tar_logo_recon = GetLogo();
//    pr_theta_tar_logo_recon->Draw();
//    pr_theta_tar.recon->Write();
//    
//    pr_theta_tar.truth->cd();
//    TLatex * pr_theta_tar_logo_truth = GetLogo();
//    pr_theta_tar_logo_truth->Draw();
//    pr_theta_tar.truth->Write();
//    
//    pr_theta_tar.ratio->cd();
//    TLatex * pr_theta_tar_logo_ratio = GetLogo();
//    pr_theta_tar_logo_ratio->Draw();
//    pr_theta_tar.ratio->Write();
//    
//    BDCans pr_theta_pid = run->PID(theta, 40, 0, 3.14, m_proton->pdg, base_cut);
//    
//    pr_theta_pid.recon->cd();
//    TLatex * pr_theta_pid_logo_recon = GetLogo();
//    pr_theta_pid_logo_recon->Draw();
//    pr_theta_pid.recon->Write();
//    
//    pr_theta_pid.truth->cd();
//    TLatex * pr_theta_pid_logo_truth = GetLogo();
//    pr_theta_pid_logo_truth->Draw();
//    pr_theta_pid.truth->Write();
//
//    //**** Pion ****//
//    
//    theta.name = m_pion->truetheta + ":" + m_pion->theta;
//    theta.savename = m_pion->theta;
//    theta.symbol = "#theta_{#pi^{#pm}}";
//    
//    BDCans pi_theta_top = run->TOPO(theta, 40, 0, 3.14, base_cut);
//    
//    outfile->cd();
//    
//    pr_theta_top.recon->cd();
//    TLatex * pi_theta_top_logo_recon = GetLogo();
//    pi_theta_top_logo_recon->Draw();
//    pi_theta_top.recon->Write();
//    
//    pi_theta_top.truth->cd();
//    TLatex * pi_theta_top_logo_truth = GetLogo();
//    pi_theta_top_logo_truth->Draw();
//    pi_theta_top.truth->Write();
//    
//    pi_theta_top.ratio->cd();
//    TLatex * pi_theta_top_logo_ratio = GetLogo();
//    pi_theta_top_logo_ratio->Draw();
//    pi_theta_top.ratio->Write();
//    
//    pi_theta_top.smearSN->cd();
//    TLatex * pi_theta_top_logo_smearSN = GetLogo();
//    pi_theta_top_logo_smearSN->Draw();
//    pi_theta_top.smearSN->Write();
//    
//    BDCans pi_theta_tar = run->TARGET(theta, 40, 0, 3.14, base_cut);
//    
//    pi_theta_tar.recon->cd();
//    TLatex * pi_theta_tar_logo_recon = GetLogo();
//    pi_theta_tar_logo_recon->Draw();
//    pi_theta_tar.recon->Write();
//    
//    pi_theta_tar.truth->cd();
//    TLatex * pi_theta_tar_logo_truth = GetLogo();
//    pi_theta_tar_logo_truth->Draw();
//    pi_theta_tar.truth->Write();
//    
//    pi_theta_tar.ratio->cd();
//    TLatex * pi_theta_tar_logo_ratio = GetLogo();
//    pi_theta_tar_logo_ratio->Draw();
//    pi_theta_tar.ratio->Write();
//    
//    BDCans pi_theta_pid = run->PID(theta, 40, 0, 3.14, m_pion->pdg, base_cut);
//    
//    pi_theta_pid.recon->cd();
//    TLatex * pi_theta_pid_logo_recon = GetLogo();
//    pi_theta_pid_logo_recon->Draw();
//    pi_theta_pid.recon->Write();
//    
//    pi_theta_pid.truth->cd();
//    TLatex * pi_theta_pid_logo_truth = GetLogo();
//    pi_theta_pid_logo_truth->Draw();
//    pi_theta_pid.truth->Write();
//
//    //**** Muon ****//
//    
//    theta.name = m_muon->truetheta + ":" + m_muon->theta;
//    theta.savename = m_muon->theta;
//    theta.symbol = "#theta_{#mu^{#pm}}";
//    
//    BDCans mu_theta_top = run->TOPO(theta, 40, 0, 3.14, base_cut);
//    
//    outfile->cd();
//    
//    mu_theta_top.recon->cd();
//    TLatex * mu_theta_top_logo_recon = GetLogo();
//    mu_theta_top_logo_recon->Draw();
//    mu_theta_top.recon->Write();
//    
//    mu_theta_top.truth->cd();
//    TLatex * mu_theta_top_logo_truth = GetLogo();
//    mu_theta_top_logo_truth->Draw();
//    mu_theta_top.truth->Write();
//    
//    mu_theta_top.ratio->cd();
//    TLatex * mu_theta_top_logo_ratio = GetLogo();
//    mu_theta_top_logo_ratio->Draw();
//    mu_theta_top.ratio->Write();
//    
//    mu_theta_top.smearSN->cd();
//    TLatex * mu_theta_top_logo_smearSN = GetLogo();
//    mu_theta_top_logo_smearSN->Draw();
//    mu_theta_top.smearSN->Write();
//    
//    BDCans mu_theta_tar = run->TARGET(theta, 40, 0, 3.14, base_cut);
//    
//    mu_theta_tar.recon->cd();
//    TLatex * mu_theta_tar_logo_recon = GetLogo();
//    mu_theta_tar_logo_recon->Draw();
//    mu_theta_tar.recon->Write();
//    
//    mu_theta_tar.truth->cd();
//    TLatex * mu_theta_tar_logo_truth = GetLogo();
//    mu_theta_tar_logo_truth->Draw();
//    mu_theta_tar.truth->Write();
//    
//    mu_theta_tar.ratio->cd();
//    TLatex * mu_theta_tar_logo_ratio = GetLogo();
//    mu_theta_tar_logo_ratio->Draw();
//    mu_theta_tar.ratio->Write();
//    
//    BDCans mu_theta_pid = run->PID(theta, 40, 0, 3.14, m_muon->pdg, base_cut);
//    
//    mu_theta_pid.recon->cd();
//    TLatex * mu_theta_pid_logo_recon = GetLogo();
//    mu_theta_pid_logo_recon->Draw();
//    mu_theta_pid.recon->Write();
//    
//    mu_theta_pid.truth->cd();
//    TLatex * mu_theta_pid_logo_truth = GetLogo();
//    mu_theta_pid_logo_truth->Draw();
//    mu_theta_pid.truth->Write();
//    //******************** THETA END ********************//
//    
//    //******************** PHI START ********************//
//    Variable phi;
//    phi.units = "degrees";
//    
//    //**** Proton ****//
//    
//    phi.name = m_proton->truephi + ":" + m_proton->phi;
//    phi.savename = m_proton->phi;
//    phi.symbol = "#phi_{p}";
//    
//    BDCans pr_phi_top = run->TOPO(phi, 40, 0, 3.14, base_cut);
//    
//    outfile->cd();
//    
//    pr_phi_top.recon->cd();
//    TLatex * pr_phi_top_logo_recon = GetLogo();
//    pr_phi_top_logo_recon->Draw();
//    pr_phi_top.recon->Write();
//    
//    pr_phi_top.truth->cd();
//    TLatex * pr_phi_top_logo_truth = GetLogo();
//    pr_phi_top_logo_truth->Draw();
//    pr_phi_top.truth->Write();
//    
//    pr_phi_top.ratio->cd();
//    TLatex * pr_phi_top_logo_ratio = GetLogo();
//    pr_phi_top_logo_ratio->Draw();
//    pr_phi_top.ratio->Write();
//    
//    pr_phi_top.smearSN->cd();
//    TLatex * pr_phi_top_logo_smearSN = GetLogo();
//    pr_phi_top_logo_smearSN->Draw();
//    pr_phi_top.smearSN->Write();
//    
//    BDCans pr_phi_tar = run->TARGET(phi, 40, 0, 3.14, base_cut);
//    
//    pr_phi_tar.recon->cd();
//    TLatex * pr_phi_tar_logo_recon = GetLogo();
//    pr_phi_tar_logo_recon->Draw();
//    pr_phi_tar.recon->Write();
//    
//    pr_phi_tar.truth->cd();
//    TLatex * pr_phi_tar_logo_truth = GetLogo();
//    pr_phi_tar_logo_truth->Draw();
//    pr_phi_tar.truth->Write();
//    
//    pr_phi_tar.ratio->cd();
//    TLatex * pr_phi_tar_logo_ratio = GetLogo();
//    pr_phi_tar_logo_ratio->Draw();
//    pr_phi_tar.ratio->Write();
//    
//    BDCans pr_phi_pid = run->PID(phi, 40, 0, 3.14, m_proton->pdg, base_cut);
//    
//    pr_phi_pid.recon->cd();
//    TLatex * pr_phi_pid_logo_recon = GetLogo();
//    pr_phi_pid_logo_recon->Draw();
//    pr_phi_pid.recon->Write();
//    
//    pr_phi_pid.truth->cd();
//    TLatex * pr_phi_pid_logo_truth = GetLogo();
//    pr_phi_pid_logo_truth->Draw();
//    pr_phi_pid.truth->Write();
//
//    
//    //**** Pion ****//
//    
//    phi.name = m_pion->truephi + ":" + m_pion->phi;
//    phi.savename = m_pion->phi;
//    phi.symbol = "#phi_{#pi^{#pm}}";
//    
//    BDCans pi_phi_top = run->TOPO(phi, 40, 0, 3.14, base_cut);
//    
//    outfile->cd();
//    
//    pi_phi_top.recon->cd();
//    TLatex * pi_phi_top_logo_recon = GetLogo();
//    pi_phi_top_logo_recon->Draw();
//    pi_phi_top.recon->Write();
//    
//    pi_phi_top.truth->cd();
//    TLatex * pi_phi_top_logo_truth = GetLogo();
//    pi_phi_top_logo_truth->Draw();
//    pi_phi_top.truth->Write();
//    
//    pi_phi_top.ratio->cd();
//    TLatex * pi_phi_top_logo_ratio = GetLogo();
//    pi_phi_top_logo_ratio->Draw();
//    pi_phi_top.ratio->Write();
//    
//    pi_phi_top.smearSN->cd();
//    TLatex * pi_phi_top_logo_smearSN = GetLogo();
//    pi_phi_top_logo_smearSN->Draw();
//    pi_phi_top.smearSN->Write();
//    
//    BDCans pi_phi_tar = run->TARGET(phi, 40, 0, 3.14, base_cut);
//    
//    pi_phi_tar.recon->cd();
//    TLatex * pi_phi_tar_logo_recon = GetLogo();
//    pi_phi_tar_logo_recon->Draw();
//    pi_phi_tar.recon->Write();
//    
//    pi_phi_tar.truth->cd();
//    TLatex * pi_phi_tar_logo_truth = GetLogo();
//    pi_phi_tar_logo_truth->Draw();
//    pi_phi_tar.truth->Write();
//    
//    pi_phi_tar.ratio->cd();
//    TLatex * pi_phi_tar_logo_ratio = GetLogo();
//    pi_phi_tar_logo_ratio->Draw();
//    pi_phi_tar.ratio->Write();
//    
//    BDCans pi_phi_pid = run->PID(phi, 40, 0, 3.14, m_pion->pdg, base_cut);
//    
//    pi_phi_pid.recon->cd();
//    TLatex * pi_phi_pid_logo_recon = GetLogo();
//    pi_phi_pid_logo_recon->Draw();
//    pi_phi_pid.recon->Write();
//    
//    pi_phi_pid.truth->cd();
//    TLatex * pi_phi_pid_logo_truth = GetLogo();
//    pi_phi_pid_logo_truth->Draw();
//    pi_phi_pid.truth->Write();
//
//    //**** Muon ****//
//    
//    phi.name = m_muon->truephi + ":" + m_muon->phi;
//    phi.savename = m_muon->phi;
//    phi.symbol = "#phi_{#mu^{#pm}}";
//    
//    BDCans mu_phi_top = run->TOPO(phi, 40, 0, 3.14, base_cut);
//    
//    outfile->cd();
//    
//    mu_phi_top.recon->cd();
//    TLatex * mu_phi_top_logo_recon = GetLogo();
//    mu_phi_top_logo_recon->Draw();
//    mu_phi_top.recon->Write();
//    
//    mu_phi_top.truth->cd();
//    TLatex * mu_phi_top_logo_truth = GetLogo();
//    mu_phi_top_logo_truth->Draw();
//    mu_phi_top.truth->Write();
//    
//    mu_phi_top.ratio->cd();
//    TLatex * mu_phi_top_logo_ratio = GetLogo();
//    mu_phi_top_logo_ratio->Draw();
//    mu_phi_top.ratio->Write();
//    
//    mu_phi_top.smearSN->cd();
//    TLatex * mu_phi_top_logo_smearSN = GetLogo();
//    mu_phi_top_logo_smearSN->Draw();
//    mu_phi_top.smearSN->Write();
//    
//    BDCans mu_phi_tar = run->TARGET(phi, 40, 0, 3.14, base_cut);
//    
//    mu_phi_tar.recon->cd();
//    TLatex * mu_phi_tar_logo_recon = GetLogo();
//    mu_phi_tar_logo_recon->Draw();
//    mu_phi_tar.recon->Write();
//    
//    mu_phi_tar.truth->cd();
//    TLatex * mu_phi_tar_logo_truth = GetLogo();
//    mu_phi_tar_logo_truth->Draw();
//    mu_phi_tar.truth->Write();
//    
//    mu_phi_tar.ratio->cd();
//    TLatex * mu_phi_tar_logo_ratio = GetLogo();
//    mu_phi_tar_logo_ratio->Draw();
//    mu_phi_tar.ratio->Write();
//    
//    BDCans mu_phi_pid = run->PID(phi, 40, 0, 3.14, m_muon->pdg, base_cut);
//    
//    mu_phi_pid.recon->cd();
//    TLatex * mu_phi_pid_logo_recon = GetLogo();
//    mu_phi_pid_logo_recon->Draw();
//    mu_phi_pid.recon->Write();
//    
//    mu_phi_pid.truth->cd();
//    TLatex * mu_phi_pid_logo_truth = GetLogo();
//    mu_phi_pid_logo_truth->Draw();
//    mu_phi_pid.truth->Write();
//
//    //******************** PHI END ********************//
//
//    //******************** DPTT ±300 START ********************//
//    Variable dpTT;
//    dpTT.units = "MeV/#it{c}";
//    dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT;
//    dpTT.savename = m_recovars->dpTT + "300";
//    dpTT.symbol = "#deltap_{TT}";
//    
//    BDCans dpTT_top = run->TOPO(dpTT, 39, -300, 300, base_cut);
//    
//    outfile->cd();
//    
//    dpTT_top.recon->cd();
//    TLatex * dpTT_top_logo_recon = GetLogo();
//    dpTT_top_logo_recon->Draw();
//    dpTT_top.recon->Write();
//    
//    dpTT_top.truth->cd();
//    TLatex * dpTT_top_logo_truth = GetLogo();
//    dpTT_top_logo_truth->Draw();
//    dpTT_top.truth->Write();
//    
//    dpTT_top.ratio->cd();
//    TLatex * dpTT_top_logo_ratio = GetLogo();
//    dpTT_top_logo_ratio->Draw();
//    dpTT_top.ratio->Write();
//    
//    dpTT_top.smearSN->cd();
//    TLatex * dpTT_top_logo_smearSN = GetLogo();
//    dpTT_top_logo_smearSN->Draw();
//    dpTT_top.smearSN->Write();
//    
//    BDCans dpTT_tar = run->TARGET(dpTT, 39, -300, 300, base_cut);
//    
//    dpTT_tar.recon->cd();
//    TLatex * dpTT_tar_logo_recon = GetLogo();
//    dpTT_tar_logo_recon->Draw();
//    dpTT_tar.recon->Write();
//    
//    dpTT_tar.truth->cd();
//    TLatex * dpTT_tar_logo_truth = GetLogo();
//    dpTT_tar_logo_truth->Draw();
//    dpTT_tar.truth->Write();
//    
//    dpTT_tar.ratio->cd();
//    TLatex * dpTT_tar_logo_ratio = GetLogo();
//    dpTT_tar_logo_ratio->Draw();
//    dpTT_tar.ratio->Write();
//    
//    //******************** DPTT END ********************//
//    
//    //******************** DPTT ±600 START ********************//
//    dpTT.savename = m_recovars->dpTT + "600";
//    
//    BDCans dpTT600_top = run->TOPO(dpTT, 79, -600, 600, base_cut);
//    
//    outfile->cd();
//    
//    dpTT600_top.recon->cd();
//    TLatex * dpTT600_top_logo_recon = GetLogo();
//    dpTT600_top_logo_recon->Draw();
//    dpTT600_top.recon->Write();
//    
//    dpTT600_top.truth->cd();
//    TLatex * dpTT600_top_logo_truth = GetLogo();
//    dpTT600_top_logo_truth->Draw();
//    dpTT600_top.truth->Write();
//    
//    dpTT600_top.ratio->cd();
//    TLatex * dpTT600_top_logo_ratio = GetLogo();
//    dpTT600_top_logo_ratio->Draw();
//    dpTT600_top.ratio->Write();
//    
//    dpTT600_top.smearSN->cd();
//    TLatex * dpTT600_top_logo_smearSN = GetLogo();
//    dpTT600_top_logo_smearSN->Draw();
//    dpTT600_top.smearSN->Write();
//    
//    BDCans dpTT600_tar = run->TARGET(dpTT, 39, -300, 300, base_cut);
//    
//    dpTT600_tar.recon->cd();
//    TLatex * dpTT600_tar_logo_recon = GetLogo();
//    dpTT600_tar_logo_recon->Draw();
//    dpTT600_tar.recon->Write();
//    
//    dpTT600_tar.truth->cd();
//    TLatex * dpTT600_tar_logo_truth = GetLogo();
//    dpTT600_tar_logo_truth->Draw();
//    dpTT600_tar.truth->Write();
//    
//    dpTT600_tar.ratio->cd();
//    TLatex * dpTT600_tar_logo_ratio = GetLogo();
//    dpTT600_tar_logo_ratio->Draw();
//    dpTT600_tar.ratio->Write();
    
    //******************** DPTT END ********************//
   
//    DrawingTools * test = new DrawingTools(m_infilename, m_reconame);
//
//
//    TH2D * hist_1 = test->GetHisto( (m_proton->trueP + ":" + m_proton->P).c_str() , 30, 0., 2000.0, 30, 0., 2000.0, "proton p (MeV/c)","accum_level>5 ");
//    TH1D * hist_2 = test->GetHisto(m_recovars->dpTT, 59, -300., 300, "#deltap_{TT} (MeV/c)","accum_level>5 ");
//    
//    DrawingTools::KinMap pmap = test->KinArray( (m_proton->trueP + ":" + m_proton->P).c_str(), 29, -1000, 1000, "proton p_{TT} (MeV/c)", "accum_level>5 ");
//    
//    outfile->cd();
//    hist_1->Write();
//    hist_2->Write();
//    
//    pmap.recon->Write();
//    pmap.truth->Write();
//    pmap.smear->Write();
//    pmap.ratio->Write();

//    outfile->Close();
//    delete outfile;
    
//    m_outfile
    
    
    
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

    delete plots;
    
    return 0;
}
