#include "TCanvas.h"
#include "TObject.h"

//#include "TDirectory.h"
//#include "TROOT.h"
#include "TClass.h"
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
#include "DrawingTools.h"
#include "TH1D.h"
#include "THStack.h"
#include "TLegend.h"
#include "TFile.h"

//#include <TStyle.h>

using namespace std;
using namespace PlotUtils;

//These are the tuples used for this exercise
const string testing_mc("/pnfs/minerva/persistent/users/dcoplowe/CC1P1Pi_PL13C_180816/merged_CC1P1Pi_CC1P1Pi_run00013200.root");

const string flag("CC1P1Pi");

//This is the POT for Data and Monte Carlo for the tuples that we use in this exercise.
//const double total_mc_pot = 1.79e19;

void MomentumDists()
{
    ROOT::Cintex::Cintex::Enable();
    
    cout<<"Producing Momentum Distributions for MC files."<<endl;
    
    TString savename = "CC1P1Pi_TEST";
    
    //Produce independent particles vars hists:
    string part_snam[3] = {"mu","pr","pi"};
    string part_name[3] = {"Muon", "Proton", "Pion"};
    string part_symb[3] = {"#mu^{-}", "p", "#pi^{#pm}"};
    int    part_colo[3] = {DrawingStyle::DSProton, DrawingStyle::DSPion, DrawingStyle::DSMuon};
    
    string var_unit[3] = {" (MeV/#it{c})", " (MeV/#it{c})", " (MeV)" };
    string var_symb[3] = {" #it{p}", " |#it{p}_{T}|", " E" };
    
    double var_range[6] = {0., 4000., 0., 4000., 0., 4000.,};
    
    string rec_var[3] = {"mom", "pTMag", "E"};
    string rec_name[3] = {"Momentum", "|#it{p}_{T}|", "Energy"};
    
    string true_var[3];
    string true_name[3];
    
    for(int i = 0; i < 3; i++){
        true_var[i] = "true" + rec_var[i];
        true_name[i] = "True " + rec_name[i];
    }
    
    TFile * outfile = new TFile(Form("plots_%s_.root", savename.Data()),"RECREATE");
    outfile->cd();
    
    DrawingTools * plot = new DrawingTools(testing_mc);
    
    for(int i=0; i<3; i++){
    
        //DrawingTools * plot = new DrawingTools(testing_mc);
        
        TString common_cuts_p = "accum_level > 4";
        TString tmp_part_name = part_name[i];
        TString tmp_part_snam = part_snam[i];
        TString tmp_part_symb = part_symb[i];
        
        MnvH1D * h_mom_p0 = plot->GetRecoHisto(Form("CC1P1Pi_%s_mom",tmp_part_snam.Data()),20, 0., 4000., Form("#it{p}_{%s} (MeV/#it{c})",tmp_part_symb.Data()), Form("%s",common_cuts_p.Data()));
        
        int color = part_colo[i];
        plot->ColFill(h_mom_p0, color);
        
        TCanvas * c_bare = new TCanvas(Form("%s_mom",tmp_part_name.Data()), "", 450, 450);
        c_bare->cd();
        h_mom_p0->Draw("HIST");
       // TLegend * pot_bare = plot->GetPOT(0.7,0.8);
        //pot_bare->Draw();
        outfile->cd();
        c_bare->Write();
        c_bare->Print(Form("%s_mom.eps", tmp_part_name.Data()));
        
        //delete h_mom_p0;

      /*  MnvH1D * h_mom_p1 = plot->GetRecoHisto(Form("CC1P1Pi_%s_mom",tmp_part_snam.Data()),20, 0., 4000., Form("#it{p}_{%s} (MeV/#it{c})",tmp_part_symb.Data()), Form("%s && CC1P1Pi_%s_PDG == 2212", common_cuts_p.Data(), tmp_part_snam.Data()));
        MnvH1D * h_mom_p2 = plot->GetRecoHisto(Form("CC1P1Pi_%s_mom",tmp_part_snam.Data()),20, 0., 4000., Form("#it{p}_{%s} (MeV/#it{c})",tmp_part_symb.Data()), Form("%s && TMath::Abs(CC1P1Pi_%s_PDG == 211)", common_cuts_p.Data(), tmp_part_snam.Data()));
        MnvH1D * h_mom_p3 = plot->GetRecoHisto(Form("CC1P1Pi_%s_mom",tmp_part_snam.Data()),20, 0., 4000., Form("#it{p}_{%s} (MeV/#it{c})",tmp_part_symb.Data()), Form("%s && CC1P1Pi_%s_PDG == 13", common_cuts_p.Data(), tmp_part_snam.Data()));
        MnvH1D * h_mom_p4 = plot->GetRecoHisto(Form("CC1P1Pi_%s_mom",tmp_part_snam.Data()),20, 0., 4000., Form("#it{p}_{%s} (MeV/#it{c})",tmp_part_symb.Data()), Form("%s && CC1P1Pi_%s_PDG != 13 && TMath::Abs(CC1P1Pi_%s_PDG) != 211 && CC1P1Pi_%s_PDG != 2212", common_cuts_p.Data(), tmp_part_snam.Data(), tmp_part_snam.Data(), tmp_part_snam.Data()));
        
        TH1D * s_h_mom_p1 = new TH1D(h_mom_p1->GetStatError());
        TH1D * s_h_mom_p2 = new TH1D(h_mom_p2->GetStatError());
        TH1D * s_h_mom_p3 = new TH1D(h_mom_p3->GetStatError());
        TH1D * s_h_mom_p4 = new TH1D(h_mom_p4->GetStatError());
        
        plot->ColFill(s_h_mom_p1, DrawingStyle::DSProton);
        plot->ColFill(s_h_mom_p2, DrawingStyle::DSPion);
        plot->ColFill(s_h_mom_p3, DrawingStyle::DSMuon);
        plot->ColFill(s_h_mom_p4, DrawingStyle::DSOther);
        
        THStack * hs_part = new THStack();
        hs_part->Add(s_h_mom_p1);
        hs_part->Add(s_h_mom_p2);
        hs_part->Add(s_h_mom_p3);
        hs_part->Add(s_h_mom_p4);
        
        TCanvas * c_parts = new TCanvas(Form("%s_mom_part_breakdown",tmp_part_name.Data()), "", 450, 450);
        c_parts->cd();
        hs_part->Draw();
        //TLegend * pot_parts = plot->GetPOT(0.7,0.8);
        //pot_parts->Draw();
        //c_parts->Write();
        c_parts->Print(Form("%s_mom_part_breakdown.eps", tmp_part_name.Data()));
        
     
        /*TString common_cuts_i = "accum_level > 4 && mc_current == 1 && mc_intType"
        MnvH1D * h_mom_i1 = plot->GetRecoHisto(Form("CC1P1Pi_%s_mom",part_snam[i]),20, 0., 4000., Form("#it{p}_{%s} (MeV/#it{c})",part_symb[i]), Form("%s%s", common_cuts_i.Data(), " == 1"));//QE
        MnvH1D * h_mom_i2 = plot->GetRecoHisto(Form("CC1P1Pi_%s_mom",part_snam[i]),20, 0., 4000., Form("#it{p}_{%s} (MeV/#it{c})",part_symb[i]), Form("%s%s", common_cuts_i.Data(), " == 2"));//Res
        MnvH1D * h_mom_i3 = plot->GetRecoHisto(Form("CC1P1Pi_%s_mom",part_snam[i]),20, 0., 4000., Form("#it{p}_{%s} (MeV/#it{c})",part_symb[i]), Form("%s%s", common_cuts_i.Data(), " == 3"));//Dis
        MnvH1D * h_mom_i4 = plot->GetRecoHisto(Form("CC1P1Pi_%s_mom",part_snam[i]),20, 0., 4000., Form("#it{p}_{%s} (MeV/#it{c})",part_symb[i]), Form("%s%s", common_cuts_i.Data(), " >= 4"));//Other
        MnvH1D * h_mom_i5 = plot->GetRecoHisto(Form("CC1P1Pi_%s_mom",part_snam[i]),20, 0., 4000., Form("#it{p}_{%s} (MeV/#it{c})",part_symb[i]), Form("%s && mc_current != 1", common_cuts_p.Data()));//NC/no current
        h_mom_i4->Add(h_mom_i5);//Add NC ints to other.
        
        TH1D * s_h_mom_i1 = new TH1D(h_mom_i1->GetStatError());
        TH1D * s_h_mom_i2 = new TH1D(h_mom_i2->GetStatError());
        TH1D * s_h_mom_i3 = new TH1D(h_mom_i3->GetStatError());
        TH1D * s_h_mom_i4 = new TH1D(h_mom_i4->GetStatError());
        
        
        THStack * hs_ints = new THStack();
        hs_ints->Add(s_h_mom_i1);
        hs_ints->Add(s_h_mom_i2);
        hs_ints->Add(s_h_mom_i3);
        hs_ints->Add(s_h_mom_i4);
        hs_ints->Add(s_h_mom_i5);
        
        delete h_mom_p0;
        delete h_mom_p1;
        delete h_mom_p2;
        delete h_mom_p3;
        delete h_mom_p4;
        
        delete s_h_mom_p1;
        delete s_h_mom_p2;
        delete s_h_mom_p3;
        delete s_h_mom_p4;
        
        delete hs_part;
        
        delete h_mom_i1;
        delete h_mom_i2;
        delete h_mom_i3;
        delete h_mom_i4;
        delete h_mom_i5;
        
        delete s_h_mom_i1;
        delete s_h_mom_i2;
        delete s_h_mom_i3;
        delete s_h_mom_i4;
        delete s_h_mom_i5;
        
        delete hs_ints;*/
        
    }
    
    outfile->Close();
    
    //delete plot;
    
    
    
    
    
    std::vector<TString> cut_names;
    cut_names.push_back("Vertex");
    cut_names.push_back("3 Tracks");
    cut_names.push_back("Muon Track");
    cut_names.push_back("Contained Vtx");
    cut_names.push_back("PID: p/#pi^{+}");
    
   // EffPurTools * m_ep = new EffPurTools(testing_mc, cut_names);
   /*
    MnvH1D * purcut = m_ep->PurVSCuts("mc_intType == 2 && mc_current == 1 && mc_resID == 0");
    
    if(purcut){
        TCanvas * pur_can = new TCanvas("pur_can", "", 500,500);
        pur_can->cd();
        purcut->Draw();
        pur_can->Print("first_pur_plot.eps");
    }
    
    MnvH1D * effcut = m_ep->EffVSCuts("mc_intType == 2 && mc_current == 1 && mc_resID == 0");
    
    if(effcut){
        TCanvas * eff_can = new TCanvas("eff_can", "", 500,500);
        eff_can->cd();
        effcut->Draw();
        eff_can->Print("first_eff_plot.eps");
    }
    */
    
    
  /*  MnvH1D * effvar = m_ep->EffVSVar("mc_w", 10, 0, 2000, "mc_current == 1","W^{2} (MeV^{2})");
    
    if(effvar){
        TCanvas * eff_vac_can = new TCanvas("eff_vac_can", "", 500,500);
        eff_vac_can->cd();
        effvar->Draw();
        eff_vac_can->Print("first_eff_var_plot.eps");
    }*/
}


int main()
{
    MomentumDists();
    return 0;
}
