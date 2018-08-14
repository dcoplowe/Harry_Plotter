#include <TCanvas.h>
#include <TObject.h>

//#include "TDirectory.h"
//#include "TROOT.h"
#include <TClass.h>
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

#include <EffPurTools.h>
#include <DrawingTools.h>
#include <BreakdownTools.h>
#include <DataClasses.h>
// #include "PartKinMaps.h"

#include <TH1D.h>
#include <TH2D.h>
#include <THStack.h>
#include <TLegend.h>
#include <TFile.h>
#include <TDatime.h>
#include <TLatex.h>
#include <TDirectory.h>
#include <TMath.h>
#include <TPie.h>

//dumping of array
#define dump_array3(par) std::cout << std::fixed    << \
  std::setw(10) << std::left   << #par   << ": \t"  << \
  std::setw(10) << std::right  << par[0] << " "     << \
  std::setw(10) << std::right  << par[1] << " "     << \
  std::setw(10) << std::right  << par[2] << std::endl;

#define get_par(par) std::cout << std::fixed    << \
  std::setw(10) << std::left   << #par   << ": \t"  << std::endl;

#define GET_PAR_NAME(par) (#par)
//#include <TStyle.h>

using namespace std;

const string test_min_mc("/pnfs/minerva/persistent/users/dcoplowe/CC1P1Pi_PL13C_030317/grid/central_value/minerva/ana/v10r8p9/00/01/32/00/SIM_minerva_00013200_Subruns_0001-0002-0003-0004_CC1P1PiAnalysis_Ana_Tuple_v10r8p9-dcoplowe.root");
const string test_t2k_mc("/data/t2k/coplowe/numuCC1P1PiOutput/march17/neutP6BWA/FGD1/nall_data_FGD1_type2_wome_fboff_minaclev0_130517/AnaOut_sub00001.root");

class ProducePlots {

public:
    enum RunOpts
    {
        EffVSN1Cuts = 0,
        EffVSCuts, 
        W, 
        VtxPosition2DPur,
        VtxPosition, 
        VtxPosition2DFGDSegs, 
        VtxPosition2D, 
        VtxPosition1DPur, 
        VtxPosition1DFGDSegs, 
        VtxPosition1D,
        NFGDSegments, 
        DPTTFGDSegs, 
        DPTT, 
        PIDScore, 
        StartPosition1D,
        StartPosition2DEff, 
        StartPositionAll, 
        StartPosition2DPur, 
        StartPosition2DFGDSegs, 
        StartPosition2D, 
        StartPositionPur1D,
        StartPosition1DFGDSegs, 
        FGDSegments, 
        CrossingAngle, 
        cTheta, 
        PTheta, 
        Phi, 
        Theta, 
        Mom,
        NTracks,
        All, //29
        NotSet       
    };

    static inline std::string ToString(ProducePlots::RunOpts var){
        if(ProducePlots::EffVSN1Cuts == var) return std::string(GET_PAR_NAME(EffVSN1Cuts));
        else if(ProducePlots::EffVSCuts == var) return std::string(GET_PAR_NAME(EffVSCuts));
        else if(ProducePlots::W == var) return std::string(GET_PAR_NAME(W));
        else if(ProducePlots::VtxPosition2DPur == var) return std::string(GET_PAR_NAME(VtxPosition2DPur));
        else if(ProducePlots::VtxPosition == var) return std::string(GET_PAR_NAME(VtxPosition));
        else if(ProducePlots::VtxPosition2DFGDSegs == var) return std::string(GET_PAR_NAME(VtxPosition2DFGDSegs));
        else if(ProducePlots::VtxPosition2D == var) return std::string(GET_PAR_NAME(VtxPosition2D));
        else if(ProducePlots::VtxPosition1DPur == var) return std::string(GET_PAR_NAME(VtxPosition1DPur));
        else if(ProducePlots::VtxPosition1DFGDSegs == var) return std::string(GET_PAR_NAME(VtxPosition1DFGDSegs));
        else if(ProducePlots::VtxPosition1D == var) return std::string(GET_PAR_NAME(VtxPosition1D));
        else if(ProducePlots::NFGDSegments == var) return std::string(GET_PAR_NAME(NFGDSegments));
        else if(ProducePlots::DPTTFGDSegs == var) return std::string(GET_PAR_NAME(DPTTFGDSegs));
        else if(ProducePlots::DPTT == var) return std::string(GET_PAR_NAME(DPTT));
        else if(ProducePlots::PIDScore == var) return std::string(GET_PAR_NAME(PIDScore));
        else if(ProducePlots::StartPosition1D == var) return std::string(GET_PAR_NAME(StartPosition1D));
        else if(ProducePlots::StartPosition2DEff == var) return std::string(GET_PAR_NAME(StartPosition2DEff));
        else if(ProducePlots::StartPositionAll == var) return std::string(GET_PAR_NAME(StartPositionAll));
        else if(ProducePlots::StartPosition2DPur == var) return std::string(GET_PAR_NAME(StartPosition2DPur));
        else if(ProducePlots::StartPosition2DFGDSegs == var) return std::string(GET_PAR_NAME(StartPosition2DFGDSegs));
        else if(ProducePlots::StartPosition2D == var) return std::string(GET_PAR_NAME(StartPosition2D));
        else if(ProducePlots::StartPositionPur1D == var) return std::string(GET_PAR_NAME(StartPositionPur1D));
        else if(ProducePlots::StartPosition1DFGDSegs == var) return std::string(GET_PAR_NAME(StartPosition1DFGDSegs));
        else if(ProducePlots::FGDSegments == var) return std::string(GET_PAR_NAME(FGDSegments));
        else if(ProducePlots::CrossingAngle == var) return std::string(GET_PAR_NAME(CrossingAngle));
        else if(ProducePlots::cTheta == var) return std::string(GET_PAR_NAME(cTheta));
        else if(ProducePlots::PTheta == var) return std::string(GET_PAR_NAME(PTheta));
        else if(ProducePlots::Phi == var) return std::string(GET_PAR_NAME(Phi));
        else if(ProducePlots::Theta == var) return std::string(GET_PAR_NAME(Theta));
        else if(ProducePlots::Mom == var) return std::string(GET_PAR_NAME(Mom));
        else if(ProducePlots::NTracks == var) return std::string(GET_PAR_NAME(NTracks));
        else if(ProducePlots::All == var) return std::string(GET_PAR_NAME(All)); //28;
        else if(ProducePlots::NotSet == var) return std::string(GET_PAR_NAME(NotSet));
        else return std::string("Unknown");
    }

    ProducePlots(Experiment::Name exp, std::string infilename, std::string outfilename, bool debug, bool realdata, RunOpts opts);
    ~ProducePlots();
    
    // void SetSaveName(std::string var){ m_savename = var; }
    
//    void ProduceGroup();

    void MakePlots();
    
    TLatex * GetLogo();
    void PrintLogo(TCanvas *& can);
    
    TLatex * GetSignal(string extra = "");
    
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

    std::string GetPosCuts(Variable pos, int dim1cut, int dim2cut, std::string cuts);

    void RunWithCuts(){ m_cut_onoff_high = 3; }
    void RunWithCutsOnly(int cut);

private:
    std::string m_infilename;
    bool m_realdata;

    int m_accum_level;
    int m_branch;

    Experiment * m_experiment;

    KinematicVars * m_recovars;
    
    std::string m_savename;

    RunOpts m_opts;

    int m_cut_onoff_low;
    int m_cut_onoff_high;
    
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

    std::string m_old_signal;

    std::string m_NameXYZ[3];

    void DrawDetectorLines(TCanvas * can, int xyz);

    bool m_verbose;

    bool DrawPlot(ProducePlots::RunOpts var1, ProducePlots::RunOpts var2 = ProducePlots::NotSet);//{ return (m_opts == ); }

    // std::string GetParName()

};

ProducePlots::ProducePlots(Experiment::Name exp, std::string infilename, std::string outfilename, bool debug, bool realdata, RunOpts opts) : m_infilename(infilename),
m_realdata(realdata), m_accum_level(-999), m_branch(-999), m_savename(outfilename), m_opts(opts), m_cut_onoff_low(0), m_cut_onoff_high(1) {

    cout << "Experiment: " <<  Experiment::ToString(exp) << endl;

    cout << "Opts : " << ToString(m_opts) << endl;

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

    if(m_opts != ProducePlots::All){
        //Find .root, append ToString(ProducePlots::XXX) + ".root"
        size_t ff = m_savename.find(".root");
        if(ff != std::string::npos){
            m_savename = m_savename.substr(0,ff);
            m_savename += "_";
            m_savename += ToString(m_opts);
            m_savename += ".root";
            // cout << "m_savename = " << m_savename << endl; 
        }
    }

    cout << "Saving file as " << m_savename << endl;

    // cout << " = m_experiment->GGetSignal()" << m_experiment->GetSignal() << endl;

    m_runbd = new BreakdownTools( m_infilename, m_experiment->GetRecoName(), m_experiment->GetTopologies(), m_experiment->GetTarVarName() );
    cout << "m_runbd " <<endl;
    if(!m_realdata){
        m_runtruthbd = new BreakdownTools(m_infilename, m_experiment->GetTrueName(), m_experiment->GetTopologies(), m_experiment->GetTarVarName());
        cout << "m_runtruthbd " <<endl;
    }

    m_old_signal = "true_ntracks == 3 && truemu_ntracks == 1 && truep_ntracks == 1 && truepi_ntracks == 1 && target == 1";
    
    // std::vector<std::string> selection_cuts;
    // selection_cuts.push_back("Vertex");
    // selection_cuts.push_back("3 Tracks");
    // selection_cuts.push_back("Muon Track");
    // selection_cuts.push_back("Contained Vtx");
    // selection_cuts.push_back("PID: p/#pi^{+}");
    // selection_cuts.push_back("Michel Sense");
    
    if(!m_realdata) m_runep = new EffPurTools(m_infilename, m_experiment->GetCutNames(), m_experiment->GetRecoName(), m_experiment->GetTrueName() );
    
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

void ProducePlots::RunWithCutsOnly(int cut)
{  
    m_cut_onoff_low = cut; 
    m_cut_onoff_high = cut + 1;
        //Find .root, append ToString(ProducePlots::XXX) + ".root"
    size_t ff = m_savename.find(".root");
    if(ff != std::string::npos){
        m_savename = m_savename.substr(0,ff);
        m_savename += "_cut";
        stringstream ss;
        ss << cut;
        m_savename += ss.str();
        m_savename += ".root";
            // cout << "m_savename = " << m_savename << endl; 
    }        
}

bool ProducePlots::DrawPlot(ProducePlots::RunOpts var1, ProducePlots::RunOpts var2)
{
    if(m_opts == ProducePlots::All) return true;
    else if(var2 == ProducePlots::NotSet) return (m_opts == var1);
    else return (m_opts == var1 || m_opts == var2); 
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
    mom.SetSName(part->P.GetSName());
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
    var.SetSName( (nudir ? part->ctheta_nudir.GetSName() : part->ctheta.GetSName()) );
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
        TH1D * H_pur = m_runep->PurVSVar(var.GetName().c_str(), var.GetNBins(), var.GetBinning(), m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlusPS).GetSignal(), cuts, var.GetSymbol() + (var.GetUnits().empty() ? "" : " (" + var.GetUnits() + ")"));
        H_pur->SetLineColor(DrawingStyle::HYDROGEN);

        TH1D * CH_pur = m_runep->PurVSVar(var.GetName().c_str(), var.GetNBins(), var.GetBinning(), m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlus).GetSignal(), cuts, var.GetSymbol() + (var.GetUnits().empty() ? "" : " (" + var.GetUnits() + ")"));
        CH_pur->SetLineColor(DrawingStyle::CARBON);

        H_pur->Draw();
        CH_pur->Draw("SAME");

        TLegend * pur_leg = m_runbd->Legend(0.15, 0.1);
        pur_leg->AddEntry(H_pur, m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlusPS).GetSymbol().c_str(), "l" );
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

