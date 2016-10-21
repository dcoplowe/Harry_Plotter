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
#include "TDatime.h"

//#include <TStyle.h>

using namespace std;
//using namespace PlotUtils;

//These are the tuples used for this exercise
//const string testing_mc("/pnfs/minerva/persistent/users/dcoplowe/merged_CC1P1Pi_CC1P1Pi_run00013200.root");

//-->
//const string testing_mc("/pnfs/minerva/persistent/users/dcoplowe/CC1P1Pi_PL13C_041016/grid/central_value/minerva/ana/v10r8p9/00/01/32/00/SIM_minerva_00013200_Subruns_0101-0102-0103-0104-0105_CC1P1PiAnalysis_Ana_Tuple_v10r8p9-dcoplowe.root");

const string testing_mc("/pnfs/minerva/persistent/users/dcoplowe/merged_CC1P1Pi_CC1P1Pi_run00013213.root");

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
    
    //TString savename = "CC1P1PiP_TEST";
    
    //Produce independent particles vars hists:
    string part_snam[3] = {"mu","pr","pi"};
    string part_name[3] = {"Muon", "Proton", "Pion"};
    string part_symb[3] = {"#mu^{-}", "p", "#pi^{#pm}"};
    int    part_colo[3] = {DrawingStyle::Proton, DrawingStyle::Pion, DrawingStyle::Muon};
    
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
            
            //Momentum:
            //int mom_bin[3] = { 20, 20, 20};
            //double mom_range[6] = {0., 30., 0., 2., 0., 1.2};
            //double truemom_range[6] = {0., 40., 0., 20., 0., 30.};
            
            for(int j = 0; j < 2; j++){//PID Method: dEdX or LL.
            
                stringstream sj;
                sj << j;
                
                for(int al = 4; al < 6; al++){
                
                    stringstream sal;
                    sal << al;
                    
                    std::string common_cuts_mom = target + " && accum_level[" + sj.str() + "] >" + sal.str();
                    std::string part_name_ = std_partvar;// + hyp[j];
                    
                    const int mom_nbins = mom_bin[ i ];
                    const double mom_low = mom_range[ 2*i ];
                    const double mom_high = mom_range[ 2*i + 1 ];
                    
                    string tmp_hyp;
                    if(i == 0){
                        tmp_hyp = "";
                        j++;//Only do one hyp loop.
                    }
                    else tmp_hyp = hyp[j];
                    
                    std::string mom_name = part_name_ + "_truemom/1000:" + part_name_ + tmp_hyp + "_mom/1000";
                    std::string mom_title = "Reco " + var_symb[0] + "_{" + part_symb[i] + "} " + var_unit[0] + ";True " + var_symb[0] + "_{" + part_symb[i] + "} " + var_unit[0];//Real;Truth
                    
                    if(debug) cout << "Mom: Working 1" << endl;
                    KinMap mom_pr_map = plot->KinArray(TString(mom_name), mom_nbins, mom_low, mom_high, TString(mom_title),  TString(Form("%s && %s_PDG == 2212", common_cuts_mom.c_str(), part_name_.c_str())));
                    if(debug) cout << "Mom: Working 2" << endl;
                    //KinMap mom_pi_map = plot->KinArray(TString(mom_name), mom_nbins, mom_low, mom_high, TString(mom_title),  TString(Form("%s && TMath::Abs(%s_PDG) == 211", common_cuts_mom.c_str(), part_name_.c_str())));
                    
                    KinMap mom_piP_map = plot->KinArray(TString(mom_name), mom_nbins, mom_low, mom_high, TString(mom_title),  TString(Form("%s && %s_PDG == 211", common_cuts_mom.c_str(), part_name_.c_str())));
                    KinMap mom_piM_map = plot->KinArray(TString(mom_name), mom_nbins, mom_low, mom_high, TString(mom_title),  TString(Form("%s && %s_PDG == -211", common_cuts_mom.c_str(), part_name_.c_str())));
                    
                    
                    if(debug) cout << "Mom: Working 3" << endl;
                    KinMap mom_mum_map = plot->KinArray(TString(mom_name), mom_nbins, mom_low, mom_high, TString(mom_title),  TString(Form("%s && %s_PDG == 13", common_cuts_mom.c_str(), part_name_.c_str())));
                    if(debug) cout << "Mom: Working 4" << endl;
                    KinMap mom_mup_map = plot->KinArray(TString(mom_name), mom_nbins, mom_low, mom_high, TString(mom_title),  TString(Form("%s && %s_PDG == -13", common_cuts_mom.c_str(), part_name_.c_str())));
                    if(debug) cout << "Mom: Working 5" << endl;
                    KinMap mom_p0_map = plot->KinArray(TString(mom_name), mom_nbins, mom_low, mom_high, TString(mom_title),  TString(Form("%s && %s_PDG == 111", common_cuts_mom.c_str(), part_name_.c_str())));
                    if(debug) cout << "Mom: Working 6" << endl;
                    KinMap mom_ka_map = plot->KinArray(TString(mom_name), mom_nbins, mom_low, mom_high, TString(mom_title),  TString(Form("%s && TMath::Abs(%s_PDG) == 321", common_cuts_mom.c_str(), part_name_.c_str())));
                    if(debug) cout << "Mom: Working 7" << endl;
                    //KinMap mom_kz_map = plot->KinArray(TString(mom_name), mom_nbins, mom_low, mom_high, TString(mom_title),  TString(Form("%s && %s_PDG == 311", common_cuts_mom.c_str(), part_name_.c_str())));
                    if(debug) cout << "Mom: Working 8" << endl;
                    std::string mom_other = part_name_ + "_PDG != 2212 && TMath::Abs(" + part_name_ + "_PDG) != 211 && TMath::Abs(" + part_name_ + "_PDG) != 13 && " + part_name_ + "_PDG != 111 && TMath::Abs(" + part_name_ + "_PDG) != 321 && " + part_name_ + "_PDG != 311 &&" + part_name_ + "_PDG != -999";
                    KinMap mom_ot_map = plot->KinArray(TString(mom_name), mom_nbins, mom_low, mom_high, TString(mom_title),  TString(Form("%s && %s", common_cuts_mom.c_str(), mom_other.c_str())));
                    
                    std::vector<KinMap> mom_map;
                    std::vector<std::string> mom_map_names;
                    std::vector<int> mom_map_cols;
                    
                    mom_map.push_back( mom_pr_map );    mom_map_names.push_back("p");           mom_map_cols.push_back( DrawingStyle::Proton );
                    mom_map.push_back( mom_piP_map );   mom_map_names.push_back("#pi^{+}");     mom_map_cols.push_back( DrawingStyle::Pion   );
                    mom_map.push_back( mom_piM_map );   mom_map_names.push_back("#pi^{-}");     mom_map_cols.push_back( DrawingStyle::Ka0    );
                    mom_map.push_back( mom_mum_map);    mom_map_names.push_back("#mu^{-}");     mom_map_cols.push_back( DrawingStyle::MuonM  );
                    mom_map.push_back( mom_mup_map);    mom_map_names.push_back("#mu^{+}");     mom_map_cols.push_back( DrawingStyle::MuonP  );
                    mom_map.push_back( mom_p0_map );    mom_map_names.push_back("#pi^{0}");     mom_map_cols.push_back( DrawingStyle::Pi0    );
                    mom_map.push_back( mom_ka_map );    mom_map_names.push_back("K^{#pm}");     mom_map_cols.push_back( DrawingStyle::Kaon   );
                    //mom_map.push_back( mom_kz_map );    mom_map_names.push_back("K^{0}");       mom_map_cols.push_back( DrawingStyle::Ka0    );
                    mom_map.push_back( mom_ot_map );    mom_map_names.push_back("Other");       mom_map_cols.push_back( DrawingStyle::Other  );
                    
                    std::vector<TH1D*> mom_recon;
                    std::vector<TH1D*> mom_truth;
                    std::vector<TH2D*> mom_smear;
                    std::vector<TH1D*> mom_ratio;
                    
                    int mom_map_size = (int)mom_map.size();
                    
                    for(int mpc = 0; mpc < mom_map_size; mpc++){
                        plot->ColFill( mom_map[mpc].recon, mom_map_cols[mpc]);
                        plot->ColFill( mom_map[mpc].truth, mom_map_cols[mpc]);
                        plot->ColFill( mom_map[mpc].ratio, mom_map_cols[mpc]);
                        
                        mom_recon.push_back( mom_map[mpc].recon );
                        mom_truth.push_back( mom_map[mpc].truth );
                        mom_smear.push_back( mom_map[mpc].smear );
                        mom_ratio.push_back( mom_map[mpc].ratio );
                    }
                    
                    std::vector<double> mom_recon_per = plot->GetPercentage( mom_recon );
                    std::vector<double> mom_truth_per = plot->GetPercentage( mom_truth );
                    std::vector<double> mom_ratio_per = plot->GetPercentage( mom_ratio );
                    
                    string mom_recon_title = Form(";%s;%s", mom_recon[0]->GetXaxis()->GetTitle(), mom_recon[0]->GetYaxis()->GetTitle());
                    string mom_truth_title = Form(";%s;%s", mom_truth[0]->GetXaxis()->GetTitle(), mom_truth[0]->GetYaxis()->GetTitle());
                    string mom_smear_title = Form(";%s;%s", mom_smear[0]->GetXaxis()->GetTitle(), mom_smear[0]->GetYaxis()->GetTitle());
                    string mom_ratio_title = Form(";%s;%s", mom_ratio[0]->GetXaxis()->GetTitle(), mom_ratio[0]->GetYaxis()->GetTitle());
                    
                    THStack * mom_recon_tot = new THStack( (part_name_ + tmp_hyp + "_recon").c_str() , mom_recon_title.c_str());
                    THStack * mom_truth_tot = new THStack( (part_name_ + tmp_hyp + "_truth").c_str(), mom_truth_title.c_str());
                    THStack * mom_ratio_tot = new THStack( (part_name_ + tmp_hyp + "_ratio").c_str(),  mom_ratio_title.c_str());
                    TH2D * mom_smear_tot = (TH2D*)mom_smear[0]->Clone( (part_name_ + tmp_hyp + "_smear").c_str() );//Just add all of these histos.
                    
                    TLegend * mom_recon_leg = plot->Legend(0.25, 0.4, 0.551, 0.362);
                    TLegend * mom_truth_leg = plot->Legend(0.25, 0.4, 0.551, 0.362);
                    TLegend * mom_ratio_leg = plot->Legend(0.25, 0.4, 0.551, 0.362);
                    
                    for(int mpc = 1; mpc < mom_map_size + 1; mpc++){
                        
                        mom_recon_tot->Add( mom_recon[mom_map_size - mpc] );
                        mom_truth_tot->Add( mom_truth[mom_map_size - mpc] );
                        mom_ratio_tot->Add( mom_ratio[mom_map_size - mpc] );
                        
                        if( (mpc - 1) < mom_map_size){
                            string mom_part_names = mom_map_names[mpc - 1];
                            mom_recon_leg->AddEntry(mom_recon[ mpc - 1 ], Form("%s (%.2f%%)", mom_part_names.c_str(), mom_recon_per[ mpc - 1 ]), "f");
                            mom_truth_leg->AddEntry(mom_truth[ mpc - 1 ], Form("%s (%.2f%%)", mom_part_names.c_str(), mom_truth_per[ mpc - 1 ]), "f");
                            mom_ratio_leg->AddEntry(mom_ratio[ mpc - 1 ], Form("%s (%.2f%%)", mom_part_names.c_str(), mom_ratio_per[ mpc - 1 ]), "f");
                        }
                        
                        if(mpc < mom_map_size) mom_smear_tot->Add( mom_smear[mpc] );
                    }
                    //-------------------------//
                    
                    TLegend * mom_recon_pot = plot->GetPOT(0.521,0.781);
                    TLegend * mom_truth_pot = plot->GetPOT(0.521,0.781);
                    TLegend * mom_ratio_pot = plot->GetPOT(0.521,0.781);
                    
                    string ac_lev = "_AL";
                    if(al == 4){
                        ac_lev += "5";
                    }
                    else ac_lev += "6";
                    
                    TCanvas * mom_recon_can = new TCanvas( (part_name_ + tmp_hyp + ac_lev + "_recon").c_str(), "", 500, 500);
                    mom_recon_can->cd();
                    mom_recon_tot->Draw();
                    mom_recon_leg->Draw();
                    mom_recon_pot->Draw();
                    outfile->cd();
                    mom_recon_can->Write();
                    
                    TCanvas * mom_truth_can = new TCanvas( (part_name_ + tmp_hyp + ac_lev + "_truth").c_str(), "", 500, 500);
                    mom_truth_can->cd();
                    mom_truth_tot->Draw();
                    mom_truth_leg->Draw();
                    mom_truth_pot->Draw();
                    outfile->cd();
                    mom_truth_can->Write();
                    
                    TCanvas * mom_smear_can = new TCanvas( (part_name_ + tmp_hyp + ac_lev + "_smear").c_str(), "", 500, 500);
                    mom_smear_can->cd();
                    mom_smear_tot->Draw("COLZ");
                    //mom_smear_pot->Draw();
                    outfile->cd();
                    mom_smear_can->Write();
                    
                    TCanvas * mom_ratio_can = new TCanvas( (part_name_ + tmp_hyp + ac_lev + "_ratio").c_str(), "", 500, 500);
                    mom_ratio_can->cd();
                    mom_ratio_tot->Draw();
                    mom_ratio_leg->Draw();
                    mom_ratio_pot->Draw();
                    outfile->cd();
                    mom_ratio_can->Write();
                    
                }
            }
            
            if(i>0){
            //Scores:
               // GetRecoHisto(Form("CC1P1PiP_%s_mom/1000",tmp_part_snam.Data()),mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s",common_cuts_score_p.Data()))
                for(int j = 0; j < 2; j++){//LL dEdX loop
                    
                    stringstream sj;
                    sj << j;
                    
                    for(int al = 4; al < 6; al++){
                        
                        stringstream sal;
                        sal << al;
                        
                        std::string common_cuts_score = target + " && accum_level[" + sj.str() + "] > " + sal.str();
                        
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
                            //TH1D * score_pi = plot->GetRecoHisto(TString(std_score.c_str()), score_nins, score_low, score_high, Form("%s %sScore", part_name[i].c_str(), AltTit.c_str()), Form("%s && TMath::Abs(%s_PDG) == 211", common_cuts_score.c_str(), std_partvar.c_str()));
                            
                            TH1D * score_piP = plot->GetRecoHisto(TString(std_score.c_str()), score_nins, score_low, score_high, Form("%s %sScore", part_name[i].c_str(), AltTit.c_str()), Form("%s && %s_PDG == 211", common_cuts_score.c_str(), std_partvar.c_str()));
                            
                            TH1D * score_piM = plot->GetRecoHisto(TString(std_score.c_str()), score_nins, score_low, score_high, Form("%s %sScore", part_name[i].c_str(), AltTit.c_str()), Form("%s && %s_PDG == -211", common_cuts_score.c_str(), std_partvar.c_str()));
                            
                            
                            if(debug) cout << "Working 3" << endl;
                            TH1D * score_mum = plot->GetRecoHisto(TString(std_score.c_str()), score_nins, score_low, score_high, Form("%s %sScore", part_name[i].c_str(), AltTit.c_str()), Form("%s && %s_PDG == 13", common_cuts_score.c_str(), std_partvar.c_str()));
                            
                            if(debug) cout << "Working 3" << endl;
                            TH1D * score_mup = plot->GetRecoHisto(TString(std_score.c_str()), score_nins, score_low, score_high, Form("%s %sScore", part_name[i].c_str(), AltTit.c_str()), Form("%s && %s_PDG == -13", common_cuts_score.c_str(), std_partvar.c_str()));
                            
                            if(debug) cout << "Working 4" << endl;
                            TH1D * score_p0 = plot->GetRecoHisto(TString(std_score.c_str()), score_nins, score_low, score_high, Form("%s %sScore", part_name[i].c_str(), AltTit.c_str()), Form("%s && %s_PDG == 111", common_cuts_score.c_str(), std_partvar.c_str()));
                            
                            if(debug) cout << "Working 5" << endl;
                            TH1D * score_ka = plot->GetRecoHisto(TString(std_score.c_str()), score_nins, score_low, score_high, Form("%s %sScore", part_name[i].c_str(), AltTit.c_str()), Form("%s && TMath::Abs(%s_PDG) == 321", common_cuts_score.c_str(), std_partvar.c_str()));
                            
                            if(debug) cout << "Working 6" << endl;
                            //TH1D * score_kz = plot->GetRecoHisto(TString(std_score.c_str()), score_nins, score_low, score_high, Form("%s %sScore", part_name[i].c_str(), AltTit.c_str()), Form("%s && %s_PDG == 311", common_cuts_score.c_str(), std_partvar.c_str()));
                            
                            if(debug) cout << "Working 7" << endl;
                            //std::string std_p_other =  std_partvar + "_PDG != 2212 && TMath::Abs(" + std_partvar + "_PDG) != 211 && " + std_partvar + "_PDG != 13 && " + std_partvar + "_PDG != 111 && TMath::Abs(" + std_partvar + "_PDG) != 311 && " + std_partvar + "_PDG != 311 && " + std_partvar + "_PDG != -999 && " + std_partvar + "_PDG != -13";
                            //TH1D * score_ot = plot->GetRecoHisto(TString(std_score.c_str()), score_nins, score_low, score_high, Form("%s %sScore", part_name[i].c_str(), AltTit.c_str()), Form("%s && %s", common_cuts_score.c_str(), std_p_other.c_str()));
                            
                            std::string std_p_other =  std_partvar + "_PDG != 2212 && TMath::Abs(" + std_partvar + "_PDG) != 211 && " + std_partvar + "_PDG != 13 && " + std_partvar + "_PDG != 111 && TMath::Abs(" + std_partvar + "_PDG) != 311 && " + std_partvar + "_PDG != -999 && " + std_partvar + "_PDG != -13";
                            TH1D * score_ot = plot->GetRecoHisto(TString(std_score.c_str()), score_nins, score_low, score_high, Form("%s %sScore", part_name[i].c_str(), AltTit.c_str()), Form("%s && %s", common_cuts_score.c_str(), std_p_other.c_str()));
                            
                            plot->ColFill(score_pr, DrawingStyle::Proton);
                            plot->ColFill(score_piP, DrawingStyle::Pion);
                            plot->ColFill(score_piM, DrawingStyle::Ka0);
                            plot->ColFill(score_mum, DrawingStyle::MuonM);
                            plot->ColFill(score_mup, DrawingStyle::MuonP);
                            plot->ColFill(score_p0, DrawingStyle::Pi0);
                            plot->ColFill(score_ka, DrawingStyle::Kaon);
                            //plot->ColFill(score_kz, DrawingStyle::Ka0);
                            plot->ColFill(score_ot, DrawingStyle::Other);
                            
                            std::vector<TH1D*> score_h;
                            std::vector<std::string> score_names;
                            score_h.push_back( score_pr ); score_names.push_back("p");
                            score_h.push_back( score_piP ); score_names.push_back("#pi^{+}");
                            score_h.push_back( score_piM ); score_names.push_back("#pi^{-}");
                            score_h.push_back( score_mum ); score_names.push_back("#mu^{-}");
                            score_h.push_back( score_mup ); score_names.push_back("#mu^{+}");
                            score_h.push_back( score_p0 ); score_names.push_back("#pi^{0}");
                            score_h.push_back( score_ka ); score_names.push_back("K^{#pm}");
                            //score_h.push_back( score_kz ); score_names.push_back("K^{0}");
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
                                
                                string ac_lev = "_AL";
                                if(al == 4){
                                    ac_lev += "5";
                                }
                                else ac_lev += "6";
                                
                                TCanvas * score_can = new TCanvas((std_h_score + ac_lev).c_str(), "", 500,500);
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
                }
            }
            //-----------------------------------------------------------------------------------------------------------------------------------------------------------------//
        }
        
        //dpTT Distributions:
        //We want to understand the following in dpTT, dpTT_pi_dir, dpTT_pi, dpTT_pr and dpTT_pr_dir:
        //Want to have four sets of the following dists.
        //What is the make up 1) topologically  --> CC1P1PiP, CC2P, CC2Pi, CC1Pi01P, CC1Pi01Pi, Other <-- Use only PDG codes to determine these
        //                    2) nuclear medium --> H, C, Other,
        //                    3) mis-PID effect --> Correct P,Pi : Correct P, Wrong Pi : Correct Pi, Wrong P : Wrong P,Pi, Other <-- Use only PDG codes to determine these
        //
        //One for each of the following: dEdX (+ Michel), LLR (+ Michel)
        
        std::vector<string> dpTT_type;
        std::vector<string> truedpTT_type;
        std::vector<string> dpTT_type_title;

        dpTT_type.push_back( (flag + "dpTT").c_str() );         truedpTT_type.push_back( (flag + "truedpTT").c_str() );         dpTT_type_title.push_back( "#hat{#nu}#times#hat{#it{l}}   " );
        dpTT_type.push_back( (flag + "dpTT_pi").c_str() );      truedpTT_type.push_back( (flag + "truedpTT_pi").c_str() );      dpTT_type_title.push_back( "#hat{#nu}#times#hat{p}_{#pi}   " );
        dpTT_type.push_back( (flag + "dpTT_pi_dir").c_str() );  truedpTT_type.push_back( (flag + "truedpTT_pi_dir").c_str() );  dpTT_type_title.push_back( "#hat{#nu}#times#hat{d}_{#pi}   " );
        dpTT_type.push_back( (flag + "dpTT_pr").c_str() );      truedpTT_type.push_back( (flag + "truedpTT_pr").c_str() );      dpTT_type_title.push_back( "#hat{#nu}#times#hat{p}_{p}   " );
        dpTT_type.push_back( (flag + "dpTT_pr_dir").c_str() );  truedpTT_type.push_back( (flag + "truedpTT_pr_dir").c_str() );  dpTT_type_title.push_back( "#hat{#nu}#times#hat{d}_{p}   " );
        
        for(int i = 0; i < (int)dpTT_type.size(); i++){//dpTT distribution.
            string dpTT_title = dpTT_type_title[i] + " #delta#it{p}^{reco}_{TT} (GeV/#it{c});" + dpTT_type_title[i] + " #delta#it{p}^{true}_{TT} (GeV/#it{c})";
            string dpTT_var = truedpTT_type[i] + ":" + dpTT_type[i] + "_EX";
            for(int j = 0; j < 2; j++){//PID method (will actually be accum level[0,1] > cut)
                for(int k = 4; k < 6; k++){//dEdX (+ Michel), LLR (+ Michel) --> cut 4(5)
                    
                    //Base cuts:
                    stringstream sj;
                    sj << j;
                    stringstream sk;
                    sk << k;
                    string base_cuts = "target_region == 1 && accum_level[" + sj.str() + "] > " + sk.str();
                    
                    //General dpTT title:
                    string dpTT_g_title = dpTT_type[i];
                    
                    if(j == 0){
                        dpTT_g_title  += "_dEdX";
                    }
                    else dpTT_g_title  += "_LLR";
                    
                    dpTT_g_title += "_AL";
                    
                    if(k == 4){
                        dpTT_g_title += "5";
                    }
                    else dpTT_g_title += "6";
                    
                    string dpTT_top_title = "top_" + dpTT_g_title;
                    
                    //1) topologically  --> CC1P1PiP, CC2P, CC2Pi, CC1Pi01P, CC1Pi01Pi, Other <-- Use only PDG codes to determine these
                    string CC1P1PiP = flag + "mu_PDG == 13 && ((" + flag + "pr_PDG == 2212 && " + flag + "pi_PDG == 211 ) ||  (" + flag + "pr_PDG == 211 && " + flag + "pi_PDG == 2212 ))";
                    KinMap dpTT_CC1P1PiP_map = plot->KinArray(TString(dpTT_var), 21, -300, 300, TString(dpTT_title),  TString(Form("%s && %s", base_cuts.c_str(), CC1P1PiP.c_str())));

                    string CC1P1PiM = flag + "mu_PDG == 13 && ((" + flag + "pr_PDG == 2212 && " + flag + "pi_PDG == -211 ) ||  (" + flag + "pr_PDG == -211 && " + flag + "pi_PDG == 2212 ))";
                    KinMap dpTT_CC1P1PiM_map = plot->KinArray(TString(dpTT_var), 21, -300, 300, TString(dpTT_title),  TString(Form("%s && %s", base_cuts.c_str(), CC1P1PiM.c_str())));
                    
                    string CC2P = flag + "mu_PDG == 13 && " + flag + "pr_PDG == 2212 && " + flag + "pi_PDG == 2212 ";
                    KinMap dpTT_CC2P_map = plot->KinArray(TString(dpTT_var), 21, -300, 300, TString(dpTT_title),  TString(Form("%s && %s", base_cuts.c_str(), CC2P.c_str())));
                    
                    string CC2Pi = flag + "mu_PDG == 13 && " + flag + "pr_PDG == 211 && " + flag + "pi_PDG == 211 ";
                    KinMap dpTT_CC2Pi_map = plot->KinArray(TString(dpTT_var), 21, -300, 300, TString(dpTT_title),  TString(Form("%s && %s", base_cuts.c_str(), CC2Pi.c_str())));
                    
                    string CC1Pi01P = flag + "mu_PDG == 13 && ((" + flag + "pr_PDG == 2212 && " + flag + "pi_PDG == 111 ) ||  (" + flag + "pr_PDG == 111 && " + flag + "pi_PDG == 2212 ))";
                    KinMap dpTT_CC1Pi01P_map = plot->KinArray(TString(dpTT_var), 21, -300, 300, TString(dpTT_title),  TString(Form("%s && %s", base_cuts.c_str(), CC1Pi01P.c_str())));
                    
                    string CC1Pi01Pi = flag + "mu_PDG == 13 && ((" + flag + "pr_PDG == 111 && " + flag + "pi_PDG == 211 ) ||  (" + flag + "pr_PDG == 211 && " + flag + "pi_PDG == 111 ))";
                    KinMap dpTT_CC1Pi01Pi_map = plot->KinArray(TString(dpTT_var), 21, -300, 300, TString(dpTT_title),  TString(Form("%s && %s", base_cuts.c_str(), CC1Pi01Pi.c_str())));
                    
                    /*string TOther = "(" + flag + "mu_PDG == 13 ||" + flag + "mu_PDG == -13) && !( ((" + flag + "pr_PDG == 111 && " + flag + "pi_PDG == 211 ) ||  (" + flag + "pr_PDG == 211 && " + flag + "pi_PDG == 111 ))";
                    TOther += "|| ((" + flag + "pr_PDG == 2212 && " + flag + "pi_PDG == 211 ) ||  (" + flag + "pr_PDG == 211 && " + flag + "pi_PDG == 2212 ))";
                    TOther += "|| "+ flag + "pr_PDG == 2212 && " + flag + "pi_PDG == 2212 ";
                    TOther += "|| (" + flag + "pr_PDG == 211 && " + flag + "pi_PDG == 211)";
                    TOther += ")";*/
                    KinMap dpTT_TOther_map = plot->KinArray(TString(dpTT_var), 21, -300, 300, TString(dpTT_title),  TString(Form("%s", base_cuts.c_str())));//No cuts just sum away the other topologies.
                    
                    std::vector<KinMap> dpTT_top_map;
                    std::vector<std::string> dpTT_top_names;
                    std::vector<int> dpTT_top_cols;
                    
                    dpTT_top_map.push_back( dpTT_CC1P1PiP_map );    dpTT_top_names.push_back( "CC1p1#pi^{+}" );         dpTT_top_cols.push_back( DrawingStyle::T1P1Pi);
                    dpTT_top_map.push_back( dpTT_CC1P1PiM_map );    dpTT_top_names.push_back( "CC1p1#pi^{-}" );         dpTT_top_cols.push_back( DrawingStyle::T1P1PiM );
                    dpTT_top_map.push_back( dpTT_CC2P_map );        dpTT_top_names.push_back( "CC2p");                  dpTT_top_cols.push_back( DrawingStyle::T2Pr );
                    dpTT_top_map.push_back( dpTT_CC2Pi_map );       dpTT_top_names.push_back( "CC2#pi^{+}" );           dpTT_top_cols.push_back( DrawingStyle::T2Pi );
                    dpTT_top_map.push_back( dpTT_CC1Pi01P_map );    dpTT_top_names.push_back( "CC1p1#pi^{0}" );         dpTT_top_cols.push_back( DrawingStyle::T1P1Pi0 );
                    dpTT_top_map.push_back( dpTT_CC1Pi01Pi_map );   dpTT_top_names.push_back( "CC1#pi^{0}1#pi^{+}" );   dpTT_top_cols.push_back( DrawingStyle::T1Pi1Pi0 );
                    dpTT_top_map.push_back( dpTT_TOther_map );      dpTT_top_names.push_back( "Other" );                dpTT_top_cols.push_back( DrawingStyle::Other );
                    
                    std::vector<TH1D*> dpTT_top_recon;
                    std::vector<TH1D*> dpTT_top_truth;
                    std::vector<TH2D*> dpTT_top_smear;
                    std::vector<TH1D*> dpTT_top_ratio;
                    
                    for(int mpc = 0; mpc < (int)dpTT_top_map.size(); mpc++){
                        plot->ColFill( dpTT_top_map[mpc].recon, dpTT_top_cols[mpc]);
                        plot->ColFill( dpTT_top_map[mpc].truth, dpTT_top_cols[mpc]);
                        plot->ColFill( dpTT_top_map[mpc].ratio, dpTT_top_cols[mpc]);
                        
                        if(mpc < (int)dpTT_top_map.size() - 1 ){//Remove all the other topologies from other:
                        dpTT_top_map[ (int)dpTT_top_map.size() - 1 ].recon->Add(dpTT_top_map[mpc].recon, -1.);
                        dpTT_top_map[ (int)dpTT_top_map.size() - 1 ].truth->Add(dpTT_top_map[mpc].truth, -1.);
                        dpTT_top_map[ (int)dpTT_top_map.size() - 1 ].smear->Add(dpTT_top_map[mpc].smear, -1.);
                        dpTT_top_map[ (int)dpTT_top_map.size() - 1 ].ratio->Add(dpTT_top_map[mpc].ratio, -1.);
                        }
                        
                        dpTT_top_recon.push_back( dpTT_top_map[mpc].recon );
                        dpTT_top_truth.push_back( dpTT_top_map[mpc].truth );
                        dpTT_top_smear.push_back( dpTT_top_map[mpc].smear );
                        dpTT_top_ratio.push_back( dpTT_top_map[mpc].ratio );
                    }
                    
                    std::vector<double> dpTT_top_recon_per = plot->GetPercentage( dpTT_top_recon );
                    std::vector<double> dpTT_top_truth_per = plot->GetPercentage( dpTT_top_truth );
                    std::vector<double> dpTT_top_ratio_per = plot->GetPercentage( dpTT_top_ratio );
                    
                    string dpTT_top_recon_title = Form(";%s;%s", dpTT_top_recon[0]->GetXaxis()->GetTitle(), dpTT_top_recon[0]->GetYaxis()->GetTitle());
                    string dpTT_top_truth_title = Form(";%s;%s", dpTT_top_truth[0]->GetXaxis()->GetTitle(), dpTT_top_truth[0]->GetYaxis()->GetTitle());
                    string dpTT_top_smear_title = Form(";%s;%s", dpTT_top_smear[0]->GetXaxis()->GetTitle(), dpTT_top_smear[0]->GetYaxis()->GetTitle());
                    string dpTT_top_ratio_title = Form(";%s;%s", dpTT_top_ratio[0]->GetXaxis()->GetTitle(), dpTT_top_ratio[0]->GetYaxis()->GetTitle());
                    
                    THStack * dpTT_top_recon_tot = new THStack( (dpTT_top_title + "_recon").c_str() , dpTT_top_recon_title.c_str());
                    THStack * dpTT_top_truth_tot = new THStack( (dpTT_top_title + "_truth").c_str(), dpTT_top_truth_title.c_str());
                    THStack * dpTT_top_ratio_tot = new THStack( (dpTT_top_title + "_ratio").c_str(),  dpTT_top_ratio_title.c_str());
                    TH2D * dpTT_top_smear_tot = (TH2D*)dpTT_top_smear[0]->Clone( (dpTT_top_title + "_smear").c_str() );//Just add all of these histos.
                    
                    TLegend * dpTT_top_recon_leg = plot->Legend(0.25, 0.4, 0.551, 0.362);
                    TLegend * dpTT_top_truth_leg = plot->Legend(0.25, 0.4, 0.551, 0.362);
                    TLegend * dpTT_top_ratio_leg = plot->Legend(0.25, 0.4, 0.551, 0.362);
                    
                    for(int mpc = 1; mpc < (int)dpTT_top_map.size() + 1; mpc++){
                        
                        dpTT_top_recon_tot->Add( dpTT_top_recon[(int)dpTT_top_map.size() - mpc] );
                        dpTT_top_truth_tot->Add( dpTT_top_truth[(int)dpTT_top_map.size() - mpc] );
                        dpTT_top_ratio_tot->Add( dpTT_top_ratio[(int)dpTT_top_map.size() - mpc] );
                        
                        if( (mpc - 1) < (int)dpTT_top_map.size()){
                            string dpTT_top_name = dpTT_top_names[mpc - 1];
                            dpTT_top_recon_leg->AddEntry(dpTT_top_recon[ mpc - 1 ], Form("%s (%.2f%%)", dpTT_top_name.c_str(), dpTT_top_recon_per[ mpc - 1 ]), "f");
                            dpTT_top_truth_leg->AddEntry(dpTT_top_truth[ mpc - 1 ], Form("%s (%.2f%%)", dpTT_top_name.c_str(), dpTT_top_truth_per[ mpc - 1 ]), "f");
                            dpTT_top_ratio_leg->AddEntry(dpTT_top_ratio[ mpc - 1 ], Form("%s (%.2f%%)", dpTT_top_name.c_str(), dpTT_top_ratio_per[ mpc - 1 ]), "f");
                        }
                        
                        if(mpc < (int)dpTT_top_map.size()) dpTT_top_smear_tot->Add( dpTT_top_smear[mpc] );
                    }
                    //-------------------------//
                    
                    TLegend * dpTT_top_recon_pot = plot->GetPOT(0.521,0.781);
                    TLegend * dpTT_top_truth_pot = plot->GetPOT(0.521,0.781);
                    TLegend * dpTT_top_ratio_pot = plot->GetPOT(0.521,0.781);
                    
                    TCanvas * dpTT_top_recon_can = new TCanvas( (dpTT_top_title + "_recon").c_str(), "", 500, 500);
                    dpTT_top_recon_can->cd();
                    dpTT_top_recon_tot->Draw();
                    dpTT_top_recon_leg->Draw();
                    dpTT_top_recon_pot->Draw();
                    outfile->cd();
                    dpTT_top_recon_can->Write();
                    
                    TCanvas * dpTT_top_truth_can = new TCanvas( (dpTT_top_title+ "_truth").c_str(), "", 500, 500);
                    dpTT_top_truth_can->cd();
                    dpTT_top_truth_tot->Draw();
                    dpTT_top_truth_leg->Draw();
                    dpTT_top_truth_pot->Draw();
                    outfile->cd();
                    dpTT_top_truth_can->Write();
                    
                    TCanvas * dpTT_top_smear_can = new TCanvas( (dpTT_top_title + "_smear").c_str(), "", 500, 500);
                    dpTT_top_smear_can->cd();
                    dpTT_top_smear_tot->Draw("COLZ");
                    //mom_smear_pot->Draw();
                    outfile->cd();
                    dpTT_top_smear_can->Write();
                    
                    TCanvas * dpTT_top_ratio_can = new TCanvas( (dpTT_top_title + "_ratio").c_str(), "", 500, 500);
                    dpTT_top_ratio_can->cd();
                    dpTT_top_ratio_tot->Draw();
                    dpTT_top_ratio_leg->Draw();
                    dpTT_top_ratio_pot->Draw();
                    outfile->cd();
                    dpTT_top_ratio_can->Write();
                    
                    //------------------------------------------------------------------------------------------------------------------------------------------------------
                    //------------------------------------------------------------------------------------------------------------------------------------------------------
                    //2) nuclear medium --> H, C, Other + signal overlay.
                    
                    string dpTT_tar_title = "tar_" + dpTT_g_title;
                    
                    KinMap dpTT_Htar_map = plot->KinArray(TString(dpTT_var), 21, -300, 300, TString(dpTT_title),  TString(Form("%s && mc_targetZ == 1", base_cuts.c_str())));
                    KinMap dpTT_Ctar_map = plot->KinArray(TString(dpTT_var), 21, -300, 300, TString(dpTT_title),  TString(Form("%s && mc_targetZ == 6", base_cuts.c_str())));
                    KinMap dpTT_Pbtar_map = plot->KinArray(TString(dpTT_var), 21, -300, 300, TString(dpTT_title),  TString(Form("%s && mc_targetZ == 82", base_cuts.c_str())));
                    string tar_other = "mc_targetZ != 1 && mc_targetZ != 6 && mc_targetZ != 82 && mc_targetZ != -999";
                    KinMap dpTT_Othertar_map = plot->KinArray(TString(dpTT_var), 21, -300, 300, TString(dpTT_title),  TString(Form("%s && %s", base_cuts.c_str(), tar_other.c_str())));
                    
                    std::vector<KinMap> dpTT_tar_map;
                    std::vector<std::string> dpTT_tar_names;
                    std::vector<int> dpTT_tar_cols;
                    
                    dpTT_tar_map.push_back( dpTT_Htar_map );        dpTT_tar_names.push_back( "Hydrogen" ); dpTT_tar_cols.push_back( DrawingStyle::Yellow );
                    dpTT_tar_map.push_back( dpTT_Ctar_map );        dpTT_tar_names.push_back( "Carbon" );   dpTT_tar_cols.push_back( DrawingStyle::Blue );
                    dpTT_tar_map.push_back( dpTT_Pbtar_map );       dpTT_tar_names.push_back( "Lead" );     dpTT_tar_cols.push_back( DrawingStyle::Red );
                    dpTT_tar_map.push_back( dpTT_Othertar_map );    dpTT_tar_names.push_back( "Other" );    dpTT_tar_cols.push_back( DrawingStyle::Gray );
                    
                    std::vector<TH1D*> dpTT_tar_recon;
                    std::vector<TH1D*> dpTT_tar_truth;
                    
                    for(int mpc = 0; mpc < (int)dpTT_tar_map.size(); mpc++){
                        plot->ColFill( dpTT_tar_map[mpc].recon, dpTT_tar_cols[mpc]);
                        plot->ColFill( dpTT_tar_map[mpc].truth, dpTT_tar_cols[mpc]);
                
                        dpTT_tar_recon.push_back( dpTT_tar_map[mpc].recon );
                        dpTT_tar_truth.push_back( dpTT_tar_map[mpc].truth );
                    }

                    std::vector<double> dpTT_tar_recon_per = plot->GetPercentage( dpTT_tar_recon );
                    std::vector<double> dpTT_tar_truth_per = plot->GetPercentage( dpTT_tar_truth );
                    
                    string dpTT_tar_recon_title = Form(";%s;%s", dpTT_tar_recon[0]->GetXaxis()->GetTitle(), dpTT_tar_recon[0]->GetYaxis()->GetTitle());
                    string dpTT_tar_truth_title = Form(";%s;%s", dpTT_tar_truth[0]->GetXaxis()->GetTitle(), dpTT_tar_truth[0]->GetYaxis()->GetTitle());
                    
                    THStack * dpTT_tar_recon_tot = new THStack( (dpTT_tar_title + "_recon").c_str() , dpTT_tar_recon_title.c_str());
                    THStack * dpTT_tar_truth_tot = new THStack( (dpTT_tar_title + "_truth").c_str(), dpTT_tar_truth_title.c_str());
                    
                    TLegend * dpTT_tar_recon_leg = plot->Legend(0.25, 0.4, 0.551, 0.362);
                    TLegend * dpTT_tar_truth_leg = plot->Legend(0.25, 0.4, 0.551, 0.362);
                    
                    for(int mpc = 1; mpc < (int)dpTT_tar_map.size() + 1; mpc++){
                        
                        dpTT_tar_recon_tot->Add( dpTT_tar_recon[(int)dpTT_tar_map.size() - mpc] );
                        dpTT_tar_truth_tot->Add( dpTT_tar_truth[(int)dpTT_tar_map.size() - mpc] );
                        
                        if( (mpc - 1) < (int)dpTT_tar_map.size()){
                            string dpTT_tar_name = dpTT_tar_names[mpc - 1];
                            dpTT_tar_recon_leg->AddEntry(dpTT_tar_recon[ mpc - 1 ], Form("%s (%.2f%%)", dpTT_tar_name.c_str(), dpTT_tar_recon_per[ mpc - 1 ]), "f");
                            dpTT_tar_truth_leg->AddEntry(dpTT_tar_truth[ mpc - 1 ], Form("%s (%.2f%%)", dpTT_tar_name.c_str(), dpTT_tar_truth_per[ mpc - 1 ]), "f");
                        }
                    }
                    //-------------------------//
                    
                    string tar_sig = "truth_n_pro == 1 && truth_n_piP == 1 && truth_n_muo == 1 && mc_nFSPart == 3 && mc_targetZ == 1  && mc_current == 1 && TMath::RadToDeg()*truth_mu_Theta < 20 && TMath::RadToDeg()*truth_mu_Theta >= 0";
                    KinMap dpTT_Sigtar_map = plot->KinArray(TString(dpTT_var), 21, -300, 300, TString(dpTT_title),  TString(Form("%s && %s", base_cuts.c_str(), tar_sig.c_str())));
                    
                    dpTT_Sigtar_map.recon->SetLineStyle(2);
                    dpTT_Sigtar_map.recon->SetLineWidth(2);
                    dpTT_Sigtar_map.recon->SetLineColor(1);
                    dpTT_tar_recon_leg->AddEntry( dpTT_Sigtar_map.recon, "H-CC1p1#pi^{+} #theta_{#mu} < 20^{#circ}", "l");
                    
                    dpTT_Sigtar_map.truth->SetLineStyle(2);
                    dpTT_Sigtar_map.truth->SetLineWidth(2);
                    dpTT_Sigtar_map.truth->SetLineColor(1);
                    dpTT_tar_truth_leg->AddEntry( dpTT_Sigtar_map.truth, "H-CC1p1#pi^{+} #theta_{#mu} < 20^{#circ}", "l");

                    TLegend * dpTT_tar_recon_pot = plot->GetPOT(0.521,0.781);
                    TLegend * dpTT_tar_truth_pot = plot->GetPOT(0.521,0.781);
                    
                    TCanvas * dpTT_tar_recon_can = new TCanvas( (dpTT_tar_title + "_recon").c_str(), "", 500, 500);
                    dpTT_tar_recon_can->cd();
                    dpTT_tar_recon_tot->Draw();
                    dpTT_Sigtar_map.recon->Draw("SAME");
                    dpTT_tar_recon_leg->Draw();
                    dpTT_tar_recon_pot->Draw();
                    outfile->cd();
                    dpTT_tar_recon_can->Write();
                    
                    TCanvas * dpTT_tar_truth_can = new TCanvas( (dpTT_tar_title+ "_truth").c_str(), "", 500, 500);
                    dpTT_tar_truth_can->cd();
                    dpTT_tar_truth_tot->Draw();
                    dpTT_Sigtar_map.truth->Draw("SAME");
                    dpTT_tar_truth_leg->Draw();
                    dpTT_tar_truth_pot->Draw();
                    outfile->cd();
                    dpTT_tar_truth_can->Write();
                    
                    //------------------------------------------------------------------------------------------------------------------------------------------------------
                    //------------------------------------------------------------------------------------------------------------------------------------------------------
                    //3) mis-PID effect --> Correct P,Pi : Correct P, Wrong Pi : Correct Pi, Wrong P : Wrong P,Pi, Other <-- Use only PDG codes to determine these
                    
                    string dpTT_mis_title = "mis_" + dpTT_g_title;
                    
                    if(debug) cout << "dpTT Mis: Working 1" << endl;
                    string misCPCPi = flag + "pr_PDG == 2212 && " + flag + "pi_PDG == 211";
                    KinMap dpTT_misCPCPi_map = plot->KinArray(TString(dpTT_var), 21, -300, 300, TString(dpTT_title),  TString(Form("%s && %s", base_cuts.c_str(), misCPCPi.c_str())));
                    
                    if(debug) cout << "dpTT Mis: Working 2" << endl;
                    string misCPWPi = flag + "pr_PDG == 2212 && " + flag + "pi_PDG != 211";
                    KinMap dpTT_misCPWPi_map = plot->KinArray(TString(dpTT_var), 21, -300, 300, TString(dpTT_title),  TString(Form("%s && %s", base_cuts.c_str(), misCPWPi.c_str())));
                    
                    if(debug) cout << "dpTT Mis: Working 3" << endl;
                    string misSWPPi = flag + "pr_PDG == 211 && " + flag + "pi_PDG != 2212";
                    KinMap dpTT_misSWPPi_map = plot->KinArray(TString(dpTT_var), 21, -300, 300, TString(dpTT_title),  TString(Form("%s && %s", base_cuts.c_str(), misSWPPi.c_str())));
                    
                    if(debug) cout << "dpTT Mis: Working 4" << endl;
                    string misWPCPi = flag + "pr_PDG != 2212 && " + flag + "pi_PDG == 211";
                    KinMap dpTT_misWPCPi_map = plot->KinArray(TString(dpTT_var), 21, -300, 300, TString(dpTT_title),  TString(Form("%s && %s", base_cuts.c_str(), misWPCPi.c_str())));
                    
                    if(debug) cout << "dpTT Mis: Working 5" << endl;
                    string misWPWPi = flag + "pr_PDG != 2212 && " + flag + "pi_PDG != 211";
                    KinMap dpTT_misWPWPi_map = plot->KinArray(TString(dpTT_var), 21, -300, 300, TString(dpTT_title),  TString(Form("%s && %s", base_cuts.c_str(), misWPWPi.c_str())));
                    
                    if(debug) cout << "dpTT Mis: Working 6" << endl;
                    //string misOther = "!( (" + misCPCPi + ") || (" + misCPWPi + ") || ("  + misSWPPi + ") || ("  + misWPCPi + ") || ("  + misWPWPi + ") )";
                    KinMap dpTT_misOther_map = plot->KinArray(TString(dpTT_var), 21, -300, 300, TString(dpTT_title),  TString(Form("%s", base_cuts.c_str())));

                    if(debug) cout << "dpTT Mis: Working 7" << endl;

                    std::vector<KinMap> dpTT_mis_map;
                    std::vector<std::string> dpTT_mis_names;
                    std::vector<int> dpTT_mis_cols;
                    
                    dpTT_mis_map.push_back( dpTT_misCPCPi_map );   dpTT_mis_names.push_back( "Correct p,#pi^{+}" );         dpTT_mis_cols.push_back( DrawingStyle::Yellow);
                    dpTT_mis_map.push_back( dpTT_misCPWPi_map );   dpTT_mis_names.push_back( "Correct p, Wrong #pi^{+}" );  dpTT_mis_cols.push_back( DrawingStyle::Blue );
                    dpTT_mis_map.push_back( dpTT_misSWPPi_map );   dpTT_mis_names.push_back( "Mis-PID correct p,#pi^{+}");  dpTT_mis_cols.push_back( DrawingStyle::Red );
                    dpTT_mis_map.push_back( dpTT_misWPCPi_map );   dpTT_mis_names.push_back( "Wrong p, Correct #pi^{+}" );  dpTT_mis_cols.push_back( DrawingStyle::Green );
                    dpTT_mis_map.push_back( dpTT_misWPWPi_map );   dpTT_mis_names.push_back( "Wrong p, Wrong #pi^{+}" );    dpTT_mis_cols.push_back( DrawingStyle::LBlue );
                    dpTT_mis_map.push_back( dpTT_misOther_map );   dpTT_mis_names.push_back( "Other" );                     dpTT_mis_cols.push_back( DrawingStyle::Other );
                    
                    if(debug) cout << "dpTT Mis: Working 8" << endl;

                    
                    std::vector<TH1D*> dpTT_mis_recon;
                    std::vector<TH1D*> dpTT_mis_truth;
                    
                    for(int mpc = 0; mpc < (int)dpTT_mis_map.size(); mpc++){
                        plot->ColFill( dpTT_mis_map[mpc].recon, dpTT_mis_cols[mpc]);
                        plot->ColFill( dpTT_mis_map[mpc].truth, dpTT_mis_cols[mpc]);
                        
                        if(mpc < (int)dpTT_mis_map.size() - 1 ){//Remove all the other topologies from other:
                            dpTT_mis_map[ (int)dpTT_mis_map.size() - 1 ].recon->Add(dpTT_mis_map[mpc].recon, -1.);
                            dpTT_mis_map[ (int)dpTT_mis_map.size() - 1 ].truth->Add(dpTT_mis_map[mpc].truth, -1.);
                        }
                    
                        dpTT_mis_recon.push_back( dpTT_mis_map[mpc].recon );
                        dpTT_mis_truth.push_back( dpTT_mis_map[mpc].truth );
                    }
                    
                    if(debug) cout << "dpTT Mis: Working 9" << endl;
                    
                    std::vector<double> dpTT_mis_recon_per = plot->GetPercentage( dpTT_mis_recon );
                    std::vector<double> dpTT_mis_truth_per = plot->GetPercentage( dpTT_mis_truth );
                    
                    string dpTT_mis_recon_title = Form(";%s;%s", dpTT_mis_recon[0]->GetXaxis()->GetTitle(), dpTT_mis_recon[0]->GetYaxis()->GetTitle());
                    string dpTT_mis_truth_title = Form(";%s;%s", dpTT_mis_truth[0]->GetXaxis()->GetTitle(), dpTT_mis_truth[0]->GetYaxis()->GetTitle());
                    
                    THStack * dpTT_mis_recon_tot = new THStack( (dpTT_mis_title + "_recon").c_str() , dpTT_mis_recon_title.c_str());
                    THStack * dpTT_mis_truth_tot = new THStack( (dpTT_mis_title + "_truth").c_str(), dpTT_mis_truth_title.c_str());
                    
                    TLegend * dpTT_mis_recon_leg = plot->Legend(0.25, 0.4, 0.551, 0.362);
                    TLegend * dpTT_mis_truth_leg = plot->Legend(0.25, 0.4, 0.551, 0.362);
                    
                    if(debug) cout << "dpTT Mis: Working 10" << endl;
                    
                    for(int mpc = 1; mpc < (int)dpTT_mis_map.size() + 1; mpc++){
                        
                        dpTT_mis_recon_tot->Add( dpTT_mis_recon[(int)dpTT_mis_map.size() - mpc] );
                        dpTT_mis_truth_tot->Add( dpTT_mis_truth[(int)dpTT_mis_map.size() - mpc] );
                        
                        if( (mpc - 1) < (int)dpTT_mis_map.size()){
                            string dpTT_mis_name = dpTT_mis_names[mpc - 1];
                            dpTT_mis_recon_leg->AddEntry(dpTT_mis_recon[ mpc - 1 ], Form("%s (%.2f%%)", dpTT_mis_name.c_str(), dpTT_mis_recon_per[ mpc - 1 ]), "f");
                            dpTT_mis_truth_leg->AddEntry(dpTT_mis_truth[ mpc - 1 ], Form("%s (%.2f%%)", dpTT_mis_name.c_str(), dpTT_mis_truth_per[ mpc - 1 ]), "f");
                        }
                    }
                    //-------------------------//
                    
                    if(debug) cout << "dpTT Mis: Working 11" << endl;
                    
                    TLegend * dpTT_mis_recon_pot = plot->GetPOT(0.521,0.781);
                    TLegend * dpTT_mis_truth_pot = plot->GetPOT(0.521,0.781);
                    
                    TCanvas * dpTT_mis_recon_can = new TCanvas( (dpTT_mis_title + "_recon").c_str(), "", 500, 500);
                    dpTT_mis_recon_can->cd();
                    dpTT_mis_recon_tot->Draw();
                    dpTT_mis_recon_leg->Draw();
                    dpTT_mis_recon_pot->Draw();
                    outfile->cd();
                    dpTT_mis_recon_can->Write();
                    if(debug) cout << "dpTT Mis: Working 12" << endl;

                    TCanvas * dpTT_mis_truth_can = new TCanvas( (dpTT_mis_title+ "_truth").c_str(), "", 500, 500);
                    dpTT_mis_truth_can->cd();
                    dpTT_mis_truth_tot->Draw();
                    dpTT_mis_truth_leg->Draw();
                    dpTT_mis_truth_pot->Draw();
                    outfile->cd();
                    dpTT_mis_truth_can->Write();
                }
            }
        }
        
        std::vector<TString> cut_names;
        cut_names.push_back("Vertex");
        cut_names.push_back("3 Tracks");
        cut_names.push_back("Muon Track");
        cut_names.push_back("Contained Vtx");
        cut_names.push_back("PID: p/#pi^{#pm}");
        cut_names.push_back("Michel Tags");
        
        string signal_def_truth = "truth_n_pro == 1 && truth_n_piP == 1 && truth_n_muo == 1 && mc_nFSPart == 3 && mc_targetZ == 1  && mc_current == 1 && TMath::RadToDeg()*truth_mu_Theta < 20 && TMath::RadToDeg()*truth_mu_Theta >= 0";// && truth_true_target_region == 1";
        
        EffPurTools * m_ep = new EffPurTools(file.c_str(), cut_names, true /*debug*/);//Here shouldn't the requirement of being in the scintillator target region be a cut? I think so...
        TH1D * effcuts0 = m_ep->EffVSCuts(TString(signal_def_truth));
        TH1D * purcuts0 = m_ep->PurVSCuts(TString( (signal_def_truth + " && truth_true_target_region == 1").c_str() ));
        
        TH1D * effcuts1 = m_ep->EffVSCuts(TString(signal_def_truth), 1);
        TH1D * purcuts1 = m_ep->PurVSCuts(TString( (signal_def_truth + " && truth_true_target_region == 1").c_str() ), 1);
        
        effcuts0->SetLineColor(DrawingStyle::Blue);
        purcuts0->SetLineColor(DrawingStyle::Blue);
        purcuts0->SetLineStyle(2);
        
        effcuts1->SetLineColor(DrawingStyle::Red);
        purcuts1->SetLineColor(DrawingStyle::Red);
        purcuts1->SetLineStyle(2);
        
        TLegend * effpur_leg = plot->Legend(0.3,0.1);
        effpur_leg->AddEntry(effcuts0, "Eff. dEdX PID Method", "l");
        effpur_leg->AddEntry(purcuts0, "Pur. dEdX PID Method", "l");
        effpur_leg->AddEntry(effcuts1, "Eff. LLR PID Method", "l");
        effpur_leg->AddEntry(purcuts1, "Pur. LLR PID Method", "l");
        
        TLegend * effpur_pot = plot->GetPOT(0.521,0.781);
        
        TCanvas * effpur_can = new TCanvas("effpurVScuts","",500,500);
        effpur_can->cd();
        effcuts0->GetYaxis()->SetTitle("Efficieny/Purity");
        effcuts0->Draw("HIST");
        purcuts0->Draw("SAMEHIST");
        effcuts1->Draw("SAMEHIST");
        purcuts1->Draw("SAMEHIST");
        effpur_leg->Draw();
        effpur_pot->Draw();
        outfile->cd();
        effpur_can->Write();
    
        string old_sig_def = "mc_intType == 2 && mc_current == 1";
        TH1D * effcuts_old = m_ep->EffVSCuts(TString(old_sig_def));
        TH1D * purcuts_old = m_ep->PurVSCuts(TString(old_sig_def));
        
        effcuts_old->SetLineColor(DrawingStyle::Red);
        purcuts_old->SetLineColor(DrawingStyle::Red);
        purcuts_old->SetLineStyle(2);
        
        TLegend * old_effpur_leg = plot->Legend(0.3,0.1);
        old_effpur_leg->AddEntry(effcuts_old, "Old Eff. dEdX PID Method", "l");
        old_effpur_leg->AddEntry(purcuts_old, "Old Pur. dEdX PID Method", "l");
        old_effpur_leg->AddEntry(effcuts0, "New Eff. dEdX PID Method", "l");
        old_effpur_leg->AddEntry(purcuts0, "New Pur. dEdX PID Method", "l");
    
        TLegend * old_effpur_pot = plot->GetPOT(0.521,0.781);
        
        TCanvas * old_effpur_can = new TCanvas("effpurVScuts_oldSigDef","",500,500);
        old_effpur_can->cd();
        effcuts0->GetYaxis()->SetTitle("Efficieny/Purity");
        effcuts0->Draw("HIST");
        purcuts0->Draw("SAMEHIST");
        effcuts_old->Draw("SAMEHIST");
        purcuts_old->Draw("SAMEHIST");
        old_effpur_leg->Draw();
        old_effpur_pot->Draw();
        outfile->cd();
        old_effpur_can->Write();
        
        
        //Eff. of dpTT for signal
        std::vector<string> truthdpTT_type;
        std::vector<string> dpTT_save;
        
        truthdpTT_type.push_back( "truth_truedpTT" );           dpTT_save.push_back( "_dpTT_EX" );
        truthdpTT_type.push_back( "truth_truedpTT_pi" );        dpTT_save.push_back( "_dpTT_pi_EX" );
        truthdpTT_type.push_back( "truth_truedpTT_pi_dir" );    dpTT_save.push_back( "_dpTT_pi_dir_EX" );
        truthdpTT_type.push_back( "truth_truedpTT_pr" );        dpTT_save.push_back( "_dpTT_pr_EX" );
        truthdpTT_type.push_back( "truth_truedpTT_pr_dir" );    dpTT_save.push_back( "_dpTT_pr_dir_EX" );
        
        /*for(int i = 0; i < (int)truthdpTT_type.size(); i++){
        
            TH1D * eff_dpTT = m_ep->EffVSVar(TString( truthdpTT_type[i].c_str() ),21, -300, 300, signal_def_truth, "accum" TString((dpTT_type_title[i] + "#delta#it{p}_{TT} (MeV/#it{c})").c_str()));
            
            TCanvas * eff_dpTT_can = new TCanvas( ("eff_" + dpTT_save[i]).c_str() , "", 500, 500);
            eff_dpTT_can->cd();
            eff_dpTT->Draw("HIST");
            TLegend * eff_dpTT_pot = plot->GetPOT(0.521,0.781);
            eff_dpTT_pot->Draw();
            
            outfile->cd();
            eff_dpTT_can->Write();
            
            delete eff_dpTT;
            delete eff_dpTT_can;
            
            TH1D * pur_dpTT = m_ep->PurVSVar(TString( (dpTT_type[i] + "_EX").c_str() ), 21, -300, 300, signal_def_truth, TString((dpTT_type_title[i] + "#delta#it{p}_{TT} (MeV/#it{c})").c_str()));
            
            TCanvas * pur_dpTT_can = new TCanvas( ("pur_" + dpTT_save[i]).c_str() , "", 500, 500);
            pur_dpTT_can->cd();
            pur_dpTT->Draw("HIST");
            TLegend * pur_dpTT_pot = plot->GetPOT(0.521,0.781);
            pur_dpTT_pot->Draw();
            
            outfile->cd();
            pur_dpTT_can->Write();
            
            delete pur_dpTT;
            delete pur_dpTT_can;
        }*/
        
        string pcount_st[2] = {"pro", "piP"};//Signal def should be greater than zero protons (any protons); -- Should accum_level be a factor? I am not sure...
        string pname_st[2] = {"pr", "pi"};
        string psym_st[2] = {"p","#pi^{+}"};
        
        //Eff. vs E_{p,pi} -- for use in determining how best to approach improving three tracks:
        for(int i = 0; i < 2; i++){
            for (int pid_meth = 0; pid_meth < 2; pid_meth++) {
            
                TH1D * part_eff_E = m_ep->EffVSVar( ("truth_" + pname_st[i] + "_E/1000").c_str() , 50, 0, 6, ("truth_n_" + pcount_st[i] + " > 0 && truth_accum_level[" +  + "] > ").c_str(), ("E^{true}_" + psym_st[i] + " GeV").c_str());
                
                TCanvas * part_eff_E_can = new TCanvas( ("eff_" + pname_st[i] + "_E").c_str() , "", 500, 500);
                part_eff_E_can->cd();
                part_eff_E->Draw("HIST");
                TLegend * part_eff_E_can_pot = plot->GetPOT(0.521,0.781);
                part_eff_E_can_pot->Draw();
                
                outfile->cd();
                part_eff_E_can->Write();
                
                delete part_eff_E;
                delete part_eff_E_can;
            }
        }
        
        
    
        outfile->Close();

}

