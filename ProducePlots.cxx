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
#include "PartKinMaps.h"

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

const string testing_mc("/pnfs/minerva/persistent/users/dcoplowe/CC1P1Pi_PL13C_030317/grid/central_value/minerva/ana/v10r8p9/00/01/32/00/SIM_minerva_00013200_Subruns_0001-0002-0003-0004_CC1P1PiAnalysis_Ana_Tuple_v10r8p9-dcoplowe.root");

class ProducePlots {

public:
    ProducePlots(EXP::EXP exp, std::string filename, bool debug, bool realdata);
    ~ProducePlots();
    
    void SetSaveName(std::string var){ m_savename = var; }
    void SetTrueTreeName(std::string var){ m_truename = var; }
    void SetRecoTreeName(std::string var){ m_reconame = var; }
    
//    void ProduceGroup();

    void MakePlots();
    
    TLatex * GetLogo();
    void PrintLogo(TCanvas *& can);
    
    TLatex * GetSignal();
    
    //Particle kinematic variables:
    void MakeMomPlots(Particle * part, Int_t nbins, Double_t * bins, std::string cuts);
    void MakeMomPlots(Particle * part, Int_t nbins, Double_t low, Double_t high, std::string cuts);
    
    void MakeThetaPlots(Particle * part, Int_t nbins, Double_t * bins, std::string cuts);
    void MakeThetaPlots(Particle * part, Int_t nbins, Double_t low, Double_t high, std::string cuts);

    void MakePhiPlots(Particle * part, Int_t nbins, Double_t * bins, std::string cuts);
    void MakePhiPlots(Particle * part, Int_t nbins, Double_t low, Double_t high, std::string cuts);

    void MakeCosThetaPlots(Particle * part, Int_t nbins, Double_t * bins, std::string cuts);
    void MakeCosThetaPlots(Particle * part, Int_t nbins, Double_t low, Double_t high, std::string cuts);

    void MakeScorePlots(Particle * part, Int_t nbins, Double_t * bins, std::string cuts);
    void MakeScorePlots(Particle * part, Int_t nbins, Double_t low, Double_t high, std::string cuts);
    
    void ProduceGroup(Variable var, Int_t nbins, Double_t * bins, std::string cuts);
    void ProduceGroup(Variable var, Int_t nbins, Double_t low, Double_t high, std::string cuts);
    
    void EffPart(Variable var, Int_t nbins, Double_t low, Double_t high, std::string signal_def, std::string cuts = "");
    void TruthPart(Variable var, Int_t nbins, Double_t low, Double_t high, std::string cuts = "", int ac_lev = -1, int branch = 0);
    
    void MakeDir(std::string name);
    void cdDir(std::string name = ""){  m_outfile->cd((m_savename + ":/" + name).c_str()); }//Default is the root dir.
    
private:
    EXP::EXP m_experiment;
    std::string m_infilename;
    bool m_realdata;

    KinematicVars * m_recovars;
    
    std::string m_savename;
    
    TFile * m_outfile;// = new TFile(m_savename.c_str(), "RECREATE");

    std::string m_truename;
    std::string m_reconame;
    std::string m_exper_logo;
    std::string m_exper_signal;
    
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
    
    BreakdownTools  * m_runbd;
    BreakdownTools  * m_runtruthbd;
    EffPurTools     * m_runep;
};

ProducePlots::ProducePlots(EXP::EXP exp, std::string filename, bool debug, bool realdata) : m_experiment(exp), m_infilename(filename),
m_realdata(realdata) {
    
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
    
    m_exper_signal = "H-CC1p1#pi^{+} (1.5 < E_{#mu} GeV < 20,  #theta_{#mu} < 25 Deg.)";
    
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
    
    m_runbd = new BreakdownTools(m_infilename, m_reconame);
    m_runtruthbd = new BreakdownTools(m_infilename, m_truename);
    
    std::vector<std::string> selection_cuts;
    selection_cuts.push_back("Vertex");
    selection_cuts.push_back("3 Tracks");
    selection_cuts.push_back("Muon Track");
    selection_cuts.push_back("Contained Vtx");
    selection_cuts.push_back("PID: p/#pi^{+}");
    selection_cuts.push_back("Michel Sense");
    
    m_runep = new EffPurTools(m_infilename, selection_cuts);
    
    m_outfile = new TFile(m_savename.c_str(), "RECREATE");
}

ProducePlots::~ProducePlots(){
    
    if(m_outfile){
        if(m_outfile->IsOpen()) m_outfile->Close();
        delete m_outfile;
    }
    
    delete m_recovars;
    delete m_runbd;
}

void ProducePlots::PrintLogo(TCanvas *& can){
    can->cd();
    TLatex * logo = GetLogo();
    logo->Draw();
}

void ProducePlots::ProduceGroup(Variable var, Int_t nbins, Double_t low, Double_t high, std::string cuts){
    ProduceGroup(var, nbins, m_runbd->SetBinning(nbins, low, high), cuts);
}

