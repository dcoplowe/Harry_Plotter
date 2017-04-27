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
#include "DataClasses.h"
// #include "PartKinMaps.h"

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

const string test_min_mc("/pnfs/minerva/persistent/users/dcoplowe/CC1P1Pi_PL13C_030317/grid/central_value/minerva/ana/v10r8p9/00/01/32/00/SIM_minerva_00013200_Subruns_0001-0002-0003-0004_CC1P1PiAnalysis_Ana_Tuple_v10r8p9-dcoplowe.root");
const string test_t2k_mc("/data/t2k/coplowe/numuCC1P1PiOutput/march17/neutP6BWA/FGD1/nall_data_070417_wome/allTree.root");

class ProducePlots {

public:
    ProducePlots(Experiment::Name exp, std::string filename, bool debug, bool realdata);
    ~ProducePlots();
    
    void SetSaveName(std::string var){ m_savename = var; }
    
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

    void MakeCosThetaPlots(Particle * part, Int_t nbins, Double_t * bins, std::string cuts, bool nudir = false);
    void MakeCosThetaPlots(Particle * part, Int_t nbins, Double_t low, Double_t high, std::string cuts, bool nudir = false);

    // void MakeScorePlots(Particle * part, Int_t nbins, Double_t * bins, std::string cuts);
    // void MakeScorePlots(Particle * part, Int_t nbins, Double_t low, Double_t high, std::string cuts);
    
    void ProduceGroup(Variable var, Int_t nbins, Double_t * bins, std::string cuts);
    void ProduceGroup(Variable var, Int_t nbins, Double_t low, Double_t high, std::string cuts);
    
    void EffPart(Variable var, Int_t nbins, Double_t low, Double_t high, std::string signal_def, std::string cuts = "");
    void PurPart(Variable var, std::string signal_def, std::string cuts);

    void TruthPart(Variable var, Int_t nbins, Double_t low, Double_t high, std::string cuts = "", int ac_lev = -1, int branch = 0);
    
    void MakeDir(std::string name);
    void cdDir(std::string name = ""){  m_outfile->cd((m_savename + ":/" + name).c_str()); }//Default is the root dir.
    
private:
    std::string m_infilename;
    bool m_realdata;

    Experiment * m_experiment;

    KinematicVars * m_recovars;
    
    std::string m_savename;
    
    TFile * m_outfile;// = new TFile(m_savename.c_str(), "RECREATE");
    
    std::string GetDate();
    
    //tmp POT bool (off for t2k):
    bool m_getPOT;
    
//    TFile * m_outfile;
    
    Particle * m_muon;
    Particle * m_proton;
    Particle * m_pion;
    
    Particle * m_proton_alt;
    Particle * m_pion_alt;
    
    BreakdownTools  * m_runbd;
    BreakdownTools  * m_runtruthbd;
    EffPurTools     * m_runep;
};

ProducePlots::ProducePlots(Experiment::Name exp, std::string filename, bool debug, bool realdata) : m_infilename(filename),
m_realdata(realdata) {
    
    cout << "Experiment: " <<  Experiment::ToString(exp) << endl;
    
    m_experiment = new Experiment(exp);
    
    if(exp == Experiment::T2K){
        m_getPOT = false;
    }
    else if(exp == Experiment::MIN){
        m_getPOT = true;
    }
    
    m_recovars = new KinematicVars(exp);//Setup reco var names
    //For truth tree seems like all we need is the following form: 'truth_pi_E'
    
    m_muon = m_recovars->muon;
    m_proton = m_recovars->proton;
    m_pion = m_recovars->pion;
    
    if(exp == Experiment::MIN){
        m_proton_alt = m_recovars->proton_alt;
        m_pion_alt = m_recovars->pion_alt;
    }
    
    m_savename = Experiment::ToString(exp) + "_CC1P1PiPlus_Plots_" + GetDate() + ".root";
    
    cout << "Saving file as " << m_savename << endl;
    
    m_runbd = new BreakdownTools( m_infilename, m_experiment->GetRecoName(), m_experiment->GetTopologies(), m_experiment->GetTarVarName() );
    if(!m_realdata) m_runtruthbd = new BreakdownTools(m_infilename, m_experiment->GetTrueName(), m_experiment->GetTopologies(), m_experiment->GetTarVarName());
    
    // std::vector<std::string> selection_cuts;
    // selection_cuts.push_back("Vertex");
    // selection_cuts.push_back("3 Tracks");
    // selection_cuts.push_back("Muon Track");
    // selection_cuts.push_back("Contained Vtx");
    // selection_cuts.push_back("PID: p/#pi^{+}");
    // selection_cuts.push_back("Michel Sense");
    
    if(!m_realdata) m_runep = new EffPurTools(m_infilename, m_experiment->GetCutNames(), m_experiment->GetRecoName(), m_experiment->GetTrueName() );
    
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
    ProduceGroup(var, nbins, DrawingTools::SetBinning(nbins, low, high), cuts);
}

