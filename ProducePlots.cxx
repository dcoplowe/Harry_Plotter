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

//dumping of array
#define dump_array3(par) std::cout << std::fixed    << \
  std::setw(10) << std::left   << #par   << ": \t"  << \
  std::setw(10) << std::right  << par[0] << " "     << \
  std::setw(10) << std::right  << par[1] << " "     << \
  std::setw(10) << std::right  << par[2] << std::endl;

#define dump_now(par) std::cout << std::fixed    << \
  std::setw(10) << std::left   << #par   << ": \t"  << std::endl;

//#include <TStyle.h>

using namespace std;

const string test_min_mc("/pnfs/minerva/persistent/users/dcoplowe/CC1P1Pi_PL13C_030317/grid/central_value/minerva/ana/v10r8p9/00/01/32/00/SIM_minerva_00013200_Subruns_0001-0002-0003-0004_CC1P1PiAnalysis_Ana_Tuple_v10r8p9-dcoplowe.root");
const string test_t2k_mc("/data/t2k/coplowe/numuCC1P1PiOutput/march17/neutP6BWA/FGD1/nall_data_FGD1_type2_wome_020517/AnaOut_sub10000.root");

class ProducePlots {

public:
    ProducePlots(Experiment::Name exp, std::string infilename, std::string outfilename, bool debug, bool realdata);
    ~ProducePlots();
    
    // void SetSaveName(std::string var){ m_savename = var; }
    
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
    
    void PositionPlot(Variable var, Int_t nbins, Double_t * bins, std::string cuts, int fgd = 1);
    void PositionPlot(Variable var, Int_t nbins, Double_t low, Double_t high, std::string cuts, int fgd = 1);

    void EffPart(Variable var, Int_t nbins, Double_t low, Double_t high, std::string signal_def, std::string cuts = "");
    void PurPart(Variable var, std::string cuts);

    void TruthPart(Variable var, Int_t nbins, Double_t low, Double_t high, std::string cuts = "", int ac_lev = -1, int branch = 0);
    
    void MakeDir(std::string name);
    void cdDir(std::string name = ""){  m_outfile->cd((m_savename + ":/" + name).c_str()); }//Default is the root dir.
    
    void Verbose(bool var){ m_verbose = var; }

    void Make2DPlots(Variable x_var, Variable y_var, std::string savename, std::string cuts);

    void SetBranchToPlot(int accum_level = -999, int branch = -999);

private:
    std::string m_infilename;
    bool m_realdata;

    int m_accum_level;
    int m_branch;

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

    std::string m_NameXYZ[3];

    void DrawDetectorLines(TCanvas * can, int xyz);

    bool m_verbose;
};

ProducePlots::ProducePlots(Experiment::Name exp, std::string infilename, std::string outfilename, bool debug, bool realdata) : m_infilename(infilename),
m_realdata(realdata), m_accum_level(-999), m_branch(-999), m_savename(outfilename) {

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
    
    if(m_savename.empty()) m_savename = Experiment::ToString(exp) + "_CC1P1PiPlus_Plots_" + GetDate() + ".root";
    // else{
    //     if(m_savename.find(".root") == std::string::npos){
    //         //If no file extension add one:
    //     } 
    //     else if(m_savename.find( GetDate().c_str() ) == std::string::npos){
    //         //Append 
    //     }

    // }

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

    m_verbose = false;

    m_NameXYZ[0] = "X";
    m_NameXYZ[1] = "Y";
    m_NameXYZ[2] = "Z";
}

ProducePlots::~ProducePlots(){

    if(m_outfile){
        if(m_outfile->IsOpen()) m_outfile->Close();
        delete m_outfile;
    }
    
    delete m_recovars;
    delete m_runbd;
}

