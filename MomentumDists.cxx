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
const string testing_mc("/pnfs/minerva/persistent/users/dcoplowe/merged_CC1P1Pi_CC1P1Pi_run00013200.root");

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
    
    string var_unit[3] = {" (GeV/#it{c})", " (GeV/#it{c})", " (GeV)" };
    string var_symb[3] = {" #it{p}", " |#it{p}_{T}|", " E" };
    
    string pdg_cut[3] = { (flag + "mu_PDG == 13").c_str(), (flag + "pr_PDG == 2212").c_str(), ("TMath::Abs(" + flag + "_pi_PDG) == 211").c_str()};
    
    int mom_bin[3] = { 20, 20, 20};
    double mom_range[6] = {0., 30., 0., 2., 0., 1.2};
    double truemom_range[6] = {0., 40., 0., 20., 0., 30.};

    std::string hyp[2] = {"_EX", "_LL"};
    
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
    cut_names.push_back("Michel Tags");
    
//    EffPurTools * m_ep = new EffPurTools(file, cut_names, debug);
//    m_ep->EffVSCuts();
    
    
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

            std::string std_partvar = flag + part_snam[i];
            
            char buffer0[100];
            sprintf(buffer0,"%d",0);
            std::string common_cuts_mom = target + " && accum_level[" + buffer0 + "] > 4";
            
            //Momentum:
            //int mom_bin[3] = { 20, 20, 20};
            //double mom_range[6] = {0., 30., 0., 2., 0., 1.2};
            //double truemom_range[6] = {0., 40., 0., 20., 0., 30.};
            
            for(int j = 0; j < 2; j++){
            
                std::string part_name_ = std_partvar + hyp[j];
               
                const int mom_nbins = mom_bin[ i ];
                const double mom_low = mom_range[ 2*i ];
                const double mom_high = mom_range[ 2*i + 1 ];
                
                std::string mom_name = part_name_ + "truemom/1000:" + part_name_ + hyp[j] + "mom/1000";
                std::string mom_title = "Reco " + var_symb[0] + "_{" + part_symb[i] + "} " + var_unit[0] + ";True " + var_symb[0] + "_{" + part_symb[i] + "} " + var_unit[0];//Real;Truth
                
                if(debug) cout << "Mom: Working 1" << endl;
                KinMap mom_pr = plot->KinArray(TString(mom_name), mom_nbins, mom_low, mom_high, TString(mom_title),  TString(Form("%s && %s_PDG == 2212", common_cuts_mom.c_str(), part_name_.c_str())));
                /*if(debug) cout << "Mom: Working 2" << endl;
                KinMap mom_pi = plot->KinArray(TString(mom_name), &mom_nbins, &mom_low, &mom_high, TString(mom_title),  TString(Form("%s && TMath::Abs(%s_PDG) == 211", common_cuts_mom.c_str(), part_name_.c_str())));
                if(debug) cout << "Mom: Working 3" << endl;
                KinMap mom_mu = plot->KinArray(TString(mom_name), mom_nbins, mom_low, mom_high, TString(mom_title),  TString(Form("%s && %s_PDG == 13", common_cuts_mom.c_str(), part_name_.c_str())));
                if(debug) cout << "Mom: Working 4" << endl;
                std::string mom_other = part_name_ + "_PDG != 2212 && TMath::Abs(" + part_name_ + "_PDG) != 211 && " + part_name_ + "_PDG != 13 && " + part_name_ + "_PDG != -999" ;
                KinMap mom_ot = plot->KinArray(TString(mom_name), mom_nbins, mom_low, mom_high, TString(mom_title),  TString(Form("%s && %s", common_cuts_mom.c_str(), mom_other.c_str())));

                TH1D * mom_pr_recon   = mom_pr.recon;
                TH1D * mom_pr_truth   = mom_pr.truth;
                //TH2D * mom_pr_smear   = mom_pr.smear;
                TH1D * mom_pr_rtratio = mom_pr.rtratio;
                
                TH1D * mom_pi_recon   = mom_pi.recon;
                TH1D * mom_pi_truth   = mom_pi.truth;
                //TH2D * mom_pi_smear   = mom_pi.smear;
                TH1D * mom_pi_rtratio = mom_pi.rtratio;
                
                TH1D * mom_mu_recon   = mom_mu.recon;
                TH1D * mom_mu_truth   = mom_mu.truth;
                //TH2D * mom_mu_smear   = mom_mu.smear;
                TH1D * mom_mu_rtratio = mom_mu.rtratio;
                
                TH1D * mom_ot_recon   = mom_ot.recon;
                TH1D * mom_ot_truth   = mom_ot.truth;
                //TH2D * mom_ot_smear   = mom_ot.smear;
                TH1D * mom_ot_rtratio = mom_ot.rtratio;
                
                std::vector<TH1D*> mom_reco_list;
                std::vector<std::string> mom_reco_names;
                
                mom_reco_list.push_back( mom_pr_recon ); mom_reco_names.push_back("p");
                mom_reco_list.push_back( mom_pi_recon ); mom_reco_names.push_back("#pi^{#pm}");
                mom_reco_list.push_back( mom_mu_recon ); mom_reco_names.push_back("#mu^{-}");
                mom_reco_list.push_back( mom_ot_recon ); mom_reco_names.push_back("Other");
                
                std::vector<double> mom_reco_per = plot->GetPercentage(mom_reco_list);
                
                if((int)mom_reco_list.size() == (int)mom_reco_names.size() && (int)mom_reco_names.size() == (int)mom_reco_per.size()){
                    
                    string st_mom_reco_title = mom_pr_recon->GetXaxis()->GetTitle();
                    THStack * st_mom_reco = new THStack( (part_name_ + "_reco").c_str() , (";Reco " + st_mom_reco_title +";Counts").c_str());
                    TLegend * mom_reco_leg = plot->Legend(0.25, 0.4, 0.551, 0.362);

                    int n_moms_reco = (int)mom_reco_list.size();
                    for(int dd = 1; dd < n_moms_reco + 1; dd++){
                        TH1D * tmp_hist = mom_reco_list[ n_moms_reco - dd ];//Loop in opposite order;
                        st_mom_reco->Add(tmp_hist);
                        mom_reco_leg->AddEntry(tmp_hist,Form("%s (%.2f%%)",mom_reco_names[ n_moms_reco - dd ].c_str(), mom_reco_per[ n_moms_reco - dd ]) ,"f");
                    }
                    
                    TCanvas * mom_rec_can = new TCanvas((part_name_ + "_reco").c_str(), "", 500,500);
                    mom_rec_can->cd();
                    st_mom_reco->Draw();
                    mom_reco_leg->Draw();
                    TLegend * pot_mom_reco = plot->GetPOT(0.521,0.781);
                    pot_score_reco->Draw();
                    outfile->cd();
                    mom_rec_can->Write();
                }

                std::vector<TH1D*> mom_true_list;
                std::vector<std::string> mom_true_names;
                
                mom_true_list.push_back( mom_pr_recon ); mom_true_names.push_back("p");
                mom_true_list.push_back( mom_pi_recon ); mom_true_names.push_back("#pi^{#pm}");
                mom_true_list.push_back( mom_mu_recon ); mom_true_names.push_back("#mu^{-}");
                mom_true_list.push_back( mom_ot_recon ); mom_true_names.push_back("Other");

                
                
                //TLegend * score_leg = plot->Legend(0.25, 0.4, 0.551, 0.362);
                //THStack * hs_score = new THStack(("h + std_h_score).c_str(),Form(";%s %sScore;Counts", part_name[i].c_str(), AltTit.c_str()));
                */
                //-------------------------//
            }
            
            
            
         /*   if(i>0){
            //Scores:
               // GetRecoHisto(Form("CC1P1Pi_%s_mom/1000",tmp_part_snam.Data()),mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s",common_cuts_score_p.Data()))
                for(int j = 0; j < 2; j++){//LL dEdX loop
                    
                    char buffer[100];
                    sprintf(buffer,"%d",0);
                    std::string common_cuts_score = target + " && accum_level[" + buffer + "] > 4";

                    cout << " common_cuts_score == " << common_cuts_score << endl;
                    
                    //Only one momentum for now: dEdX...
                    
                    double score_low = 0.;
                    double score_high = 1.;
                    int score_nins = 20;
                    
                    if(j > 0){
                        score_low = -100.;
                        score_high = 100.;
                        score_nins = 40;
                    }
                
                    for (int hyp_i = 0; hyp_i < 2; hyp_i++) {//Alt hyp loop
                        
                        //Score:
                        std::string std_score =  std_partvar + hyp[j] + "_score";
                        cout << std_score << endl;
                        std::string AltTit = "";
                        std::string std_h_score = "hs" + part_name[i] + hyp[j] + "_scores";
                        
                        if(hyp_i>0){
                            std_score += "_altH";
                            AltTit = "Alt. ";
                            std_h_score += "_altH";
                        }
                        
                        if(debug) cout << "Working 1" << endl;
                        TH1D * score_pr = plot->GetRecoHisto(TString(std_score.c_str()), score_nins, score_low, score_high, Form("%s %sScore", part_name[i].c_str(), AltTit.c_str()), Form("%s && %s_PDG == 2212", common_cuts_score.c_str(), std_partvar.c_str()));
                        
                        if(debug) cout << "Working 2" << endl;
                        TH1D * score_pi = plot->GetRecoHisto(TString(std_score.c_str()), score_nins, score_low, score_high, Form("%s %sScore", part_name[i].c_str(), AltTit.c_str()), Form("%s && TMath::Abs(%s_PDG) == 211", common_cuts_score.c_str(), std_partvar.c_str()));
                        
                        if(debug) cout << "Working 3" << endl;
                        TH1D * score_mum = plot->GetRecoHisto(TString(std_score.c_str()), score_nins, score_low, score_high, Form("%s %sScore", part_name[i].c_str(), AltTit.c_str()), Form("%s && %s_PDG == 13", common_cuts_score.c_str(), std_partvar.c_str()));
                        
                        if(debug) cout << "Working 3" << endl;
                        TH1D * score_mup = plot->GetRecoHisto(TString(std_score.c_str()), score_nins, score_low, score_high, Form("%s %sScore", part_name[i].c_str(), AltTit.c_str()), Form("%s && %s_PDG == -13", common_cuts_score.c_str(), std_partvar.c_str()));
                        
                        if(debug) cout << "Working 4" << endl;
                        TH1D * score_p0 = plot->GetRecoHisto(TString(std_score.c_str()), score_nins, score_low, score_high, Form("%s %sScore", part_name[i].c_str(), AltTit.c_str()), Form("%s && %s_PDG == 111", common_cuts_score.c_str(), std_partvar.c_str()));
                        
                        if(debug) cout << "Working 5" << endl;
                        TH1D * score_ka = plot->GetRecoHisto(TString(std_score.c_str()), score_nins, score_low, score_high, Form("%s %sScore", part_name[i].c_str(), AltTit.c_str()), Form("%s && TMath::Abs(%s_PDG) == 321", common_cuts_score.c_str(), std_partvar.c_str()));
                        
                        if(debug) cout << "Working 6" << endl;
                        TH1D * score_kz = plot->GetRecoHisto(TString(std_score.c_str()), score_nins, score_low, score_high, Form("%s %sScore", part_name[i].c_str(), AltTit.c_str()), Form("%s && %s_PDG == 311", common_cuts_score.c_str(), std_partvar.c_str()));
                        
                        if(debug) cout << "Working 7" << endl;
                        std::string std_p_other =  std_partvar + "_PDG != 2212 && TMath::Abs(" + std_partvar + "_PDG) != 211 && " + std_partvar + "_PDG != 13 && " + std_partvar + "_PDG != 111 && TMath::Abs(" + std_partvar + "_PDG) != 311 && " + std_partvar + "_PDG != 311 && " + std_partvar + "_PDG != -999 && " + std_partvar + "_PDG != -13";
                        TH1D * score_ot = plot->GetRecoHisto(TString(std_score.c_str()), score_nins, score_low, score_high, Form("%s %sScore", part_name[i].c_str(), AltTit.c_str()), Form("%s && %s", common_cuts_score.c_str(), std_p_other.c_str()));
                        
                        plot->ColFill(score_pr, DrawingStyle::DSProton);
                        plot->ColFill(score_pi, DrawingStyle::DSPion);
                        plot->ColFill(score_mum, DrawingStyle::DSMuonM);
                        plot->ColFill(score_mup, DrawingStyle::DSMuonP);
                        plot->ColFill(score_p0, DrawingStyle::DSPi0);
                        plot->ColFill(score_ka, DrawingStyle::DSKaon);
                        plot->ColFill(score_kz, DrawingStyle::DSKa0);
                        plot->ColFill(score_ot, DrawingStyle::DSOther);

                        std::vector<TH1D*> score_h;
                        std::vector<std::string> score_names;
                        score_h.push_back( score_pr ); score_names.push_back("p");
                        score_h.push_back( score_pi ); score_names.push_back("#pi^{#pm}");
                        score_h.push_back( score_mum ); score_names.push_back("#mu^{-}");
                        score_h.push_back( score_mup ); score_names.push_back("#mu^{+}");
                        score_h.push_back( score_p0 ); score_names.push_back("#pi^{0}");
                        score_h.push_back( score_ka ); score_names.push_back("K^{#pm}");
                        score_h.push_back( score_kz ); score_names.push_back("K^{0}");
                        score_h.push_back( score_ot ); score_names.push_back("Other");
                        
                        std::vector<double> score_per = plot->GetPercentage(score_h);
                        
                        if((int)score_h.size() == (int)score_names.size() && (int)score_names.size() == (int)score_per.size()){
                            
                            TLegend * score_leg = plot->Legend(0.25, 0.4, 0.551, 0.362);
                            THStack * hs_score = new THStack(std_h_score.c_str(),Form(";%s %sScore;Counts", part_name[i].c_str(), AltTit.c_str()));
                            
                            int n_scores = (int)score_h.size();
                            for(int dd = 1; dd < n_scores + 1; dd++){
                                TH1D * tmp_hist = score_h[ n_scores - dd ];//Loop in opposite order;
                                hs_score->Add(tmp_hist);
                                score_leg->AddEntry(tmp_hist,Form("%s (%.2f%%)",score_names[ n_scores - dd ].c_str(), score_per[ n_scores - dd ]) ,"f");
                            }
                            
                            TCanvas * score_can = new TCanvas(std_h_score.c_str(), "", 500,500);
                            score_can->cd();
                            hs_score->Draw();
                            score_leg->Draw();
                            TLegend * pot_score = plot->GetPOT(0.521,0.781);
                            pot_score->Draw();
                            outfile->cd();
                            score_can->Write();
                        }
                
                    }
                }
            } */
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
    
    TH1D * h_mom_p1 = plot->GetRecoHisto(Form("CC1P1Pi_%s_%s/1000",particle.Data(), var.Data()), mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s && CC1P1Pi_%s_PDG == 2212", common_cuts_score_p.Data(), tmp_part_snam.Data()));
    TH1D * h_mom_p2 = plot->GetRecoHisto(Form("CC1P1Pi_%s_%s/1000",particle.Data(), var.Data()), mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s && TMath::Abs(CC1P1Pi_%s_PDG) == 211", common_cuts_score_p.Data(), tmp_part_snam.Data()));
    TH1D * h_mom_p3 = plot->GetRecoHisto(Form("CC1P1Pi_%s_%s/1000",particle.Data(), var.Data()), mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s && CC1P1Pi_%s_PDG == 13", common_cuts_score_p.Data(), tmp_part_snam.Data()));
    TH1D * h_mom_p4 = plot->GetRecoHisto(Form("CC1P1Pi_%s_%s/1000",particle.Data(), var.Data()), mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s && CC1P1Pi_%s_PDG != 13 && TMath::Abs(CC1P1Pi_%s_PDG) != 211 && CC1P1Pi_%s_PDG != 2212", common_cuts_score_p.Data(), tmp_part_snam.Data(), tmp_part_snam.Data(), tmp_part_snam.Data()));
    
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