/*TCanvas * AnalysisPlots::VarVsInts(){
    
}

TCanvas * AnalysisPlots:: VarVsParticles(TString var, int nbins, double x_low, double x_high, TString particle, TString part_symb){
    
    TH1D * h_mom_p1 = plot->GetRecoHisto(Form("CC1P1PiP_%s_%s/1000",particle.Data(), var.Data()), mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s && CC1P1PiP_%s_PDG == 2212", common_cuts_score_p.Data(), tmp_part_snam.Data()));
    TH1D * h_mom_p2 = plot->GetRecoHisto(Form("CC1P1PiP_%s_%s/1000",particle.Data(), var.Data()), mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s && TMath::Abs(CC1P1PiP_%s_PDG) == 211", common_cuts_score_p.Data(), tmp_part_snam.Data()));
    TH1D * h_mom_p3 = plot->GetRecoHisto(Form("CC1P1PiP_%s_%s/1000",particle.Data(), var.Data()), mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s && CC1P1PiP_%s_PDG == 13", common_cuts_score_p.Data(), tmp_part_snam.Data()));
    TH1D * h_mom_p4 = plot->GetRecoHisto(Form("CC1P1PiP_%s_%s/1000",particle.Data(), var.Data()), mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s && CC1P1PiP_%s_PDG != 13 && TMath::Abs(CC1P1PiP_%s_PDG) != 211 && CC1P1PiP_%s_PDG != 2212", common_cuts_score_p.Data(), tmp_part_snam.Data(), tmp_part_snam.Data(), tmp_part_snam.Data()));
    
    plot->ColFill(h_mom_p1, DrawingStyle::Proton);
    plot->ColFill(h_mom_p2, DrawingStyle::Pion);
    plot->ColFill(h_mom_p3, DrawingStyle::Muon);
    plot->ColFill(h_mom_p4, DrawingStyle::Other);
    
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
    
    TDatime today;
    int day = today.GetDay();
    int mon = today.GetMonth();
    int year = today.GetYear();
    year -= 2000;
    
    cout << "day = " << day << " mon = " << mon << " year = " << year << endl;
    
    stringstream sday, smon, syear;
    
    sday << day;
    smon << mon;
    syear << year;
    
    string savename = "CC1P1PiP_Plots_" + day + mon + year + ".root";
    
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
