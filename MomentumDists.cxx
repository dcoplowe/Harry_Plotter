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

//#include <PlotUtils/MnvH1D.h>
//#include <PlotUtils/MnvH2D.h>
//#include <PlotUtils/TargetUtils.h>
//#include <PlotUtils/MnvPlotter.h>
//#include "Cintex/Cintex.h"

#include "EffPurTools.h"
#include "DrawingTools.h"
#include "TH1D.h"
#include "TH2D.h"
#include "THStack.h"
#include "TLegend.h"
#include "TFile.h"

//#include <TStyle.h>

using namespace std;
//using namespace PlotUtils;

//These are the tuples used for this exercise
const string testing_mc("/pnfs/minerva/scratch/users/dcoplowe/CC1P1Pi_R13200_290916_1/grid/central_value/minerva/ana/v10r8p9/00/01/32/00/SIM_minerva_00013200_Subruns_0001-0002-0003-0004_CC1P1PiAnalysis_Ana_Tuple_v10r8p9-dcoplowe.root");

const string flag("sel_");

/*class AnalysisPlots {
public:
    
    AnalysisPlots();
    ~AnalysisPlots();
    
    TCanvas * VarVsInts();
    TCanvas * VarVsParticles();
    
};*/

//This is the POT for Data and Monte Carlo for the tuples that we use in this exercise.
//const double total_mc_pot = 1.79e19;

