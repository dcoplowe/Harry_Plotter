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
        
        truedpTT = "sel_truedpTT_EX";
        truedpT = "sel_truedpT_EX";
        truedalphaT = "sel_truedalphaT_EX";
        truedphiT = "sel_truedphiT_EX";
        
        dpTT_alt = "sel_dpTT_LL";
        dpT_alt = "sel_dpT_LL";
        dalphaT_alt = "sel_dalphaT_LL";
        dphiT_alt = "sel_dphiT_LL";
        
    }
    else if(exp == EXP::MIN){
        
        muon = new Particle(exp, "mu_EX", "sel_");
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
        
        truedpTT = "sel_truedpTT_EX";
        truedpT = "sel_truedpT_EX";
        truedalphaT = "sel_truedalphaT_EX";
        truedphiT = "sel_truedphiT_EX";
        
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

    void MakePlots();
    
    TLatex * GetLogo();
    
private:
    EXP::EXP m_experiment;
    std::string m_infilename;
    
    KinematicVars * m_recovars;
    
    std::string m_savename;
    
    std::string m_truename;
    std::string m_reconame;
    std::string m_exper_logo;
    
    std::string GetDate();
    
    //tmp POT bool (off for t2k):
    bool m_getPOT;
    
    Particle * m_muon;
    Particle * m_proton;
    Particle * m_pion;
    
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
}

MomentumDists::~MomentumDists(){
    delete m_recovars;
}

void MomentumDists::MakePlots(){
    
    TFile * outfile = new TFile(m_savename.c_str(), "RECREATE");

    BreakdownTools * test = new BreakdownTools(m_infilename, m_reconame);
    
    Variable var;
    var.name = m_proton->trueP + ":" + m_proton->P;
    var.units = "MeV/#it{c}";
    var.symbol = "#it{p}_{p}";
    var.savename = m_proton->P;
    
    BDCans canstest = test->PID(var, 40, 0, 2000, m_proton->pdg);
    
    var.savename += "_1_";
    test->FullBreakDown();
    BDCans canstest2 = test->PID(var, 40, 0, 2000, m_proton->pdg);

    
    outfile->cd();
    canstest.recon->cd();
    TLatex * canstest_logo = GetLogo();
    canstest_logo->Draw();
    
    canstest.recon->Write();
    canstest2.recon->Write();
    
    canstest.truth->Write();
    canstest2.truth->Write();
    
    canstest.ratio->Write();
    canstest2.ratio->Write();
    
    canstest.smear->Write();
    canstest2.smear->Write();
   
    canstest.smearSN->Write();
    canstest2.smearSN->Write();
    
    
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

    outfile->Close();
    delete outfile;
    
//    m_outfile
    
    
    
}


std::string MomentumDists::GetDate(){
    
    TDatime today;
    int day = today.GetDay();
    int mon = today.GetMonth();
    int year = today.GetYear();
    year -= 2000;
    
    stringstream sday, smon, syear;
    
    sday << day;
    smon << mon;
    syear << year;
    
    return (sday.str() + smon.str() + syear.str());
    
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
