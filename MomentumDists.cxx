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


using namespace std;
using namespace PlotUtils;

//These are the tuples used for this exercise
const string testing_mc("/pnfs/minerva/persistent/users/dcoplowe/CC1P1Pi_PL13C_080816/grid/central_value/minerva/ana/v10r8p9/00/01/32/00/SIM_minerva_00013200_Subruns_0001-0002-0003-0004_CC1P1PiAnalysis_Ana_Tuple_v10r8p9-dcoplowe.root");

const string flag("CC1P1Pi");

//This is the POT for Data and Monte Carlo for the tuples that we use in this exercise.
const double total_mc_pot = 1.79e19;

void MomentumDists()
{
    
    ROOT::Cintex::Cintex::Enable();
    
    cout<<"Producing Momentum Distributions for MC files."<<endl;
    
    TFile * infile = new TFile(testing_mc.c_str(), "READ");

    TFile * outfile = new TFile("MomentumDists.root","RECREATE");
    
    TTree * intree;
    infile->GetObject("CC1P1Pi",intree);
    
    //Produce independent particles vars hists:
    
    string part_snam[3] = {"mu","pr","pi"};
    string part_name[3] = {"Muon", "Proton", "Pion"};
    string part_symb[3] = {"#mu^{-}", "p", "#pi^{+}"};
    
    string var_unit[3] = {" (MeV/#it{c})", " (MeV/#it{c})", " (MeV)" };
    string var_symb[3] = {"#it{p}", " |#it{p}_{T}|", " E" };

    
    string rec_var[3] = {"mom", "pTMag", "E"};
    string rec_name[3] = {"Momentum", "|#it{p}_{T}|", "Energy"};
    
    string true_var[3];// = {"turemom", "truepTMag", };
    string true_name[3];// = {"Momentum", "|p_{T}|", "Energy"};
    
    for(int i = 0; i < 3; i++){
        true_var[i] = "true" + rec_var[i];
        true_name[i] = "True " + rec_name[i];
    }
    
    for(int i = 0; i < 3; i++){
        cout << " Making Plots for " << part_name[i] << "." << endl;
        
        for(int j = 0; j < 3; j++){
            string tmp_h_name = part_snam[i] + "_" + rec_var[j];
            string tmp_h_title = part_name[i] + " " rec_name[j] + "; Reco. " + var_symb[j] + var_unit[j] + "; Counts";
            
            cout << "Hist Name: " << tmp_h_name << endl;
            cout << "    Title: " << tmp_h_title << endl;
            
            MnvH1D * rec_h = new MnvH1D( tmp_h_name.c_str() , tmp_h_title.c_str(), 30, 0., 4000.);
            
        }
        

        
        
    }
    
    
}


int main()
{
    MomentumDists();
    return 0;
}
