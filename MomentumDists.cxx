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
//const string testing_mc("/pnfs/minerva/persistent/users/dcoplowe/merged_CC1P1Pi_CC1P1Pi_run00013200.root");

//-->
const string testing_mc("/pnfs/minerva/persistent/users/dcoplowe/CC1P1Pi_PL13C_041016/grid/central_value/minerva/ana/v10r8p9/00/01/32/00/SIM_minerva_00013200_Subruns_0101-0102-0103-0104-0105_CC1P1PiAnalysis_Ana_Tuple_v10r8p9-dcoplowe.root");

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
            
                char buffer0[100];
                sprintf(buffer0,"%d",j);
                std::string common_cuts_mom = target + " && accum_level[" + buffer0 + "] > 4";
                
                
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
                KinMap mom_pi_map = plot->KinArray(TString(mom_name), mom_nbins, mom_low, mom_high, TString(mom_title),  TString(Form("%s && TMath::Abs(%s_PDG) == 211", common_cuts_mom.c_str(), part_name_.c_str())));
                if(debug) cout << "Mom: Working 3" << endl;
                KinMap mom_mum_map = plot->KinArray(TString(mom_name), mom_nbins, mom_low, mom_high, TString(mom_title),  TString(Form("%s && %s_PDG == 13", common_cuts_mom.c_str(), part_name_.c_str())));
                if(debug) cout << "Mom: Working 4" << endl;
                KinMap mom_mup_map = plot->KinArray(TString(mom_name), mom_nbins, mom_low, mom_high, TString(mom_title),  TString(Form("%s && %s_PDG == -13", common_cuts_mom.c_str(), part_name_.c_str())));
                if(debug) cout << "Mom: Working 5" << endl;
                KinMap mom_p0_map = plot->KinArray(TString(mom_name), mom_nbins, mom_low, mom_high, TString(mom_title),  TString(Form("%s && %s_PDG == 111", common_cuts_mom.c_str(), part_name_.c_str())));
                if(debug) cout << "Mom: Working 6" << endl;
                KinMap mom_ka_map = plot->KinArray(TString(mom_name), mom_nbins, mom_low, mom_high, TString(mom_title),  TString(Form("%s && TMath::Abs(%s_PDG) == 321", common_cuts_mom.c_str(), part_name_.c_str())));
                if(debug) cout << "Mom: Working 7" << endl;
                KinMap mom_kz_map = plot->KinArray(TString(mom_name), mom_nbins, mom_low, mom_high, TString(mom_title),  TString(Form("%s && %s_PDG == 311", common_cuts_mom.c_str(), part_name_.c_str())));
                if(debug) cout << "Mom: Working 8" << endl;
                std::string mom_other = part_name_ + "_PDG != 2212 && TMath::Abs(" + part_name_ + "_PDG) != 211 && TMath::Abs(" + part_name_ + "_PDG) != 13 && " + part_name_ + "_PDG != 111 && TMath::Abs(" + part_name_ + "_PDG) != 321 && " + part_name_ + "_PDG != 311 &&" + part_name_ + "_PDG != -999";
                KinMap mom_ot_map = plot->KinArray(TString(mom_name), mom_nbins, mom_low, mom_high, TString(mom_title),  TString(Form("%s && %s", common_cuts_mom.c_str(), mom_other.c_str())));

                std::vector<KinMap> mom_map;
                std::vector<std::string> mom_map_names;
                std::vector<int> mom_map_cols;
                
                mom_map.push_back( mom_pr_map );    mom_map_names.push_back("p");           mom_map_cols.push_back( DrawingStyle::Proton );
                mom_map.push_back( mom_pi_map );    mom_map_names.push_back("#pi^{#pm}");   mom_map_cols.push_back( DrawingStyle::Pion   );
                mom_map.push_back( mom_mum_map);    mom_map_names.push_back("#mu^{-}");     mom_map_cols.push_back( DrawingStyle::MuonM  );
                mom_map.push_back( mom_mup_map);    mom_map_names.push_back("#mu^{+}");     mom_map_cols.push_back( DrawingStyle::MuonP  );
                mom_map.push_back( mom_p0_map );    mom_map_names.push_back("#pi^{0}");     mom_map_cols.push_back( DrawingStyle::Pi0    );
                mom_map.push_back( mom_ka_map );    mom_map_names.push_back("K^{#pm}");     mom_map_cols.push_back( DrawingStyle::Kaon   );
                mom_map.push_back( mom_kz_map );    mom_map_names.push_back("K^{0}");       mom_map_cols.push_back( DrawingStyle::Ka0    );
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
                THStack * mom_ratio_tot = new THStack( (part_name_ + tmp_hyp + "_reco").c_str(),  mom_ratio_title.c_str());
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
                
                TCanvas * mom_recon_can = new TCanvas( (part_name_ + tmp_hyp + "_recon").c_str(), "", 500, 500);
                mom_recon_can->cd();
                mom_recon_tot->Draw();
                mom_recon_leg->Draw();
                mom_recon_pot->Draw();
                outfile->cd();
                mom_recon_can->Write();
                
                TCanvas * mom_truth_can = new TCanvas( (part_name_ + tmp_hyp + "_truth").c_str(), "", 500, 500);
                mom_truth_can->cd();
                mom_truth_tot->Draw();
                mom_truth_leg->Draw();
                mom_truth_pot->Draw();
                outfile->cd();
                mom_truth_can->Write();
                
                TCanvas * mom_smear_can = new TCanvas( (part_name_ + tmp_hyp + "_smear").c_str(), "", 500, 500);
                mom_smear_can->cd();
                mom_smear_tot->Draw("COLZ");
                //mom_smear_pot->Draw();
                outfile->cd();
                mom_smear_can->Write();
                
                TCanvas * mom_ratio_can = new TCanvas( (part_name_ + tmp_hyp + "_ratio").c_str(), "", 500, 500);
                mom_ratio_can->cd();
                mom_ratio_tot->Draw();
                mom_ratio_leg->Draw();
                mom_ratio_pot->Draw();
                outfile->cd();
                mom_ratio_can->Write();
                
            }
            
            
            
            if(i>0){
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
                        
                        plot->ColFill(score_pr, DrawingStyle::Proton);
                        plot->ColFill(score_pi, DrawingStyle::Pion);
                        plot->ColFill(score_mum, DrawingStyle::MuonM);
                        plot->ColFill(score_mup, DrawingStyle::MuonP);
                        plot->ColFill(score_p0, DrawingStyle::Pi0);
                        plot->ColFill(score_ka, DrawingStyle::Kaon);
                        plot->ColFill(score_kz, DrawingStyle::Ka0);
                        plot->ColFill(score_ot, DrawingStyle::Other);

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
            }
            //-----------------------------------------------------------------------------------------------------------------------------------------------------------------//
        }
        
        std::vector<TString> cut_names;
        cut_names.push_back("Vertex");
        cut_names.push_back("3 Tracks");
        cut_names.push_back("Muon Track");
        cut_names.push_back("Contained Vtx");
        cut_names.push_back("PID: p/#pi^{+}");
        cut_names.push_back("Michel Tags");
        
        string signal_def_truth = "truth_n_pro == 1 && truth_n_piP == 1 && truth_n_muo == 1 && mc_nFSPart == 3 && mc_targetZ == 1";// && truth_true_target_region == 1";
        
        EffPurTools * m_ep = new EffPurTools(file, cut_names, debug);
        TH1D * effcuts0 = m_ep->EffVSCuts(TString(signal_def_truth));
        TH1D * purcuts0 = m_ep->PurVSCuts(TString(signal_def_truth));
        
        TH1D * effcuts1 = m_ep->EffVSCuts(TString(signal_def_truth), 1);
        TH1D * purcuts1 = m_ep->PurVSCuts(TString(signal_def_truth), 1);
        
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
        effpur_leg->AddEntry(effcuts1, "Pur. LLR PID Method", "l");
        
        TLegend * effpur_pot = plot->GetPOT(0.521,0.781);
        
        TCanvas * effpur_can = new TCanvas("EffPurVSCuts_Branch0","",500,500);
        effpur_can->cd();
        effcuts0->Draw("HIST");
        purcuts0->Draw("SAMEHIST");
        effpur_leg->Draw();
        effpur_pot->Draw();
        outfile->cd();
        effpur_can->Write();
    
    outfile->Close();

}