void ProducePlots::ProduceGroup(Variable var, Int_t nbins, Double_t * bins, std::string cuts){//, Int_t setsave = 11111??

    if(m_outfile->IsOpen()){
        
        BDCans var_top = m_runbd->TOPO(var, nbins, bins, cuts);
        BDCans var_tar = m_runbd->TARGET(var, nbins, bins, cuts);
        
        BDCans var_pid;
        if(!var.pdg.empty()) var_pid = m_runbd->PID(var, nbins, bins, var.pdg, cuts);
        
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
        var_tar.ratio->Write();
        if(!var.pdg.empty()) var_pid.ratio->Write();
        
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
    else std::cout << "ProducePlots::ProduceGroup : ERROR : File is not open..." << std::endl;
    
}

void ProducePlots::MakeMomPlots(Particle * part, Int_t nbins, Double_t * bins, std::string cuts){
    Variable mom;
    mom.units = "MeV/#it{c}";
    mom.name = part->trueP + ":" + part->P;
    mom.symbol = "#it{p}_{" + part->GetSymbol() + "}";
    mom.savename = part->P;
    mom.pdg = part->pdg;
    ProduceGroup(mom, nbins, bins, cuts);
}

void ProducePlots::MakeMomPlots(Particle * part, Int_t nbins, Double_t low, Double_t high, std::string cuts){
    MakeMomPlots(part, nbins, m_runbd->SetBinning(nbins, low, high), cuts);
}

void ProducePlots::MakeThetaPlots(Particle * part, Int_t nbins, Double_t * bins, std::string cuts){
    Variable theta;
    //    theta.units = "degrees";
    //    theta.name = m_proton->trueP + "*TMath::RadToDeg():" + m_proton->P + "*TMath::RadToDeg()";//This probably wont work as the code looks for :: to make a split... Add fix.
    theta.units = "rad";
    theta.symbol = "#theta_{" + part->GetSymbol() + "}";
    theta.name = part->truetheta + ":" + part->theta;//This probably wont work as the code looks for :: to make a split... Add fix.
    theta.savename = part->theta;
    theta.pdg = part->pdg;
    ProduceGroup(theta, nbins, bins, cuts);
}

void ProducePlots::MakeThetaPlots(Particle * part, Int_t nbins, Double_t low, Double_t high, std::string cuts){
    MakeThetaPlots(part, nbins, m_runbd->SetBinning(nbins, low, high), cuts);
}

void ProducePlots::MakePhiPlots(Particle * part, Int_t nbins, Double_t * bins, std::string cuts){
    Variable phi;
    //    phi.units = "degrees";
    //    phi.name = m_proton->trueP + "*TMath::RadToDeg():" + m_proton->P + "*TMath::RadToDeg()";//This probably wont work as the code looks for :: to make a split... Add fix.
    phi.units = "rad";
    phi.symbol = "#phi_{" + part->GetSymbol() + "}";
    phi.name = part->truephi + ":" + part->phi;//This probably wont work as the code looks for :: to make a split... Add fix.
    phi.savename = part->phi;
    phi.pdg = part->pdg;
    ProduceGroup(phi, nbins, bins, cuts);
}

void ProducePlots::MakePhiPlots(Particle * part, Int_t nbins, Double_t low, Double_t high, std::string cuts){
    MakePhiPlots(part, nbins, m_runbd->SetBinning(nbins, low, high), cuts);
}

void ProducePlots::MakeCosThetaPlots(Particle * part, Int_t nbins, Double_t * bins, std::string cuts){
    Variable phi;
    //    phi.units = "degrees";
    //    phi.name = m_proton->trueP + "*TMath::RadToDeg():" + m_proton->P + "*TMath::RadToDeg()";//This probably wont work as the code looks for :: to make a split... Add fix.
    phi.units = "rad";
    phi.symbol = "#phi_{" + part->GetSymbol() + "}";
    phi.name = part->truephi + ":" + part->phi;//This probably wont work as the code looks for :: to make a split... Add fix.
    phi.savename = part->phi;
    phi.pdg = part->pdg;
    ProduceGroup(phi, nbins, bins, cuts);
}

void ProducePlots::MakeCosThetaPlots(Particle * part, Int_t nbins, Double_t low, Double_t high, std::string cuts){
    MakeCosThetaPlots(part, nbins, m_runbd->SetBinning(nbins, low, high), cuts);
}

void ProducePlots::MakeScorePlots(Particle * part, Int_t nbins, Double_t * bins, std::string cuts){
    ;
}

void ProducePlots::MakeScorePlots(Particle * part, Int_t nbins, Double_t low, Double_t high, std::string cuts){
    MakeScorePlots(part, nbins, m_runbd->SetBinning(nbins, low, high), cuts);
}

void ProducePlots::EffPart(Variable var, Int_t nbins, Double_t low, Double_t high, std::string signal_def, std::string cuts){
    //Produce eff. and truth dist from truth tree.
    if(m_outfile->IsOpen()){
        
        int colour = DrawingStyle::Other;
        if(var.name.find("mu") != std::string::npos) colour = DrawingStyle::MuonM;
        else if(var.name.find("pi") != std::string::npos) colour = DrawingStyle::PionP;
        else if(var.name.find("pr") != std::string::npos) colour = DrawingStyle::Proton;
        else colour = DrawingStyle::Other;
        
        TCanvas * effdists = new TCanvas( (var.name + "_eff").c_str(), "", 900,800);
        effdists->cd();
        TH1D * tmp_eff = m_runep->EffVSVar(var.name.c_str(), nbins, low, high, signal_def, cuts, (var.symbol + " (" + var.units + ")")/*.c_str()*/ );//->Draw();
        tmp_eff->SetLineColor(colour);
        tmp_eff->Draw();
    
        TLatex * sig = GetSignal();
        sig->Draw();
        
        PrintLogo(effdists);
        
        effdists->Write();

        delete sig;
        delete tmp_eff;
        delete effdists;
    }
}

void ProducePlots::TruthPart(Variable var, Int_t nbins, Double_t low, Double_t high, std::string cuts, int ac_lev, int branch){
    
    if(m_outfile->IsOpen()){
        int colour = DrawingStyle::Other;
        if(var.name.find("mu") != std::string::npos) colour = DrawingStyle::MuonM;
        else if(var.name.find("pi") != std::string::npos) colour = DrawingStyle::PionP;
        else if(var.name.find("pr") != std::string::npos) colour = DrawingStyle::Proton;
        else colour = DrawingStyle::Other;
        
        TCanvas * truthdists = new TCanvas( (var.name + "_truth").c_str(), "", 900,800);
        truthdists->cd();
        TH1D * tmp_truth = m_runtruthbd->GetHisto(var.name, nbins, low, high, (var.symbol + " (" + var.units + ")"), cuts);
        tmp_truth->SetFillColor( colour );
        tmp_truth->SetLineColor( kBlack );
        tmp_truth->Draw();

        TLegend * leg = m_runbd->Legend(0.2,0.1);
        TH1D * tmp_passcuts;

        if(ac_lev != -1){
            stringstream sac_lev, sbranch;
            sac_lev << ac_lev;
            sbranch << branch;
            string pass_cuts = cuts + " && truth_accum_level[" + sbranch.str() + "] > " + sac_lev.str();
            tmp_passcuts = m_runtruthbd->GetHisto(var.name, nbins, low, high, (var.symbol + " (" + var.units + ")"), pass_cuts);
            tmp_passcuts->SetLineStyle( 2 );//Dashed
            tmp_passcuts->SetLineColor( kWhite );
            tmp_passcuts->Draw("SAME");

            TLegend * leg = m_runbd->Legend(0.2,0.1);
            leg->AddEntry(tmp_truth,    "Sig. Def.",      "f");
            leg->AddEntry(tmp_passcuts, "w/ Cuts Passed", "l");
        }

        TLatex * sig = GetSignal();
        sig->Draw();
        
        PrintLogo(truthdists);
        
        truthdists->Write();
        
        delete sig;
        delete tmp_truth;
        if(tmp_passcuts) delete tmp_passcuts;
        delete leg;
        delete truthdists;
    }
}


void ProducePlots::MakeDir(std::string name){
    if(m_outfile->IsOpen()){
        cdDir();
        TDirectory * tmp_dir = m_outfile->GetDirectory(name.c_str());
        if (!tmp_dir) {
            m_outfile->mkdir(name.c_str());
            cdDir(name);
//            m_outfile->cd((m_savename + ":/" + name).c_str());
        }
        else{
//            cout << "ProducePlots::MakeDir : Directory exists, entering : " << name << endl;
            cdDir(name);
        }
    }
    else cout << "ProducePlots::MakeDir : File is closed..." << endl;
}

void ProducePlots::MakePlots(){
    
    //dEdX (0) and LL mom. (1) reco plots:
    //W/WO michel tag.
    
    //Pass all cuts check://Only consider dEdX ( accum_level[0]) recon for now:
//    string base_dir = "dEdX";

    string EX_base_cut = "accum_level[0] > 5 && " + m_pion->michel + " == 1";
    string LL_base_cut = "accum_level[1] > 5 && " + m_pion_alt->michel + " == 1";//Alt michel too
    //**************************************** Mom START ****************************************//
    
    //Proton:
    // MakeDir("Mom/dEdX/Proton");
    // MakeMomPlots(m_proton, 40, 0, 2000, EX_base_cut);
    
    // //Pion:
    // MakeDir("Mom/dEdX/Pion");
    // MakeMomPlots(m_pion, 40, 0, 2000, EX_base_cut);
    
    // //Muon:
    // MakeDir("Mom/dEdX/Muon");
    // MakeMomPlots(m_muon, 40, 0, 6000, EX_base_cut);
    
    // //Proton:
    // MakeDir("Mom/LL/Proton");
    // MakeMomPlots(m_proton_alt, 40, 0, 2000, LL_base_cut);
    
    // //Pion:
    // MakeDir("Mom/LL/Pion");
    // MakeMomPlots(m_pion_alt, 40, 0, 2000, LL_base_cut);
    
    // //**************************************** Mom END ****************************************//
    // //*****************************************************************************************//
    // //*****************************************************************************************//
    // //*****************************************************************************************//
    // //**************************************** Theta START ************************************//
    
    // //Proton:
    // MakeDir("Theta/dEdX/Proton");
    // MakeThetaPlots(m_proton, 40, 0, TMath::TwoPi(), EX_base_cut);
    
    // //Pion:
    // MakeDir("Theta/dEdX/Pion");
    // MakeThetaPlots(m_pion, 40, 0, TMath::TwoPi(), EX_base_cut);

    // //Muon:
    // MakeDir("Theta/dEdX/Muon");
    // MakeThetaPlots(m_muon, 40, 0, TMath::TwoPi(), EX_base_cut);
    
    // //Proton:
    // MakeDir("Theta/LL/Proton");
    // MakeThetaPlots(m_proton_alt, 40, 0, TMath::TwoPi(), LL_base_cut);
    
    // //Pion:
    // MakeDir("Theta/LL/Pion");
    // MakeThetaPlots(m_pion_alt, 40, 0, TMath::TwoPi(), LL_base_cut);
    
    // //**************************************** Theta END **************************************//
    // //*****************************************************************************************//
    // //*****************************************************************************************//
    // //*****************************************************************************************//
    // //**************************************** Phi START ************************************//

    // //Proton:
    // MakeDir("Phi/dEdX/Proton");
    // MakePhiPlots(m_proton, 40, -TMath::Pi(), TMath::Pi(), EX_base_cut);
    
    // //Pion:
    // MakeDir("Phi/dEdX/Pion");
    // MakePhiPlots(m_pion, 40, -TMath::Pi(), TMath::Pi(), EX_base_cut);
    
    // //Muon:
    // MakeDir("Phi/dEdX/Muon");
    // MakePhiPlots(m_muon, 40, -TMath::Pi(), TMath::Pi(), EX_base_cut);

    // //Proton:
    // MakeDir("Phi/LL/Proton");
    // MakePhiPlots(m_proton_alt, 40, -TMath::Pi(), TMath::Pi(), LL_base_cut);
    
    // //Pion:
    // MakeDir("Phi/LL/Pion");
    // MakePhiPlots(m_pion_alt, 40, -TMath::Pi(), TMath::Pi(), EX_base_cut);
    
    // //**************************************** Theta END **************************************//
    // //*****************************************************************************************//
    // //*****************************************************************************************//
    // //*****************************************************************************************//
    // //**************************************** DPTT START ************************************//
    
    Variable dpTT;
    dpTT.units = "MeV/#it{c}";
    dpTT.symbol = "#deltap_{TT}";
    dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT;
    
    MakeDir("dpTT/dEdX/normal");
    dpTT.savename = m_recovars->dpTT;// + "300";
    ProduceGroup(dpTT, 39, -300, 300, EX_base_cut);
   
    // MakeDir("dpTT/dEdX/pm1000");
    // dpTT.savename = m_recovars->dpTT + "1000";
    // ProduceGroup(dpTT, 99, -1000, 1000, (EX_base_cut + " && " + m_recovars->dpTT + " != -999."));
    
    // MakeDir("dpTT/dEdX/pm7000");
    // dpTT.savename = m_recovars->dpTT + "7000";
    // ProduceGroup(dpTT, 199, -7000, 7000, (EX_base_cut + " && " + m_recovars->dpTT + " != -999."));

    // MakeDir("dpTT/dEdX/tmumom");
    // dpTT.symbol = "#deltap_{TT}";
    // dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_tmumom;
    // dpTT.savename = m_recovars->dpTT_tmumom;
    // ProduceGroup(dpTT, 39, -300, 300, EX_base_cut);
    
    // MakeDir("dpTT/dEdX/tmumom");
    // dpTT.symbol = "#deltap_{TT}";
    // dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_tpimom;
    // dpTT.savename = m_recovars->dpTT_tpimom;
    // ProduceGroup(dpTT, 39, -300, 300, EX_base_cut);
    
    // MakeDir("dpTT/dEdX/tprmom");
    // dpTT.symbol = "#deltap_{TT}";
    // dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_tprmom;
    // dpTT.savename = m_recovars->dpTT_tprmom;
    // ProduceGroup(dpTT, 39, -300, 300, EX_base_cut);
    
    // MakeDir("dpTT/dEdX/tnudir");
    // dpTT.symbol = "#deltap_{TT}";
    // dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_tnudir;
    // dpTT.savename = m_recovars->dpTT_tnudir;
    // ProduceGroup(dpTT, 39, -300, 300, EX_base_cut);
    
    // MakeDir("dpTT/dEdX/pimom");
    // dpTT.symbol = "#deltap_{TT}";
    // dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_pi;
    // dpTT.savename = m_recovars->dpTT_pi;
    // ProduceGroup(dpTT, 39, -300, 300, EX_base_cut);
    
    // MakeDir("dpTT/dEdX/prmom");
    // dpTT.symbol = "#deltap_{TT}";
    // dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_pr;
    // dpTT.savename = m_recovars->dpTT_pr;
    // ProduceGroup(dpTT, 39, -300, 300, EX_base_cut);
    
    // MakeDir("dpTT/dEdX/pidir");
    // dpTT.symbol = "#deltap_{TT}";
    // dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_pidir;
    // dpTT.savename = m_recovars->dpTT_pidir;
    // ProduceGroup(dpTT, 39, -300, 300, EX_base_cut);
    
    // MakeDir("dpTT/dEdX/prdir");
    // dpTT.symbol = "#deltap_{TT}";
    // dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_prdir;
    // dpTT.savename = m_recovars->dpTT_prdir;
    // ProduceGroup(dpTT, 39, -300, 300, EX_base_cut);
    
    // //LL Method
    // MakeDir("dpTT/LL");
    // dpTT.savename = m_recovars->dpTT_alt;// + "300";
    // ProduceGroup(dpTT, 39, -300, 300, EX_base_cut);
    
    // MakeDir("dpTT/LL/pm1000");
    // dpTT.savename = m_recovars->dpTT_alt + "1000";
    // ProduceGroup(dpTT, 99, -1000, 1000, (LL_base_cut + " && " + m_recovars->dpTT_alt + " != -999."));
    
    // MakeDir("dpTT/LL/pm7000");
    // dpTT.savename = m_recovars->dpTT_alt + "7000";
    // ProduceGroup(dpTT, 199, -7000, 7000, (LL_base_cut + " && " + m_recovars->dpTT_alt + " != -999."));
    
    // MakeDir("dpTT/LL/tmumom");
    // dpTT.symbol = "#deltap_{TT}";
    // dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_tmumom_alt;
    // dpTT.savename = m_recovars->dpTT_tmumom_alt;
    // ProduceGroup(dpTT, 39, -300, 300, EX_base_cut);
    
    // MakeDir("dpTT/LL/tmumom");
    // dpTT.symbol = "#deltap_{TT}";
    // dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_tpimom_alt;
    // dpTT.savename = m_recovars->dpTT_tpimom_alt;
    // ProduceGroup(dpTT, 39, -300, 300, LL_base_cut);
    
    // MakeDir("dpTT/LL/tprmom");
    // dpTT.symbol = "#deltap_{TT}";
    // dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_tprmom_alt;
    // dpTT.savename = m_recovars->dpTT_tprmom_alt;
    // ProduceGroup(dpTT, 39, -300, 300, LL_base_cut);
    
    // MakeDir("dpTT/LL/tnudir");
    // dpTT.symbol = "#deltap_{TT}";
    // dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_tnudir_alt;
    // dpTT.savename = m_recovars->dpTT_tnudir_alt;
    // ProduceGroup(dpTT, 39, -300, 300, LL_base_cut);
    
    // MakeDir("dpTT/LL/pimom");
    // dpTT.symbol = "#deltap_{TT}";
    // dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_pi_alt;
    // dpTT.savename = m_recovars->dpTT_pi_alt;
    // ProduceGroup(dpTT, 39, -300, 300, LL_base_cut);
    
    // MakeDir("dpTT/LL/prmom");
    // dpTT.symbol = "#deltap_{TT}";
    // dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_pr_alt;
    // dpTT.savename = m_recovars->dpTT_pr_alt;
    // ProduceGroup(dpTT, 39, -300, 300, LL_base_cut);
    
    // MakeDir("dpTT/LL/pidir");
    // dpTT.symbol = "#deltap_{TT}";
    // dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_pidir_alt;
    // dpTT.savename = m_recovars->dpTT_pidir_alt;
    // ProduceGroup(dpTT, 39, -300, 300, LL_base_cut);
    
    // MakeDir("dpTT/LL/prdir");
    // dpTT.symbol = "#deltap_{TT}";
    // dpTT.name = m_recovars->truedpTT + ":" + m_recovars->dpTT_prdir_alt;
    // dpTT.savename = m_recovars->dpTT_prdir_alt;
    // ProduceGroup(dpTT, 39, -300, 300, LL_base_cut);

    // //**************************************** DPTT END ***************************************//
    // //*****************************************************************************************//
    // //*****************************************************************************************//
    // //*****************************************************************************************//
    // //************************************** W Mass Start *************************************//

    // MakeDir("W");

    // Variable W_mass;
    // W_mass.units = "MeV";
    // W_mass.symbol = "W";
    // W_mass.name = "mc_w";
    // W_mass.savename = "W_EX";

    // TCanvas * w_dist_EL = m_runbd->TARGETSingle(W_mass, 200, 0, 3000, EX_base_cut);
    // PrintLogo(w_dist_EL);
    // w_dist_EL->Write();
    
    // W_mass.savename = "W_LL";
    // TCanvas * w_dist_LL = m_runbd->TARGETSingle(W_mass, 200, 0, 3000, LL_base_cut);
    // PrintLogo(w_dist_LL);
    // w_dist_LL->Write();
    
    //**************************************** W Mass END *************************************//
    //*****************************************************************************************//
    //*****************************************************************************************//
    //*****************************************************************************************//
    //******************************** Efficiency/Purity START ********************************//
    
    
    //*****************************************************************************************//
    //*************************************** VS cuts START ***********************************//

    if(!m_realdata){

        MakeDir("Efficiency/Cuts/dEdX");
        std::string signal_def_new = "truth_n_pro == 1 && truth_n_piP == 1 && truth_n_muo == 1 && mc_nFSPart == 3 && mc_targetZ == 1";
        signal_def_new += " && mc_current == 1 && TMath::RadToDeg()*truth_mu_Theta < 25. && TMath::RadToDeg()*truth_mu_Theta >= 0.";
        signal_def_new += " && truth_true_target_region == 1 && truth_mu_E < 20000. && truth_mu_E > 0.";

        std::string signal_def_old = "truth_n_pro == 1 && truth_n_piP == 1 && truth_n_muo == 1 && mc_nFSPart == 3 && mc_targetZ == 1";
        signal_def_old += " && mc_current == 1 && TMath::RadToDeg()*truth_mu_Theta < 25. && TMath::RadToDeg()*truth_mu_Theta >= 0.";

        m_runep->Debug();

        TCanvas * eff_pur_cuts_EX = new TCanvas("eff_pur_cuts_dEdX","", 600, 800);
        eff_pur_cuts_EX->cd();
        TH1D * eff_EX_new = m_runep->EffVSCuts( signal_def_new );//->Draw("HIST");
        TH1D * pur_EX_new = m_runep->PurVSCuts( signal_def_new );//->Draw("HISTSAME");
        TH1D * eff_EX_old = m_runep->EffVSCuts( signal_def_old );//->Draw("HIST");
        TH1D * pur_EX_old = m_runep->PurVSCuts( signal_def_old );//->Draw("HISTSAME");
        TH1D * eff_EX_mic = m_runep->EffVSCuts( (signal_def_new + " && truth_pi_EX_michel == 1") );//->Draw("HIST");
        TH1D * pur_EX_mic = m_runep->PurVSCuts( (signal_def_new + " && truth_pi_EX_michel == 1") );//->Draw("HISTSAME");    
    
        eff_EX_new->Draw("HIST");
        pur_EX_new->Draw("HISTSAME");
        eff_EX_old->SetLineStyle(7);
        pur_EX_old->SetLineStyle(7);
        eff_EX_old->Draw("HISTSAME");
        pur_EX_old->Draw("HISTSAME");
        eff_EX_mic->SetLineStyle(7);
        pur_EX_mic->SetLineStyle(7);
        eff_EX_mic->Draw("HISTSAME");
        pur_EX_mic->Draw("HISTSAME");

        TLegend * eff_pur_cuts_EX_leg = m_runbd->Legend(0.2,0.1);
        eff_pur_cuts_EX_leg->AddEntry(eff_EX_new, "Efficiency (New)", "l");
        eff_pur_cuts_EX_leg->AddEntry(pur_EX_new, "Purity (New)", "l");
        eff_pur_cuts_EX_leg->AddEntry(eff_EX_old, "Efficiency (Old)", "l");
        eff_pur_cuts_EX_leg->AddEntry(pur_EX_old, "Purity (Old)", "l");
        eff_pur_cuts_EX_leg->AddEntry(eff_EX_mic, "Eff. (New w #pi ME tag)", "l");
        eff_pur_cuts_EX_leg->AddEntry(pur_EX_mic, "Pur. (New w #pi ME tag)", "l");
        eff_pur_cuts_EX_leg->Draw();
    
        eff_pur_cuts_EX->Write();
    
        delete eff_EX_new;
        delete pur_EX_new;
        delete eff_EX_old;
        delete pur_EX_old;
        delete eff_EX_mic;
        delete pur_EX_mic;
        delete eff_pur_cuts_EX_leg;
        delete eff_pur_cuts_EX;
    
        MakeDir("Efficiency/Cuts/LL");

        TCanvas * eff_pur_cuts_LL = new TCanvas("eff_pur_cuts_LL","", 600, 800);
        eff_pur_cuts_LL->cd();

        m_runep->Debug();
        TH1D * eff_LL_new = m_runep->EffVSCuts( signal_def_new, 1 );//->Draw("HIST");
        m_runep->Debug();
        TH1D * pur_LL_new = m_runep->PurVSCuts( signal_def_new, 1 );//->Draw("HISTSAME");
        TH1D * eff_LL_old = m_runep->EffVSCuts( signal_def_old, 1 );//->Draw("HIST");
        TH1D * pur_LL_old = m_runep->PurVSCuts( signal_def_old, 1 );//->Draw("HISTSAME");
        TH1D * eff_LL_mic = m_runep->EffVSCuts( (signal_def_new + " && truth_pi_michel == 1"), 1 );//->Draw("HIST");
        TH1D * pur_LL_mic = m_runep->PurVSCuts( (signal_def_new + " && truth_pi_michel == 1"), 1 );//->Draw("HISTSAME");

        eff_LL_new->Draw("HIST");
        pur_LL_new->Draw("HISTSAME");
        eff_LL_old->SetLineStyle(7);
        pur_LL_old->SetLineStyle(7);
        eff_LL_old->Draw("HISTSAME");
        pur_LL_old->Draw("HISTSAME");
        eff_LL_mic->SetLineStyle(4);
        pur_LL_mic->SetLineStyle(4);
        eff_LL_mic->Draw("HISTSAME");
        pur_LL_mic->Draw("HISTSAME");

        TLegend * eff_pur_cuts_LL_leg = m_runbd->Legend(0.2,0.1);
        eff_pur_cuts_LL_leg->AddEntry(eff_LL_new, "Efficiency (New)", "l");
        eff_pur_cuts_LL_leg->AddEntry(pur_LL_new, "Purity (New)", "l");
        eff_pur_cuts_LL_leg->AddEntry(eff_LL_old, "Efficiency (Old)", "l");
        eff_pur_cuts_LL_leg->AddEntry(pur_LL_old, "Purity (Old)", "l");
        eff_pur_cuts_LL_leg->AddEntry(eff_LL_mic, "Eff. (New w #pi ME tag)", "l");
        eff_pur_cuts_LL_leg->AddEntry(pur_LL_mic, "Pur. (New w #pi ME tag)", "l");
        eff_pur_cuts_LL_leg->Draw();
    
        eff_pur_cuts_LL->Write();

        delete eff_LL_new;
        delete pur_LL_new; 
        delete eff_LL_old;
        delete pur_LL_old;
        delete eff_LL_mic;
        delete pur_LL_mic;
        delete eff_pur_cuts_LL_leg;
        delete eff_pur_cuts_LL;
    
    //*****************************************************************************************//
    //**************************************** VS cuts END ************************************//

    
    //*****************************************************************************************//
    //*************************************** VS eff. START ***********************************//
    
        for(int build = 0; build < 2; build++){

            string mom_type = "dEdX";
            if(build == 1) mom_type = "LL";

            string cut_dEdX = "truth_accum_level[" + string(build == 0 ? "0" : "1") + "] > 5";// && truth_pi_michel == 1";
        
            Int_t truemom_bins[3] = {40, 40, 40};
            Double_t truemom_low[3] = {0., 0., 1500.};
            Double_t truemom_hig[3] = {2000., 2000., 20000.};

            string true_sym[3] = {"p", "#pi^{+}", "#mu^{-}"};
            string true_nam[3] = {"pr", "pi", "mu"};

            Variable truemom;
            truemom.units = "MeV/#it{c}";
            for(int p = 0; p < 3; p++){
                truemom.name = "truth_" + true_nam[p] + "_mom";
                truemom.symbol = "#it{p}_{" + true_sym[p] + "}";

                if(p == 2) m_runep->Debug();//ON

                MakeDir("Efficiency/" + mom_type + "/Mom");
                EffPart(truemom, truemom_bins[p], truemom_low[p], truemom_hig[p], signal_def_new, cut_dEdX);

                if(p == 2) m_runep->Debug();//OFF

                if(build == 0){
                    MakeDir("Truth/Mom");
                    TruthPart(truemom, truemom_bins[p], truemom_low[p], truemom_hig[p], signal_def_new);
                }
            }
        
            Variable truetheta;//Is this range between 0 and pi?
            truetheta.units = "Rad.";
            for(int p = 0; p < 3; p++){
                truetheta.name = "truth_" + true_nam[p] + "_Theta";
                truetheta.symbol = "#theta_{" + true_sym[p] + "}";

                MakeDir("Efficiency/" + mom_type + "/Theta");
                EffPart(truetheta, 25, 0., TMath::Pi(), signal_def_new, cut_dEdX);

                if(build == 0){
                    MakeDir("Truth/Theta");
                    TruthPart(truetheta, 25, 0, TMath::Pi(), signal_def_new);
                }
            }
        
            Variable truephi;
            truephi.units = "Rad.";
            for(int p = 0; p < 3; p++){
                truephi.name = "truth_" + true_nam[p] + "_Phi";
                truephi.symbol = "#phi_{" + true_sym[p] + "}";

                MakeDir("Efficiency/" + mom_type + "/Phi");
                EffPart(truephi, 25, -TMath::Pi(), TMath::Pi(), signal_def_new, cut_dEdX);

                if(build == 0){
                    MakeDir("Truth/Phi");
                    TruthPart(truephi, 25, -TMath::Pi(), TMath::Pi(), signal_def_new);
                }
            }
        }
    
        Variable trueW;
        trueW.units = "MeV";
        trueW.name = "mc_w";
        trueW.symbol = "W";
        MakeDir("Truth/W");
        TruthPart(trueW, 40, 0, 3000, signal_def_new, 5);
        MakeDir("Truth/W/EX");
        TruthPart(trueW, 40, 0, 3000, signal_def_new, 5);
        MakeDir("Truth/W/LL");
        TruthPart(trueW, 40, 0, 3000, signal_def_new, 5, 1);
    
    //*****************************************************************************************//
    //**************************************** VS eff. END ************************************//
    
    //********************************** Efficiency/Purity END ********************************//
    //*****************************************************************************************//
    //*****************************************************************************************//
    //*****************************************************************************************//
    //*************************************** Truth START *************************************//
    
//    //*****************************************************************************************//
//    //*************************************** VS eff. START ***********************************//
////   m_runtruthbd
//    
//    for(int build = 0; build < 2; build++){
//        
//        string mom_type = "dEdX";
//        if(build == 1) mom_type = "LL";
//        
//        MakeDir("Truth/" + mom_type + "/Mom");
//        string cut_dEdX = "truth_accum_level[" + string(build == 0 ? "0" : "1") + "] > 5";// && truth_pi_michel == 1";
//        
//        Int_t truemom_bins[3] = {40, 40, 40};
//        Double_t truemom_low[3] = {0., 0., 1500.};
//        Double_t truemom_hig[3] = {2000., 2000., 20000.};
//        
//        string true_sym[3] = {"p", "#pi^{+}", "#mu^{-}"};
//        string true_nam[3] = {"pr", "pi", "mu"};
//        
//        Variable truemom;
//        truemom.units = "MeV/#it{c}";
//        for(int p = 0; p < 3; p++){
//            truemom.name = "truth_" + true_nam[p] + "_mom";
//            truemom.symbol = "#it{p}_{" + true_sym[p] + "}";
//            TruthPart(truemom, nbins, low, high, cuts);
//        }
//        
//        MakeDir("Truth/" + mom_type + "/Theta");
//        Variable truetheta;//Is this range between 0 and pi?
//        truetheta.units = "Rad.";
//        for(int p = 0; p < 3; p++){
//            truetheta.name = "truth_" + true_nam[p] + "_Theta";
//            truetheta.symbol = "#theta_{" + true_sym[p] + "}";
//            EffPart(truetheta, 40, 0., TMath::TwoPi(), signal_def_new, cut_dEdX);
//        }
//        
//        MakeDir("Truth/" + mom_type + "/Phi");
//        Variable truephi;
//        truephi.units = "Rad.";
//        for(int p = 0; p < 3; p++){
//            truephi.name = "truth_" + true_nam[p] + "_Phi";
//            truephi.symbol = "#theta_{" + true_sym[p] + "}";
//            EffPart(truephi, 40, -TMath::Pi(), TMath::Pi(), signal_def_new, cut_dEdX);
//        }
//    }
//    
    //****************************************** Truth END ************************************//
    //*****************************************************************************************//
    
    }
    //CLOSE THE DIRECTORY
    if(m_outfile->IsOpen()) m_outfile->Close();
    if(m_outfile) delete m_outfile;
    //DONE
}


std::string ProducePlots::GetDate(){

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

TLatex * ProducePlots::GetLogo(){
    return new TLatex(0.0, 0.1, ("#font[62]{" + m_exper_logo + "}").c_str() );
}

TLatex * ProducePlots::GetSignal(){
    return new TLatex(0.0, 0.1, ("#font[62]{#it{" + m_exper_signal + "}}").c_str() );
}

int main(int argc, char *argv[])
{

    string filename = testing_mc;
    string savename = "";// = "CC1P1PiP_Plots_" + sday.str() + smon.str() + syear.str() + ".root";
    bool debug = false;
    EXP::EXP experiment = EXP::UNKNOWN;
    bool realdata = false;

    char cc;
    while((cc = getopt(argc, argv, "i:o:d::t::m::r::")) != -1){
        switch (cc){
            case 'i': filename = std::string(optarg); break;
            case 'o': savename = optarg; break;
            case 'd': debug = true; break;
            case 't': experiment = EXP::T2K; break;
            case 'm': experiment = EXP::MIN; break;
            case 'r': realdata = true; break;

            default: return 1;
        }
    }
    
    if(experiment == EXP::UNKNOWN){
        cout << "**** Experiment not defined ****" << endl;
        cout << "For     T2K : -t" << endl;
        cout << "For MINERvA : -m" << endl;
        return 0;
    }

    ProducePlots * plots = new ProducePlots(experiment, filename, debug, realdata);
    
    if(!savename.empty()) plots->SetSaveName(savename);
    
    plots->MakePlots();

//    delete plots; -- TODO: Fix this
    
    return 0;
}