std::string ProducePlots::GetPosCuts(Variable pos, int dim1cut, int dim2cut, std::string cuts){
    stringstream dim1cuts, dim2cuts, dim1th_low, dim2th_low, dim1th_high, dim2th_high;                    

    dim1cuts << dim1cut;
    dim2cuts << dim2cut;
    
    dim1th_low << t2kgeometry::fgd1tpcmin_offset[dim1cut];
    dim1th_high << t2kgeometry::fgd1tpcmax_offset[dim1cut];

    dim2th_low << t2kgeometry::fgd1tpcmin_offset[dim2cut];
    dim2th_high << t2kgeometry::fgd1tpcmax_offset[dim2cut];

    string start_pos_cuts = cuts;

    //1st Dim : True vars:
    //1st Dim : Reco vars:
    if(!start_pos_cuts.empty()) start_pos_cuts += " && ";
    //1st Dim : Reco vars min:
    start_pos_cuts += dim1th_low.str();
    start_pos_cuts += " < ";
    start_pos_cuts += pos.GetName();
    start_pos_cuts += "[";
    start_pos_cuts += dim1cuts.str();
    start_pos_cuts += "]";
    start_pos_cuts += " && ";
                    //1st Dim : Reco vars min:
    start_pos_cuts += pos.GetName();
    start_pos_cuts += "[";
    start_pos_cuts += dim1cuts.str();
    start_pos_cuts += "]";
    start_pos_cuts += " < ";
    start_pos_cuts += dim1th_high.str();

                    //2nd Dim : Reco vars:
    start_pos_cuts += " && ";
                    //2nd Dim : Reco vars min:
    start_pos_cuts += dim2th_low.str();
    start_pos_cuts += " < ";
    start_pos_cuts += pos.GetName();
    start_pos_cuts += "[";
    start_pos_cuts += dim2cuts.str();
    start_pos_cuts += "]";
    start_pos_cuts += " && ";
                    //2nd Dim : Reco vars min:
    start_pos_cuts += pos.GetName();
    start_pos_cuts += "[";
    start_pos_cuts += dim2cuts.str();
    start_pos_cuts += "]";
    start_pos_cuts += " < ";
    start_pos_cuts += dim2th_high.str();

    return start_pos_cuts;
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

    //Configure Start
    m_outfile = new TFile(m_savename.c_str(), "RECREATE");

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

        //Configure End

        for(unsigned int ptls = 0; ptls < list.size(); ptls++){
            Particle * party = list[ptls];
            if(m_verbose) cout << party->GetName() << " plots being produced" << endl;
            //**************************************** Mom START ****************************************//
            if(DrawPlot(ProducePlots::Mom)){
                for(int cut_onoff = m_cut_onoff_low; cut_onoff < m_cut_onoff_high; cut_onoff++){
                    MakeDir("Mom" + branchnames[br] + "/" + party->GetName() );
                    string tmp_cuts = basecuts[br];
                    cout << "DAVID 1 " << endl;
                    // cout << " tmp_cuts ======= " << tmp_cuts << endl;

                    if(cut_onoff == 1){ 
                        // cout << "DAVID 1 " << endl;
                        tmp_cuts += " && 0.05 < ";
                        tmp_cuts += party->MyPID.GetName();
                        party->P.SetSName( party->P.GetSName() + "_PIDcut" );
                        MakeDir("Mom" + branchnames[br] + "/" + party->GetName() + "/PIDcut");
                    }
                    else if(cut_onoff == 2){ 
                        // cout << "DAVID 2 " << endl;
                        tmp_cuts += " && ";
                        tmp_cuts += m_experiment->GetTruePScuts();
                        party->P.SetSName( party->P.GetSName() + "_PScut" );
                        MakeDir("Mom" + branchnames[br] + "/" + party->GetName() + "/PScut");
                    }
                    else if(cut_onoff == 3){ 
                        tmp_cuts += " && 0.05 < ";
                        tmp_cuts += party->MyPID.GetName();
                        tmp_cuts += " && ";
                        tmp_cuts += m_experiment->GetTruePScuts();
                        party->P.SetSName( party->P.GetSName() + "_PIDPScut" );
                        MakeDir("Mom" + branchnames[br] + "/" + party->GetName() + "/PIDPScut");
                    }
                    // cout << " tmp_cuts  a   ======= " << tmp_cuts << endl;
                    MakeMomPlots(party, party->P.GetNBins(), party->P.GetBinning(),tmp_cuts);
                }
            }
            //**************************************** Mom END ****************************************//
            
            if(m_experiment->GetType() == Experiment::MIN){
                if(m_verbose) cout << "Making MINERvA specific plots" << endl;

                //**************************************** PTheta START ****************************************//
                if(DrawPlot(ProducePlots::PTheta)){
                    MakeDir("PTheta" + branchnames[br] + "/" + party->GetName() );
                    // Make2DPlots(party->P, party->theta, "ptheta", basecuts[br]);
                    for(int cut_onoff = m_cut_onoff_low; cut_onoff < m_cut_onoff_high; cut_onoff++){
                        string tmp_cuts = basecuts[br];
                        string tmp_sname = "ptheta";
                        if(cut_onoff == 1){ 
                            tmp_cuts += " && 0.05 < ";
                            tmp_cuts += party->MyPID.GetName();
                            tmp_sname += "_PIDcut";
                            // party->theta.SetSName( party->theta.GetSName() + "_PIDcut" );
                            MakeDir("PTheta" + branchnames[br] + "/" + party->GetName() + "/PIDcut");
                        }
                        else if(cut_onoff == 2){ 
                            tmp_cuts += " && ";
                            tmp_cuts += m_experiment->GetTruePScuts();
                            tmp_sname += "_PScuts";    
                            MakeDir("PTheta" + branchnames[br] + "/" + party->GetName() + "/PScut");
                        }
                        else if(cut_onoff == 3){ 
                            tmp_cuts += " && 0.05 < ";
                            tmp_cuts += party->MyPID.GetName();
                            tmp_cuts += " && ";
                            tmp_cuts += m_experiment->GetTruePScuts();
                            tmp_sname += "_PIDPScut";
                            MakeDir("PTheta" + branchnames[br] + "/" + party->GetName() + "/PIDPScut");
                        }
                        Make2DPlots(party->P, party->theta, tmp_sname, tmp_cuts);
                    }
                }
                //**************************************** PTheta END ****************************************//

                //**************************************** Theta START ************************************//
                if(DrawPlot(ProducePlots::Theta)){
                    for(int cut_onoff = m_cut_onoff_low; cut_onoff < m_cut_onoff_high; cut_onoff++){
                        MakeDir("Theta" + branchnames[br] + "/" + party->GetName() );
                        string tmp_cuts = basecuts[br];
                        if(cut_onoff == 1){ 
                            tmp_cuts += " && 0.05 < ";
                            tmp_cuts += party->MyPID.GetName();
                            party->theta.SetSName( party->theta.GetSName() + "_PScut" );
                            MakeDir("Theta" + branchnames[br] + "/" + party->GetName() + "/PScut");                            
                        }
                        else if(cut_onoff == 2){ 
                            tmp_cuts += " && ";
                            tmp_cuts += m_experiment->GetTruePScuts();
                            party->theta.SetSName( party->theta.GetSName() + "_PIDcut" );
                            MakeDir("Theta" + branchnames[br] + "/" + party->GetName() + "/PIDcut");                            
                        }
                        else if(cut_onoff == 3){ 
                            tmp_cuts += " && 0.05 < ";
                            tmp_cuts += party->MyPID.GetName();
                            tmp_cuts += " && ";
                            tmp_cuts += m_experiment->GetTruePScuts();
                            party->theta.SetSName( party->theta.GetSName() + "_PIDPScut" );
                            MakeDir("Theta" + branchnames[br] + "/" + party->GetName() + "/PIDPScut");
                        }
                        MakeThetaPlots(party, party->theta.GetNBins(), party->theta.GetBinning(), tmp_cuts);
                    }
                }
                //**************************************** Theta END **************************************//

                if(DrawPlot(ProducePlots::Phi)){
                //**************************************** Phi START ************************************//
                    // MakeDir("Phi" + branchnames[br] + "/" + party->GetName() );
                    // MakePhiPlots(party, party->phi.GetNBins(), party->phi.GetBinning(), basecuts[br]);
                //**************************************** Phi END **************************************//                
                    for(int cut_onoff = m_cut_onoff_low; cut_onoff < m_cut_onoff_high; cut_onoff++){
                        MakeDir("Phi" + branchnames[br] + "/" + party->GetName());
                        string tmp_cuts = basecuts[br];
                        if(cut_onoff == 1){ 
                            tmp_cuts += " && 0.05 < ";
                            tmp_cuts += party->MyPID.GetName();
                            party->phi.SetSName( party->phi.GetSName() + "_PIDcut" );
                            MakeDir("Phi" + branchnames[br] + "/" + party->GetName() + "/PIDcut");
                        }
                        else if(cut_onoff == 2){ 
                            tmp_cuts += " && ";
                            tmp_cuts += m_experiment->GetTruePScuts();
                            party->phi.SetSName( party->phi.GetSName() + "_PIDcut" );
                            MakeDir("Phi" + branchnames[br] + "/" + party->GetName() + "/PIDcut");                            
                        }
                        else if(cut_onoff == 3){ 
                            tmp_cuts += " && 0.05 < ";
                            tmp_cuts += party->MyPID.GetName();
                            tmp_cuts += " && ";
                            tmp_cuts += m_experiment->GetTruePScuts();
                            party->phi.SetSName( party->phi.GetSName() + "_PIDPScut" );
                            MakeDir("Phi" + branchnames[br] + "/" + party->GetName() + "/PIDPScut");
                        }
                        //**************************************** Phi START ************************************//
                        MakePhiPlots(party, party->phi.GetNBins(), party->phi.GetBinning(), tmp_cuts);
                        //**************************************** Phi END **************************************// 
                    }
                }
            }

            if(m_experiment->GetType() == Experiment::T2K){

                if(m_verbose) cout << "Making T2K specific plots" << endl;
                
                //**************************************** Phi START ************************************//
                if(DrawPlot(ProducePlots::PTheta)){
                    for(int cut_onoff = m_cut_onoff_low; cut_onoff < m_cut_onoff_high; cut_onoff++){
                        MakeDir("PcTheta" + branchnames[br] + "/" + party->GetName() );
                        string tmp_cuts = basecuts[br];
                        string tmp_sname = "pcTheta";
                        if(cut_onoff == 1){ 
                            tmp_cuts += " && 0.05 < ";
                            tmp_cuts += party->MyPID.GetName();
                            tmp_sname += "_PIDcut";
                            MakeDir("PcTheta" + branchnames[br] + "/" + party->GetName() + "/PIDcut");
                            tmp_sname += "PIDcut";
                        }
                        else if(cut_onoff == 2){
                            tmp_cuts += " && ";
                            tmp_cuts += m_experiment->GetTruePScuts();
                            tmp_sname += "_PScuts";    
                            MakeDir("PcTheta" + branchnames[br] + "/" + party->GetName() + "/PScut");
                        }
                        else if(cut_onoff == 3){ 
                            tmp_cuts += " && 0.05 < ";
                            tmp_cuts += party->MyPID.GetName();
                            tmp_cuts += " && ";
                            tmp_cuts += m_experiment->GetTruePScuts();
                            tmp_sname += "_PIDPScuts"; 
                            MakeDir("PcTheta" + branchnames[br] + "/" + party->GetName() + "/PIDPScut");
                        }
                        Make2DPlots(party->P, party->ctheta_nudir, tmp_sname, tmp_cuts); 
                    }
                }
                //**************************************** Phi END **************************************//

                //**************************************** cosTheta START ************************************//
                if(DrawPlot(ProducePlots::cTheta)){
                    if(m_verbose) cout << "Cos Theta";

                    for(int cut_onoff = m_cut_onoff_low; cut_onoff < m_cut_onoff_high; cut_onoff++){
                        MakeDir("cTheta/" + party->GetName() );
                        string tmp_cuts = basecuts[br];
                        string tmp_sname = "";
                        if(cut_onoff == 1){ 
                            tmp_cuts += " && 0.05 < ";
                            tmp_cuts += party->MyPID.GetName();
                            MakeDir("cTheta/" + party->GetName() + "/PIDcut");
                            tmp_sname = "_PIDcut";
                        }
                        else if(cut_onoff == 2){
                            tmp_cuts += " && ";
                            tmp_cuts += m_experiment->GetTruePScuts();
                            tmp_sname += "_PScuts";    
                            MakeDir("cTheta/" + party->GetName() + "/PScut");
                        }
                        else if(cut_onoff == 3){ 
                            tmp_cuts += " && 0.05 < ";
                            tmp_cuts += party->MyPID.GetName();
                            tmp_cuts += " && ";
                            tmp_cuts += m_experiment->GetTruePScuts();
                            tmp_sname += "_PIDPScuts"; 
                            MakeDir("cTheta" + branchnames[br] + "/" + party->GetName() + "/PIDPScut");
                        }
                        // 
                        party->ctheta.SetSName( party->ctheta.GetName() + tmp_sname );
                        party->ctheta_nudir.SetSName( party->ctheta_nudir.GetName() + tmp_sname );

                        MakeCosThetaPlots(party, party->ctheta.GetNBins(), party->ctheta.GetBinning(), tmp_cuts);
                        MakeCosThetaPlots(party, party->ctheta_nudir.GetNBins(), party->ctheta_nudir.GetBinning(), tmp_cuts, true);
                    }
                    if(m_verbose) cout << " -- Done" << endl;

                    party->ctheta.SetSName( party->ctheta.GetName() );
                    party->ctheta_nudir.SetSName( party->ctheta_nudir.GetName() );

                }
                //**************************************** cosTheta END **************************************//

                //**************************************** Crossing Angle START ************************************//

                if(DrawPlot(ProducePlots::CrossingAngle)){
                    if(m_verbose) cout << "Crossing Angle";

                    for(int cut_onoff = m_cut_onoff_low; cut_onoff < m_cut_onoff_high; cut_onoff++){
                        MakeDir("CrossingAngle/" + party->GetName());
                        party->cross_angle.SetSName(party->cross_angle.GetName());
                        string tmp_cuts = basecuts[br];
                        if(cut_onoff == 1){ 
                            tmp_cuts += " && 0.05 < ";
                            tmp_cuts += party->MyPID.GetName();
                            party->cross_angle.SetSName( party->cross_angle.GetSName() + "_PIDcut" );
                            MakeDir("CrossingAngle/" + party->GetName() + "/PIDcut");
                        }
                        else if(cut_onoff == 2){ 
                            tmp_cuts += " && ";
                            tmp_cuts += m_experiment->GetTruePScuts();
                            party->cross_angle.SetSName( party->cross_angle.GetSName() + "_PScut" );
                            MakeDir("CrossingAngle/" + party->GetName() + "/PScut");
                        }
                        else if(cut_onoff == 3){ 
                            tmp_cuts += " && 0.05 < ";
                            tmp_cuts += party->MyPID.GetName();
                            tmp_cuts += " && ";
                            tmp_cuts += m_experiment->GetTruePScuts();
                            party->cross_angle.SetSName( party->cross_angle.GetSName() + "_PIDPScut" );
                            MakeDir("CrossingAngle/" + party->GetName() + "/PIDPScut");
                        }

                        TCanvas * ca_can = m_runbd->TARGETSingle(party->cross_angle, party->cross_angle.GetNBins(),
                           party->cross_angle.GetBinning(), tmp_cuts);
                        ca_can->cd();
                        // m_runbd->GetHisto(party->cross_angle.GetName(), party->cross_angle.GetNBins(),
                        // party->cross_angle.GetBinning(), "", basecuts[br])->Draw();
                        //TODO: This is actually for checking that the plotter is working correctly.
                        PrintLogo(ca_can);
                        ca_can->Write();
                        delete ca_can;
                        // Purity of the crossing angle:
                        PurPart(party->cross_angle, tmp_cuts); 
                    }
                    if(m_verbose) cout << " -- Done" << endl;
                }   
                //**************************************** Crossing Angle END **************************************//

                if(DrawPlot(ProducePlots::FGDSegments)){
                //**************************************** FGD Segment START ************************************//
                // 1) Particle type FGD segment
                // 2) Tot. No. of segments (1,2,3)

                    if(m_verbose) cout << "FGD Segments";
                    MakeDir("FGDSegments");

                    Variable startfgd = party->fgd_start;
                    TH1D * startfgd_h = m_runbd->GetHisto(startfgd.GetName(), startfgd.GetNBins(), startfgd.GetBinning(), 
                        startfgd.GetSymbol(), basecuts[br]);
                    startfgd_h->SetFillColor( party->info.GetColor() );

                    TCanvas * startfgd_c = new TCanvas(party->fgd_start.GetName().c_str(), "", 400, 400);
                    startfgd_h->Draw();
                    PrintLogo(startfgd_c);
                    startfgd_c->Write();

                    if(m_verbose) cout << " -- Done" << endl;
                }
                //**************************************** FGD Segment END **************************************//

                if(DrawPlot(ProducePlots::PIDScore)){

                    string alev_s1 = "accum_level[0][1] > 4";
                        //**************************************** PID Score START ************************************//
                    MakeDir("LLScores/" + party->GetName());

                    Variable pid_scores[4] = { party->PIDScoreMu, party->PIDScoreEl, party->PIDScorePr, party->PIDScorePi };
                    std::vector<PDGInfo> pdg_list;
                    pdg_list.push_back( PDGInfo(13,     "muon",     "#mu^{#pm}",    true) );
                    pdg_list.push_back( PDGInfo(2212,   "proton",   "p",            true) );
                    pdg_list.push_back( PDGInfo(211,    "pion",     "#pi^{#pm}",    true) );
                    pdg_list.push_back( PDGInfo(11,     "electron", "e^{#pm}",      true) );

                    for(int cutline = 0; cutline < 1; cutline++){
                        for(int npid = 0; npid < 4; npid++){
                            Variable pid = pid_scores[npid];
                            pid.SetSName(pid.GetName() + (cutline == 0 ? "" : "_wCutLine"));
                            TCanvas * pid_can = m_runbd->PID(pid, party->pdg.GetName(), alev_s1, pdg_list);
                            if(cutline > 0){
                                pid_can->cd();
                                m_runbd->DrawCutLine(0.05, DrawingTools::Right);
                            }
                            pid_can->Write();
                            delete pid_can;
                            //Variable var, std::string pdgvar, std::string cuts, std::vector<PDGInfo> pdglist
                        }
                    }
                    pdg_list.clear();
                        //**************************************** PID Score END ************************************//

                    // selpi_fgd_pull_pi,
                    // selpi_fgd_pull_pr,
                    // selpi_fgd_pull_mu,


                }

                //**************************************** Start Position START ************************************//                
                if(m_verbose) cout << "Start Position";
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

                    string start_pos_cuts = GetPosCuts(party->startpos, dim1cut, dim2cut, GetPosCuts(party->truestartpos, dim1cut, dim2cut, basecuts[br]));
                    
                    // cout << start_pos_cuts << endl;

                    Variable start_pos(party->truestartpos.GetName() + "[" + dimss.str() + "]:" + party->startpos.GetName()  + "[" + dimss.str() + "]",
                        party->GetSymbol() + " " + m_NameXYZ[dim] + " Start Position", "mm");
                    // This probably wont work as the code looks for :: to make a split... Add fix.
                    start_pos.SetPDG(party->pdg.GetName());

                    if(DrawPlot(ProducePlots::StartPosition1D, ProducePlots::StartPositionAll)){
                        for(int cut_onoff = m_cut_onoff_low; cut_onoff < m_cut_onoff_high; cut_onoff++){  
                            MakeDir("StartPosition/" + party->GetName() );  
                            string tmp_cuts = start_pos_cuts;
                            start_pos.SetSName(party->startpos.GetName() + m_NameXYZ[dim] );
                            if(cut_onoff == 1){ 
                                tmp_cuts += " && 0.05 < ";
                                tmp_cuts += party->MyPID.GetName();
                                MakeDir("StartPosition/" + party->GetName() + "/PIDcut");
                                start_pos.SetSName( start_pos.GetSName() + "_PIDcut");
                            }
                            else if(cut_onoff == 2){ 
                                tmp_cuts += " && ";
                                tmp_cuts += m_experiment->GetTruePScuts();
                                MakeDir("StartPosition/" + party->GetName() + "/PScuts");
                                start_pos.SetSName( start_pos.GetSName() + "_PScuts");
                            }
                            else if(cut_onoff == 3){ 
                                tmp_cuts += " && 0.05 < ";
                                tmp_cuts += party->MyPID.GetName();
                                tmp_cuts += " && ";
                                tmp_cuts += m_experiment->GetTruePScuts();
                                start_pos.SetSName( start_pos.GetSName() + "_PIDPScut");
                                MakeDir("StartPosition/" + party->GetName() + "/PIDPScut");
                            }

                            PositionPlot(start_pos, dimnbins[dim], t2kgeometry::fgd1tpcmin_offset[dim], t2kgeometry::fgd1tpcmax_offset[dim],
                            tmp_cuts);
                        }
                    }

                    start_pos.SetSName(party->startpos.GetName() + m_NameXYZ[dim] );

                    //**************************************** 1D END ************************************//
                    if(DrawPlot(ProducePlots::StartPosition1DFGDSegs, ProducePlots::StartPositionAll)){
                        for(int fgdseg = 0; fgdseg < 2; fgdseg++){
                            stringstream fgdsegs;
                            fgdsegs << fgdseg;
                            string start_pos_fgdseg_cuts = start_pos_cuts + " && " + party->fgd_start.GetName() + " == " + fgdsegs.str();
                            Variable start_pos_fgdseg(start_pos);

                            for(int cut_onoff = m_cut_onoff_low; cut_onoff < m_cut_onoff_high; cut_onoff++){  
                                MakeDir("StartPosition/" + party->GetName() );  
                                string tmp_cuts = start_pos_fgdseg_cuts;
                                string tmp_sname = "";
                                if(cut_onoff == 1){ 
                                    tmp_cuts += " && 0.05 < ";
                                    tmp_cuts += party->MyPID.GetName();
                                    MakeDir("StartPosition/" + party->GetName() + "/PIDcut");
                                    tmp_sname = "_PIDcut";
                                }
                                else if(cut_onoff == 2){ 
                                    tmp_cuts += " && ";
                                    tmp_cuts += m_experiment->GetTruePScuts();
                                    tmp_sname = "_PScuts";
                                    MakeDir("StartPosition/" + party->GetName() + "/PScuts");
                                }
                                else if(cut_onoff == 3){ 
                                    tmp_cuts += " && 0.05 < ";
                                    tmp_cuts += party->MyPID.GetName();
                                    tmp_cuts += " && ";
                                    tmp_cuts += m_experiment->GetTruePScuts();
                                    tmp_sname = "_PIDPScuts";
                                    MakeDir("StartPosition/" + party->GetName() + "/PIDPScut");
                                }

                                start_pos_fgdseg.SetSName( start_pos_fgdseg.GetSName() + "_" + (fgdseg == 0 ? "nofgdseg" : "fgdseg")
                                    + tmp_sname );
                                PositionPlot(start_pos_fgdseg, dimnbins[dim], t2kgeometry::fgd1tpcmin_offset[dim], 
                                    t2kgeometry::fgd1tpcmax_offset[dim], tmp_cuts);
                            }  
                        }
                    }
                    //**************************************** 1D END ************************************//
    
                //**************************************** 1D END ************************************//
                
                //**************************************** 1D Purity START ************************************//

                    if(m_verbose) cout << " plots made, now producing purity dists." << endl;

                    // Variable start_pos_dim(party->startpos.GetName()  + "[" + dimss.str() + "]", dimnbins[dim], t2kgeometry::fgd1tpcmin_offset[dim], t2kgeometry::fgd1tpcmax_offset[dim], start_pos.GetSymbol(), "mm", start_pos.GetSName() + "_pur", party->pdg.GetName());
                    // PurPart: 
                    if(DrawPlot(ProducePlots::StartPositionPur1D, ProducePlots::StartPositionAll)){
                        for (int cut_onoff = m_cut_onoff_low; cut_onoff < m_cut_onoff_high; cut_onoff++){
                            string tmp_cuts = start_pos_cuts;
                            string tmp_sname = "";
                            if(cut_onoff == 1){ 
                                tmp_cuts += " && 0.05 < ";
                                tmp_cuts += party->MyPID.GetName();
                                tmp_sname = "_PIDcut";
                            }
                            else if(cut_onoff == 2){ 
                                tmp_cuts += " && ";
                                tmp_cuts += m_experiment->GetTruePScuts();
                                tmp_sname = "_PScuts";
                            }   
                            else if(cut_onoff == 3){ 
                                tmp_cuts += " && 0.05 < ";
                                tmp_cuts += party->MyPID.GetName();
                                tmp_cuts += " && ";
                                tmp_cuts += m_experiment->GetTruePScuts();
                                tmp_sname = "_PIDPScuts";
                            }

                            TH1D * start_pos_H_pur = m_runep->PurVSVar(party->startpos.GetName()  + "[" + dimss.str() + "]", dimnbins[dim],
                                t2kgeometry::fgd1tpcmin_offset[dim], t2kgeometry::fgd1tpcmax_offset[dim],
                                m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlusPS).GetSignal(), tmp_cuts,
                                start_pos.GetSymbol() + " (" + start_pos.GetUnits() + ")");

                            TH1D * start_pos_CH_pur = m_runep->PurVSVar(party->startpos.GetName()  + "[" + dimss.str() + "]", dimnbins[dim], 
                                t2kgeometry::fgd1tpcmin_offset[dim], t2kgeometry::fgd1tpcmax_offset[dim],
                                m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlus).GetSignal(), tmp_cuts,
                                start_pos.GetSymbol() + " (" + start_pos.GetUnits() + ")");

                            TCanvas * start_pos_pur_c = new TCanvas((start_pos.GetSName() + tmp_sname + "_pur").c_str(), "", 400, 400);
                            start_pos_pur_c->cd();

                            start_pos_H_pur->SetLineColor(DrawingStyle::Blue);
                            start_pos_H_pur->Draw();

                            start_pos_CH_pur->SetLineColor(DrawingStyle::Yellow);
                            start_pos_CH_pur->Draw("SAME");

                            TLegend * start_pos_pur_leg = m_runbd->Legend(0.15, 0.1);
                            start_pos_pur_leg->AddEntry(start_pos_H_pur, m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlusPS).GetSymbol().c_str(), "l" );
                            start_pos_pur_leg->AddEntry(start_pos_CH_pur, m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlus).GetSymbol().c_str(), "l" );
                            start_pos_pur_leg->Draw();

                            PrintLogo(start_pos_pur_c);

                            start_pos_pur_c->Write();

                            delete start_pos_H_pur;
                            delete start_pos_CH_pur;
                            delete start_pos_pur_leg;
                            delete start_pos_pur_c;
                        }
                    }
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
 
                        if(DrawPlot(ProducePlots::StartPosition2D, ProducePlots::StartPositionAll)){
                            for (int cut_onoff = m_cut_onoff_low; cut_onoff < m_cut_onoff_high; cut_onoff++){
                                string tmp_cuts = start_pos_cuts;
                                string tmp_sname = "";
                                if(cut_onoff == 1){ 
                                    tmp_cuts += " && 0.05 < ";
                                    tmp_cuts += party->MyPID.GetName();
                                    tmp_sname = "_PIDcut";
                                }
                                else if(cut_onoff == 2){ 
                                    tmp_cuts += " && ";
                                    tmp_cuts += m_experiment->GetTruePScuts();
                                    tmp_sname = "_PScuts";
                                }  
                                else if(cut_onoff == 3){ 
                                    tmp_cuts += " && 0.05 < ";
                                    tmp_cuts += party->MyPID.GetName();
                                    tmp_cuts += " && ";
                                    tmp_cuts += m_experiment->GetTruePScuts();
                                    tmp_sname = "_PIDPScuts";
                                }

                                TH2D * start_pos2D_h = m_runbd->GetHisto(start_pos2D.GetName(), dimnbins[dim], t2kgeometry::fgd1tpcmin_offset[dim], 
                                    t2kgeometry::fgd1tpcmax_offset[dim], dimnbins[dim2], t2kgeometry::fgd1tpcmin_offset[dim2], 
                                    t2kgeometry::fgd1tpcmax_offset[dim2], "Start Position " + m_NameXYZ[dim] + " (mm);Start Position " +
                                        m_NameXYZ[dim2] + " (mm)", tmp_cuts);

                                TCanvas * start_pos2D_c = new TCanvas( (start_pos2D.GetSName() + tmp_sname ).c_str(), "", 400, 400);
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

                            }
                        }

                        //**************************************** NFGD Segs Start ************************************//

                        if(DrawPlot(ProducePlots::StartPosition2DFGDSegs, ProducePlots::StartPositionAll)){
                            for(int fgdseg = 0; fgdseg < 2; fgdseg++){
                                stringstream fgdsegs;
                                fgdsegs << fgdseg;
                                string start_pos_fgdseg_cuts = start_pos_cuts + " && " + party->fgd_start.GetName() + " == " + fgdsegs.str();
                            // Variable start_pos_fgdseg(start_pos);
                            // start_pos_fgdseg.SetSName( start_pos_fgdseg.GetSName() + "_" + (fgdseg == 0 ? "nofgdseg" : "fgdseg") );

                                for (int cut_onoff = m_cut_onoff_low; cut_onoff < m_cut_onoff_high; cut_onoff++){
                                    string tmp_cuts = start_pos_fgdseg_cuts;
                                    string tmp_sname = "";
                                    if(cut_onoff == 1){ 
                                        tmp_cuts += " && 0.05 < ";
                                        tmp_cuts += party->MyPID.GetName();
                                        tmp_sname = "_PIDcut";
                                    }
                                    else if(cut_onoff == 2){ 
                                        tmp_cuts += " && ";
                                        tmp_cuts += m_experiment->GetTruePScuts();
                                        tmp_sname = "_PScuts";
                                    }
                                    else if(cut_onoff == 3){ 
                                        tmp_cuts += " && 0.05 < ";
                                        tmp_cuts += party->MyPID.GetName();
                                        tmp_cuts += " && ";
                                        tmp_cuts += m_experiment->GetTruePScuts();
                                        tmp_sname = "_PIDPScuts";
                                    }

                                    TH2D * start_pos2D_fgdseg_h = m_runbd->GetHisto(start_pos2D.GetName(), dimnbins[dim], 
                                        t2kgeometry::fgd1tpcmin_offset[dim], t2kgeometry::fgd1tpcmax_offset[dim], dimnbins[dim2], 
                                        t2kgeometry::fgd1tpcmin_offset[dim2], t2kgeometry::fgd1tpcmax_offset[dim2], "Start Position " +
                                        m_NameXYZ[dim] + " (mm);Start Position " + m_NameXYZ[dim2] + " (mm)", tmp_cuts);

                                    TCanvas * start_pos2D_fgdseg_c = new TCanvas( (start_pos2D.GetSName() + "_" +
                                        (fgdseg == 0 ? "nofgdseg" : "fgdseg") + tmp_sname ).c_str(), "", 400, 400);
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
                            }
                        }
                        //**************************************** NFGD Segs End ************************************//

                        //**************************************** 2D END ************************************//
                
                        //**************************************** 2D Purity Start ************************************//
                        if(DrawPlot(ProducePlots::StartPosition2DPur, ProducePlots::StartPositionAll)){

                            for (int cut_onoff = m_cut_onoff_low; cut_onoff < m_cut_onoff_high; cut_onoff++){
                                string tmp_cuts = start_pos_cuts;
                                string tmp_sname = "";
                                if(cut_onoff == 1){ 
                                    tmp_cuts += " && 0.05 < ";
                                    tmp_cuts += party->MyPID.GetName();
                                    tmp_sname = "_PIDcut";
                                }
                                else if(cut_onoff == 2){ 
                                    tmp_cuts += " && ";
                                    tmp_cuts += m_experiment->GetTruePScuts();
                                    tmp_sname = "_PScuts";
                                }
                                else if(cut_onoff == 3){ 
                                    tmp_cuts += " && 0.05 < ";
                                    tmp_cuts += party->MyPID.GetName();
                                    tmp_cuts += " && ";
                                    tmp_cuts += m_experiment->GetTruePScuts();
                                    tmp_sname = "_PIDPScuts";
                                }

                                TH2D * start_pos2D_pur_h = m_runep->PurVSVar(start_pos2D.GetName(), dimnbins[dim], 
                                    t2kgeometry::fgd1tpcmin_offset[dim], t2kgeometry::fgd1tpcmax_offset[dim], 
                                    dimnbins[dim2], t2kgeometry::fgd1tpcmin_offset[dim2], t2kgeometry::fgd1tpcmax_offset[dim2],
                                    m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlusPS).GetSignal(),
                                    tmp_cuts, "Start Position " + m_NameXYZ[dim] + " (mm);Start Position " + m_NameXYZ[dim2] + " (mm)");

                                TCanvas * start_pos2D_pur_c = new TCanvas( (start_pos2D.GetSName() + tmp_sname + "_pur").c_str(), "", 400, 400);
                                start_pos2D_pur_c->cd();
                                start_pos2D_pur_h->Draw("COLZ");
                                m_runbd->GetPOT(0.1,0.1)->Draw();
                                GetSignal()->Draw();

                                double fgd_box_low[2] = { t2kgeometry::fgd1min[dim], t2kgeometry::fgd1min[dim2] };
                                double fgd_box_hig[2] = { t2kgeometry::fgd1max[dim], t2kgeometry::fgd1max[dim2] };

                            // double tpc1_box_low[2] = { t2kgeometry::tpc1min[dim], t2kgeometry::tpc1min[dim2] };
                            // double tpc1_box_hig[2] = { t2kgeometry::tpc1max[dim], t2kgeometry::tpc1max[dim2] };

                                double tpc2_box_low[2] = { t2kgeometry::tpc2min[dim], t2kgeometry::tpc2min[dim2] };
                                double tpc2_box_hig[2] = { t2kgeometry::tpc2max[dim], t2kgeometry::tpc2max[dim2] };

                                m_runbd->DrawBox(fgd_box_low, fgd_box_hig);
                                m_runbd->DrawBox(tpc2_box_low, tpc2_box_hig, DrawingStyle::Yellow);

                                m_runbd->GetPOT(0.1,0.1)->Draw();

                                PrintLogo(start_pos2D_pur_c);

                                start_pos2D_pur_c->Write();

                                delete start_pos2D_pur_h;
                                delete start_pos2D_pur_c;
                            }
                        }
                        //**************************************** 2D Purity END ************************************//

                        //**************************************** 2D Efficiency Start ************************************//
                        if(DrawPlot(ProducePlots::StartPosition2DEff, ProducePlots::StartPositionAll)){
                            Variable truestart_pos2D(party->truth_startpos.GetName() + "[" + dim2ss.str() + "]:" + party->truth_startpos.GetName()  + "[" + dimss.str()+ "]",
                                party->GetSymbol() + " " + m_NameXYZ[dim] + m_NameXYZ[dim2] + " Start Position", "mm");
                                    // This probably wont work as the code looks for :: to make a split... Add fix.
                            start_pos2D.SetSName(party->startpos.GetName() + m_NameXYZ[dim] + m_NameXYZ[dim2] );
                            start_pos2D.SetPDG(party->pdg.GetName());

                            for (int cut_onoff = m_cut_onoff_low; cut_onoff < m_cut_onoff_high; cut_onoff++){
                                string truth_start_pos_cuts = GetPosCuts(party->truth_startpos, dim1cut, dim2cut, basecuts[br]);    
                                string tmp_sname = "";
                                if(cut_onoff == 1 || cut_onoff == 3) continue;
                                else if(cut_onoff == 2){ 
                                    truth_start_pos_cuts += " && ";
                                    truth_start_pos_cuts += m_experiment->GetTruePScuts();
                                    tmp_sname = "_PScuts";
                                }

                                // Can't add the PID cut here as we don't have the variable in the truth tree.... :(

                                TH2D * start_pos2D_eff_h = m_runep->EffVSVar(truestart_pos2D.GetName(), dimnbins[dim], t2kgeometry::fgd1tpcmin_offset[dim], 
                                    t2kgeometry::fgd1tpcmax_offset[dim], dimnbins[dim2], t2kgeometry::fgd1tpcmin_offset[dim2], t2kgeometry::fgd1tpcmax_offset[dim2],
                                    m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlusPS).GetSignal(),
                                    truth_start_pos_cuts, "Start Position " + m_NameXYZ[dim] + " (mm);Start Position " + m_NameXYZ[dim2] +" (mm)");

                                TCanvas * start_pos2D_eff_c = new TCanvas( (start_pos2D.GetSName() + tmp_sname + "_eff").c_str(), "", 400, 400);
                                start_pos2D_eff_c->cd();
                                start_pos2D_eff_h->Draw("COLZ");
                                m_runbd->GetPOT(0.1,0.1)->Draw();
                                GetSignal()->Draw();

                                double fgd_box_low[2] = { t2kgeometry::fgd1min[dim], t2kgeometry::fgd1min[dim2] };
                                double fgd_box_hig[2] = { t2kgeometry::fgd1max[dim], t2kgeometry::fgd1max[dim2] };

                                // double tpc1_box_low[2] = { t2kgeometry::tpc1min[dim], t2kgeometry::tpc1min[dim2] };
                                // double tpc1_box_hig[2] = { t2kgeometry::tpc1max[dim], t2kgeometry::tpc1max[dim2] };

                                double tpc2_box_low[2] = { t2kgeometry::tpc2min[dim], t2kgeometry::tpc2min[dim2] };
                                double tpc2_box_hig[2] = { t2kgeometry::tpc2max[dim], t2kgeometry::tpc2max[dim2] };

                                m_runbd->DrawBox(fgd_box_low, fgd_box_hig);
                                m_runbd->DrawBox(tpc2_box_low, tpc2_box_hig, DrawingStyle::Yellow);

                                m_runbd->GetPOT(0.1,0.1)->Draw();

                                PrintLogo(start_pos2D_eff_c);

                                start_pos2D_eff_c->Write();

                                delete start_pos2D_eff_h;
                                delete start_pos2D_eff_c;
                            }
                        //**************************************** 2D Efficiency END ************************************//
                        }
                        //**************************************** Start Position END ************************************//
                    }
                }
            }

        }

        Variable dpTT(m_recovars->truedpTT + ":" + m_recovars->dpTT, "#delta#it{p}_{TT}", "MeV/#it{c}");
        dpTT.SetSName(m_recovars->dpTT);

        if(DrawPlot(ProducePlots::DPTT)){
            if(m_verbose) cout << "Now producing dpTT plots." << endl;
            //************************************** dpTT Start *************************************//            
            for (int cut_onoff = m_cut_onoff_low; cut_onoff < m_cut_onoff_high; cut_onoff++){
                MakeDir("dpTT" + branchnames[br]);
                string tmp_cuts = basecuts[br];
                string tmp_sname = m_recovars->dpTT;
                if(cut_onoff == 1){ 
                    tmp_cuts += " && 0.05 < ";
                    tmp_cuts += m_muon->MyPID.GetName();
                    tmp_cuts += " && 0.05 < ";
                    tmp_cuts += m_pion->MyPID.GetName();
                    tmp_cuts += " && 0.05 < ";
                    tmp_cuts += m_proton->MyPID.GetName();
                    tmp_sname += "_PIDcut";
                }
                else if(cut_onoff == 2){ 
                    tmp_cuts += " && ";
                    tmp_cuts += m_experiment->GetTruePScuts();
                    tmp_sname += "_PScuts";
                }
                else if(cut_onoff == 3){ 
                    tmp_cuts += " && 0.05 < ";
                    tmp_cuts += m_muon->MyPID.GetName();
                    tmp_cuts += " && 0.05 < ";
                    tmp_cuts += m_pion->MyPID.GetName();
                    tmp_cuts += " && 0.05 < ";
                    tmp_cuts += m_proton->MyPID.GetName();
                    tmp_cuts += " && ";
                    tmp_cuts += m_experiment->GetTruePScuts();
                    tmp_sname += "_PIDPScuts";
                }

                // cout << "tmp_sname = " << tmp_sname << endl;

                // cout << "1) dpTT.GetSName() = " << dpTT.GetSName() << endl;

                dpTT.SetSName(tmp_sname + "_nb39");

                ProduceGroup(dpTT, 39, -300, 300, basecuts[br]);

                TCanvas * dpTT_pur = new TCanvas( (dpTT.GetSName() + "_pur").c_str(), "", 400, 400);
                dpTT_pur->cd();
                m_runep->PurVSVar(m_recovars->dpTT, 39, -300., 300., m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlusPS).GetSignal(),
                    tmp_cuts, dpTT.GetAxisTitle())->Draw("HIST");
                dpTT_pur->Write();
                delete dpTT_pur;

                MakeDir("dpTT" + branchnames[br] + "/nb29");
                dpTT.SetSName(tmp_sname + "_nb29");
                ProduceGroup(dpTT, 29, -300, 300, tmp_cuts);

                // cout << "2) dpTT.GetSName() = " << dpTT.GetSName() << endl;

                dpTT_pur = new TCanvas( (dpTT.GetSName() + "_pur").c_str(), "", 400, 400);
                dpTT_pur->cd();
                m_runep->PurVSVar(m_recovars->dpTT, 29, -300., 300., m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlusPS).GetSignal(), 
                    tmp_cuts, dpTT.GetAxisTitle())->Draw("HIST");
                dpTT_pur->Write();
                delete dpTT_pur;

                MakeDir("dpTT" + branchnames[br] + "/nb25");
                dpTT.SetSName(tmp_sname + "_nb25");
                ProduceGroup(dpTT, 25, -300, 300, tmp_cuts);

                dpTT_pur = new TCanvas( (tmp_sname + "_pur").c_str(), "", 400, 400);
                dpTT_pur->cd();
                m_runep->PurVSVar(m_recovars->dpTT, 25, -300., 300., m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlusPS).GetSignal(), 
                    tmp_cuts, dpTT.GetAxisTitle())->Draw("HIST");
                dpTT_pur->Write();
                delete dpTT_pur;

                MakeDir("dpTT" + branchnames[br] + "/nb19");
                dpTT.SetSName(tmp_sname + "_nb19");
                ProduceGroup(dpTT, 19, -300, 300, tmp_cuts);

                // cout << "4) dpTT.GetSName() = " << dpTT.GetSName() << endl;

                dpTT_pur = new TCanvas( (tmp_sname + "_pur").c_str(), "", 400, 400);
                dpTT_pur->cd();
                m_runep->PurVSVar(m_recovars->dpTT, 19, -300., 300., m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlusPS).GetSignal(), 
                    tmp_cuts, dpTT.GetAxisTitle())->Draw("HIST");
                dpTT_pur->Write();
                delete dpTT_pur;
            }
        }

        if(m_experiment->GetType() == Experiment::MIN && DrawPlot(ProducePlots::DPTT)){
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
            if(m_verbose) cout << "Making T2K specific plots" << endl;

            if(DrawPlot(ProducePlots::DPTTFGDSegs)){
                MakeDir("dpTT" + branchnames[br] + "/NFGDSegments" );
                // cdDir("dpTT" + branchnames[br]);   
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

                    for (int cut_onoff = m_cut_onoff_low; cut_onoff < m_cut_onoff_high; cut_onoff++){
                        string tmp_cuts = basecuts[br];
                        string tmp_sname = "";
                        if(cut_onoff == 1){ 
                            tmp_cuts += " && 0.05 < ";
                            tmp_cuts += m_muon->MyPID.GetName();
                            tmp_cuts += " && 0.05 < ";
                            tmp_cuts += m_pion->MyPID.GetName();
                            tmp_cuts += " && 0.05 < ";
                            tmp_cuts += m_proton->MyPID.GetName();
                            tmp_sname += "_PIDcut";
                        }
                        else if(cut_onoff == 2){ 
                            tmp_cuts += " && ";
                            tmp_cuts += m_experiment->GetTruePScuts();
                            tmp_sname += "_PScuts";
                        }
                        else if(cut_onoff == 3){ 
                            tmp_cuts += " && 0.05 < ";
                            tmp_cuts += m_muon->MyPID.GetName();
                            tmp_cuts += " && 0.05 < ";
                            tmp_cuts += m_pion->MyPID.GetName();
                            tmp_cuts += " && 0.05 < ";
                            tmp_cuts += m_proton->MyPID.GetName();
                            tmp_cuts += " && ";
                            tmp_cuts += m_experiment->GetTruePScuts();
                            tmp_sname += "_PIDPScuts";
                        }
                        dpTT.SetSName(m_recovars->dpTT + "_" + fgdsegs.str() + "FGDSeg" + tmp_sname);
                        ProduceGroup(dpTT, 29, -300, 300, start_pos_fgdseg_cuts);
                    }
                }
            }

            string segcuts = m_muon->fgd_start.GetName() + "!= -999 && " + m_proton->fgd_start.GetName() + " != -999 && ";
            segcuts += m_pion->fgd_start.GetName();
            segcuts += " != -999";

            Variable nfgdsegments(m_muon->fgd_start.GetName() + " + " + m_proton->fgd_start.GetName() + " + " + m_pion->fgd_start.GetName(),"","");

            //************************************** No. FGD Segments Start *************************************//
            if(DrawPlot(ProducePlots::NFGDSegments)){
                MakeDir("FGDSegments");
                
                TH1D * nfgdsegments_h = m_runbd->GetHisto(nfgdsegments.GetName(), 3, 1., 4., "N tracks with FGD Segments", basecuts[br] + " && " + segcuts);
                for(int nloop = 0; nloop < nfgdsegments_h->GetNbinsX(); nloop++) nfgdsegments_h->GetXaxis()->SetBinLabel(nloop+1, Form("%d", nloop+1) );
                    nfgdsegments_h->SetFillColor(DrawingStyle::Yellow);

                TCanvas * nfgdsegments_c = new TCanvas("NFGDSegments", "", 400, 400);
                nfgdsegments_h->Draw();
                PrintLogo(nfgdsegments_c);
                nfgdsegments_c->Write();
            }
        //************************************** No. FGD Segments End *************************************//

            string alev_s1 = "accum_level[0][1]";
            string alev_s2 = "accum_level[0][3]";

            if(DrawPlot(ProducePlots::NTracks)){

            //                     s1      s2
            //  "Event Quality"         0
            //  "Contains Tracks"       1
            //  "Vertex ID"             2
            //  "3 Tracks"              3
            //  "N TPC Tracks"          4      
            //  "FGD"                       5
            //  "Track Charges"     5       6
            //  "Common Vertex"     6       7

            // SetBranchAlias(1, (m_detName + "-3TPC-" + MECUT + "MEcut").c_str(), 0, 1);
            // SetBranchAlias(3, (m_detName + "-2TPC-pi" + m_detName + "-" + MECUT + "MEcut").c_str(), 1, 1);

                // We want to make some cuts here:
                // In the plots we want to see how much CC1P1Pi+ (ignoring if it's on H):
                // So want to select the true components:
                // true_ntracks == 3 && truemu_ntracks == 1 && truep_ntracks == 1 && truepi_ntracks == 1
                string tSig = "true_ntracks == 3 && truemu_ntracks == 1 && truep_ntracks == 1 && truepi_ntracks == 1";
                string tBkg = "!(" + tSig + ")";

                string tBkg1 = "true_ntracks > 3 && truemu_ntracks == 1 && truep_ntracks > 0 && truepi_ntracks == 1";
                string tBkg2 = "true_ntracks > 3 && truemu_ntracks == 1 && truep_ntracks == 1 && truepi_ntracks > 0";
                string tBkg3 = tBkg;//"!(" + tSig + ") && " + "!(" + tBkg1 + ") && " + "!(" + tBkg2 + ")";

                // string tBkg3 = "true_ntracks > 3 && truemu_ntracks == 1 && truep_ntracks > 0 && truepi_ntracks > 0";
                // string tBkg4 = "!(" + tSig + ") && " + "!(" + tBkg1 + ") && " + "!(" + tBkg2 + ") && !(" + tBkg3 + ")";                
                // tBkg3 = "(" + tBkg3 + ") && !(" + tSig + ") && !(" + tBkg1 + ") && !(" + tBkg2 + ")";

                if(m_verbose) cout << "Producing NTPC Tracks" << endl;
                MakeDir("NTracks");
                // GetHisto(std::string var, Int_t nbins, Double_t low, Double_t high, std::string xy_title, std::string cuts)
                // TH1D * ntracks_h = m_runbd->GetHisto(m_recovars->ntracks.GetName(), 21,0,21, m_recovars->ntracks.GetAxisTitle());
                // ntracks_h->SetLineColor(DrawingStyle::Blue);
                //TH1D * GetHisto(std::string var, Int_t nbins, Double_t low, Double_t high, std::string xy_title = "", std::string cuts = "");
                // std::string tmp_ntrack = m_recovars->ntracks.GetName(); 
                // TH1D * ntracks_h2 = m_runbd->GetHisto( "(" + tmp_ntrack + " > 20 ? 21 : " + tmp_ntrack + ")", 21,0,21, 
                //     m_recovars->ntracks.GetAxisTitle());
                // ntracks_h2->SetLineColor(DrawingStyle::Yellow);

                TH1D * ntrks_sig = m_runbd->GetHisto(m_recovars->ntracks.GetName(), 21,0,21, m_recovars->ntracks.GetAxisTitle(),    (tSig + " && " + alev_s1 + " > 3"));
                ntrks_sig->SetFillColor(DrawingStyle::Yellow);
                ntrks_sig->SetLineColor(kBlack);
                ntrks_sig->SetLineWidth(2);

                // TH1D * ntrks_bkg1 = m_runbd->GetHisto(m_recovars->ntracks.GetName(), 21,0,21, m_recovars->ntracks.GetAxisTitle(),   (tBkg1 + " && " + alev_s1 + " > 3"));
                // ntrks_bkg1->SetFillColor(DrawingStyle::Red);
                // ntrks_bkg1->SetLineColor(kBlack);
                // ntrks_bkg1->SetLineWidth(2);

                // TH1D * ntrks_bkg2 = m_runbd->GetHisto(m_recovars->ntracks.GetName(), 21,0,21, m_recovars->ntracks.GetAxisTitle(),   (tBkg2 + " && " + alev_s1 + " > 3"));
                // ntrks_bkg2->SetFillColor(DrawingStyle::Green);
                // ntrks_bkg2->SetLineColor(kBlack);
                // ntrks_bkg2->SetLineWidth(2);

                TH1D * ntrks_bkg3 = m_runbd->GetHisto(m_recovars->ntracks.GetName(), 21,0,21, m_recovars->ntracks.GetAxisTitle(),   (tBkg3 + " && " + alev_s1 + " > 3"));
                // ntrks_bkg3->SetFillColor(DrawingStyle::Other);
                ntrks_bkg3->SetFillColor(DrawingStyle::Blue);
                ntrks_bkg3->SetLineColor(kBlack);
                ntrks_bkg3->SetLineWidth(2);

                TH1D * ntrks_check = m_runbd->GetHisto(m_recovars->ntracks.GetName(), 21,0,21, m_recovars->ntracks.GetAxisTitle(),  (alev_s1 + " > 3"));
                // ntrks_bkg3->SetFillColor(DrawingStyle::Other);
                ntrks_check->SetLineColor(kRed);
                ntrks_check->SetLineWidth(2);

                //TH1D * GetHisto(std::string var, Int_t nbins, Double_t low, Double_t high, std::string xy_title = "", std::string cuts = "");
                // std::string tmp_ntrack = m_recovars->ntracks.GetName(); 
                // TH1D * no_tracks_h2 = m_runbd->GetHisto( "(" + tmp_ntrack + " > 20 ? 21 : " + tmp_ntrack + ")", 21,0,21, 
                //     m_recovars->ntracks.GetAxisTitle());
                // no_tracks_h2->SetLineColor(DrawingStyle::Yellow);

                THStack * ntks = new THStack("NTracks", (";" + m_recovars->ntracks.GetAxisTitle() + ";Counts").c_str());
                ntks->Add(ntrks_bkg3);
                // ntks->Add(ntrks_bkg2);
                // ntks->Add(ntrks_bkg1);
                ntks->Add(ntrks_sig);

                TLegend * ntks_leg = m_runbd->Legend(0.15,0.1);
                // ntks_leg->AddEntry(ntrks_sig, "CC1p1#pi^{+}", "f");
                ntks_leg->AddEntry(ntrks_sig, "Signal (CC1p1#pi^{+})", "f");
                // ntks_leg->AddEntry(ntrks_bkg1, "CCNp1#pi^{+} (N > 0)", "f");
                // ntks_leg->AddEntry(ntrks_bkg2, "CC1pM#pi^{+} (M > 0)", "f");
                ntks_leg->AddEntry(ntrks_bkg3, "Background", "f");
                // ntks_leg->AddEntry(ntrks_bkg3, "Other", "f");

                TCanvas * ntracks_can = new TCanvas("NTracks","", 400, 400);
                ntracks_can->cd();
                ntks->Draw();
                // ntrks_check->Draw("SAME");
                ntks_leg->Draw();
                PrintLogo(ntracks_can);

                ntracks_can->Write();
                // ----------------------------------------------------------------------------------------------------------------------------
                // ----------------------------------------------------------------------------------------------------------------------------
                // ----------------------------------------------------------------------------------------------------------------------------

                if(m_verbose) cout << "Producing NTPC Tracks" << endl;
                MakeDir("NTPCTracks");
                TH1D * tpctrks_sig = m_runbd->GetHisto(m_recovars->ntpctracks.GetName(), 21,0,21, m_recovars->ntpctracks.GetAxisTitle(),    (tSig + " && " + alev_s1 + " > 0"));
                tpctrks_sig->SetFillColor(DrawingStyle::Yellow);
                tpctrks_sig->SetLineColor(kBlack);
                tpctrks_sig->SetLineWidth(2);

                // TH1D * tpctrks_bkg1 = m_runbd->GetHisto(m_recovars->ntpctracks.GetName(), 21,0,21, m_recovars->ntpctracks.GetAxisTitle(),   (tBkg1 + " && " + alev_s1 + " > 0"));
                // tpctrks_bkg1->SetFillColor(DrawingStyle::Red);
                // tpctrks_bkg1->SetLineColor(kBlack);
                // tpctrks_bkg1->SetLineWidth(2);

                // TH1D * tpctrks_bkg2 = m_runbd->GetHisto(m_recovars->ntpctracks.GetName(), 21,0,21, m_recovars->ntpctracks.GetAxisTitle(),   (tBkg2 + " && " + alev_s1 + " > 0"));
                // tpctrks_bkg2->SetFillColor(DrawingStyle::Green);
                // tpctrks_bkg2->SetLineColor(kBlack);
                // tpctrks_bkg2->SetLineWidth(2);

                TH1D * tpctrks_bkg3 = m_runbd->GetHisto(m_recovars->ntpctracks.GetName(), 21,0,21, m_recovars->ntpctracks.GetAxisTitle(),   (tBkg3 + " && " + alev_s1 + " > 0"));
                // tpctrks_bkg3->SetFillColor(DrawingStyle::Other);
                tpctrks_bkg3->SetFillColor(DrawingStyle::Blue);
                tpctrks_bkg3->SetLineColor(kBlack);
                tpctrks_bkg3->SetLineWidth(2);

                TH1D * tpctrks_check = m_runbd->GetHisto(m_recovars->ntpctracks.GetName(), 21,0,21, m_recovars->ntpctracks.GetAxisTitle(),  (alev_s1 + " > 0"));
                // tpctrks_bkg3->SetFillColor(DrawingStyle::Other);
                tpctrks_check->SetLineColor(kRed);
                tpctrks_check->SetLineWidth(2);

                //TH1D * GetHisto(std::string var, Int_t nbins, Double_t low, Double_t high, std::string xy_title = "", std::string cuts = "");
                // std::string tmp_ntrack = m_recovars->ntpctracks.GetName(); 
                // TH1D * no_tracks_h2 = m_runbd->GetHisto( "(" + tmp_ntrack + " > 20 ? 21 : " + tmp_ntrack + ")", 21,0,21, 
                //     m_recovars->ntracks.GetAxisTitle());
                // no_tracks_h2->SetLineColor(DrawingStyle::Yellow);

                THStack * tpctks = new THStack("NTPCTracks", (";" + m_recovars->ntpctracks.GetAxisTitle() + ";Counts").c_str());
                tpctks->Add(tpctrks_bkg3);
                // tpctks->Add(tpctrks_bkg2);
                // tpctks->Add(tpctrks_bkg1);
                tpctks->Add(tpctrks_sig);

                TLegend * tpctks_leg = m_runbd->Legend(0.15,0.1);
                // tpctks_leg->AddEntry(tpctrks_sig, "CC1p1#pi^{+}", "f");
                tpctks_leg->AddEntry(tpctrks_sig, "Signal (CC1p1#pi^{+})", "f");
                // tpctks_leg->AddEntry(tpctrks_bkg1, "CCNp1#pi^{+} (N > 0)", "f");
                // tpctks_leg->AddEntry(tpctrks_bkg2, "CC1pM#pi^{+} (M > 0)", "f");
                tpctks_leg->AddEntry(tpctrks_bkg3, "Background", "f");
                // tpctks_leg->AddEntry(tpctrks_bkg3, "Other", "f");

                TCanvas * ntpctracks_can = new TCanvas("NTPCTracks","", 400, 400);
                ntpctracks_can->cd();
                tpctks->Draw();
                // tpctrks_check->Draw("SAME");
                tpctks_leg->Draw();
                PrintLogo(ntpctracks_can);
                // no_tracks_h2->Draw("SAME");

                ntpctracks_can->Write();

                // ----------------------------------------------------------------------------------------------------------------------------
                // ----------------------------------------------------------------------------------------------------------------------------
                // ----------------------------------------------------------------------------------------------------------------------------
                // ----------------------------------------------------------------------------------------------------------------------------

                MakeDir("NTPCTracksCharges");

                double tpc3pos[4]; //{ 3+,  2+,1- (Signal),  }
                const char * tpc3pos_name[4] = { "3 +ve", "2 +ve, 1 -ve", "1 +ve, 2 -ve", "3 -ve" };

                for(int i = 0; i < 4; i++){
                    string acut = alev_s1 + " > 2 && 3 == " + m_recovars->ntpctracks.GetName() + " && ";
                    acut += Form("ntpcposQualityFV == %i && ntpcnegQualityFV == %i", 3 - i, i);
                    // string acut_neg = Form("ntpcnegQualityFV == %i", i);
                    tpc3pos[i] = m_runbd->GetHisto("NTOYS", 1,0.,2., "", acut)->Integral();
                }

                double tpc2pos[3];
                const char * tpc2pos_name[3] = { "2 +ve", "1 +ve, 1 -ve", "2 -ve" };
                for(int i = 0; i < 3; i++){
                    string acut = alev_s2 + " > 2 && 2 == " + m_recovars->ntpctracks.GetName() + " && ";
                    acut += Form("ntpcposQualityFV == %i && ntpcnegQualityFV == %i", 2 - i, i);
                    // string acut_neg = Form("ntpcnegQualityFV == %i", i);
                    tpc2pos[i] = m_runbd->GetHisto("NTOYS", 1,0.,2., "", acut)->Integral();
                }

                int colours[4] = { DrawingStyle::Blue, DrawingStyle::Yellow, DrawingStyle::Red, DrawingStyle::Gray };

                TPie * tpc3ch = new TPie("tpc3ch", "Three TPC Tracks", 4, tpc3pos, colours, tpc3pos_name);
                TPie * tpc2ch = new TPie("tpc2ch", "FGD Contained", 3, tpc2pos, colours, tpc2pos_name);

                TCanvas * tpc3chtracks_can = new TCanvas("N3TPCTracksCharges","", 400, 400);
                // tpc3chtracks_can->Divide(2);
                tpc3chtracks_can->cd();
                tpc3ch->Draw("3d");
                tpc3ch->SetEntryRadiusOffset(1, 0.1);

                TCanvas * tpc2chtracks_can = new TCanvas("N2TPCTracksCharges","", 400, 400);
                tpc2chtracks_can->cd(2);
                tpc2ch->Draw("3d");
                tpc2ch->SetEntryRadiusOffset(1, 0.1);
                // no_tracks_h2->Draw("SAME");

                tpc3chtracks_can->Write();
                tpc2chtracks_can->Write();
                // ----------------------------------------------------------------------------------------------------------------------------
                // ----------------------------------------------------------------------------------------------------------------------------
                // ----------------------------------------------------------------------------------------------------------------------------

            }

            if(DrawPlot(ProducePlots::VtxPosition2D)){

                MakeDir("VtxPosition");
                int dimnbins[3] = {40, 40, 40};
                
                string vtx_pos_cuts = alev_s1 + " > 1";

                for(int dim1 = 0; dim1 < 2; dim1++){
                    stringstream dim1ss; 
                    dim1ss << dim1;
                    for(int dim2 = dim1 + 1; dim2 < 3; dim2++){
                        stringstream dim2ss; 
                        dim2ss << dim2;

                        // 01, 02, 12, 
                        string vtx_pos2D = m_recovars->vtx_pos.GetName() + "[" + dim2ss.str() + "]:" + m_recovars->vtx_pos.GetName() + "[" + dim1ss.str() + "]";

                        TH2D * vtx_pos2D_h = m_runbd->GetHisto(vtx_pos2D, dimnbins[dim1], t2kgeometry::fgd1min_offset[dim1], 
                            t2kgeometry::fgd1max_offset[dim1], dimnbins[dim2], t2kgeometry::fgd1min_offset[dim2], 
                            t2kgeometry::fgd1max_offset[dim2], "Vertex Position " + m_NameXYZ[dim1] + " (mm);Vertex Position " + m_NameXYZ[dim2] + " (mm)", vtx_pos_cuts);

                    // DrawLine(double x_low, double y_low, double x_high, double y_high, int color = kGray + 2, int style = 1, int width = 2);

                        TCanvas * vtx_pos2D_c = new TCanvas((m_recovars->vtx_pos.GetSName() + "_" + m_NameXYZ[dim1] + m_NameXYZ[dim2]).c_str(), "", 400, 400);
                        vtx_pos2D_c->cd();
                        vtx_pos2D_h->Draw("COLZ");
                        double box_low[2] = { t2kgeometry::fgd1min[dim1], t2kgeometry::fgd1min[dim2] };
                        double box_hig[2] = { t2kgeometry::fgd1max[dim1], t2kgeometry::fgd1max[dim2] };
                        m_runbd->DrawBox(box_low, box_hig);
                        m_runbd->GetPOT(0.1,0.1)->Draw();
                        PrintLogo(vtx_pos2D_c);

                        vtx_pos2D_c->Write();

                        delete vtx_pos2D_h;
                        delete vtx_pos2D_c;

                    }
                }
            }

            if(DrawPlot(ProducePlots::PIDScore)){
                string alev_s1 = "accum_level[0][1] > 4";
                                    //**************************************** PID Score START ************************************//
                MakeDir("PullScores");

                Variable pid_scores[3];
                int pid_nbins = 30;
                double pid_low = -15.;
                double pid_high = 15.;

                // std::string name, int nbins, Double_t low, Double_t high, std::string symbol = "", std::string units = "", std::string savename = ""
                pid_scores[0] = Variable("selpi_fgd_pull_pi", pid_nbins, pid_low, pid_high, "Pion Pull", "");
                pid_scores[1] = Variable("selpi_fgd_pull_pr", pid_nbins, pid_low, pid_high, "Proton Pull", "");
                pid_scores[2] = Variable("selpi_fgd_pull_mu", pid_nbins, pid_low, pid_high, "Muon Pull", "");

                std::vector<PDGInfo> pdg_list;
                pdg_list.push_back( PDGInfo(13,     "muon",     "#mu^{#pm}",    true) );
                pdg_list.push_back( PDGInfo(2212,   "proton",   "p",            true) );
                pdg_list.push_back( PDGInfo(211,    "pion",     "#pi^{#pm}",    true) );
                // pdg_list.push_back( PDGInfo(11,     "electron", "e^{#pm}",      true) );

                for(int cutline = 0; cutline < 1; cutline++){
                    for(int npid = 0; npid < 3; npid++){
                        Variable pid = pid_scores[npid];
                        pid.SetSName(pid.GetName() + (cutline == 0 ? "" : "_wCutLine"));
                        TCanvas * pid_can = m_runbd->PID(pid, m_pion->pdg.GetName(), alev_s1, pdg_list);
                        if(cutline > 0){
                            pid_can->cd();
                            m_runbd->DrawCutLine(0.05, DrawingTools::Right);
                        }
                        pid_can->Write();
                        delete pid_can;
                                        //Variable var, std::string pdgvar, std::string cuts, std::vector<PDGInfo> pdglist
                    }
                }
                pdg_list.clear();
                                    //**************************************** PID Score END ************************************//
            }

            //**************************************** Vertex Position START ************************************//
            // MakeDir("VtxPosition");
            // int dimnbins[3] = {40, 40, 40};
            //     // if(which_fgd){
                // }
            // for(int dim = 0; dim < 3; dim++){
            //     stringstream dimss; 
            //     dimss << dim;

            //     int dim1cut, dim2cut;
            //     string dim1cuts, dim2cuts;

            //     if(dim == 0){
            //         dim1cuts = "1";
            //         dim2cuts = "2";
            //         dim1cut = 1;
            //         dim2cut = 2;
            //     }
            //     else if(dim == 1){
            //         dim1cuts = "0";
            //         dim2cuts = "2";
            //         dim1cut = 0;
            //         dim2cut = 2; 
            //     }
            //     else{
            //         dim1cuts = "0";
            //         dim2cuts = "1";
            //         dim1cut = 0;
            //         dim2cut = 1; 
            //     }

            //     stringstream dim1th_low, dim2th_low, dim1th_high, dim2th_high;                    
            //     dim1th_low << t2kgeometry::fgd1min_offset[dim1cut];
            //     dim1th_high << t2kgeometry::fgd1max_offset[dim1cut];

            //     dim2th_low << t2kgeometry::fgd1min_offset[dim2cut];
            //     dim2th_high << t2kgeometry::fgd1max_offset[dim2cut];

            //     string vtx_pos_cuts = basecuts[br];
            //         //1st Dim : True vars:
            //     vtx_pos_cuts += " && ";
            //         //1st Dim : True vars min:
            //     vtx_pos_cuts += dim1th_low.str();
            //     vtx_pos_cuts += " < ";
            //     vtx_pos_cuts += m_recovars->truevtx_pos.GetName();
            //     vtx_pos_cuts += "[";
            //     vtx_pos_cuts += dim1cuts;
            //     vtx_pos_cuts += "]";
            //     vtx_pos_cuts += " && ";
            //         //1st Dim : True vars min:
            //     vtx_pos_cuts += m_recovars->truevtx_pos.GetName();
            //     vtx_pos_cuts += "[";
            //     vtx_pos_cuts += dim1cuts;
            //     vtx_pos_cuts += "]";
            //     vtx_pos_cuts += " < ";
            //     vtx_pos_cuts += dim1th_high.str();

            //         //1st Dim : Reco vars:
            //     vtx_pos_cuts += " && ";
            //         //1st Dim : Reco vars min:
            //     vtx_pos_cuts += dim1th_low.str();
            //     vtx_pos_cuts += " < ";
            //     vtx_pos_cuts += m_recovars->vtx_pos.GetName();
            //     vtx_pos_cuts += "[";
            //     vtx_pos_cuts += dim1cuts;
            //     vtx_pos_cuts += "]";
            //     vtx_pos_cuts += " && ";
            //         //1st Dim : Reco vars min:
            //     vtx_pos_cuts += m_recovars->vtx_pos.GetName();
            //     vtx_pos_cuts += "[";
            //     vtx_pos_cuts += dim1cuts;
            //     vtx_pos_cuts += "]";
            //     vtx_pos_cuts += " < ";
            //     vtx_pos_cuts += dim1th_high.str();

            //         //2nd Dim : True vars:
            //     vtx_pos_cuts += " && ";
            //         //2nd Dim : True vars min:
            //     vtx_pos_cuts += dim2th_low.str();
            //     vtx_pos_cuts += " < ";
            //     vtx_pos_cuts += m_recovars->truevtx_pos.GetName();
            //     vtx_pos_cuts += "[";
            //     vtx_pos_cuts += dim2cuts;
            //     vtx_pos_cuts += "]";
            //     vtx_pos_cuts += " && ";
            //         //2nd Dim : True vars min:
            //     vtx_pos_cuts += m_recovars->truevtx_pos.GetName();
            //     vtx_pos_cuts += "[";
            //     vtx_pos_cuts += dim2cuts;
            //     vtx_pos_cuts += "]";
            //     vtx_pos_cuts += " < ";
            //     vtx_pos_cuts += dim2th_high.str();

            //         //2nd Dim : Reco vars:
            //     vtx_pos_cuts += " && ";
            //         //2nd Dim : Reco vars min:
            //     vtx_pos_cuts += dim2th_low.str();
            //     vtx_pos_cuts += " < ";
            //     vtx_pos_cuts += m_recovars->vtx_pos.GetName();
            //     vtx_pos_cuts += "[";
            //     vtx_pos_cuts += dim2cuts;
            //     vtx_pos_cuts += "]";
            //     vtx_pos_cuts += " && ";
            //         //2nd Dim : Reco vars min:
            //     vtx_pos_cuts += m_recovars->vtx_pos.GetName();
            //     vtx_pos_cuts += "[";
            //     vtx_pos_cuts += dim2cuts;
            //     vtx_pos_cuts += "]";
            //     vtx_pos_cuts += " < ";
            //     vtx_pos_cuts += dim2th_high.str();

            //     Variable vtx_pos(m_recovars->truevtx_pos.GetName() + "[" + dimss.str() + "]:" + m_recovars->vtx_pos.GetName() + "[" + dimss.str() + "]", m_recovars->vtx_pos.GetSymbol(), m_recovars->vtx_pos.GetUnits());
            //         // This probably wont work as the code looks for :: to make a split... Add fix.
            //     vtx_pos.SetSName(m_recovars->vtx_pos.GetName() + m_NameXYZ[dim] );
            //     // start_pos.SetPDG(part->pdg.GetName());

            //     if(DrawPlot(ProducePlots::VtxPosition1D, ProducePlots::VtxPosition)){
            //         //**************************************** 1D START ************************************//
            //         ProduceGroup(vtx_pos, dimnbins[dim], t2kgeometry::fgd1min_offset[dim], t2kgeometry::fgd1max_offset[dim], vtx_pos_cuts);//TODO: Add detector lines to this plot.
            //         //**************************************** 1D END ************************************//
            //     }
            //     //**************************************** 1D NFGD Segs VTX Pos START ************************************//

            //     if(DrawPlot(ProducePlots::VtxPosition1DFGDSegs, ProducePlots::VtxPosition)){
            //     //Vertex Position for NFGD Segments: 1, 2, 3:
            //         for(int nsegs = 1; nsegs < 4; nsegs++){
            //             stringstream snsegs; 
            //             snsegs << nsegs;
            //             string seg_vtx_cut = nfgdsegments.GetName() + " == " + snsegs.str() + " && " + vtx_pos_cuts + " && " + segcuts;
            //             Variable vtx_pos_fgdseg(vtx_pos);
            //             vtx_pos_fgdseg.SetSName( vtx_pos_fgdseg.GetSName() + "_" + snsegs.str() + "fgdseg" );
            //             vtx_pos_fgdseg.SetSymbol( vtx_pos_fgdseg.GetSymbol() + " " + snsegs.str() + "FGD Segment(s)" );

            //             ProduceGroup(vtx_pos_fgdseg, dimnbins[dim], t2kgeometry::fgd1min_offset[dim], t2kgeometry::fgd1max_offset[dim], seg_vtx_cut);
            //         //TODO: Add detector lines to this plot.
            //         }
            //     }
            //     //**************************************** 1D NFGD Segs VTX Pos END ************************************//


            //     if(DrawPlot(ProducePlots::VtxPosition1DPur, ProducePlots::VtxPosition)){
            //     //**************************************** 1D Purity START ************************************//
            //         TH1D * vtx_pos_H_pur = m_runep->PurVSVar(m_recovars->vtx_pos.GetName() + "[" + dimss.str() + "]", dimnbins[dim], t2kgeometry::fgd1min_offset[dim], t2kgeometry::fgd1max_offset[dim],
            //             m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlusPS).GetSignal(), vtx_pos_cuts,
            //             m_recovars->vtx_pos.GetSymbol() + " " + m_NameXYZ[dim] + " (" + m_recovars->vtx_pos.GetUnits() + ")");

            //         TH1D * vtx_pos_CH_pur = m_runep->PurVSVar(m_recovars->vtx_pos.GetName() + "[" + dimss.str() + "]", dimnbins[dim], t2kgeometry::fgd1min_offset[dim], t2kgeometry::fgd1max_offset[dim],
            //             m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlus).GetSignal(), vtx_pos_cuts,
            //             m_recovars->vtx_pos.GetSymbol() + " " + m_NameXYZ[dim] + " (" + m_recovars->vtx_pos.GetUnits() + ")");

            //         TCanvas * vtx_pos_pur_c = new TCanvas((vtx_pos.GetSName() + "_pur").c_str(), "", 400, 400);
            //         vtx_pos_pur_c->cd();

            //         vtx_pos_H_pur->SetLineColor(DrawingStyle::Blue);
            //         vtx_pos_H_pur->Draw();

            //         vtx_pos_CH_pur->SetLineColor(DrawingStyle::Yellow);
            //         vtx_pos_CH_pur->Draw("SAME");

            //         TLegend * vtx_pos_pur_leg = m_runbd->Legend(0.15, 0.1);
            //         vtx_pos_pur_leg->AddEntry(vtx_pos_H_pur, m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlusPS).GetSymbol().c_str(), "l" );
            //         vtx_pos_pur_leg->AddEntry(vtx_pos_CH_pur, m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlus).GetSymbol().c_str(), "l" );
            //         vtx_pos_pur_leg->Draw();

            //         m_runbd->GetPOT(0.1, 0.1)->Draw();
            //         PrintLogo(vtx_pos_pur_c);

            //         vtx_pos_pur_c->Write();

            //         delete vtx_pos_H_pur;
            //         delete vtx_pos_CH_pur;
            //         delete vtx_pos_pur_leg;
            //         delete vtx_pos_pur_c;
            //     }

            //     //**************************************** 1D Purity END ************************************//

            //     for(int dim2 = dim + 1; dim2 < 3; dim2++){
            //         stringstream dim2ss;
            //         dim2ss << dim2;

            //         string vtx_pos2D = m_recovars->vtx_pos.GetName() + "[" + dim2ss.str() + "]:" + m_recovars->vtx_pos.GetName() + "[" + dimss.str() + "]";
            //     //**************************************** 2D START ************************************//
                  
            //         if(DrawPlot(ProducePlots::VtxPosition2D, ProducePlots::VtxPosition)){
            //             TH2D * vtx_pos2D_h = m_runbd->GetHisto(vtx_pos2D, dimnbins[dim], t2kgeometry::fgd1min_offset[dim], 
            //                 t2kgeometry::fgd1max_offset[dim], dimnbins[dim2], t2kgeometry::fgd1min_offset[dim2], 
            //                 t2kgeometry::fgd1max_offset[dim2], "Start Position " + m_NameXYZ[dim] + " (mm);Start Position " + m_NameXYZ[dim2] + " (mm)", vtx_pos_cuts);

            //         // DrawLine(double x_low, double y_low, double x_high, double y_high, int color = kGray + 2, int style = 1, int width = 2);

            //             TCanvas * vtx_pos2D_c = new TCanvas((vtx_pos.GetSName() + m_NameXYZ[dim2]).c_str(), "", 400, 400);
            //             vtx_pos2D_c->cd();
            //             vtx_pos2D_h->Draw("COLZ");
            //             double box_low[2] = { t2kgeometry::fgd1min[dim], t2kgeometry::fgd1min[dim2] };
            //             double box_hig[2] = { t2kgeometry::fgd1max[dim], t2kgeometry::fgd1max[dim2] };
            //             m_runbd->DrawBox(box_low, box_hig);
            //             m_runbd->GetPOT(0.1,0.1)->Draw();
            //             PrintLogo(vtx_pos2D_c);

            //             vtx_pos2D_c->Write();

            //             delete vtx_pos2D_h;
            //             delete vtx_pos2D_c;
            //         }

            //         double box_low[2] = { t2kgeometry::fgd1min[dim], t2kgeometry::fgd1min[dim2] };
            //         double box_hig[2] = { t2kgeometry::fgd1max[dim], t2kgeometry::fgd1max[dim2] };

            //         if(DrawPlot(ProducePlots::VtxPosition2DFGDSegs, ProducePlots::VtxPosition)){
            //         //Vertex Position for NFGD Segments: 1, 2, 3:
            //             for(int nsegs = 1; nsegs < 4; nsegs++){
            //                 stringstream snsegs; 
            //                 snsegs << nsegs;
            //                 string seg_vtx_cut = nfgdsegments.GetName() + " == " + snsegs.str() + " && " + vtx_pos_cuts + " && " + segcuts;

            //                 TH2D * vtx_pos2D_fgdseg_h = m_runbd->GetHisto(vtx_pos2D, dimnbins[dim], t2kgeometry::fgd1min_offset[dim], 
            //                     t2kgeometry::fgd1max_offset[dim], dimnbins[dim2], t2kgeometry::fgd1min_offset[dim2], 
            //                     t2kgeometry::fgd1max_offset[dim2], "Start Position " + m_NameXYZ[dim] + " (mm);Start Position " + m_NameXYZ[dim2] + " (mm)", seg_vtx_cut);

            //                 TCanvas * vtx_pos2D_fgdseg_c = new TCanvas((vtx_pos.GetSName() + m_NameXYZ[dim2] + "_" + snsegs.str() + "fgdseg").c_str(), "", 400, 400);
            //                 vtx_pos2D_fgdseg_c->cd();
            //                 vtx_pos2D_fgdseg_h->Draw("COLZ");
                           
            //                 m_runbd->DrawBox(box_low, box_hig);
            //                 m_runbd->GetPOT(0.1,0.1)->Draw();
            //                 PrintLogo(vtx_pos2D_fgdseg_c);

            //                 vtx_pos2D_fgdseg_c->Write();

            //                 delete vtx_pos2D_fgdseg_h;
            //                 delete vtx_pos2D_fgdseg_c;
            //             }
            //         }

            //     //**************************************** 2D END ************************************//  

            //         if(DrawPlot(ProducePlots::VtxPosition2DPur, ProducePlots::VtxPosition)){
            //             //**************************************** 2D Purity START ************************************//
            //             TH2D * vtx_pos2D_pur_h = m_runep->PurVSVar(vtx_pos2D, dimnbins[dim], t2kgeometry::fgd1min_offset[dim], 
            //                 t2kgeometry::fgd1max_offset[dim], dimnbins[dim2], t2kgeometry::fgd1min_offset[dim2], 
            //                 t2kgeometry::fgd1max_offset[dim2], m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlusPS).GetSignal(),
            //                 vtx_pos_cuts ,"Start Position " + m_NameXYZ[dim] + " (mm);Start Position " + m_NameXYZ[dim2] + " (mm)");

            //             TCanvas * vtx_pos2D_pur_c = new TCanvas((vtx_pos.GetSName() + m_NameXYZ[dim2] + "_pur").c_str(), "", 400, 400);
            //             vtx_pos2D_pur_c->cd();
            //             vtx_pos2D_pur_h->Draw("COLZ");
            //             m_runbd->DrawBox(box_low, box_hig);
            //             m_runbd->GetPOT(0.1,0.1)->Draw();
            //             PrintLogo(vtx_pos2D_pur_c);
            //             GetSignal()->Draw();

            //             vtx_pos2D_pur_c->Write();

            //             delete vtx_pos2D_pur_h;
            //             delete vtx_pos2D_pur_c;

            //             //**************************************** 2D Purity END ************************************//
            //         }
            //     }
            // }
                //**************************************** Vertex Position END ************************************//
        }

        if(m_experiment->GetType() == Experiment::MIN && DrawPlot(ProducePlots::W)){
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

            // Three TPC Tracks:                
            std::vector<string> cnames3TPC; //= { "Event Quality", "Contains Tracks", "3 Tracks", "N TPC Tracks", "FGD Contained", "Track Charges" };//, "Common Vertex" }; 
            cnames3TPC.push_back("Event Quality");
            cnames3TPC.push_back("Contains Tracks");
            cnames3TPC.push_back("3 Tracks");
            cnames3TPC.push_back("N TPC Tracks");
            cnames3TPC.push_back("Track Charges");

            // Two TPC Tracks
            std::vector<string> cnames2TPC; //= { "Event Quality", "Contains Tracks", "3 Tracks", "N TPC Tracks", "FGD Contained", "Track Charges" };//, "Common Vertex" }; 
            cnames2TPC.push_back("Event Quality");
            cnames2TPC.push_back("Contains Tracks");
            cnames2TPC.push_back("3 Tracks");
            cnames2TPC.push_back("N TPC Tracks");
            cnames2TPC.push_back("FGD Contained");
            cnames2TPC.push_back("Track Charges");

            // Plot the PS signal:
            string tSig = "true_ntracks == 3 && truemu_ntracks == 1 && truep_ntracks == 1 && truepi_ntracks == 1";
            tSig += "&& truemu_truemom > 250. && truepi_truemom > 250. && truep_truemom > 450.";

            string tSigH = tSig + " && target == 1";

            if(DrawPlot(ProducePlots::EffVSCuts)){
                MakeDir("Efficiency/Cuts" + branchnames[br]);

                m_runep->SetCutNames(cnames3TPC);

                TH1D * eff3 = m_runep->EffVSCuts( tSig, 1, "", 5);
                TH1D * pur3 = m_runep->PurVSCuts( tSig, 1, "", 5);

                eff3->SetLineColor(DrawingStyle::Blue);
                pur3->SetLineColor(DrawingStyle::Blue);
                pur3->SetLineStyle(7);

                TH1D * eff3H = m_runep->EffVSCuts( tSigH, 1, "", 5);
                TH1D * pur3H = m_runep->PurVSCuts( tSigH, 1, "", 5);

                eff3H->SetLineColor(DrawingStyle::Yellow);
                pur3H->SetLineColor(DrawingStyle::Yellow);
                pur3H->SetLineStyle(7);

                TLegend * eff_pur_3TPC_leg = m_runbd->Legend(0.2,0.1);
                eff_pur_3TPC_leg->AddEntry((TObject*)0x0,   "CC1p1#pi^{+}", "");
                eff_pur_3TPC_leg->AddEntry(eff3,            "Efficiency", "l");
                eff_pur_3TPC_leg->AddEntry(pur3,            "Purity", "l");
                eff_pur_3TPC_leg->AddEntry((TObject*)0x0,   "", "");
                eff_pur_3TPC_leg->AddEntry((TObject*)0x0,   "CC1p1#pi^{+} on Hydrogen", "");
                eff_pur_3TPC_leg->AddEntry(eff3H,           "Efficiency", "l");
                eff_pur_3TPC_leg->AddEntry(pur3H,           "Purity", "l");

                TCanvas * eff_pur_3TPC = new TCanvas("eff_pur_3TPC","", 600, 800);

                eff_pur_3TPC->cd();
                eff3->GetYaxis()->SetTitle("#varepsilon/Pur. (%)");
                eff3->Draw();
                pur3->Draw("SAME");
                eff3H->Draw("SAME");
                pur3H->Draw("SAME");
                eff_pur_3TPC_leg->Draw();

                eff_pur_3TPC->Write();

                m_runep->SetCutNames(cnames2TPC);

                TH1D * eff2 = m_runep->EffVSCuts( tSig, 3, "", 6);
                TH1D * pur2 = m_runep->PurVSCuts( tSig, 3, "", 6);

                eff2->SetLineColor(DrawingStyle::Blue);
                pur2->SetLineColor(DrawingStyle::Blue);
                pur2->SetLineStyle(7);

                TH1D * eff2H = m_runep->EffVSCuts( tSigH, 3, "", 6);
                TH1D * pur2H = m_runep->PurVSCuts( tSigH, 3, "", 6);

                eff2H->SetLineColor(DrawingStyle::Yellow);
                pur2H->SetLineColor(DrawingStyle::Yellow);
                pur2H->SetLineStyle(7);

                TLegend * eff_pur_2TPC_leg = m_runbd->Legend(0.2,0.1);
                eff_pur_2TPC_leg->AddEntry((TObject*)0x0,   "CC1p1#pi^{+}", "");
                eff_pur_2TPC_leg->AddEntry(eff2,            "Efficiency", "l");
                eff_pur_2TPC_leg->AddEntry(pur2,            "Purity", "l");
                eff_pur_2TPC_leg->AddEntry((TObject*)0x0,   "", "");
                eff_pur_2TPC_leg->AddEntry((TObject*)0x0,   "CC1p1#pi^{+} on Hydrogen", "");
                eff_pur_2TPC_leg->AddEntry(eff2H,           "Efficiency", "l");
                eff_pur_2TPC_leg->AddEntry(pur2H,           "Purity", "l");

                TCanvas * eff_pur_2TPC = new TCanvas("eff_pur_2TPC","", 600, 800);

                eff_pur_2TPC->cd();
                eff2->GetYaxis()->SetTitle("#varepsilon/Pur. (%)");
                eff2->Draw();
                pur2->Draw("SAME");
                eff2H->Draw("SAME");
                pur2H->Draw("SAME");
                eff_pur_2TPC_leg->Draw();

                eff_pur_2TPC->Write();

                // Make the list of cut names:

                 //                     s1      s2
            //  "Event Quality"         0
            //  "Contains Tracks"       1
            //  "Vertex ID"             2
            //  "3 Tracks"              3
            //  "N TPC Tracks"          4      
            //  "FGD"                       5
            //  "Track Charges"     5       6
            //  "Common Vertex"     6       7

                // TCanvas * eff_pur_cuts = new TCanvas("eff_pur_cuts","", 600, 800);
                // eff_pur_cuts->cd();
                // TH1D * eff_new = m_runep->EffVSCuts( m_old_signal, br);
                // TH1D * pur_new = m_runep->PurVSCuts( m_old_signal, br);

                // TH1D * eff_CC1P1Pi = m_runep->EffVSCuts( m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlus).GetSignal(), br);
                // TH1D * pur_CC1P1Pi = m_runep->PurVSCuts( m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlus).GetSignal(), br);

                // TH1D * eff_CC1P1PiInc = m_runep->EffVSCuts( m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlusInc).GetSignal(), br);
                // TH1D * pur_CC1P1PiInc = m_runep->PurVSCuts( m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlusInc).GetSignal(), br);

                // TH1D * eff_new_PS = m_runep->EffVSCuts( m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlusPS).GetSignal(), br);
                // TH1D * pur_new_PS = m_runep->PurVSCuts( m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlusPS).GetSignal(), br);

                // eff_new->SetLineColor(DrawingStyle::Yellow);
                // // eff_new->SetLineStyle(7);
                // pur_new->SetLineColor(DrawingStyle::Yellow);
                // pur_new->SetLineStyle(7);

                // eff_CC1P1Pi->SetLineColor(DrawingStyle::Red);
                // // eff_CC1P1Pi->SetLineStyle(7);
                // pur_CC1P1Pi->SetLineColor(DrawingStyle::Red);
                // pur_CC1P1Pi->SetLineStyle(7);

                // eff_CC1P1PiInc->SetLineColor(DrawingStyle::Blue);
                // // eff_CC1P1Pi->SetLineStyle(7);
                // pur_CC1P1PiInc->SetLineColor(DrawingStyle::Blue);
                // pur_CC1P1PiInc->SetLineStyle(7);

                // eff_new_PS->SetLineColor(DrawingStyle::Green);
                // // eff_new_PS->SetLineStyle(7);
                // pur_new_PS->SetLineColor(DrawingStyle::Green);
                // pur_new_PS->SetLineStyle(7);


                // eff_new->GetYaxis()->SetTitle("Pur/Eff. (%)");
                // eff_new->Draw("HIST");
                // pur_new->Draw("HISTSAME");

                // eff_CC1P1Pi->Draw("HISTSAME");
                // pur_CC1P1Pi->Draw("HISTSAME");

                // eff_CC1P1PiInc->Draw("HISTSAME");
                // pur_CC1P1PiInc->Draw("HISTSAME");

                // eff_new_PS->Draw("HISTSAME");
                // pur_new_PS->Draw("HISTSAME");

                // TLegend * eff_pur_cuts_leg = m_runbd->Legend(0.2,0.1);
                // eff_pur_cuts_leg->AddEntry(eff_new, ("Efficiency (" + Topology::ToString(Topology::HCC1P1PiPlus, 1) + ")").c_str(), "l");
                // eff_pur_cuts_leg->AddEntry(pur_new, ("Purity ("     + Topology::ToString(Topology::HCC1P1PiPlus, 1) + ")").c_str(), "l");

                // eff_pur_cuts_leg->AddEntry(eff_new_PS, ("Efficiency (" + m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlusPS).GetSymbol() + ")").c_str(), "l");
                // eff_pur_cuts_leg->AddEntry(pur_new_PS, ("Purity ("     + m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlusPS).GetSymbol() + ")").c_str(), "l");

                // eff_pur_cuts_leg->AddEntry(eff_CC1P1Pi, ("Efficiency (" + m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlus).GetSymbol() + ")").c_str(), "l");
                // eff_pur_cuts_leg->AddEntry(pur_CC1P1Pi, ("Purity ("     + m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlus).GetSymbol() + ")").c_str(), "l");

                // eff_pur_cuts_leg->AddEntry(eff_CC1P1PiInc, ("Efficiency (" + m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlusInc).GetSymbol() + ")").c_str(), "l");
                // eff_pur_cuts_leg->AddEntry(pur_CC1P1PiInc, ("Purity ("     + m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlusInc).GetSymbol() + ")").c_str(), "l");

                // if(m_experiment->GetType() == Experiment::MIN){
                //     std::string signal_def_old = "truth_n_pro == 1 && truth_n_piP == 1 && truth_n_muo == 1 && mc_nFSPart == 3 && mc_targetZ == 1";
                //     signal_def_old += " && mc_current == 1 && TMath::RadToDeg()*truth_mu_Theta < 25. && TMath::RadToDeg()*truth_mu_Theta >= 0.";

                //     TH1D * eff_old = m_runep->EffVSCuts( signal_def_old, br);
                //     TH1D * pur_old = m_runep->PurVSCuts( signal_def_old, br);

                //     eff_old->SetLineStyle(7);
                //     pur_old->SetLineStyle(7);
                //     eff_old->Draw("HISTSAME");
                //     pur_old->Draw("HISTSAME");

                //     eff_pur_cuts_leg->AddEntry(eff_old, "Efficiency (Old Sig. Def.)", "l");
                //     eff_pur_cuts_leg->AddEntry(pur_old, "Purity (Old Sig. Def.)", "l");

                //     eff_pur_cuts_leg->Draw();            
                //     eff_pur_cuts->Write();

                //     delete eff_old;
                //     delete pur_old;
                // }
                // else{
                //     eff_pur_cuts_leg->Draw();            
                //     eff_pur_cuts->Write();
                // }

                // delete eff_new;
                // delete pur_new;
                // delete eff_CC1P1Pi;
                // delete pur_CC1P1Pi;
                // delete eff_new_PS;
                // delete pur_new_PS;
                
                // delete eff_pur_cuts_leg;
                // delete eff_pur_cuts;

            //******************************** Efficiency/Purity END ********************************//
            }
            //******************************** Efficiency/Purity N - 1 Cuts START ********************************//

            if(m_experiment->GetType() == Experiment::T2K && DrawPlot(ProducePlots::EffVSN1Cuts)){
                MakeDir("Efficiency/N1Cuts");

                m_runep->SetCutNames(cnames3TPC);

                TCanvas * N1_3TPC = new TCanvas("EffPurN1_3TPC", "", 800, 300);
                TH1D * effN1_3TPC = m_runep->EffVSN1Cuts( tSig, 1, "", 5);
                TH1D * purN1_3TPC = m_runep->PurVSN1Cuts( tSig, 1, "", 5);

                TLegend * N1_3TPC_name = m_runbd->Legend(0.2,0.1);
                N1_3TPC_name->AddEntry((TObject*)0x0, "CC1p1#pi^{+}", "");

                TLegend * N1_3TPC_leg = m_runbd->Legend(0.2,0.1);
                N1_3TPC_leg->AddEntry(effN1_3TPC, "Efficiency", "");
                N1_3TPC_leg->AddEntry(purN1_3TPC, "Purity", "");

                N1_3TPC->cd();
                effN1_3TPC->GetYaxis()->SetTitle("#varepsilon/Pur. (%)");
                effN1_3TPC->Draw();
                purN1_3TPC->Draw("SAME");
                N1_3TPC_name->Draw();
                N1_3TPC_leg->Draw();

                N1_3TPC->Write();

                TCanvas * N1_3TPCH = new TCanvas("EffPurN1_3TPCH", "", 800, 300);
                TH1D * effN1_3TPCH = m_runep->EffVSN1Cuts( tSigH, 1, "", 5);
                TH1D * purN1_3TPCH = m_runep->PurVSN1Cuts( tSigH, 1, "", 5);

                TLegend * N1_3TPCH_name = m_runbd->Legend(0.2,0.1);
                N1_3TPCH_name->AddEntry((TObject*)0x0, "CC1p1#pi^{+} on Hydrogen", "");

                TLegend * N1_3TPCH_leg = m_runbd->Legend(0.2,0.1);
                N1_3TPCH_leg->AddEntry(effN1_3TPCH, "Efficiency", "");
                N1_3TPCH_leg->AddEntry(purN1_3TPCH, "Purity", "");

                N1_3TPCH->cd();
                effN1_3TPCH->GetYaxis()->SetTitle("#varepsilon/Pur. (%)");
                effN1_3TPCH->Draw();
                purN1_3TPCH->Draw("SAME");
                N1_3TPCH_name->Draw();
                N1_3TPCH_leg->Draw();

                N1_3TPCH->Write();

                m_runep->SetCutNames(cnames2TPC);

                TCanvas * N1_2TPC = new TCanvas("EffPurN1_2TPC", "", 800, 300);

                TH1D * effN1_2TPC = m_runep->EffVSN1Cuts( tSig, 3, "", 6);
                TH1D * purN1_2TPC = m_runep->PurVSN1Cuts( tSig, 3, "", 6);

                TLegend * N1_2TPC_name = m_runbd->Legend(0.2,0.1);
                N1_2TPC_name->AddEntry((TObject*)0x0, "CC1p1#pi^{+}", "");

                TLegend * N1_2TPC_leg = m_runbd->Legend(0.2,0.1);
                N1_2TPC_leg->AddEntry(effN1_2TPC, "Efficiency", "");
                N1_2TPC_leg->AddEntry(purN1_2TPC, "Purity", "");

                N1_2TPC->cd();
                effN1_2TPC->GetYaxis()->SetTitle("#varepsilon/Pur. (%)");
                effN1_2TPC->Draw();
                purN1_2TPC->Draw("SAME");
                N1_2TPC_name->Draw();
                N1_2TPC_leg->Draw();

                N1_2TPC->Write();

                TCanvas * N1_2TPCH = new TCanvas("EffPurN1_2TPCH", "", 800, 300);

                TH1D * effN1_2TPCH = m_runep->EffVSN1Cuts( tSigH, 3, "", 6);
                TH1D * purN1_2TPCH = m_runep->PurVSN1Cuts( tSigH, 3, "", 6);

                TLegend * N1_2TPCH_name = m_runbd->Legend(0.2,0.1);
                N1_2TPCH_name->AddEntry((TObject*)0x0, "CC1p1#pi^{+} on Hydrogen", "");

                TLegend * N1_2TPCH_leg = m_runbd->Legend(0.2,0.1);
                N1_2TPCH_leg->AddEntry(effN1_2TPCH, "Efficiency", "");
                N1_2TPCH_leg->AddEntry(purN1_2TPCH, "Purity", "");

                N1_2TPCH->cd();
                effN1_2TPCH->GetYaxis()->SetTitle("#varepsilon/Pur. (%)");
                effN1_2TPCH->Draw();
                purN1_2TPCH->Draw("SAME");
                N1_2TPCH_name->Draw();
                N1_2TPCH_leg->Draw();

                N1_2TPCH->Write();

                // TH1D * effN1_new = m_runep->EffVSN1Cuts( m_old_signal, br);
                // TH1D * purN1_new = m_runep->PurVSN1Cuts( m_old_signal, br);

                // effN1_new->SetLineColor(DrawingStyle::Yellow);
                // purN1_new->SetLineColor(DrawingStyle::Yellow);
                // purN1_new->SetLineStyle(7);
                // // TH1D * effN1_CC1P1Pi = m_runep->EffVSN1Cuts( m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlus).GetSignal(), br);
                // // TH1D * purN1_CC1P1Pi = m_runep->PurVSN1Cuts( m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlus).GetSignal(), br);

                // TH1D * effN1_CC1P1PiInc = m_runep->EffVSN1Cuts( m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlusInc).GetSignal(), br);
                // TH1D * purN1_CC1P1PiInc = m_runep->PurVSN1Cuts( m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlusInc).GetSignal(), br);
                // effN1_CC1P1PiInc->SetLineColor(DrawingStyle::Blue);
                // purN1_CC1P1PiInc->SetLineColor(DrawingStyle::Blue);
                // purN1_CC1P1PiInc->SetLineStyle(7);

                // TH1D * effN1_new_PS = m_runep->EffVSN1Cuts( m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlusPS).GetSignal(), br);
                // TH1D * purN1_new_PS = m_runep->PurVSN1Cuts( m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlusPS).GetSignal(), br);

                // effN1_new_PS->SetLineColor(DrawingStyle::Green);
                // purN1_new_PS->SetLineColor(DrawingStyle::Green);
                // purN1_new_PS->SetLineStyle(7);
                

                // TLegend * eff_pur_N1cuts_leg = m_runbd->Legend(0.2,0.1);
                // eff_pur_N1cuts_leg->AddEntry(effN1_new, ("Efficiency (" + Topology::ToString(Topology::HCC1P1PiPlus, 1) + ")").c_str(), "l");
                // eff_pur_N1cuts_leg->AddEntry(purN1_new, ("Purity ("     + Topology::ToString(Topology::HCC1P1PiPlus, 1) + ")").c_str(), "l");

                // eff_pur_N1cuts_leg->AddEntry(effN1_new_PS, ("Efficiency (" + m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlusPS).GetSymbol() + ")").c_str(), "l");
                // eff_pur_N1cuts_leg->AddEntry(purN1_new_PS, ("Purity ("     + m_experiment->GetTopologies()->GetTopology(Topology::HCC1P1PiPlusPS).GetSymbol() + ")").c_str(), "l");


                // // eff_pur_N1cuts_leg->AddEntry(effN1_CC1P1Pi, ("Efficiency (" + m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlus).GetSymbol() + ")").c_str(), "l");
                // // eff_pur_N1cuts_leg->AddEntry(purN1_CC1P1Pi, ("Purity ("     + m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlus).GetSymbol() + ")").c_str(), "l");

                // eff_pur_N1cuts_leg->AddEntry(effN1_CC1P1PiInc, ("Efficiency (" + m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlusInc).GetSymbol() + ")").c_str(), "l");
                // eff_pur_N1cuts_leg->AddEntry(purN1_CC1P1PiInc, ("Purity ("     + m_experiment->GetTopologies()->GetTopology(Topology::CC1P1PiPlusInc).GetSymbol() + ")").c_str(), "l");

                // TCanvas * eff_pur_N1cuts = new TCanvas("eff_pur_N1cuts","", 600, 800);
                // eff_pur_N1cuts->cd();
                // effN1_new->GetYaxis()->SetTitle("Eff./Pur. (%)");
                // effN1_new->Draw("HIST");
                // purN1_new->Draw("HISTSAME");
                // effN1_new_PS->Draw("HISTSAME");
                // purN1_new_PS->Draw("HISTSAME");
                // effN1_CC1P1PiInc->Draw("HISTSAME");
                // purN1_CC1P1PiInc->Draw("HISTSAME");
                // eff_pur_N1cuts_leg->Draw();

                // eff_pur_N1cuts->Write();

                // delete effN1_new;
                // delete purN1_new;
                // delete effN1_new_PS;
                // delete purN1_new_PS;
                // delete effN1_CC1P1PiInc;
                // delete purN1_CC1P1PiInc;
                // delete eff_pur_N1cuts_leg;
                // delete eff_pur_N1cuts;
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

TLatex * ProducePlots::GetSignal(string extra){
    return new TLatex(0.0, 0.1, ("#font[62]{#it{" + m_experiment->GetSigDef() + extra + "}}").c_str() );
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

    //cuts_on : 0 - off, 1 - on, 2 - only cut with cuts on
    int cuts_on = 0;   

    ProducePlots::RunOpts run_opt = ProducePlots::All;

    char cc;
    while((cc = getopt(argc, argv, "i:o:d::t::m::r::v::a:b:e:c:")) != -1){
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
            case 'e': run_opt = (ProducePlots::RunOpts)atoi(optarg); break; //Produce only plot of type in RunOpts
            case 'c': cuts_on = atoi(optarg); break; //cuts_on : 0 - off, 1 - on all cut types, 2 - PID cut, 3 - PS cuts, 4 - PID + PS cuts
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

    ProducePlots * plots = new ProducePlots(experiment, filename, savename, debug, realdata, run_opt);
    plots->Verbose(verbose);
    plots->SetBranchToPlot(accum_level, branch_no);//Make this function better.

    if(cuts_on == 1){ 
        cout << "Running with cuts on. " << endl;
        plots->RunWithCuts(); 
    }
    else if(cuts_on > 1){
        cout << "Running with cuts only " << endl;
        plots->RunWithCutsOnly(cuts_on - 1);
    }

    // if(!savename.empty()) plots->SetSaveName(savename);
    
    plots->MakePlots();

//    delete plots; -- TODO: Fix this
    
    return 0;
}