void ProducePlots::SetBranchToPlot(int accum_level, int branch)
{ 
    m_accum_level = accum_level; 
    m_branch = branch; 

    // cout << "ProducePlots::SetBranchToPlot: m_accum_level = " << m_accum_level << " m_branch = " << m_branch << endl;

    if(m_branch != -999 && m_experiment->GetType() == Experiment::T2K){
        //For now assume type 1 run:
        if(m_branch == 1){
            std::vector<std::string> cut_list;
            cut_list.push_back("Event Quality");
            cut_list.push_back("> 0 Tracks");
            cut_list.push_back("Quality and Fiducial");
            cut_list.push_back("Veto");
            cut_list.push_back("3 TPC Tracks (+2, -1)");
            cut_list.push_back("Common Vertex");
            m_runep->SetNCuts(6);
            m_runep->SetCutNames(cut_list);
        }
    }
    // if(m_accum_level != -999 && m_experiment->GetType() == Experiment::T2K){
    // }
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

void ProducePlots::PositionPlot(Variable var, Int_t nbins, Double_t low, Double_t high, std::string cuts, int fgd){
    PositionPlot(var, nbins, DrawingTools::SetBinning(nbins, low, high), cuts, fgd);
}

void ProducePlots::PositionPlot(Variable var, Int_t nbins, Double_t * bins, std::string cuts, int fgd){//, Int_t setsave = 11111??

    if(m_outfile->IsOpen()){
        // Check which dimension we are working in:
        int xyz = 0;
        for(int i = 0; i < 3; i++){
            if(var.GetName().find(Form("[%d]", i)) != std::string::npos){
                xyz = i;
                break;
            }
        }

        BDCans var_top = m_runbd->TOPO(var, nbins, bins, cuts);
        DrawDetectorLines(var_top.recon, xyz);
        DrawDetectorLines(var_top.truth, xyz);

        BDCans var_tar = m_runbd->TARGET(var, nbins, bins, cuts);
        DrawDetectorLines(var_tar.recon, xyz);
        DrawDetectorLines(var_tar.truth, xyz);
    
        BDCans var_pid;
        if(!var.GetPDG().empty()){
            var_pid = m_runbd->PID(var, nbins, bins, var.GetPDG(), cuts);
            DrawDetectorLines(var_pid.recon, xyz);
            DrawDetectorLines(var_pid.truth, xyz);
        }
        
        // cout << "ProducePlots::PositionPlot : Now saving" << endl;

        //Recon Vars:
        PrintLogo(var_top.recon);
        PrintLogo(var_tar.recon);
        if(!var.GetPDG().empty()) PrintLogo(var_pid.recon);
        
        var_top.recon->Write();
        var_tar.recon->Write();
        if(!var.GetPDG().empty()) var_pid.recon->Write();

        // cout << "ProducePlots::PositionPlot : Done 1" << endl;

        
        //Truth Vars:
        PrintLogo(var_top.truth);
        PrintLogo(var_tar.truth);
        if(!var.GetPDG().empty()) PrintLogo(var_pid.truth);

        // cout << "ProducePlots::PositionPlot : Done 2" << endl;

        
        var_top.truth->Write();
        var_tar.truth->Write();
        if(!var.GetPDG().empty()) var_pid.truth->Write();

        // cout << "ProducePlots::PositionPlot : Done 3" << endl;
        
        //Ratio Vars:
        PrintLogo(var_top.ratio);
        PrintLogo(var_tar.ratio);
        if(!var.GetPDG().empty()) PrintLogo(var_pid.ratio);
        
                // cout << "ProducePlots::PositionPlot : Done 4" << endl;


        var_top.ratio->Write();
        var_tar.ratio->Write();
        if(!var.GetPDG().empty()) var_pid.ratio->Write();
        
        // cout << "ProducePlots::PositionPlot : Done 5" << endl;


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
        
                // cout << "ProducePlots::PositionPlot : Done 6" << endl;


        var_top.smearSN->Write();
//        var_tar.smearSN->Write();
//        if(!var.GetPDG().empty()) var_pid.smearSN->Write();

        // cout << "ProducePlots::PositionPlot : Done 7" << endl;

    }
    else std::cout << "ProducePlots::ProduceGroup : ERROR : File is not open..." << std::endl;
    
}

void ProducePlots::DrawDetectorLines(TCanvas * can, int xyz){

    THStack * stack = (THStack*)m_runbd->GetObjectFromCanvas(can, "THStack");
    if(stack){
        // cout << "Stack exists" << endl;
        can->cd();

        m_runbd->DrawLine(stack, t2kgeometry::fgd1min[xyz]);                
        m_runbd->DrawLine(stack, t2kgeometry::fgd1max[xyz]);
        m_runbd->DrawLine(stack, t2kgeometry::tpc2min[xyz]);
        m_runbd->DrawLine(stack, t2kgeometry::tpc2max[xyz]);

        //        //Draw a name of detector in between lines:
        // double x_range = m_runbd->GetFirstHistFromStack(stack)->GetXaxis()->GetXmax() - m_runbd->GetFirstHistFromStack(stack)->GetXaxis()->GetXmin();
        // double x_scale = TMath::Abs(m_runbd->GetFirstHistFromStack(stack)->GetXaxis()->GetXmin())/x_range;
        // // cout << "x_range = " << x_range << " x_scale = " << x_scale << endl;

        // double x_width = (t2kgeometry::fgd1max[xyz] -  t2kgeometry::fgd1min[xyz])/2;
        // double x_center = t2kgeometry::fgd1min[xyz] + x_width + x_scale;

        // x_width *= 1/x_range;
        // x_center *= 1/x_range;
        // // cout << "x_width = " << x_width << " x_center = " << x_center << endl;

        // double y_range = m_runbd->GetFirstHistFromStack(stack)->GetYaxis()->GetXmax() - m_runbd->GetFirstHistFromStack(stack)->GetYaxis()->GetXmin();
        // // cout << "y_range = " << y_range << endl;

        // double y_width = m_runbd->GetFirstHistFromStack(stack)->GetYaxis()->GetXmax();
        // y_width -= m_runbd->GetFirstHistFromStack(stack)->GetYaxis()->GetXmin();
        // y_width *= 1/2.;

        // double y_center = m_runbd->GetFirstHistFromStack(stack)->GetYaxis()->GetXmin() + y_width;

        // y_width *= 1/y_range;
        // y_center *= 1/y_range;
        // // cout << "y_width = " << y_width << " y_center = " << y_center << endl;

        // TLegend * leg = new TLegend(x_center - 0.2*x_width, y_center - 0.2*y_width, x_center + 0.2*x_width, y_center + 0.1*y_width);
         TLegend * leg = new TLegend(0.5, 0.5, 0.6, 0.6);
        leg->AddEntry((TObject*)0, "Sub Det.","");
        leg->SetFillStyle(0);
        leg->SetTextColor(kGray + 2);
        leg->Draw(); 
    }
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

void ProducePlots::Make2DPlots(Variable x_var, Variable y_var, std::string savename, std::string cuts)
{
    
    TH2D * var_h = m_runbd->GetHisto(y_var.GetName() + ":" + x_var.GetName(), x_var.GetNBins(), x_var.GetBinning(), y_var.GetNBins(), y_var.GetBinning(),
        x_var.GetAxisTitle() + ";" + y_var.GetAxisTitle(), cuts);

    TCanvas * var_c = new TCanvas(savename.c_str() , "", 400, 400);
    var_c->cd();
    var_h->Draw("COLZ");
    m_runbd->GetPOT(0.1,0.1)->Draw();
    PrintLogo(var_c);

    var_c->Write();
    delete var_h;
    delete var_c;

    TH2D * pur_h = m_runep->PurVSVar(y_var.GetName() + ":" + x_var.GetName(), x_var.GetNBins(), x_var.GetBinning(), y_var.GetNBins(), y_var.GetBinning(),
        m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlus).GetSignal(), cuts, x_var.GetAxisTitle() + ";" + y_var.GetAxisTitle());

    TCanvas * pur_c = new TCanvas( (savename + "_pur").c_str() , "", 400, 400);
    pur_c->cd();
    pur_h->Draw("COLZ");
    m_runbd->GetPOT(0.1,0.1)->Draw();
    PrintLogo(pur_c);
    GetSignal()->Draw();

    pur_c->Write();
    delete pur_h;
    delete pur_c;
}

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

void ProducePlots::PurPart(Variable var, std::string cuts){
    //Produce purity. and reco dists. from truth tree.
    if(m_outfile->IsOpen()){
        
        TCanvas * purdists = new TCanvas( (var.GetName() + "_pur").c_str(), "", 900,800);
        purdists->cd();
        TH1D * H_pur = m_runep->PurVSVar(var.GetName().c_str(), var.GetNBins(), var.GetBinning(), m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlus).GetSignal(), cuts, var.GetSymbol() + (var.GetUnits().empty() ? "" : " (" + var.GetUnits() + ")"));
        H_pur->SetLineColor(DrawingStyle::HYDROGEN);
        H_pur->Draw();

        TH1D * CH_pur = m_runep->PurVSVar(var.GetName().c_str(), var.GetNBins(), var.GetBinning(), m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlus).GetSignal(), cuts, var.GetSymbol() + (var.GetUnits().empty() ? "" : " (" + var.GetUnits() + ")"));
        CH_pur->SetLineColor(DrawingStyle::CARBON);
        CH_pur->Draw();

        TLegend * pur_leg = m_runbd->Legend(0.15, 0.1);
        pur_leg->AddEntry(H_pur, m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlus).GetSymbol().c_str(), "l" );
        pur_leg->AddEntry(CH_pur, m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlus).GetSymbol().c_str(), "l" );
        pur_leg->Draw();
        
        PrintLogo(purdists);
        m_runbd->GetPOT(0.1, 0.1)->Draw();

        purdists->Write();

        delete H_pur;
        delete CH_pur;
        delete pur_leg;
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
        if(m_accum_level != -999 &&  m_branch != -999) basecuts[0] = m_experiment->GetBaseCuts(m_accum_level, m_branch);//Change back to 5
        else if(m_accum_level != -999) basecuts[0] = m_experiment->GetBaseCuts(m_accum_level, 0);//Change back to 5
        else if(m_branch != -999) basecuts[0] = m_experiment->GetBaseCuts(5, m_branch);
        else basecuts[0] = m_experiment->GetBaseCuts(5, 0);
    }

    if(m_verbose) cout << "Running on Particles:" << endl;

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

            if(m_verbose) cout << party->GetName() << " plots being produced" << endl;
            
            //**************************************** Mom START ****************************************//
            MakeDir("Mom" + branchnames[br] + "/" + party->GetName() );
            MakeMomPlots(party, party->P.GetNBins(), party->P.GetBinning(), basecuts[br]);
            //**************************************** Mom END ****************************************//

            if(m_experiment->GetType() == Experiment::MIN){

                MakeDir("PTheta" + branchnames[br] + "/" + party->GetName() );
                Make2DPlots(party->P, party->theta, "ptheta", basecuts[br]);
                //**************************************** Theta START ************************************//
                MakeDir("Theta" + branchnames[br] + "/" + party->GetName() );
                MakeThetaPlots(party, party->theta.GetNBins(), party->theta.GetBinning(), basecuts[br]);
                //**************************************** Theta END **************************************//

                //**************************************** Phi START ************************************//
                MakeDir("Phi" + branchnames[br] + "/" + party->GetName() );
                MakePhiPlots(party, party->phi.GetNBins(), party->phi.GetBinning(), basecuts[br]);
                //**************************************** Phi END **************************************//                
                if(m_verbose) cout << "Making MINERva specific plots" << endl;
            }

            if(m_experiment->GetType() == Experiment::T2K){

                if(m_verbose) cout << "Making T2K specific plots" << endl;

                MakeDir("PcTheta" + branchnames[br] + "/" + party->GetName() );
                Make2DPlots(party->P, party->ctheta_nudir, "pcTheta", basecuts[br]);

                if(m_verbose) cout << "Cos Theta";

                //**************************************** cosTheta START ************************************//
                MakeDir("cTheta/" + party->GetName() );
                MakeCosThetaPlots(party, party->ctheta.GetNBins(), party->ctheta.GetBinning(), basecuts[br]);
                MakeCosThetaPlots(party, party->ctheta_nudir.GetNBins(), party->ctheta_nudir.GetBinning(), basecuts[br], true);

                if(m_verbose) cout << " -- Done" << endl;
                //**************************************** cosTheta END **************************************//

                //**************************************** Crossing Angle START ************************************//
                if(m_verbose) cout << "Crossing Angle";

                MakeDir("CrossingAngle");
                party->cross_angle.SetSName(party->cross_angle.GetName());

                // ProduceGroup(party->cross_angle, party->cross_angle.GetNBins(), party->cross_angle.GetBinning(), basecuts[br]);
                TCanvas * ca_can = m_runbd->TARGETSingle(party->cross_angle, party->cross_angle.GetNBins(), party->cross_angle.GetBinning(), basecuts[br]);
                ca_can->cd();
                m_runbd->GetHisto(party->cross_angle.GetName(), party->cross_angle.GetNBins(), party->cross_angle.GetBinning(), "", basecuts[br])->Draw();//TODO: This is actually for checking that the plotter is working correctly.
                PrintLogo(ca_can);
                ca_can->Write();

                // Purity of the crossing angle:
                PurPart(party->cross_angle, basecuts[br]); 

                if(m_verbose) cout << " -- Done" << endl;

                //**************************************** Crossing Angle END **************************************//

                //**************************************** FGD Segment START ************************************//
                // 1) Particle type FGD segment
                // 2) Tot. No. of segments (1,2,3)

                if(m_verbose) cout << "FGD Segments";

                MakeDir("FGDSegments");

                Variable startfgd = party->fgd_start;
                TH1D * startfgd_h = m_runbd->GetHisto(startfgd.GetName(), startfgd.GetNBins(), startfgd.GetBinning(), startfgd.GetSymbol(), basecuts[br]);
                startfgd_h->SetFillColor( party->info.GetColor() );

                TCanvas * startfgd_c = new TCanvas(party->fgd_start.GetName().c_str(), "", 400, 400);
                startfgd_h->Draw();
                PrintLogo(startfgd_c);
                startfgd_c->Write();

                if(m_verbose) cout << " -- Done" << endl;
                //**************************************** FGD Segment END **************************************//

                //**************************************** Start Position START ************************************//

                if(m_verbose) cout << "Start Position";
                MakeDir("StartPosition/" + party->GetName());
                int dimnbins[3] = {40, 40, 40};

                for(int dim = 0; dim < 3; dim++){
                //**************************************** 1D START ************************************//
                    stringstream dimss; 
                    dimss << dim;

                    int dim1cut, dim2cut;
                    string dim1cuts, dim2cuts;

                    if(dim == 0){
                        dim1cuts = "1";
                        dim2cuts = "2";
                        dim1cut = 1;
                        dim2cut = 2;
                    }
                    else if(dim == 1){
                        dim1cuts = "0";
                        dim2cuts = "2";
                        dim1cut = 0;
                        dim2cut = 2; 
                    }
                    else{
                        dim1cuts = "0";
                        dim2cuts = "1";
                        dim1cut = 0;
                        dim2cut = 1; 
                    }

                    stringstream dim1th_low, dim2th_low, dim1th_high, dim2th_high;                    
                    dim1th_low << t2kgeometry::fgd1tpcmin_offset[dim1cut];
                    dim1th_high << t2kgeometry::fgd1tpcmax_offset[dim1cut];

                    dim2th_low << t2kgeometry::fgd1tpcmin_offset[dim2cut];
                    dim2th_high << t2kgeometry::fgd1tpcmax_offset[dim2cut];

                    string start_pos_cuts = basecuts[br];
                    //1st Dim : True vars:
                    start_pos_cuts += " && ";
                    //1st Dim : True vars min:
                    start_pos_cuts += dim1th_low.str();
                    start_pos_cuts += " < ";
                    start_pos_cuts += party->truestartpos.GetName();
                    start_pos_cuts += "[";
                    start_pos_cuts += dim1cuts;
                    start_pos_cuts += "]";
                    start_pos_cuts += " && ";
                    //1st Dim : True vars min:
                    start_pos_cuts += party->truestartpos.GetName();
                    start_pos_cuts += "[";
                    start_pos_cuts += dim1cuts;
                    start_pos_cuts += "]";
                    start_pos_cuts += " < ";
                    start_pos_cuts += dim1th_high.str();

                    //1st Dim : Reco vars:
                    start_pos_cuts += " && ";
                    //1st Dim : Reco vars min:
                    start_pos_cuts += dim1th_low.str();
                    start_pos_cuts += " < ";
                    start_pos_cuts += party->startpos.GetName();
                    start_pos_cuts += "[";
                    start_pos_cuts += dim1cuts;
                    start_pos_cuts += "]";
                    start_pos_cuts += " && ";
                    //1st Dim : Reco vars min:
                    start_pos_cuts += party->startpos.GetName();
                    start_pos_cuts += "[";
                    start_pos_cuts += dim1cuts;
                    start_pos_cuts += "]";
                    start_pos_cuts += " < ";
                    start_pos_cuts += dim1th_high.str();

                    //2nd Dim : True vars:
                    start_pos_cuts += " && ";
                    //2nd Dim : True vars min:
                    start_pos_cuts += dim2th_low.str();
                    start_pos_cuts += " < ";
                    start_pos_cuts += party->truestartpos.GetName();
                    start_pos_cuts += "[";
                    start_pos_cuts += dim2cuts;
                    start_pos_cuts += "]";
                    start_pos_cuts += " && ";
                    //2nd Dim : True vars min:
                    start_pos_cuts += party->truestartpos.GetName();
                    start_pos_cuts += "[";
                    start_pos_cuts += dim2cuts;
                    start_pos_cuts += "]";
                    start_pos_cuts += " < ";
                    start_pos_cuts += dim2th_high.str();

                    //2nd Dim : Reco vars:
                    start_pos_cuts += " && ";
                    //2nd Dim : Reco vars min:
                    start_pos_cuts += dim2th_low.str();
                    start_pos_cuts += " < ";
                    start_pos_cuts += party->startpos.GetName();
                    start_pos_cuts += "[";
                    start_pos_cuts += dim2cuts;
                    start_pos_cuts += "]";
                    start_pos_cuts += " && ";
                    //2nd Dim : Reco vars min:
                    start_pos_cuts += party->startpos.GetName();
                    start_pos_cuts += "[";
                    start_pos_cuts += dim2cuts;
                    start_pos_cuts += "]";
                    start_pos_cuts += " < ";
                    start_pos_cuts += dim2th_high.str();

                    // cout << start_pos_cuts << endl;

                    Variable start_pos(party->truestartpos.GetName() + "[" + dimss.str() + "]:" + party->startpos.GetName()  + "[" + dimss.str() + "]", party->GetSymbol() + " " + m_NameXYZ[dim] + " Start Position", "mm");
                    // This probably wont work as the code looks for :: to make a split... Add fix.
                    start_pos.SetSName(party->startpos.GetName() + m_NameXYZ[dim] );
                    start_pos.SetPDG(party->pdg.GetName());
                    // cout << "fgd1tpc_offset " << m_NameXYZ[dim] << " min  = " << t2kgeometry::fgd1tpcmin_offset[dim] << " max  = " << t2kgeometry::fgd1tpcmax_offset[dim] << endl; 
                    PositionPlot(start_pos, dimnbins[dim], t2kgeometry::fgd1tpcmin_offset[dim], t2kgeometry::fgd1tpcmax_offset[dim], start_pos_cuts);

                    //**************************************** 1D END ************************************//
                    for(int fgdseg = 0; fgdseg < 2; fgdseg++){
                        stringstream fgdsegs;
                        fgdsegs << fgdseg;
                        string start_pos_fgdseg_cuts = start_pos_cuts + " && " + party->fgd_start.GetName() + " == " + fgdsegs.str();
                        Variable start_pos_fgdseg(start_pos);
                        start_pos_fgdseg.SetSName( start_pos_fgdseg.GetSName() + "_" + (fgdseg == 0 ? "nofgdseg" : "fgdseg") );
                        PositionPlot(start_pos_fgdseg, dimnbins[dim], t2kgeometry::fgd1tpcmin_offset[dim], t2kgeometry::fgd1tpcmax_offset[dim], start_pos_fgdseg_cuts);
                    }
                    //**************************************** 1D END ************************************//
    
                //**************************************** 1D END ************************************//
                
                //**************************************** 1D Purity START ************************************//

                    if(m_verbose) cout << " plots made, now producing purity dists." << endl;

                    // Variable start_pos_dim(party->startpos.GetName()  + "[" + dimss.str() + "]", dimnbins[dim], t2kgeometry::fgd1tpcmin_offset[dim], t2kgeometry::fgd1tpcmax_offset[dim], start_pos.GetSymbol(), "mm", start_pos.GetSName() + "_pur", party->pdg.GetName());
                    // PurPart: 

                    //Old method: Doesn't seem to give the correct purity....
                    TH1D * start_pos_H_pur = m_runep->PurVSVar(party->startpos.GetName()  + "[" + dimss.str() + "]", dimnbins[dim], t2kgeometry::fgd1tpcmin_offset[dim], t2kgeometry::fgd1tpcmax_offset[dim],
                        m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlus).GetSignal(), start_pos_cuts,
                        start_pos.GetSymbol() + " (" + start_pos.GetUnits() + ")");

                    TH1D * start_pos_CH_pur = m_runep->PurVSVar(party->startpos.GetName()  + "[" + dimss.str() + "]", dimnbins[dim], t2kgeometry::fgd1tpcmin_offset[dim], t2kgeometry::fgd1tpcmax_offset[dim],
                        m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlus).GetSignal(), start_pos_cuts,
                        start_pos.GetSymbol() + " (" + start_pos.GetUnits() + ")");

                    TCanvas * start_pos_pur_c = new TCanvas((start_pos.GetSName() + "_pur").c_str(), "", 400, 400);
                    start_pos_pur_c->cd();

                    start_pos_H_pur->SetLineColor(DrawingStyle::Blue);
                    start_pos_H_pur->Draw();

                    start_pos_CH_pur->SetLineColor(DrawingStyle::Yellow);
                    start_pos_CH_pur->Draw("SAME");

                    TLegend * start_pos_pur_leg = m_runbd->Legend(0.15, 0.1);
                    start_pos_pur_leg->AddEntry(start_pos_H_pur, m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlus).GetSymbol().c_str(), "l" );
                    start_pos_pur_leg->AddEntry(start_pos_CH_pur, m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlus).GetSymbol().c_str(), "l" );
                    start_pos_pur_leg->Draw();

                    PrintLogo(start_pos_pur_c);

                    start_pos_pur_c->Write();

                    delete start_pos_H_pur;
                    delete start_pos_CH_pur;
                    delete start_pos_pur_leg;
                    delete start_pos_pur_c;

                //**************************************** 1D Purity END ************************************//
                
                    for(int dim2 = dim + 1; dim2 < 3; dim2++){
                        stringstream dim2ss;
                        dim2ss << dim2;
                        //**************************************** 2D START ************************************//

                        Variable start_pos2D(party->startpos.GetName() + "[" + dim2ss.str() + "]:" + party->startpos.GetName()  + "[" + dimss.str()+ "]",
                            party->GetSymbol() + " " + m_NameXYZ[dim] + m_NameXYZ[dim2] + " Start Position", "mm");
                                    // This probably wont work as the code looks for :: to make a split... Add fix.
                        start_pos2D.SetSName(party->startpos.GetName() + m_NameXYZ[dim] + m_NameXYZ[dim2] );
                        start_pos2D.SetPDG(party->pdg.GetName());

                        TH2D * start_pos2D_h = m_runbd->GetHisto(start_pos2D.GetName(), dimnbins[dim], t2kgeometry::fgd1tpcmin_offset[dim], 
                            t2kgeometry::fgd1tpcmax_offset[dim], dimnbins[dim2], t2kgeometry::fgd1tpcmin_offset[dim2], 
                            t2kgeometry::fgd1tpcmax_offset[dim2], "Start Position " + m_NameXYZ[dim] + " (mm);Start Position " + m_NameXYZ[dim2] + " (mm)", start_pos_cuts);

                        TCanvas * start_pos2D_c = new TCanvas( (start_pos2D.GetSName() ).c_str(), "", 400, 400);
                        start_pos2D_c->cd();
                        start_pos2D_h->Draw("COLZ");
                        m_runbd->GetPOT(0.1,0.1)->Draw();

                        double fgd_box_low[2] = { t2kgeometry::fgd1min[dim], t2kgeometry::fgd1min[dim2] };
                        double fgd_box_hig[2] = { t2kgeometry::fgd1max[dim], t2kgeometry::fgd1max[dim2] };

                        // double tpc1_box_low[2] = { t2kgeometry::tpc1min[dim], t2kgeometry::tpc1min[dim2] };
                        // double tpc1_box_hig[2] = { t2kgeometry::tpc1max[dim], t2kgeometry::tpc1max[dim2] };

                        double tpc2_box_low[2] = { t2kgeometry::tpc2min[dim], t2kgeometry::tpc2min[dim2] };
                        double tpc2_box_hig[2] = { t2kgeometry::tpc2max[dim], t2kgeometry::tpc2max[dim2] };

                        m_runbd->DrawBox(fgd_box_low, fgd_box_hig);
                        m_runbd->DrawBox(tpc2_box_low, tpc2_box_hig, DrawingStyle::Yellow);

                        m_runbd->GetPOT(0.1,0.1)->Draw();
                        PrintLogo(start_pos2D_c);

                        start_pos2D_c->Write();

                        delete start_pos2D_h;
                        delete start_pos2D_c;

                        //**************************************** NFGD Segs Start ************************************//

                        for(int fgdseg = 0; fgdseg < 2; fgdseg++){
                            stringstream fgdsegs;
                            fgdsegs << fgdseg;
                            string start_pos_fgdseg_cuts = start_pos_cuts + " && " + party->fgd_start.GetName() + " == " + fgdsegs.str();
                            // Variable start_pos_fgdseg(start_pos);
                            // start_pos_fgdseg.SetSName( start_pos_fgdseg.GetSName() + "_" + (fgdseg == 0 ? "nofgdseg" : "fgdseg") );
                            
                            TH2D * start_pos2D_fgdseg_h = m_runbd->GetHisto(start_pos2D.GetName(), dimnbins[dim], t2kgeometry::fgd1tpcmin_offset[dim], 
                                t2kgeometry::fgd1tpcmax_offset[dim], dimnbins[dim2], t2kgeometry::fgd1tpcmin_offset[dim2], 
                                t2kgeometry::fgd1tpcmax_offset[dim2], "Start Position " + m_NameXYZ[dim] + " (mm);Start Position " + m_NameXYZ[dim2] + " (mm)", start_pos_fgdseg_cuts);

                            TCanvas * start_pos2D_fgdseg_c = new TCanvas( (start_pos2D.GetSName() + "_" + (fgdseg == 0 ? "nofgdseg" : "fgdseg") ).c_str(), "", 400, 400);
                            start_pos2D_fgdseg_c->cd();
                            start_pos2D_fgdseg_h->Draw("COLZ");
                            m_runbd->GetPOT(0.1,0.1)->Draw();

                            double fgd_box_low[2] = { t2kgeometry::fgd1min[dim], t2kgeometry::fgd1min[dim2] };
                            double fgd_box_hig[2] = { t2kgeometry::fgd1max[dim], t2kgeometry::fgd1max[dim2] };

                        // double tpc1_box_low[2] = { t2kgeometry::tpc1min[dim], t2kgeometry::tpc1min[dim2] };
                        // double tpc1_box_hig[2] = { t2kgeometry::tpc1max[dim], t2kgeometry::tpc1max[dim2] };

                            double tpc2_box_low[2] = { t2kgeometry::tpc2min[dim], t2kgeometry::tpc2min[dim2] };
                            double tpc2_box_hig[2] = { t2kgeometry::tpc2max[dim], t2kgeometry::tpc2max[dim2] };

                            m_runbd->DrawBox(fgd_box_low, fgd_box_hig);
                            m_runbd->DrawBox(tpc2_box_low, tpc2_box_hig, DrawingStyle::Yellow);

                            m_runbd->GetPOT(0.1,0.1)->Draw();
                            PrintLogo(start_pos2D_fgdseg_c);

                            start_pos2D_fgdseg_c->Write();

                            delete start_pos2D_fgdseg_h;
                            delete start_pos2D_fgdseg_c;

                        }
                        //**************************************** NFGD Segs End ************************************//

                        //**************************************** 2D END ************************************//
                
                        //**************************************** 2D Purity ************************************//

                        TH2D * start_pos2D_pur_h = m_runep->PurVSVar(start_pos2D.GetName(), dimnbins[dim], t2kgeometry::fgd1tpcmin_offset[dim], 
                            t2kgeometry::fgd1tpcmax_offset[dim], dimnbins[dim2], t2kgeometry::fgd1tpcmin_offset[dim2], t2kgeometry::fgd1tpcmax_offset[dim2],
                            m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlus).GetSignal(),
                            start_pos_cuts, "Start Position " + m_NameXYZ[dim] + " (mm);Start Position " + m_NameXYZ[dim2] +" (mm)");

                        TCanvas * start_pos2D_pur_c = new TCanvas( (start_pos2D.GetSName() + "_pur").c_str(), "", 400, 400);
                        start_pos2D_pur_c->cd();
                        start_pos2D_pur_h->Draw("COLZ");
                        m_runbd->GetPOT(0.1,0.1)->Draw();
                        GetSignal()->Draw();

                        m_runbd->DrawBox(fgd_box_low, fgd_box_hig);
                        m_runbd->DrawBox(tpc2_box_low, tpc2_box_hig, DrawingStyle::Yellow);

                        m_runbd->GetPOT(0.1,0.1)->Draw();

                        PrintLogo(start_pos2D_pur_c);

                        start_pos2D_pur_c->Write();

                        delete start_pos2D_pur_h;
                        delete start_pos2D_pur_c;
                
                        //**************************************** Start Position END ************************************//
                    }

                }

                MakeDir("LLScores/" + party->GetName());

                Variable pid_scores[4] = { party->PIDScoreMu, party->PIDScoreEl, party->PIDScorePr, party->PIDScorePi };
                std::vector<PDGInfo> pdg_list;
                pdg_list.push_back( PDGInfo(13,     "muon",     "#mu^{#pm}",    true) );
                pdg_list.push_back( PDGInfo(2212,   "proton",   "p",            true) );
                pdg_list.push_back( PDGInfo(211,    "pion",     "#pi^{#pm}",    true) );
                pdg_list.push_back( PDGInfo(11,     "electron", "e^{#pm}",      true) );

                for(int npid = 0; npid < 4; npid++){
                    Variable pid = pid_scores[npid];
                    pis.SetSName(pid.GetName());
                    m_runbd->GetPID(pid, part->pdg.GetName(), basecuts[br], pdg_list)->Write();//Variable var, std::string pdgvar, std::string cuts, std::vector<PDGInfo> pdglist
                }
            }

        }

        if(m_verbose) cout << "Now producing dpTT plots." << endl;
        //************************************** dpTT Start *************************************//
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
            if(m_verbose) cout << "Making MINERva specific plots" << endl;

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
        //************************************** dpTT End *************************************//

        if(m_experiment->GetType() == Experiment::T2K){

            cdDir("dpTT" + branchnames[br]);   
            for(int fgdseg = 1; fgdseg < 4; fgdseg++){
                stringstream fgdsegs;
                fgdsegs << fgdseg;
                string start_pos_fgdseg_cuts = basecuts[br] + " && " + m_muon->fgd_start.GetName();
                start_pos_fgdseg_cuts += " + ";
                start_pos_fgdseg_cuts += m_proton->fgd_start.GetName();
                start_pos_fgdseg_cuts += " + ";
                start_pos_fgdseg_cuts += m_pion->fgd_start.GetName();
                start_pos_fgdseg_cuts += " == ";
                start_pos_fgdseg_cuts += fgdsegs.str();
                start_pos_fgdseg_cuts += " && ";
                start_pos_fgdseg_cuts += m_muon->fgd_start.GetName();
                start_pos_fgdseg_cuts += " != -999";
                start_pos_fgdseg_cuts += " && ";
                start_pos_fgdseg_cuts += m_proton->fgd_start.GetName();
                start_pos_fgdseg_cuts += " != -999";
                start_pos_fgdseg_cuts += " && ";
                start_pos_fgdseg_cuts += m_pion->fgd_start.GetName();
                start_pos_fgdseg_cuts += " != -999";

                dpTT.SetSName(m_recovars->dpTT + "_" + fgdsegs.str() + "FGDSeg");
                ProduceGroup(dpTT, 29, -300, 300, start_pos_fgdseg_cuts);
            }

            if(m_verbose) cout << "Making T2K specific plots" << endl;
            //************************************** No. FGD Segments Start *************************************//
            cdDir("FGDSegments");

            Variable nfgdsegments(m_muon->fgd_start.GetName() + " + " + m_proton->fgd_start.GetName() + " + " + m_pion->fgd_start.GetName(),"","");
            string segcuts = m_muon->fgd_start.GetName() + "!= -999 && " + m_proton->fgd_start.GetName() + " != -999 && ";
            segcuts += m_pion->fgd_start.GetName();
            segcuts += " != -999";

            TH1D * nfgdsegments_h = m_runbd->GetHisto(nfgdsegments.GetName(), 3, 1., 4., "N tracks with FGD Segments", basecuts[br] + " && " + segcuts);
            for(int nloop = 0; nloop < nfgdsegments_h->GetNbinsX(); nloop++) nfgdsegments_h->GetXaxis()->SetBinLabel(nloop+1, Form("%d", nloop+1) );
            nfgdsegments_h->SetFillColor(DrawingStyle::Yellow);

            TCanvas * nfgdsegments_c = new TCanvas("NFGDSegments", "", 400, 400);
            nfgdsegments_h->Draw();
            PrintLogo(nfgdsegments_c);
            nfgdsegments_c->Write();

        //************************************** No. FGD Segments End *************************************//


            //**************************************** Vertex Position START ************************************//
            MakeDir("VtxPosition");
            int dimnbins[3] = {40, 40, 40};
                // if(which_fgd){
                // }
            for(int dim = 0; dim < 3; dim++){
                stringstream dimss; 
                dimss << dim;

                int dim1cut, dim2cut;
                string dim1cuts, dim2cuts;

                if(dim == 0){
                    dim1cuts = "1";
                    dim2cuts = "2";
                    dim1cut = 1;
                    dim2cut = 2;
                }
                else if(dim == 1){
                    dim1cuts = "0";
                    dim2cuts = "2";
                    dim1cut = 0;
                    dim2cut = 2; 
                }
                else{
                    dim1cuts = "0";
                    dim2cuts = "1";
                    dim1cut = 0;
                    dim2cut = 1; 
                }

                stringstream dim1th_low, dim2th_low, dim1th_high, dim2th_high;                    
                dim1th_low << t2kgeometry::fgd1min_offset[dim1cut];
                dim1th_high << t2kgeometry::fgd1max_offset[dim1cut];

                dim2th_low << t2kgeometry::fgd1min_offset[dim2cut];
                dim2th_high << t2kgeometry::fgd1max_offset[dim2cut];

                string vtx_pos_cuts = basecuts[br];
                    //1st Dim : True vars:
                vtx_pos_cuts += " && ";
                    //1st Dim : True vars min:
                vtx_pos_cuts += dim1th_low.str();
                vtx_pos_cuts += " < ";
                vtx_pos_cuts += m_recovars->truevtx_pos.GetName();
                vtx_pos_cuts += "[";
                vtx_pos_cuts += dim1cuts;
                vtx_pos_cuts += "]";
                vtx_pos_cuts += " && ";
                    //1st Dim : True vars min:
                vtx_pos_cuts += m_recovars->truevtx_pos.GetName();
                vtx_pos_cuts += "[";
                vtx_pos_cuts += dim1cuts;
                vtx_pos_cuts += "]";
                vtx_pos_cuts += " < ";
                vtx_pos_cuts += dim1th_high.str();

                    //1st Dim : Reco vars:
                vtx_pos_cuts += " && ";
                    //1st Dim : Reco vars min:
                vtx_pos_cuts += dim1th_low.str();
                vtx_pos_cuts += " < ";
                vtx_pos_cuts += m_recovars->vtx_pos.GetName();
                vtx_pos_cuts += "[";
                vtx_pos_cuts += dim1cuts;
                vtx_pos_cuts += "]";
                vtx_pos_cuts += " && ";
                    //1st Dim : Reco vars min:
                vtx_pos_cuts += m_recovars->vtx_pos.GetName();
                vtx_pos_cuts += "[";
                vtx_pos_cuts += dim1cuts;
                vtx_pos_cuts += "]";
                vtx_pos_cuts += " < ";
                vtx_pos_cuts += dim1th_high.str();

                    //2nd Dim : True vars:
                vtx_pos_cuts += " && ";
                    //2nd Dim : True vars min:
                vtx_pos_cuts += dim2th_low.str();
                vtx_pos_cuts += " < ";
                vtx_pos_cuts += m_recovars->truevtx_pos.GetName();
                vtx_pos_cuts += "[";
                vtx_pos_cuts += dim2cuts;
                vtx_pos_cuts += "]";
                vtx_pos_cuts += " && ";
                    //2nd Dim : True vars min:
                vtx_pos_cuts += m_recovars->truevtx_pos.GetName();
                vtx_pos_cuts += "[";
                vtx_pos_cuts += dim2cuts;
                vtx_pos_cuts += "]";
                vtx_pos_cuts += " < ";
                vtx_pos_cuts += dim2th_high.str();

                    //2nd Dim : Reco vars:
                vtx_pos_cuts += " && ";
                    //2nd Dim : Reco vars min:
                vtx_pos_cuts += dim2th_low.str();
                vtx_pos_cuts += " < ";
                vtx_pos_cuts += m_recovars->vtx_pos.GetName();
                vtx_pos_cuts += "[";
                vtx_pos_cuts += dim2cuts;
                vtx_pos_cuts += "]";
                vtx_pos_cuts += " && ";
                    //2nd Dim : Reco vars min:
                vtx_pos_cuts += m_recovars->vtx_pos.GetName();
                vtx_pos_cuts += "[";
                vtx_pos_cuts += dim2cuts;
                vtx_pos_cuts += "]";
                vtx_pos_cuts += " < ";
                vtx_pos_cuts += dim2th_high.str();

                // cout << vtx_pos_cuts << endl;

                Variable vtx_pos(m_recovars->truevtx_pos.GetName() + "[" + dimss.str() + "]:" + m_recovars->vtx_pos.GetName() + "[" + dimss.str() + "]", m_recovars->vtx_pos.GetSymbol(), m_recovars->vtx_pos.GetUnits());
                    // This probably wont work as the code looks for :: to make a split... Add fix.
                vtx_pos.SetSName(m_recovars->vtx_pos.GetName() + m_NameXYZ[dim] );
                // start_pos.SetPDG(part->pdg.GetName());

                //**************************************** 1D START ************************************//
                ProduceGroup(vtx_pos, dimnbins[dim], t2kgeometry::fgd1min_offset[dim], t2kgeometry::fgd1max_offset[dim], vtx_pos_cuts);//TODO: Add detector lines to this plot.
                //**************************************** 1D END ************************************//

                //**************************************** 1D NFGD Segs VTX Pos START ************************************//

                //Vertex Position for NFGD Segments: 1, 2, 3:
                for(int nsegs = 1; nsegs < 4; nsegs++){
                    stringstream snsegs; 
                    snsegs << nsegs;
                    string seg_vtx_cut = nfgdsegments.GetName() + " == " + snsegs.str() + " && " + vtx_pos_cuts + " && " + segcuts;
                    Variable vtx_pos_fgdseg(vtx_pos);
                    vtx_pos_fgdseg.SetSName( vtx_pos_fgdseg.GetSName() + "_" + snsegs.str() + "fgdseg" );
                    vtx_pos_fgdseg.SetSymbol( vtx_pos_fgdseg.GetSymbol() + " " + snsegs.str() + "FGD Segment(s)" );

                    ProduceGroup(vtx_pos_fgdseg, dimnbins[dim], t2kgeometry::fgd1min_offset[dim], t2kgeometry::fgd1max_offset[dim], seg_vtx_cut);
                    //TODO: Add detector lines to this plot.
                }
                //**************************************** 1D NFGD Segs VTX Pos END ************************************//


                //**************************************** 1D Purity START ************************************//

                TH1D * vtx_pos_H_pur = m_runep->PurVSVar(m_recovars->vtx_pos.GetName() + "[" + dimss.str() + "]", dimnbins[dim], t2kgeometry::fgd1min_offset[dim], t2kgeometry::fgd1max_offset[dim],
                    m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlus).GetSignal(), vtx_pos_cuts,
                    m_recovars->vtx_pos.GetSymbol() + " " + m_NameXYZ[dim] + " (" + m_recovars->vtx_pos.GetUnits() + ")");

                TH1D * vtx_pos_CH_pur = m_runep->PurVSVar(m_recovars->vtx_pos.GetName() + "[" + dimss.str() + "]", dimnbins[dim], t2kgeometry::fgd1min_offset[dim], t2kgeometry::fgd1max_offset[dim],
                    m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlus).GetSignal(), vtx_pos_cuts,
                    m_recovars->vtx_pos.GetSymbol() + " " + m_NameXYZ[dim] + " (" + m_recovars->vtx_pos.GetUnits() + ")");

                TCanvas * vtx_pos_pur_c = new TCanvas((vtx_pos.GetSName() + "_pur").c_str(), "", 400, 400);
                vtx_pos_pur_c->cd();

                vtx_pos_H_pur->SetLineColor(DrawingStyle::Blue);
                vtx_pos_H_pur->Draw();

                vtx_pos_CH_pur->SetLineColor(DrawingStyle::Yellow);
                vtx_pos_CH_pur->Draw("SAME");

                TLegend * vtx_pos_pur_leg = m_runbd->Legend(0.15, 0.1);
                vtx_pos_pur_leg->AddEntry(vtx_pos_H_pur, m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlus).GetSymbol().c_str(), "l" );
                vtx_pos_pur_leg->AddEntry(vtx_pos_CH_pur, m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlus).GetSymbol().c_str(), "l" );
                vtx_pos_pur_leg->Draw();

                m_runbd->GetPOT(0.1, 0.1)->Draw();
                PrintLogo(vtx_pos_pur_c);

                vtx_pos_pur_c->Write();

                delete vtx_pos_H_pur;
                delete vtx_pos_CH_pur;
                delete vtx_pos_pur_leg;
                delete vtx_pos_pur_c;

                //**************************************** 1D Purity END ************************************//

                for(int dim2 = dim + 1; dim2 < 3; dim2++){
                    stringstream dim2ss;
                    dim2ss << dim2;

                    string vtx_pos2D = m_recovars->vtx_pos.GetName() + "[" + dim2ss.str() + "]:" + m_recovars->vtx_pos.GetName() + "[" + dimss.str() + "]";
                //**************************************** 2D START ************************************//
                    TH2D * vtx_pos2D_h = m_runbd->GetHisto(vtx_pos2D, dimnbins[dim], t2kgeometry::fgd1min_offset[dim], 
                        t2kgeometry::fgd1max_offset[dim], dimnbins[dim2], t2kgeometry::fgd1min_offset[dim2], 
                        t2kgeometry::fgd1max_offset[dim2], "Start Position " + m_NameXYZ[dim] + " (mm);Start Position " + m_NameXYZ[dim2] + " (mm)", vtx_pos_cuts);

                    // DrawLine(double x_low, double y_low, double x_high, double y_high, int color = kGray + 2, int style = 1, int width = 2);

                    TCanvas * vtx_pos2D_c = new TCanvas((vtx_pos.GetSName() + m_NameXYZ[dim2]).c_str(), "", 400, 400);
                    vtx_pos2D_c->cd();
                    vtx_pos2D_h->Draw("COLZ");
                    double box_low[2] = { t2kgeometry::fgd1min[dim], t2kgeometry::fgd1min[dim2] };
                    double box_hig[2] = { t2kgeometry::fgd1max[dim], t2kgeometry::fgd1max[dim2] };
                    m_runbd->DrawBox(box_low, box_hig);
                    m_runbd->GetPOT(0.1,0.1)->Draw();
                    PrintLogo(vtx_pos2D_c);

                    vtx_pos2D_c->Write();

                    delete vtx_pos2D_h;
                    delete vtx_pos2D_c;

                    //Vertex Position for NFGD Segments: 1, 2, 3:
                    for(int nsegs = 1; nsegs < 4; nsegs++){
                        stringstream snsegs; 
                        snsegs << nsegs;
                        string seg_vtx_cut = nfgdsegments.GetName() + " == " + snsegs.str() + " && " + vtx_pos_cuts + " && " + segcuts;
    
                        TH2D * vtx_pos2D_fgdseg_h = m_runbd->GetHisto(vtx_pos2D, dimnbins[dim], t2kgeometry::fgd1min_offset[dim], 
                        t2kgeometry::fgd1max_offset[dim], dimnbins[dim2], t2kgeometry::fgd1min_offset[dim2], 
                        t2kgeometry::fgd1max_offset[dim2], "Start Position " + m_NameXYZ[dim] + " (mm);Start Position " + m_NameXYZ[dim2] + " (mm)", seg_vtx_cut);

                        TCanvas * vtx_pos2D_fgdseg_c = new TCanvas((vtx_pos.GetSName() + m_NameXYZ[dim2] + "_" + snsegs.str() + "fgdseg").c_str(), "", 400, 400);
                        vtx_pos2D_fgdseg_c->cd();
                        vtx_pos2D_fgdseg_h->Draw("COLZ");
                        double box_low[2] = { t2kgeometry::fgd1min[dim], t2kgeometry::fgd1min[dim2] };
                        double box_hig[2] = { t2kgeometry::fgd1max[dim], t2kgeometry::fgd1max[dim2] };
                        m_runbd->DrawBox(box_low, box_hig);
                        m_runbd->GetPOT(0.1,0.1)->Draw();
                        PrintLogo(vtx_pos2D_fgdseg_c);

                        vtx_pos2D_fgdseg_c->Write();

                        delete vtx_pos2D_fgdseg_h;
                        delete vtx_pos2D_fgdseg_c;

                    }

                //**************************************** 2D END ************************************//  

                //**************************************** 2D Purity START ************************************//

                    TH2D * vtx_pos2D_pur_h = m_runep->PurVSVar(vtx_pos2D, dimnbins[dim], t2kgeometry::fgd1min_offset[dim], 
                        t2kgeometry::fgd1max_offset[dim], dimnbins[dim2], t2kgeometry::fgd1min_offset[dim2], 
                        t2kgeometry::fgd1max_offset[dim2], m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlus).GetSignal(),
                        vtx_pos_cuts ,"Start Position " + m_NameXYZ[dim] + " (mm);Start Position " + m_NameXYZ[dim2] + " (mm)");

                    TCanvas * vtx_pos2D_pur_c = new TCanvas((vtx_pos.GetSName() + m_NameXYZ[dim2] + "_pur").c_str(), "", 400, 400);
                    vtx_pos2D_pur_c->cd();
                    vtx_pos2D_pur_h->Draw("COLZ");
                    m_runbd->DrawBox(box_low, box_hig);
                    m_runbd->GetPOT(0.1,0.1)->Draw();
                    PrintLogo(vtx_pos2D_pur_c);
                    GetSignal()->Draw();

                    vtx_pos2D_pur_c->Write();

                    delete vtx_pos2D_pur_h;
                    delete vtx_pos2D_pur_c;

                //**************************************** 2D Purity END ************************************//

                }

            }
                //**************************************** Vertex Position END ************************************//
        }

        if(m_experiment->GetType() == Experiment::MIN){
            if(m_verbose) cout << "Making W dists for MINERvA" << endl;

            //************************************** W Mass Start *************************************//
            MakeDir("W");
            Variable W_mass("mc_w", "W", "MeV");
            W_mass.SetSName(br == 0 ? "W_EX" : "W_LL");

            TCanvas * w_dist = m_runbd->TARGETSingle(W_mass, 200, 0, 3000, basecuts[br]);
            PrintLogo(w_dist);
            w_dist->Write();
            //**************************************** W Mass END *************************************//
        }

        if(!m_realdata){
            //******************************** Efficiency/Purity START ********************************//

            if(m_verbose) cout << "Producing truth plots" << endl;

            MakeDir("Efficiency/Cuts" + branchnames[br]);

            TCanvas * eff_pur_cuts = new TCanvas("eff_pur_cuts","", 600, 800);
            eff_pur_cuts->cd();
            TH1D * eff_new = m_runep->EffVSCuts( m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlus).GetSignal(), br);
            TH1D * pur_new = m_runep->PurVSCuts( m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlus).GetSignal(), br);

            TH1D * eff_CC1P1Pi = m_runep->EffVSCuts( m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlus).GetSignal(), br);
            TH1D * pur_CC1P1Pi = m_runep->PurVSCuts( m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlus).GetSignal(), br);

            TH1D * eff_CC1P1PiInc = m_runep->EffVSCuts( m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlusInc).GetSignal(), br);
            TH1D * pur_CC1P1PiInc = m_runep->PurVSCuts( m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlusInc).GetSignal(), br);


            eff_new->SetLineColor(DrawingStyle::Yellow);
            // eff_new->SetLineStyle(7);
            pur_new->SetLineColor(DrawingStyle::Yellow);
            pur_new->SetLineStyle(7);

            eff_CC1P1Pi->SetLineColor(DrawingStyle::Red);
            // eff_CC1P1Pi->SetLineStyle(7);
            pur_CC1P1Pi->SetLineColor(DrawingStyle::Red);
            pur_CC1P1Pi->SetLineStyle(7);


            eff_CC1P1PiInc->SetLineColor(DrawingStyle::Blue);
            // eff_CC1P1Pi->SetLineStyle(7);
            pur_CC1P1PiInc->SetLineColor(DrawingStyle::Blue);
            pur_CC1P1PiInc->SetLineStyle(7);

            eff_new->GetYaxis()->SetTitle("Pur/Eff. (%)");
            eff_new->Draw("HIST");
            pur_new->Draw("HISTSAME");

            eff_CC1P1Pi->Draw("HISTSAME");
            pur_CC1P1Pi->Draw("HISTSAME");

            eff_CC1P1PiInc->Draw("HISTSAME");
            pur_CC1P1PiInc->Draw("HISTSAME");

            TLegend * eff_pur_cuts_leg = m_runbd->Legend(0.2,0.1);
            eff_pur_cuts_leg->AddEntry(eff_new, ("Efficiency (" + m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlus).GetSymbol() + ")").c_str(), "l");
            eff_pur_cuts_leg->AddEntry(pur_new, ("Purity ("     + m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlus).GetSymbol() + ")").c_str(), "l");

            eff_pur_cuts_leg->AddEntry(eff_CC1P1Pi, ("Efficiency (" + m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlus).GetSymbol() + ")").c_str(), "l");
            eff_pur_cuts_leg->AddEntry(pur_CC1P1Pi, ("Purity ("     + m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlus).GetSymbol() + ")").c_str(), "l");

            eff_pur_cuts_leg->AddEntry(eff_CC1P1PiInc, ("Efficiency (" + m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlusInc).GetSymbol() + ")").c_str(), "l");
            eff_pur_cuts_leg->AddEntry(pur_CC1P1PiInc, ("Purity ("     + m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlusInc).GetSymbol() + ")").c_str(), "l");

            if(m_experiment->GetType() == Experiment::MIN){
                std::string signal_def_old = "truth_n_pro == 1 && truth_n_piP == 1 && truth_n_muo == 1 && mc_nFSPart == 3 && mc_targetZ == 1";
                signal_def_old += " && mc_current == 1 && TMath::RadToDeg()*truth_mu_Theta < 25. && TMath::RadToDeg()*truth_mu_Theta >= 0.";

                TH1D * eff_old = m_runep->EffVSCuts( signal_def_old, br);
                TH1D * pur_old = m_runep->PurVSCuts( signal_def_old, br);

                eff_old->SetLineStyle(7);
                pur_old->SetLineStyle(7);
                eff_old->Draw("HISTSAME");
                pur_old->Draw("HISTSAME");

                eff_pur_cuts_leg->AddEntry(eff_old, "Efficiency (Old Sig. Def.)", "l");
                eff_pur_cuts_leg->AddEntry(pur_old, "Purity (Old Sig. Def.)", "l");

                eff_pur_cuts_leg->Draw();            
                eff_pur_cuts->Write();

                delete eff_old;
                delete pur_old;
            }
            else{
                eff_pur_cuts_leg->Draw();            
                eff_pur_cuts->Write();
            }

            delete eff_new;
            delete pur_new;
            delete eff_CC1P1Pi;
            delete pur_CC1P1Pi;
            delete eff_pur_cuts_leg;
            delete eff_pur_cuts;

            //******************************** Efficiency/Purity END ********************************//

            //******************************** Efficiency/Purity N - 1 Cuts START ********************************//

            if(m_experiment->GetType() == Experiment::T2K){
                MakeDir("Efficiency/N1Cuts" + branchnames[br]);

                TH1D * effN1_new = m_runep->EffVSN1Cuts( m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlus).GetSignal(), br);
                TH1D * purN1_new = m_runep->PurVSN1Cuts( m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlus).GetSignal(), br);

                effN1_new->SetLineColor(DrawingStyle::Yellow);
                purN1_new->SetLineColor(DrawingStyle::Yellow);
                purN1_new->SetLineStyle(7);
                // TH1D * effN1_CC1P1Pi = m_runep->EffVSN1Cuts( m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlus).GetSignal(), br);
                // TH1D * purN1_CC1P1Pi = m_runep->PurVSN1Cuts( m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlus).GetSignal(), br);

                TH1D * effN1_CC1P1PiInc = m_runep->EffVSN1Cuts( m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlusInc).GetSignal(), br);
                TH1D * purN1_CC1P1PiInc = m_runep->PurVSN1Cuts( m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlusInc).GetSignal(), br);
                effN1_CC1P1PiInc->SetLineColor(DrawingStyle::Blue);
                purN1_CC1P1PiInc->SetLineColor(DrawingStyle::Blue);
                purN1_CC1P1PiInc->SetLineStyle(7);

                TLegend * eff_pur_N1cuts_leg = m_runbd->Legend(0.2,0.1);
                eff_pur_N1cuts_leg->AddEntry(effN1_new, ("Efficiency (" + m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlus).GetSymbol() + ")").c_str(), "l");
                eff_pur_N1cuts_leg->AddEntry(purN1_new, ("Purity ("     + m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlus).GetSymbol() + ")").c_str(), "l");

                // eff_pur_N1cuts_leg->AddEntry(effN1_CC1P1Pi, ("Efficiency (" + m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlus).GetSymbol() + ")").c_str(), "l");
                // eff_pur_N1cuts_leg->AddEntry(purN1_CC1P1Pi, ("Purity ("     + m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlus).GetSymbol() + ")").c_str(), "l");

                eff_pur_N1cuts_leg->AddEntry(effN1_CC1P1PiInc, ("Efficiency (" + m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlusInc).GetSymbol() + ")").c_str(), "l");
                eff_pur_N1cuts_leg->AddEntry(purN1_CC1P1PiInc, ("Purity ("     + m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlusInc).GetSymbol() + ")").c_str(), "l");

                TCanvas * eff_pur_N1cuts = new TCanvas("eff_pur_N1cuts","", 600, 800);
                eff_pur_N1cuts->cd();
                effN1_new->Draw("HIST");
                purN1_new->Draw("HISTSAME");
                effN1_CC1P1PiInc->Draw("HISTSAME");
                purN1_CC1P1PiInc->Draw("HISTSAME");
                eff_pur_N1cuts_leg->Draw();

                eff_pur_N1cuts->Write();

                delete effN1_new;
                delete purN1_new;
                delete effN1_CC1P1PiInc;
                delete purN1_CC1P1PiInc;
                delete eff_pur_N1cuts_leg;
                delete eff_pur_N1cuts;

            }

            //******************************** Efficiency/Purity N - 1 Cuts END ********************************//

        }

        list.clear();
    }

    if(m_verbose) cout << "Finished Making Plots" << endl;

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
    bool verbose = false;
    int accum_level = -999;
    int branch_no = -999;

    char cc;
    while((cc = getopt(argc, argv, "i:o:d::t::m::r::v::a:b:")) != -1){
        switch (cc){
            case 'i': filename = std::string(optarg); break;//input file
            case 'o': savename = std::string(optarg); break;//output file
            case 'd': debug = true; break;                  //Debug on
            case 't': experiment = Experiment::T2K; break;  //Exp type
            case 'm': experiment = Experiment::MIN; break;  //Exp type
            case 'r': realdata = true; break;               //Running w/o truth tree
            case 'v': verbose = true; break;                //Verbose on
            case 'a': accum_level = atoi(optarg); break;    //Set the accum_level to plot from
            case 'b': branch_no = atoi(optarg); break;      //Choose which branch you want to plot
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

    ProducePlots * plots = new ProducePlots(experiment, filename, savename, debug, realdata);
    plots->Verbose(verbose);
    plots->SetBranchToPlot(accum_level, branch_no);//Make this function better.

    // if(!savename.empty()) plots->SetSaveName(savename);
    
    plots->MakePlots();

//    delete plots; -- TODO: Fix this
    
    return 0;
}
