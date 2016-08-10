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
#include <MinervaUnfold/MnvUnfold.h>
#include "Cintex/Cintex.h"


using namespace std;
using namespace PlotUtils;

//These are the tuples used for this exercise
const string filename_mc("/minerva/data/users/wospakrk/Minerva101Files/Tuples/CCInclusiveReco_MC_AnaTuple_minerva1.root");

//This is the POT for Data and Monte Carlo for the tuples that we use in this exercise.
const double total_data_pot = 2.0384e18;
const double total_mc_pot = 1.79e19;

void MomentumDists()
{
    
    ROOT::Cintex::Cintex::Enable();
    
    cout<<"Producing Momentum Distributions for MC files."<<endl;
    
}


int main()
{
    MomentumDists();
    return 0;
}