void ProducePlots::ProduceGroup(Variable var, Int_t nbins, Double_t * bins, std::string cuts){//, Int_t setsave = 11111??

    if(m_outfile->IsOpen()){
        
        BDCans var_top = m_runbd->TOPO(var, nbins, bins, cuts);

        BDCans var_tar = m_runbd->TARGET(var, nbins, bins, cuts);
        
        BDCans var_pid;
        if(!var.GetPDG().empty()) var_pid = m_runbd->PID(var, nbins, bins, var.GetPDG(), cuts);
        
        //Recon Vars:
        PrintLogo(var_top.recon);
        PrintLogo(var_tar.recon);
        if(!var.GetPDG().empty()) PrintLogo(var_pid.recon);
        
        var_top.recon->Write();
        var_tar.recon->Write();
        if(!var.GetPDG().empty()) var_pid.recon->Write();
        
        //Truth Vars:
        PrintLogo(var_top.truth);
        PrintLogo(var_tar.truth);
        if(!var.GetPDG().empty()) PrintLogo(var_pid.truth);
        
        var_top.truth->Write();
        var_tar.truth->Write();
        if(!var.GetPDG().empty()) var_pid.truth->Write();
        
        //Ratio Vars:
        PrintLogo(var_top.ratio);
        PrintLogo(var_tar.ratio);
        if(!var.GetPDG().empty()) PrintLogo(var_pid.ratio);
        
        var_top.ratio->Write();
        var_tar.ratio->Write();
        if(!var.GetPDG().empty()) var_pid.ratio->Write();
        
        //Smear Vars:
        PrintLogo(var_top.smear);
//        PrintLogo(var_tar.smear);
//        if(!var.GetPDG().empty()) PrintLogo(var_pid.smear);
        
        PrintLogo(var_top.smearSN);
//        PrintLogo(var_tar.smearSN);
//        if(!var.GetPDG().empty()) PrintLogo(var_pid.smearSN);
        
        var_top.smear->Write();
//        var_tar.smear->Write();
//        if(!var.GetPDG().empty()) var_pid.smear->Write();
        
        var_top.smearSN->Write();
//        var_tar.smearSN->Write();
//        if(!var.GetPDG().empty()) var_pid.smearSN->Write();
        
    }
    else std::cout << "ProducePlots::ProduceGroup : ERROR : File is not open..." << std::endl;
    
}

void ProducePlots::MakeMomPlots(Particle * part, Int_t nbins, Double_t * bins, std::string cuts){
    // Variable(std::string name, std::string symbol, std::string units)
    Variable mom(part->trueP.GetName() + ":" + part->P.GetName(), "#it{p}_{" + part->GetSymbol() + "}", "MeV/#it{c}");
    mom.SetSName(part->P.GetName());
    mom.SetPDG(part->pdg.GetName());
    ProduceGroup(mom, nbins, bins, cuts);
}

void ProducePlots::MakeMomPlots(Particle * part, Int_t nbins, Double_t low, Double_t high, std::string cuts){
    MakeMomPlots(part, nbins, DrawingTools::SetBinning(nbins, low, high), cuts);
}

void ProducePlots::MakeThetaPlots(Particle * part, Int_t nbins, Double_t * bins, std::string cuts){
    Variable theta(part->truetheta.GetName() + ":" + part->theta.GetName(), "#theta_{" + part->GetSymbol() + "}", "rad");
    //    theta.name = m_proton->trueP + "*TMath::RadToDeg():" + m_proton->P + "*TMath::RadToDeg()";//This probably wont work as the code looks for :: to make a split... Add fix.
    theta.SetSName(part->theta.GetName());
    theta.SetPDG(part->pdg.GetName());
    ProduceGroup(theta, nbins, bins, cuts);
}