void MomentumDists(const string file, const string savename, bool debug)
{
    //ROOT::Cintex::Cintex::Enable();
    
    cout<<"Producing Momentum Distributions for MC files."<<endl;
    
    //TString savename = "CC1P1Pi_TEST";
    
    //Produce independent particles vars hists:
    string part_snam[3] = {"mu","pr","pi"};
    string part_name[3] = {"Muon", "Proton", "Pion"};
    string part_symb[3] = {"#mu^{-}", "p", "#pi^{#pm}"};
    int    part_colo[3] = {DrawingStyle::DSProton, DrawingStyle::DSPion, DrawingStyle::DSMuon};
    
    string var_unit[3] = {" (MeV/#it{c})", " (MeV/#it{c})", " (MeV)" };
    string var_symb[3] = {" #it{p}", " |#it{p}_{T}|", " E" };
    
    string pdg_cut[3] = { (flag + "mu_PDG == 13").c_str(), (flag + "pr_PDG == 2212").c_str(), ("TMath::Abs(" + flag + "_pi_PDG) == 211").c_str()};
    
    int var_bin[3] = { 20, 20, 20};
    double var_range[6] = {0., 30., 0., 2., 0., 1.2};
    double truemom_range[6] = {0., 40., 0., 20., 0., 30.};

    std::string hyp[2] = {"EX_", "LL_"};
    
    string rec_var[3] = {"mom", "pTMag", "E"};
    string rec_name[3] = {"Momentum", "|#it{p}_{T}|", "Energy"};
    
    string true_var[3];
    string true_name[3];
    
    for(int i = 0; i < 3; i++){
        true_var[i] = "true" + rec_var[i];
        true_name[i] = "True " + rec_name[i];
    }
    
    TFile * outfile = new TFile(savename.c_str(), "RECREATE");
    outfile->cd();
    
    DrawingTools * plot = new DrawingTools(file, debug);
    
    std::vector<TString> cut_names;
    cut_names.push_back("Vertex");
    cut_names.push_back("3 Tracks");
    cut_names.push_back("Muon Track");
    cut_names.push_back("Contained Vtx");
    cut_names.push_back("PID: p/#pi^{+}");
    
    EffPurTools * m_ep = new EffPurTools(file, cut_names, debug);
    
    for(int k = 0; k < 1; k++){//Target loop
        
        string target = "target_region == ";
        string tar_name;
        if(k == 0){
            target += "1";
            tar_name = "scint";
        }
        else{
            target += "2";
            tar_name = "carbon";
        }
        
        for(int i=0; i<3; i++){//Particle loop
            if(i>0){
            //Scores:
               // GetRecoHisto(Form("CC1P1Pi_%s_mom/1000",tmp_part_snam.Data()),mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s",common_cuts_p.Data()))
                std::string std_partvar = flag + part_snam[i];
                for(int j = 0; j < 2; j++){//LL dEdX loop
                    
                    char buffer[100];
                    sprintf(buffer,"%d",j);
                    std::string common_cuts = target + " && accum_level[" + buffer + "] > 4";

                    for (int hyp_i = 0; hyp_i < 2; hyp_i++) {//Alt hyp loop
                        
                        //Score:
                        std::string std_score =  std_partvar + hyp[j] + "_score";
                        cout << std_score << endl;
                        std::string AltTit = "";
                        if(hyp_i>0){
                            std_score += "_altH";
                            AltTit = "Alt. ";
                        }
                        TH1D * score_pr = plot->GetRecoHisto(TString(std_score.c_str()), 20, 0., 1., Form("%s %sScore", part_name[i].c_str(), AltTit.c_str()), Form("%s && %s_PDG == 2212", common_cuts.c_str(), std_partvar.c_str()));
                        TH1D * score_pi = plot->GetRecoHisto(TString(std_score.c_str()), 20, 0., 1., Form("%s %sScore", part_name[i].c_str(), AltTit.c_str()), Form("%s && TMath::Abs(%s_PDG) == 211", common_cuts.c_str(), std_partvar.c_str()));
                        TH1D * score_mu = plot->GetRecoHisto(TString(std_score.c_str()), 20, 0., 1., Form("%s %sScore", part_name[i].c_str(), AltTit.c_str()), Form("%s && %s_PDG == 13", common_cuts.c_str(), std_partvar.c_str()));
                        TH1D * score_p0 = plot->GetRecoHisto(TString(std_score.c_str()), 20, 0., 1., Form("%s %sScore", part_name[i].c_str(), AltTit.c_str()), Form("%s && %s_PDG == 111", common_cuts.c_str(), std_partvar.c_str()));
                        TH1D * score_ka = plot->GetRecoHisto(TString(std_score.c_str()), 20, 0., 1., Form("%s %sScore", part_name[i].c_str(), AltTit.c_str()), Form("%s && TMath::Abs(%s_PDG) == 321", common_cuts.c_str(), std_partvar.c_str()));
                        TH1D * score_kz = plot->GetRecoHisto(TString(std_score.c_str()), 20, 0., 1., Form("%s %sScore", part_name[i].c_str(), AltTit), Form("%s && %s_PDG == 311", common_cuts.c_str(), std_partvar.c_str()));
                        std::string std_p_other =  std_partvar + "_PDG != 2212 TMath::Abs(" + std_partvar + "_PDG) != 211 " + std_partvar + "_PDG != 13 && " + std_partvar + "_PDG != 111 && TMath::Abs(" + std_partvar + "_PDG) != 311 && " + std_partvar + "_PDG != 311 && " + std_partvar + "_PDG != -999";
                        TH1D * score_ot = plot->GetRecoHisto(TString(std_score.c_str()), 20, 0., 1., Form("%s %sScore", part_name[i].c_str(), AltTit.c_str()), Form("%s && %s", common_cuts.c_str(), std_p_other.c_str()));
                        
                        plot->ColFill(score_pr, DrawingStyle::DSProton);
                        plot->ColFill(score_pi, DrawingStyle::DSPion);
                        plot->ColFill(score_mu, DrawingStyle::DSMuon);
                        plot->ColFill(score_p0, DrawingStyle::DSPi0);
                        plot->ColFill(score_ka, DrawingStyle::DSKaon);
                        plot->ColFill(score_kz, DrawingStyle::DSKa0);
                        plot->ColFill(score_ot, DrawingStyle::DSOther);

                        //THStack * hs_part = new THStack(Form("hs%s_part", tmp_part_snam.Data()),Form(";#it{p}_{%s} (GeV/#it{c});Counts", tmp_part_symb.Data()));
                        
                        //std::vector<double> score_per = plot->GetPercentage(std::vector<TH1D*> histos);
                        
                        //THStack * score_st = ;
                    }
                }
            }
            //-----------------------------------------------------------------------------------------------------------------------------------------------------------------//
        }
        
                                                             /*
        TH1D * purcut = m_ep->PurVSCuts( (target + "&& mc_intType == 2 && mc_current == 1").c_str() );
        
        if(purcut){
            TCanvas * pur_can = new TCanvas( (tar_name + "_pur_vs_cuts").c_str(), "", 500,500);
            pur_can->cd();
            purcut->Draw();
            outfile->cd();
            pur_can->Write();
            //pur_can->Print("first_pur_plot.eps");
        }
        
        TH1D * effcut = m_ep->EffVSCuts("mc_intType == 2 && mc_current == 1");
        
        if(effcut){
            TCanvas * eff_can = new TCanvas( (tar_name + "_eff_vs_cuts").c_str(), "", 500,500);
            eff_can->cd();
            effcut->Draw();
            outfile->cd();
            eff_can->Write();
            //eff_can->Print("first_eff_plot.eps");
        }
    
    }*/
    
    outfile->Close();

}

/*TCanvas * AnalysisPlots::VarVsInts(){
    
}

TCanvas * AnalysisPlots:: VarVsParticles(TString var, int nbins, double x_low, double x_high, TString particle, TString part_symb){
    
    TH1D * h_mom_p1 = plot->GetRecoHisto(Form("CC1P1Pi_%s_%s/1000",particle.Data(), var.Data()), mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s && CC1P1Pi_%s_PDG == 2212", common_cuts_p.Data(), tmp_part_snam.Data()));
    TH1D * h_mom_p2 = plot->GetRecoHisto(Form("CC1P1Pi_%s_%s/1000",particle.Data(), var.Data()), mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s && TMath::Abs(CC1P1Pi_%s_PDG) == 211", common_cuts_p.Data(), tmp_part_snam.Data()));
    TH1D * h_mom_p3 = plot->GetRecoHisto(Form("CC1P1Pi_%s_%s/1000",particle.Data(), var.Data()), mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s && CC1P1Pi_%s_PDG == 13", common_cuts_p.Data(), tmp_part_snam.Data()));
    TH1D * h_mom_p4 = plot->GetRecoHisto(Form("CC1P1Pi_%s_%s/1000",particle.Data(), var.Data()), mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s && CC1P1Pi_%s_PDG != 13 && TMath::Abs(CC1P1Pi_%s_PDG) != 211 && CC1P1Pi_%s_PDG != 2212", common_cuts_p.Data(), tmp_part_snam.Data(), tmp_part_snam.Data(), tmp_part_snam.Data()));
    
    plot->ColFill(h_mom_p1, DrawingStyle::DSProton);
    plot->ColFill(h_mom_p2, DrawingStyle::DSPion);
    plot->ColFill(h_mom_p3, DrawingStyle::DSMuon);
    plot->ColFill(h_mom_p4, DrawingStyle::DSOther);
    
    THStack * hs_part = new THStack(Form("hs%s_part", tmp_part_snam.Data()),Form(";#it{p}_{%s} (GeV/#it{c});Counts", tmp_part_symb.Data()));
    hs_part->Add(h_mom_p1);
    hs_part->Add(h_mom_p2);
    hs_part->Add(h_mom_p3);
    hs_part->Add(h_mom_p4);
    
    double h_mom_p1_i = h_mom_p1->Integral();
    double h_mom_p2_i = h_mom_p2->Integral();
    double h_mom_p3_i = h_mom_p3->Integral();
    double h_mom_p4_i = h_mom_p4->Integral();
    double h_mom_p_tot = h_mom_p1_i + h_mom_p2_i + h_mom_p3_i + h_mom_p4_i;
    
    cout << " s_h_mom_p_tot = " << h_mom_p_tot << endl;
    
    h_mom_p1_i *= 100/h_mom_p_tot;
    h_mom_p2_i *= 100/h_mom_p_tot;
    h_mom_p3_i *= 100/h_mom_p_tot;
    h_mom_p4_i *= 100/h_mom_p_tot;
    
    TLegend * leg_part = plot->Legend(0.25, 0.4, 0.551,  0.362); //new TLegend(leg_ints_x, leg_ints_y, leg_ints_x + leg_ints_size_x, leg_ints_y + leg_ints_size_y);
    leg_part->AddEntry((TObject*)0, Form("Total Counts %d",(int)h_mom_p_tot), "");
    leg_part->AddEntry(h_mom_p1, Form("Proton (%.2f%%)",h_mom_p1_i), "f");
    leg_part->AddEntry(h_mom_p2, Form("#pi^{#pm} (%.2f%%)",h_mom_p2_i), "f");
    leg_part->AddEntry(h_mom_p3, Form("#mu^{-} (%.2f%%)",h_mom_p3_i), "f");
    leg_part->AddEntry(h_mom_p4, Form("Other (%.2f%%)",h_mom_p4_i), "f");
    
    TCanvas * c_parts = new TCanvas(Form("%s_mom_part_breakdown",tmp_part_name.Data()), "", 1000, 1000);
    c_parts->cd();
    hs_part->Draw();
    TLegend * pot_parts = plot->GetPOT(0.521,0.781);
    pot_parts->Draw();
    leg_part->Draw();
    return c_parts;*/
}

int main(int argc, char *argv[])
{
    string filename = testing_mc;
    string savename = "CC1P1Pi_Plots_290916.root";
    
    bool debug = false;
    
    char cc;
    while((cc = getopt(argc, argv, "i:o:d::")) != -1){
        switch (cc){
            case 'i': filename = optarg; break;
            case 'o': savename = optarg; break;
            case 'd': debug = true; break;
            default: return 1;
        }
    }
    
    MomentumDists(filename, savename, debug);
    
    return 0;
}
