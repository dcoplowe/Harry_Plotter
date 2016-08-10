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
const string filename_data("/minerva/data/users/wospakrk/Minerva101Files/Tuples/CCInclusiveReco_Data_AnaTuple_minerva1.root");

//This is the POT for Data and Monte Carlo for the tuples that we use in this exercise.
const double total_data_pot = 2.0384e18;
const double total_mc_pot = 1.79e19;

void Cross_section_Calculation()
{
    
    ROOT::Cintex::Cintex::Enable();
    
    cout<<"Enjoy MINERvA 101! \n This is the Cross Section Calculation Exercise..."<<endl;
    
    //Load the data and Monte Carlo ntuples
    TFile *infile_mc   = new TFile(filename_mc.c_str(),  "READ");
    TFile *infile_data = new TFile(filename_data.c_str(),"READ");
    
    // Your results will be stored in this file
    TFile *out = new TFile("myCrossSection.root","RECREATE");
    
    // Load the CCInclusiveReco tree from the input root files
    TTree *tree_mc, *tree_data;
    infile_mc->GetObject("CCInclusiveReco",tree_mc);
    infile_data->GetObject("CCInclusiveReco",tree_data);
    
    // Our ntuple consists of many branches
    // When we loop over the ntuple, we have to load these branches into variables
    double CCInclusiveReco_E;                                  // reconstructed neutrino energy
    double mc_incomingE;                                       // true neutrino energy
    int    CCInclusiveReco_nuHelicity;                         // helicity
    int    pass_canonical_cut;                                 // canonical cut
    int    phys_n_dead_discr_pair_upstream_prim_track_proj;    // number of dead discriminator pair upstream of the vertex of interaction
    double wgt;                                                // central value weight applied to the event
    int    mc_current;                                         // current of the interaction
    int    mc_incoming;                                        // PDG code of the incoming muon neutrino
    
    
    // These are the branches in the ntuple tree
    TBranch *b_CCInclusiveReco_E;
    TBranch *b_mc_incomingE;
    TBranch *b_CCInclusiveReco_nuHelicity;
    TBranch *b_pass_canonical_cut;
    TBranch *b_phys_n_dead_discr_pair_upstream_prim_track_proj;
    TBranch *b_wgt;
    TBranch *b_mc_current;
    TBranch *b_mc_incoming;
    
    
    // Map the branches in the mc ntuple to the appropriate variables
    tree_mc->SetBranchAddress("CCInclusiveReco_E", &CCInclusiveReco_E, &b_CCInclusiveReco_E);
    tree_mc->SetBranchAddress("mc_incomingE", &mc_incomingE, &b_mc_incomingE);
    tree_mc->SetBranchAddress("CCInclusiveReco_nuHelicity", &CCInclusiveReco_nuHelicity, &b_CCInclusiveReco_nuHelicity);
    tree_mc->SetBranchAddress("pass_canonical_cut", &pass_canonical_cut, &b_pass_canonical_cut);
    tree_mc->SetBranchAddress("phys_n_dead_discr_pair_upstream_prim_track_proj", &phys_n_dead_discr_pair_upstream_prim_track_proj, &b_phys_n_dead_discr_pair_upstream_prim_track_proj);
    tree_mc->SetBranchAddress("wgt", &wgt, &b_wgt);
    tree_mc->SetBranchAddress("mc_current", &mc_current, &b_mc_current);
    tree_mc->SetBranchAddress("mc_incoming", &mc_incoming, &b_mc_incoming);
    
    
    // Map the branches in the data ntuple to the appropriate variables
    tree_data->SetBranchAddress("CCInclusiveReco_E", &CCInclusiveReco_E, &b_CCInclusiveReco_E);
    tree_data->SetBranchAddress("phys_n_dead_discr_pair_upstream_prim_track_proj", &phys_n_dead_discr_pair_upstream_prim_track_proj, &b_phys_n_dead_discr_pair_upstream_prim_track_proj);
    tree_data->SetBranchAddress("CCInclusiveReco_nuHelicity", &CCInclusiveReco_nuHelicity, &b_CCInclusiveReco_nuHelicity);
    tree_data->SetBranchAddress("pass_canonical_cut", &pass_canonical_cut, &b_pass_canonical_cut);
    
    
    //Initialize the histograms needed to calculate the cross section
    MnvH1D *h_Neutrino_Energy_mc = new MnvH1D("h_Neutrino_Energy_mc","Neutrino Energy ; Reconstructed Neutrino Energy (GeV); Events",30,0.0,15.0);
    MnvH1D *h_Neutrino_Energy_data = new MnvH1D("h_Neutrino_Energy_data","Neutrino Energy ; Reconstructed Neutrino Energy (GeV); Events",30,0.0,15.0);
    
    // ##### Initialize the 2D migration histogram here, x axis is Reco Neutrino Energy, y axis is True Neutrino Energy.
    MnvH2D *h_Neutrino_Energy_migr = new MnvH2D("h_Neutrino_Energy_migr","Neutrino Energy ; Rec. Neutrino Energy (GeV); True Neutrino Energy (GeV)", 30, 0.0, 15.0, 30, 0.0, 15.0);

    // ##### Initialize the 1D true neutrino energy distribution here! You will need this for the unfolding procedure
    MnvH1D *h_Neutrino_Energy_true = new MnvH1D("h_Neutrino_Energy_true","Neutrino Energy ; True Neutrino Energy (GeV); Events",30,0.0,15.0);
    
    //Signal Histogram:
    MnvH1D *h_Neutrino_Energy_sig = new MnvH1D("h_Neutrino_Energy_sig","Neutrino Energy ; Rec. Neutrino Energy (GeV); Events",30,0.0,15.0);
    //Background Histohram:
    MnvH1D *h_Neutrino_Energy_bkg = new MnvH1D("h_Neutrino_Energy_bkg","Neutrino Energy ; Rec. Neutrino Energy (GeV); Events",30,0.0,15.0);
    
    // Loop over each entry in our Monte Carlo
    long nentries_mc = tree_mc->GetEntries();
    for (long i = 0; i<nentries_mc; i++){
        if(i<0) break;
        tree_mc->GetEntry(i);
        
        CCInclusiveReco_E/= 1000.0;   // convert to GeV's

        //Skip events that do not pass selection:
        if(!(CCInclusiveReco_nuHelicity == 1 && pass_canonical_cut == 1 && 1 > phys_n_dead_discr_pair_upstream_prim_track_proj && CCInclusiveReco_E < 22.0)) continue;
        
        //cout << "MC Candidate " << endl;
        
        mc_incomingE/=1000.0;         // convert to GeV's
        
        // ????? What are the reconstruction cuts for the events? See HINT on section 1
        // ????? What is the maximum neutrino energy in our sample?
        
        // Fill the central value of the reconstructed neutrino energy that pass the cuts above
        // and weigh it with the central value weight
        h_Neutrino_Energy_mc->Fill(CCInclusiveReco_E,wgt);//This is like the fake data.
        
        if(mc_current == 1 && mc_incoming == 14){
            h_Neutrino_Energy_true->Fill(mc_incomingE);
            h_Neutrino_Energy_sig->Fill(CCInclusiveReco_E, wgt);
            h_Neutrino_Energy_migr->Fill(CCInclusiveReco_E, mc_incomingE,wgt);
        }
        else{
            h_Neutrino_Energy_bkg->Fill(CCInclusiveReco_E, wgt);
        }
        
    }
        // ???? What are the signal cuts?
        // #### What are the mc_current of the charged current interaction and the pdg code of the muon neutrino?
        // #### Fill the migration histogram and true histogram here!
        
        // ???? what should be our background? Remember the cut that we applied for the signal
        // ???? Background events are those events that didn't pass our signal cut
        // #### Fill the event background histogram here!
    
    
    // Loop over each entry in our Data
    long nentries_data = tree_data->GetEntries();
    for (long i = 0; i<nentries_data; i++){
        if(i<0) break;
        tree_data->GetEntry(i);
        CCInclusiveReco_E /= 1000.0; //to GeV's
        if(!(CCInclusiveReco_nuHelicity == 1 && pass_canonical_cut == 1 && 1 > phys_n_dead_discr_pair_upstream_prim_track_proj && CCInclusiveReco_E < 22.0)) continue;
        // ##### Apply the same reconstruction cut as MC
        // ##### Apply the same maximum reconstructed neutrino energy cut.
        
        // Fill the neutrino energy histogram.
        h_Neutrino_Energy_data->Fill(CCInclusiveReco_E);      // fill the neutrino energy histogram.
        
    }
    
    // Background Subtraction:
    MnvH1D *h_background_subtracted_mc = (MnvH1D*)h_Neutrino_Energy_mc->Clone("h_Neutrino_Energy_mc");
    h_background_subtracted_mc->Add(h_Neutrino_Energy_bkg, -1.);
    
    // #### Subtract the mc background from the mc event sample
    MnvH1D *h_background_subtracted_data = (MnvH1D*)h_Neutrino_Energy_data->Clone("h_Neutrino_Energy_data");
    
    MnvH1D *h_Neutrino_Energy_bkg_scaled = (MnvH1D*)h_Neutrino_Energy_bkg->Clone("h_Neutrino_Energy_bkg_scaled");
    h_Neutrino_Energy_bkg_scaled->Scale(total_data_pot/total_mc_pot);
    
    h_background_subtracted_data->Add(h_Neutrino_Energy_bkg_scaled,-1.);
    
    // #### Create a new mc histogram and scale it to data
    // #### Subtract the scaled mc background from data event sample
    
    // Unfolding:
    MnvH1D *h_unfold_data = new MnvH1D("h_unfolded_data","Neutrino Energy ; Unfolded Neutrino Energy (GeV); Events",30,0.0,15.0);
    MnvH1D *h_unfold_mc = new MnvH1D("h_unfolded_mc","Neutrino Energy ; Unfolded Neutrino Energy (GeV); Events",30,0.0,15.0);
    // #### Refer to HINT on section 6 on how to unfold the neutrino energy data and mc
    
    MinervaUnfold::MnvUnfold unfold;
    unfold.UnfoldHisto(h_unfold_mc, h_Neutrino_Energy_migr, h_background_subtracted_mc,h_Neutrino_Energy_true,h_background_subtracted_mc,RooUnfold::kBayes,2.,false);
    
    unfold.UnfoldHisto(h_unfold_data, h_Neutrino_Energy_migr, h_background_subtracted_data,h_Neutrino_Energy_true,h_background_subtracted_data,RooUnfold::kBayes,2.,false);
    
    // Efficiency:
    string filename_eff("/minerva/data/users/wospakrk/Minerva101Files/Histos/myEfficiencyAndPurity.root");
    TFile *histFileEff = new TFile(filename_eff.c_str(),"READ");
    MnvH1D *h_Eff_NeutrinoEnergy = (MnvH1D*)histFileEff->Get("h_Neutrino_Energy_eff_mc");
    
    // Efficiency correction:
    MnvH1D *h_cross_section_mc = (MnvH1D*)h_unfold_mc->Clone("CrossSectionMC");
    MnvH1D *h_cross_section_data = (MnvH1D*)h_unfold_mc->Clone("CrossSectionData");

    // ????? Correct the mc unfold distribution by the efficiency
    // ##### Initialize the data cross section and clone the data unfold distribution
    // ????? Correct the data unfold distribution by the efficiency
    h_cross_section_mc->Divide(h_unfold_mc, h_Eff_NeutrinoEnergy);
    h_cross_section_data->Divide(h_unfold_data, h_Eff_NeutrinoEnergy);
    
    // Flux:
    string fluxfilename("/minerva/data/users/wospakrk/Minerva101Files/Histos/Flux_v10r6p13_resurrection.root");
    TFile *fluxfile = new TFile( fluxfilename.c_str(), "READ" );
    MnvH1D *flux = (MnvH1D*)fluxfile->Get("flux_E_cvweighted");
    MnvH1D *h_flux_integrated = new MnvH1D("h_flux_original"," Energy ;  Energy (GeV); Events",30,0.0,15.0);
    
    // ????? Rebin the original flux to the bin of the neutrino energy
    // ##### See HINT on Section 8.
    for(int j=1; j < h_flux_integrated->GetNbinsX()+1; ++j ){
        double e_min = h_Eff_NeutrinoEnergy->GetBinLowEdge( j );
        double e_max = h_Eff_NeutrinoEnergy->GetBinLowEdge( j+1 );
        int b_min = flux->FindBin( e_min );
        int b_max = flux->FindBin( e_max ) - 1;
        double flux_cv =  flux->Integral( b_min, b_max, "width" );
        h_flux_integrated->SetBinContent( j, flux_cv );
    }

    h_flux_integrated->Scale( 1.0e-4 );                                          // flux unit area is in m^-2, convert to cm^-2!

    MnvH1D * h_xsec_fluxint_mc = (MnvH1D*)h_cross_section_mc->Clone("h_xsec_fluxint_mc");
    MnvH1D * h_xsec_fluxint_data = (MnvH1D*)h_cross_section_data->Clone("h_xsec__fluxint_data");
    
    h_xsec_fluxint_mc->Divide(h_cross_section_mc,h_flux_integrated);
    h_xsec_fluxint_data->Divide(h_cross_section_data,h_flux_integrated);
    
    // get the number of scattering targets in the fiducial volume
    double targets = TargetUtils::Get().GetTrackerNNucleons(104, 850);	// This is just a dummy number. Refer to HINT on section 9 to get  the actual numbers.
    
    //Write to the output TFile and close them
    out->Write();
    out->Clear();
    out->Close();
    
    // Making plots of histogram using the PlotUtils functionality
    // Set up a plotter object
    MnvPlotter *plotter = new MnvPlotter( );
    
    // POT scale used for normalizing the plots
    double scalemc = 1.0 / ( targets * total_mc_pot );
    double scaledata = 1.0 / ( targets * total_data_pot );
    // ##### Scale data and mc cross section to the above scales, respectively.
    h_xsec_fluxint_mc->Scale(scalemc);
    h_xsec_fluxint_data->Scale(scaledata);
    
    double scale = total_data_pot/total_mc_pot;	//use this for for data/mc comparison
    
    //Raw distribution:
    {
        TCanvas *c;
        c = new TCanvas("Raw_dis","Raw_dis", 1200, 900);
        plotter->DrawDataMC(h_Neutrino_Energy_data,h_Neutrino_Energy_mc,scale,"R");
        plotter->AddHistoTitle("CC Inclusive Samples", .05);
        plotter->MultiPrint(c,"Raw_distribution","png");
    }
    
    // Background Subtracted:
    {
        TCanvas *c;
        c = new TCanvas("cBGSubtract","BGSubtract", 1200, 900);
        // Remember that DrawDataMC and AddHistoTitle functions?
        plotter->DrawDataMC(h_background_subtracted_data,h_background_subtracted_mc,scale,"R");
        plotter->MultiPrint(c,"BGSubtracted","png");
    }
    
    // Migration:
    {
        TCanvas *c;
        c = new TCanvas("cMigration","Migration", 1200, 900);
        // ##### You can use the SetReadHeadPalette() or setWhiteRainbowPalette to make your migration matrix easier to read.
        h_Neutrino_Energy_migr->Draw("COLZ");
        plotter->MultiPrint(c,"Migration","png");
    }
    
    // Unfolding:
    {
        TCanvas *c;
        c = new TCanvas("cUnfolding","Unfolding", 1200, 900);
        // Again, DrawDataMC() and AddHistoTitle() are your friends...
        plotter->DrawDataMC(h_unfold_data,h_unfold_mc,scale,"R");
        plotter->MultiPrint(c,"Unfolding","png");
    }
    
    // Neutrino Energy Efficiency:
    {
        TCanvas *c;
        c = new TCanvas("cEffEnu","EffEnu", 1200, 900);
        // #### Draw efficiency histo here!
        plotter->DrawDataMC(h_cross_section_data, h_cross_section_mc, scale, "R");
        plotter->MultiPrint(c,"NeutrinoEfficiency","png");
    }
    
    // Flux Rebinned:
    {
        TCanvas *c;
        c = new TCanvas("cFluxRebinned","FluxRebinned",1200,900);
        // #### Draw flux integrated histo here!
        h_flux_integrated->Draw("R");
        plotter->MultiPrint(c,"FluxRebinned","png");
    }
    
    // Cross Section:
    {
        TCanvas *c;
        c = new TCanvas("cCrossSec","Enu", 1200, 900);
        // Okay, this is the last time I will nag about DrawDataMC and AddHistoTitle...
        plotter->DrawDataMC(h_xsec_fluxint_data, h_xsec_fluxint_mc, 1., "R");
        plotter->MultiPrint(c,"CrossSection","png");
    }
    
}


int main()
{
    Cross_section_Calculation();
    return 0;
}
