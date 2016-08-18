#include "TTree.h"
#include "TBranch.h"
#include "TFile.h"
#include "TRandom3.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TObject.h"
#include "TDirectory.h"
#include "TROOT.h"
#include "TClass.h"
#include "TColor.h"
#include "TPad.h"
#include <TCanvas.h>
#include <iostream>
#include <fstream>
#include <string>
#include <PlotUtils/MnvH1D.h>
#include <PlotUtils/MnvH2D.h>
#include <PlotUtils/TargetUtils.h>
#include <PlotUtils/MnvPlotter.h>
#include "Cintex/Cintex.h"

#include "EffPurTools.h"

//#include <TStyle.h>

using namespace std;
using namespace PlotUtils;

//These are the tuples used for this exercise
const string testing_mc("/pnfs/minerva/scratch/users/dcoplowe/CC1P1PiAnalysis13/nogrid/central_value/minerva/ana/v10r8p9/00/01/32/03/SIM_minerva_00013203_Subruns_0001_CC1P1PiAnalysis_Ana_Tuple_v10r8p9-dcoplowe.root");

const string flag("CC1P1Pi");

//This is the POT for Data and Monte Carlo for the tuples that we use in this exercise.
const double total_mc_pot = 1.79e19;

void MomentumDists()
{
    //TStyle * gStyle = new TStyle();
    //gStyle->SetOptStat(0);
    //gStyle->SetOptFit(0);
    
    ROOT::Cintex::Cintex::Enable();
    
    cout<<"Producing Momentum Distributions for MC files."<<endl;
    
    TFile * infile = new TFile(testing_mc.c_str(), "READ");

    //TFile * outfile = new TFile("MomentumDists.root","RECREATE");
    
    TTree * intree;
    infile->GetObject("CC1P1Pi",intree);
    
    //Produce independent particles vars hists:
    
    string part_snam[3] = {"mu","pr","pi"};
    string part_name[3] = {"Muon", "Proton", "Pion"};
    string part_symb[3] = {"#mu^{-}", "p", "#pi^{+}"};
    
    string var_unit[3] = {" (MeV/#it{c})", " (MeV/#it{c})", " (MeV)" };
    string var_symb[3] = {" #it{p}", " |#it{p}_{T}|", " E" };
    
    double var_range[6] = {0., 4000., 0., 4000., 0., 4000.,};
    
    string rec_var[3] = {"mom", "pTMag", "E"};
    string rec_name[3] = {"Momentum", "|#it{p}_{T}|", "Energy"};
    
    string true_var[3];// = {"turemom", "truepTMag", };
    string true_name[3];// = {"Momentum", "|p_{T}|", "Energy"};
    
    for(int i = 0; i < 3; i++){
        true_var[i] = "true" + rec_var[i];
        true_name[i] = "True " + rec_name[i];
    }
    
    std::vector<TString> cut_names;
    cut_names.push_back("Vertex");
    cut_names.push_back("3 Tracks");
    cut_names.push_back("Muon Track");
    cut_names.push_back("Contained Vtx");
    cut_names.push_back("PID: p/#pi^{+}");
    
    EffPurTools * m_ep = new EffPurTools(testing_mc);
    
    MnvH1D * purcut = m_ep->PurVSCuts("mc_intType == 2 && mc_current == 1 && mc_resID == 0");
    
    if(purcut){
        TCanvas * pur_can = new TCanvas("pur_can", "", 500,500);
        pur_can->cd();
        purcut->Draw();
        pur_can->Print("first_pur_plot.eps");
    }
    
    /*
        
    for(int i = 0; i < 3; i++){
        cout << " Making Plots for " << part_name[i] << "." << endl;
        
        for(int j = 0; j < 3; j++){
            
            double tmp_lx = var_range[ 2*j ];
            double tmp_hx = var_range[ 2*j + 1 ];
            
            string tmp_h_rec_name = part_snam[i] + "_" + rec_var[j];
            string tmp_h_rec_title = part_name[i] + " " + rec_name[j] + "; Reco." + var_symb[j] + var_unit[j] + "; Counts";
            
            //printf("%s_%s_%s >> %s \n", flag.c_str(), part_snam[i].c_str(), rec_var[j].c_str() , tmp_h_rec_name.c_str());
            
            //cout << "Hist Name: " << tmp_h_rec_name << endl;
            //cout << "    Title: " << tmp_h_rec_title << endl;
            
            MnvH1D * rec_h = new MnvH1D( tmp_h_rec_name.c_str() , tmp_h_rec_title.c_str(), 30, tmp_lx, tmp_hx);
            rec_h->SetStats(kFALSE);
            rec_h->GetYaxis()->SetTitleOffset(1.4);
            rec_h->SetLineColor(kOrange+2);
            rec_h->SetLineWidth(2);
            intree->Draw(Form("%s_%s_%s >> %s", flag.c_str(), part_snam[i].c_str(), rec_var[j].c_str() , tmp_h_rec_name.c_str()), "");

            string tmp_h_true_name = part_snam[i] + "_" + true_var[j];
            string tmp_h_true_title = part_name[i] + " " + true_name[j] + "; True" + var_symb[j] + var_unit[j] + "; Counts";
            
            //cout << "Hist Name: " << tmp_h_true_name << endl;
            //cout << "    Title: " << tmp_h_true_title << endl;
            
            //printf("%s_%s_%s >> %s \n", flag.c_str(), part_snam[i].c_str(), true_var[j].c_str() , tmp_h_rec_name.c_str());
            MnvH1D * true_h = new MnvH1D( tmp_h_true_name.c_str() , tmp_h_true_title.c_str(), 30, tmp_lx, tmp_hx);
            true_h->SetStats(kFALSE);
            true_h->GetYaxis()->SetTitleOffset(1.4);
            true_h->SetLineColor(kOrange+2);
            true_h->SetLineWidth(2);
            
            intree->Draw(Form("%s_%s_%s >> %s", flag.c_str(), part_snam[i].c_str(), true_var[j].c_str() , tmp_h_true_name.c_str()), "");
            
            string twoDPlot_name = Form("%s_%s_%s", part_snam[i].c_str(), rec_var[j].c_str(), true_var[j].c_str());
            string twoDPlot_title = Form("Reco. vs. True %s; Reco. %s %s; True %s %s", var_symb[j].c_str(), var_symb[j].c_str(), var_unit[j].c_str(), var_symb[j].c_str(), var_unit[j].c_str());
            
            cout << twoDPlot_title << endl;
            
            MnvH2D * rectrue = new MnvH2D(twoDPlot_name.c_str(), twoDPlot_title.c_str(), 30, tmp_lx, tmp_hx, 30, tmp_lx, tmp_hx);
            rectrue->SetStats(kFALSE);
            rectrue->GetYaxis()->SetTitleOffset(1.4);
            
            intree->Draw(Form("%s_%s_%s:%s_%s_%s >> %s", flag.c_str(), part_snam[i].c_str(), true_var[j].c_str(), flag.c_str(), part_snam[i].c_str(), rec_var[j].c_str(), twoDPlot_name.c_str()));
            
            TCanvas * can = new TCanvas("", "", 2400, 1000);
            can->Divide(3);
            can->cd(1);
            rec_h->Draw("HIST");
            can->cd(2);
            rectrue->Draw("COLZ");
            can->cd(3);
            true_h->Draw("HIST");
            can->Print(Form("%s_%s_dists.eps",part_name[i].c_str(), rec_var[j].c_str()));
            
            //outfile->cd();
            //can->Write();
            
            delete rec_h;
            delete true_h;
            delete can;
        }
    }
     
     */
}


int main()
{
    MomentumDists();
    return 0;
}