/*TCanvas * AnalysisPlots::VarVsInts(){
    
}

TCanvas * AnalysisPlots:: VarVsParticles(TString var, int nbins, double x_low, double x_high, TString particle, TString part_symb){
    
    TH1D * h_mom_p1 = plot->GetRecoHisto(Form("CC1P1Pi_%s_%s/1000",particle.Data(), var.Data()), mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s && CC1P1Pi_%s_PDG == 2212", common_cuts_score_p.Data(), tmp_part_snam.Data()));
    TH1D * h_mom_p2 = plot->GetRecoHisto(Form("CC1P1Pi_%s_%s/1000",particle.Data(), var.Data()), mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s && TMath::Abs(CC1P1Pi_%s_PDG) == 211", common_cuts_score_p.Data(), tmp_part_snam.Data()));
    TH1D * h_mom_p3 = plot->GetRecoHisto(Form("CC1P1Pi_%s_%s/1000",particle.Data(), var.Data()), mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s && CC1P1Pi_%s_PDG == 13", common_cuts_score_p.Data(), tmp_part_snam.Data()));
    TH1D * h_mom_p4 = plot->GetRecoHisto(Form("CC1P1Pi_%s_%s/1000",particle.Data(), var.Data()), mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s && CC1P1Pi_%s_PDG != 13 && TMath::Abs(CC1P1Pi_%s_PDG) != 211 && CC1P1Pi_%s_PDG != 2212", common_cuts_score_p.Data(), tmp_part_snam.Data(), tmp_part_snam.Data(), tmp_part_snam.Data()));
    
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