void ProducePlots::MakeThetaPlots(Particle * part, Int_t nbins, Double_t low, Double_t high, std::string cuts){
    MakeThetaPlots(part, nbins, DrawingTools::SetBinning(nbins, low, high), cuts);
}

void ProducePlots::MakePhiPlots(Particle * part, Int_t nbins, Double_t * bins, std::string cuts){
    Variable phi(part->truephi.GetName() + ":" + part->phi.GetName(), "#phi_{" + part->GetSymbol() + "}", "rad");
    // This probably wont work as the code looks for :: to make a split... Add fix.
    phi.SetSName(part->phi.GetName());
    phi.SetPDG(part->pdg.GetName());
    ProduceGroup(phi, nbins, bins, cuts);
}

void ProducePlots::MakePhiPlots(Particle * part, Int_t nbins, Double_t low, Double_t high, std::string cuts){
    MakePhiPlots(part, nbins, DrawingTools::SetBinning(nbins, low, high), cuts);
}

void ProducePlots::MakeCosThetaPlots(Particle * part, Int_t nbins, Double_t * bins, std::string cuts, bool nudir){
    Variable var(part->truectheta.GetName() + ":" + (nudir ? part->ctheta_nudir.GetName() : part->ctheta.GetName()), "cos#theta_{" + part->GetSymbol() + "}", "");
    var.SetSName( (nudir ? part->ctheta_nudir.GetName() : part->ctheta.GetName()) );
    var.SetPDG(part->pdg.GetName());
    ProduceGroup(var, nbins, bins, cuts);
}

void ProducePlots::MakeCosThetaPlots(Particle * part, Int_t nbins, Double_t low, Double_t high, std::string cuts, bool nudir){
    MakeCosThetaPlots(part, nbins, DrawingTools::SetBinning(nbins, low, high), cuts);
}

// void ProducePlots::MakeScorePlots(Particle * part, Int_t nbins, Double_t * bins, std::string cuts){
//     ;
// }

// void ProducePlots::MakeScorePlots(Particle * part, Int_t nbins, Double_t low, Double_t high, std::string cuts){
//     MakeScorePlots(part, nbins, DrawingTools::SetBinning(nbins, low, high), cuts);
// }

void ProducePlots::EffPart(Variable var, Int_t nbins, Double_t low, Double_t high, std::string signal_def, std::string cuts){
    //Produce eff. and truth dist from truth tree.
    if(m_outfile->IsOpen()){
        
        int colour = DrawingStyle::Other;
        if(var.GetName().find("mu") != std::string::npos) colour = DrawingStyle::MuonM;
        else if(var.GetName().find("pi") != std::string::npos) colour = DrawingStyle::PionP;
        else if(var.GetName().find("pr") != std::string::npos) colour = DrawingStyle::Proton;
        else colour = DrawingStyle::Other;
        
        TCanvas * effdists = new TCanvas( (var.GetName() + "_eff").c_str(), "", 900,800);
        effdists->cd();
        TH1D * tmp_eff = m_runep->EffVSVar(var.GetName().c_str(), nbins, low, high, signal_def, cuts, (var.GetSymbol() + " (" + var.GetUnits() + ")")/*.c_str()*/ );//->Draw();
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

void ProducePlots::PurPart(Variable var, std::string signal_def, std::string cuts){
    //Produce purity. and reco dists. from truth tree.
    if(m_outfile->IsOpen()){
        
        int colour = DrawingStyle::Other;
        if(var.GetName().find("mu") != std::string::npos) colour = DrawingStyle::MuonM;
        else if(var.GetName().find("pi") != std::string::npos) colour = DrawingStyle::PionP;
        else if(var.GetName().find("pr") != std::string::npos) colour = DrawingStyle::Proton;
        else colour = DrawingStyle::Other;
        
        TCanvas * purdists = new TCanvas( (var.GetName() + "_pur").c_str(), "", 900,800);
        purdists->cd();
        TH1D * tmp_pur = m_runep->PurVSVar(var.GetName().c_str(), var.GetNBins(), var.GetBinning(), signal_def, (var.GetSymbol() + " (" + var.GetUnits() + ")"), cuts);
        tmp_pur->SetLineColor(colour);
        tmp_pur->Draw();
    
        TLatex * sig = GetSignal();
        sig->Draw();
        
        PrintLogo(purdists);
        
        purdists->Write();

        delete sig;
        delete tmp_pur;
        delete purdists;
    }
}

void ProducePlots::TruthPart(Variable var, Int_t nbins, Double_t low, Double_t high, std::string cuts, int ac_lev, int branch){
    
    if(m_outfile->IsOpen()){
        int colour = DrawingStyle::Other;
        if(var.GetName().find("mu") != std::string::npos) colour = DrawingStyle::MuonM;
        else if(var.GetName().find("pi") != std::string::npos) colour = DrawingStyle::PionP;
        else if(var.GetName().find("pr") != std::string::npos) colour = DrawingStyle::Proton;
        else colour = DrawingStyle::Other;
        
        TCanvas * truthdists = new TCanvas( (var.GetName() + "_truth").c_str(), "", 900,800);
        truthdists->cd();
        TH1D * tmp_truth = m_runtruthbd->GetHisto(var.GetName(), nbins, low, high, (var.GetSymbol() + " (" + var.GetUnits() + ")"), cuts);
        tmp_truth->SetFillColor( colour );
        tmp_truth->SetLineColor( kBlack );
        tmp_truth->Draw();

        TLegend * leg = m_runbd->Legend(0.2,0.1);
        TH1D * tmp_passcuts;

        if(ac_lev != -1){
            stringstream sac_lev, sbranch;
            sac_lev << ac_lev;
            sbranch << branch;
            string pass_cuts = cuts + " && truth_accum_level[" + sbranch.str() + "] > " + sac_lev.str();//TODO: Will need to add experiment base cuts 
            tmp_passcuts = m_runtruthbd->GetHisto(var.GetName(), nbins, low, high, (var.GetSymbol() + " (" + var.GetUnits() + ")"), pass_cuts);
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
        
        // delete sig;
        // delete tmp_truth;
        // // if(tmp_passcuts) delete tmp_passcuts;
        // delete leg;
        // delete truthdists;
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

    // T2K only has one reco type
    // MIN has 2

    string * basecuts;
    string * branchnames;
    int nbranches = 1;

    if(m_experiment->GetType() == Experiment::MIN && m_pion_alt && m_proton_alt){

        nbranches = 2;
        basecuts = new string[ nbranches ];

        branchnames = new string[ nbranches ];
        branchnames[0] = "/dEdX";
        branchnames[1] = "/LL";

        basecuts[0] = m_experiment->GetBaseCuts(5, 0, m_pion->michel.GetName() + " == 1 && target_region == 1");//"accum_level[0] > 5 && " + m_pion->michel + " == 1 && target_region == 1";
        basecuts[1] = m_experiment->GetBaseCuts(5, 1, m_pion_alt->michel.GetName() + " == 1 && target_region == 1");//"accum_level[1] > 5 && " + m_pion_alt->michel + " == 1 && target_region == 1";
    }
    else{
        branchnames = new string[ nbranches ];
        branchnames[0] = "";

        basecuts = new string[ nbranches ];
        basecuts[0] = m_experiment->GetBaseCuts(4, 0);//Change back to 5
    }

    for(int br = 0; br < nbranches; br++){
        std::vector<Particle*> list;

        if(br == 1 && m_experiment->GetType() == Experiment::MIN){
            list.push_back( m_proton_alt );
            list.push_back( m_pion_alt );        
        }
        else{
            list.push_back( m_proton );
            list.push_back( m_pion );
            list.push_back( m_muon );
        }

        for(unsigned int ptls = 0; ptls < list.size(); ptls++){
            Particle * party = list[ptls];
            
            //**************************************** Mom START ****************************************//
            MakeDir("Mom" + branchnames[br] + "/" + party->GetName() );
            MakeMomPlots(party, party->P.GetNBins(), party->P.GetBinning(), basecuts[br]);
            //**************************************** Mom END ****************************************//

            if(m_experiment->GetType() == Experiment::MIN){
                //**************************************** Theta START ************************************//
                MakeDir("Theta" + branchnames[br] + "/" + party->GetName() );
                MakeThetaPlots(party, party->theta.GetNBins(), party->theta.GetBinning(), basecuts[br]);
                //**************************************** Theta END **************************************//
            
                //**************************************** Phi START ************************************//
                MakeDir("Phi" + branchnames[br] + "/" + party->GetName() );
                MakePhiPlots(party, party->phi.GetNBins(), party->phi.GetBinning(), basecuts[br]);
                //**************************************** Phi END **************************************//                
            }

            if(m_experiment->GetType() == Experiment::T2K){

                //**************************************** cosTheta START ************************************//
                MakeDir("cTheta/" + party->GetName() );
                MakeCosThetaPlots(party, party->ctheta.GetNBins(), party->ctheta.GetBinning(), basecuts[br]);
                MakeCosThetaPlots(party, party->ctheta_nudir.GetNBins(), party->ctheta_nudir.GetBinning(), basecuts[br], true);
                //**************************************** cosTheta END **************************************//

                //**************************************** Crossing Angle START ************************************//
                MakeDir("CrossingAngle/" + party->GetName() );
                
                // ProduceGroup(party->cross_angle, party->cross_angle.GetNBins(), party->cross_angle.GetBinning(), basecuts[br]);
                TCanvas * ca_can = m_runbd->TARGETSingle(party->cross_angle, party->cross_angle.GetNBins(), party->cross_angle.GetBinning(), basecuts[br]);
                PrintLogo(ca_can);
                ca_can->Write();

                // Purity of the crossing angle:
                PurPart(party->cross_angle, m_experiment->GetSignal(), basecuts[br]); 

                //**************************************** Crossing Angle END **************************************//


            }

        }
        // Variable dpTT;
        MakeDir("dpTT" + branchnames[br]);
        Variable dpTT(m_recovars->truedpTT + ":" + m_recovars->dpTT, "#delta#it{p}_{TT}", "MeV/#it{c}");
        dpTT.SetSName(m_recovars->dpTT);
        ProduceGroup(dpTT, 39, -300, 300, basecuts[br]);

        MakeDir("dpTT" + branchnames[br] + "/nb29");
        dpTT.SetSName(m_recovars->dpTT + "_nb29");
        ProduceGroup(dpTT, 29, -300, 300, basecuts[br]);

        MakeDir("dpTT" + branchnames[br] + "/nb25");
        dpTT.SetSName(m_recovars->dpTT + "_nb25");
        ProduceGroup(dpTT, 25, -300, 300, basecuts[br]);

        MakeDir("dpTT" + branchnames[br] + "/nb19");
        dpTT.SetSName(m_recovars->dpTT + "_nb19");
        ProduceGroup(dpTT, 19, -300, 300, basecuts[br]);

        if(m_experiment->GetType() == Experiment::MIN){

            std::vector<std::string> dptt_list;
            dptt_list.push_back( m_recovars->dpTT_tmumom );
            dptt_list.push_back( m_recovars->dpTT_tpimom );
            dptt_list.push_back( m_recovars->dpTT_tprmom );
            dptt_list.push_back( m_recovars->dpTT_tnudir );
            dptt_list.push_back( m_recovars->dpTT_pi );
            dptt_list.push_back( m_recovars->dpTT_pr );
            dptt_list.push_back( m_recovars->dpTT_pr );
            dptt_list.push_back( m_recovars->dpTT_pidir );
            dptt_list.push_back( m_recovars->dpTT_prdir );

            for(unsigned int dptt_loop = 0; dptt_loop < dptt_list.size(); dptt_loop++){
                MakeDir("dpTT" + branchnames[br] + "/" + dptt_list[dptt_loop] );
                dpTT.SetName( m_recovars->truedpTT + ":" + dptt_list[dptt_loop] );
                dpTT.SetSName(dptt_list[dptt_loop]);
                ProduceGroup(dpTT, 39, -300, 300, basecuts[br]);
            }
        }

        list.clear();
    }
    
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
    
    // //**************************************** W Mass END *************************************//
    // //*****************************************************************************************//
    // //*****************************************************************************************//
    // //*****************************************************************************************//
    // //******************************** Efficiency/Purity START ********************************//
    
    
    // //*****************************************************************************************//
    // //*************************************** VS cuts START ***********************************//

    // if(!m_realdata){

    //     MakeDir("Efficiency/Cuts/dEdX");
    //     std::string signal_def_new = "truth_n_pro == 1 && truth_n_piP == 1 && truth_n_muo == 1 && mc_nFSPart == 3 && mc_targetZ == 1";
    //     signal_def_new += " && mc_current == 1 && TMath::RadToDeg()*truth_mu_Theta < 25. && TMath::RadToDeg()*truth_mu_Theta >= 0.";
    //     signal_def_new += " && truth_true_target_region == 1 && truth_mu_E < 20000. && truth_mu_E > 0.";

    //     std::string signal_def_old = "truth_n_pro == 1 && truth_n_piP == 1 && truth_n_muo == 1 && mc_nFSPart == 3 && mc_targetZ == 1";
    //     signal_def_old += " && mc_current == 1 && TMath::RadToDeg()*truth_mu_Theta < 25. && TMath::RadToDeg()*truth_mu_Theta >= 0.";

    //     // m_runep->Debug();

    //     TCanvas * eff_pur_cuts_EX = new TCanvas("eff_pur_cuts_dEdX","", 600, 800);
    //     eff_pur_cuts_EX->cd();
    //     TH1D * eff_EX_new = m_runep->EffVSCuts( signal_def_new );//->Draw("HIST");
    //     TH1D * pur_EX_new = m_runep->PurVSCuts( signal_def_new );//->Draw("HISTSAME");
    //     TH1D * eff_EX_old = m_runep->EffVSCuts( signal_def_old );//->Draw("HIST");
    //     TH1D * pur_EX_old = m_runep->PurVSCuts( signal_def_old );//->Draw("HISTSAME");
    //     // TH1D * eff_EX_mic = m_runep->EffVSCuts( (signal_def_new + " && truth_pi_EX_michel == 1") );//->Draw("HIST");
    //     // TH1D * pur_EX_mic = m_runep->PurVSCuts( (signal_def_new + " && truth_pi_EX_michel == 1") );//->Draw("HISTSAME");    
    
    //     eff_EX_new->Draw("HIST");
    //     pur_EX_new->Draw("HISTSAME");
    //     eff_EX_old->SetLineStyle(7);
    //     pur_EX_old->SetLineStyle(7);
    //     eff_EX_old->Draw("HISTSAME");
    //     pur_EX_old->Draw("HISTSAME");
    //     // eff_EX_mic->SetLineStyle(7);
    //     // pur_EX_mic->SetLineStyle(7);
    //     // eff_EX_mic->Draw("HISTSAME");
    //     // pur_EX_mic->Draw("HISTSAME");

    //     TLegend * eff_pur_cuts_EX_leg = m_runbd->Legend(0.2,0.1);
    //     eff_pur_cuts_EX_leg->AddEntry(eff_EX_new, "Efficiency (New)", "l");
    //     eff_pur_cuts_EX_leg->AddEntry(pur_EX_new, "Purity (New)", "l");
    //     eff_pur_cuts_EX_leg->AddEntry(eff_EX_old, "Efficiency (Old)", "l");
    //     eff_pur_cuts_EX_leg->AddEntry(pur_EX_old, "Purity (Old)", "l");
    //     // eff_pur_cuts_EX_leg->AddEntry(eff_EX_mic, "Eff. (New w #pi ME tag)", "l");
    //     // eff_pur_cuts_EX_leg->AddEntry(pur_EX_mic, "Pur. (New w #pi ME tag)", "l");
    //     eff_pur_cuts_EX_leg->Draw();
    
    //     eff_pur_cuts_EX->Write();
    
    //     TCanvas * pur_cuts_EX = new TCanvas("pur_cuts_dEdX","", 600, 800);
    //     pur_cuts_EX->cd();
    //     pur_EX_new->Draw("HIST");
    //     pur_EX_old->SetLineStyle(7);
    //     pur_EX_old->Draw("HISTSAME");

    //     TLegend * pur_cuts_EX_leg = m_runbd->Legend(0.2,0.1);
    //     pur_cuts_EX_leg->AddEntry(pur_EX_new, "Purity (New)", "l");
    //     pur_cuts_EX_leg->AddEntry(pur_EX_old, "Purity (Old)", "l");
    //     pur_cuts_EX_leg->Draw();

    //     pur_cuts_EX->Write();

    //     TCanvas * eff_cuts_EX = new TCanvas("eff_cuts_dEdX","", 600, 800);
    //     eff_cuts_EX->cd();
    //     eff_EX_new->Draw("HIST");
    //     eff_EX_old->SetLineStyle(7);
    //     eff_EX_old->Draw("HISTSAME");

    //     TLegend * eff_cuts_EX_leg = m_runbd->Legend(0.2,0.1);
    //     eff_cuts_EX_leg->AddEntry(eff_EX_new, "Eff. (New)", "l");
    //     eff_cuts_EX_leg->AddEntry(eff_EX_old, "Eff. (Old)", "l");
    //     eff_cuts_EX_leg->Draw();
    //     eff_cuts_EX->Write();

    //     delete eff_EX_new;
    //     delete pur_EX_new;
    //     delete eff_EX_old;
    //     delete pur_EX_old;
    //     // delete eff_EX_mic;
    //     // delete pur_EX_mic;
    //     delete eff_pur_cuts_EX_leg;
    //     delete eff_pur_cuts_EX;

    //     MakeDir("Efficiency/Cuts/LL");

    //     TCanvas * eff_pur_cuts_LL = new TCanvas("eff_pur_cuts_LL","", 600, 800);
    //     eff_pur_cuts_LL->cd();

    //     // m_runep->Debug();
    //     TH1D * eff_LL_new = m_runep->EffVSCuts( signal_def_new, 1 );//->Draw("HIST");
    //     // m_runep->Debug();
    //     TH1D * pur_LL_new = m_runep->PurVSCuts( signal_def_new, 1 );//->Draw("HISTSAME");
    //     TH1D * eff_LL_old = m_runep->EffVSCuts( signal_def_old, 1 );//->Draw("HIST");
    //     TH1D * pur_LL_old = m_runep->PurVSCuts( signal_def_old, 1 );//->Draw("HISTSAME");
    //     // TH1D * eff_LL_mic = m_runep->EffVSCuts( (signal_def_new + " && truth_pi_LL_michel == 1"), 1 );//->Draw("HIST");
    //     // TH1D * pur_LL_mic = m_runep->PurVSCuts( (signal_def_new + " && truth_pi_LL_michel == 1"), 1 );//->Draw("HISTSAME");

    //     eff_LL_new->Draw("HIST");
    //     pur_LL_new->Draw("HISTSAME");
    //     eff_LL_old->SetLineStyle(7);
    //     pur_LL_old->SetLineStyle(7);
    //     eff_LL_old->Draw("HISTSAME");
    //     pur_LL_old->Draw("HISTSAME");
    //     // eff_LL_mic->SetLineStyle(4);
    //     // pur_LL_mic->SetLineStyle(4);
    //     // eff_LL_mic->Draw("HISTSAME");
    //     // pur_LL_mic->Draw("HISTSAME");

    //     TLegend * eff_pur_cuts_LL_leg = m_runbd->Legend(0.2,0.1);
    //     eff_pur_cuts_LL_leg->AddEntry(eff_LL_new, "Efficiency (New)", "l");
    //     eff_pur_cuts_LL_leg->AddEntry(pur_LL_new, "Purity (New)", "l");
    //     eff_pur_cuts_LL_leg->AddEntry(eff_LL_old, "Efficiency (Old)", "l");
    //     eff_pur_cuts_LL_leg->AddEntry(pur_LL_old, "Purity (Old)", "l");
    //     // eff_pur_cuts_LL_leg->AddEntry(eff_LL_mic, "Eff. (New w #pi ME tag)", "l");
    //     // eff_pur_cuts_LL_leg->AddEntry(pur_LL_mic, "Pur. (New w #pi ME tag)", "l");
    //     eff_pur_cuts_LL_leg->Draw();
    
    //     eff_pur_cuts_LL->Write();

    //     delete eff_LL_new;
    //     delete pur_LL_new; 
    //     delete eff_LL_old;
    //     delete pur_LL_old;
    //     // delete eff_LL_mic;
    //     // delete pur_LL_mic;
    //     delete eff_pur_cuts_LL_leg;
    //     delete eff_pur_cuts_LL;
    
    // //*****************************************************************************************//
    // //**************************************** VS cuts END ************************************//

    
    // //*****************************************************************************************//
    // //*************************************** VS eff. START ***********************************//
    
    //     for(int build = 0; build < 2; build++){

    //         string mom_type = "dEdX";
    //         if(build == 1) mom_type = "LL";

    //         string cut_dEdX = "truth_accum_level[" + string(build == 0 ? "0" : "1") + "] > 5";// && truth_pi_michel == 1";
        
    //         Int_t truemom_bins[3] = {40, 40, 40};
    //         Double_t truemom_low[3] = {0., 0., 1500.};
    //         Double_t truemom_hig[3] = {2000., 2000., 20000.};

    //         string true_sym[3] = {"p", "#pi^{+}", "#mu^{-}"};
    //         string true_nam[3] = {"pr", "pi", "mu"};

    //         Variable truemom;
    //         truemom.units = "MeV/#it{c}";
    //         for(int p = 0; p < 3; p++){
    //             truemom.name = "truth_" + true_nam[p] + "_mom";
    //             truemom.symbol = "#it{p}_{" + true_sym[p] + "}";

    //             // if(p == 2) m_runep->Debug();//ON

    //             MakeDir("Efficiency/" + mom_type + "/Mom");
    //             EffPart(truemom, truemom_bins[p], truemom_low[p], truemom_hig[p], signal_def_new, cut_dEdX);

    //             // if(p == 2) m_runep->Debug();//OFF

    //             if(build == 0){
    //                 MakeDir("Truth/Mom");
    //                 TruthPart(truemom, truemom_bins[p], truemom_low[p], truemom_hig[p], signal_def_new);
    //             }
    //         }
        
    //         Variable truetheta;//Is this range between 0 and pi?
    //         truetheta.units = "Rad.";
    //         for(int p = 0; p < 3; p++){
    //             truetheta.name = "truth_" + true_nam[p] + "_Theta";
    //             truetheta.symbol = "#theta_{" + true_sym[p] + "}";

    //             MakeDir("Efficiency/" + mom_type + "/Theta");
    //             EffPart(truetheta, 25, 0., TMath::Pi(), signal_def_new, cut_dEdX);

    //             if(build == 0){
    //                 MakeDir("Truth/Theta");
    //                 TruthPart(truetheta, 25, 0, TMath::Pi(), signal_def_new);
    //             }
    //         }
        
    //         Variable truephi;
    //         truephi.units = "Rad.";
    //         for(int p = 0; p < 3; p++){
    //             truephi.name = "truth_" + true_nam[p] + "_Phi";
    //             truephi.symbol = "#phi_{" + true_sym[p] + "}";

    //             MakeDir("Efficiency/" + mom_type + "/Phi");
    //             EffPart(truephi, 25, -TMath::Pi(), TMath::Pi(), signal_def_new, cut_dEdX);

    //             if(build == 0){
    //                 MakeDir("Truth/Phi");
    //                 TruthPart(truephi, 25, -TMath::Pi(), TMath::Pi(), signal_def_new);
    //             }
    //         }
    //     }
    
    //     Variable trueW;
    //     trueW.units = "MeV";
    //     trueW.name = "mc_w";
    //     trueW.symbol = "W";
    //     MakeDir("Truth/W");
    //     TruthPart(trueW, 40, 0, 3000, signal_def_new, 5);
    //     MakeDir("Truth/W/EX");
    //     TruthPart(trueW, 40, 0, 3000, signal_def_new, 5);
    //     MakeDir("Truth/W/LL");
    //     TruthPart(trueW, 40, 0, 3000, signal_def_new, 5, 1);
    
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
    
    // }
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
    return new TLatex(0.0, 0.1, ("#font[62]{" + m_experiment->GetLogo() + "}").c_str() );
}

TLatex * ProducePlots::GetSignal(){
    return new TLatex(0.0, 0.1, ("#font[62]{#it{" + m_experiment->GetSigDef() + "}}").c_str() );
}

int main(int argc, char *argv[])
{

    string filename = "";//test_min_mc;
    string savename = "";// = "CC1P1PiP_Plots_" + sday.str() + smon.str() + syear.str() + ".root";
    bool debug = false;
    Experiment::Name experiment = Experiment::UNKNOWN;
    bool realdata = false;

    char cc;
    while((cc = getopt(argc, argv, "i:o:d::t::m::r::")) != -1){
        switch (cc){
            case 'i': filename = std::string(optarg); break;
            case 'o': savename = optarg; break;
            case 'd': debug = true; break;
            case 't': experiment = Experiment::T2K; break;
            case 'm': experiment = Experiment::MIN; break;
            case 'r': realdata = true; break;

            default: return 1;
        }
    }

    if(experiment == Experiment::UNKNOWN){
        cout << "**** Experiment not defined ****" << endl;
        cout << "For     T2K : -t" << endl;
        cout << "For MINERvA : -m" << endl;
        return 0;
    }

    if(filename.empty()){
        if(experiment == Experiment::MIN) filename = test_min_mc;
        else filename = test_t2k_mc;
        cout << "Note: Running on test file." << endl;
        cout << "      To run on specific file use (-i filename)." << endl;
    }

    ProducePlots * plots = new ProducePlots(experiment, filename, debug, realdata);
    
    if(!savename.empty()) plots->SetSaveName(savename);
    
    plots->MakePlots();

//    delete plots; -- TODO: Fix this
    
    return 0;
}
