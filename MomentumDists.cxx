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

/*class AnalysisPlots {
public:
    
    AnalysisPlots();
    ~AnalysisPlots();
    
    TCanvas * VarVsInts();
    TCanvas * VarVsParticles();
    
};*/

//This is the POT for Data and Monte Carlo for the tuples that we use in this exercise.
//const double total_mc_pot = 1.79e19;

void MomentumDists(const string file, const string savename)
{
    ROOT::Cintex::Cintex::Enable();
    
    cout<<"Producing Momentum Distributions for MC files."<<endl;
    
    //TString savename = "CC1P1Pi_TEST";
    
    //Produce independent particles vars hists:
    string part_snam[3] = {"mu","pr","pi"};
    string part_name[3] = {"Muon", "Proton", "Pion"};
    string part_symb[3] = {"#mu^{-}", "p", "#pi^{#pm}"};
    int    part_colo[3] = {DrawingStyle::DSProton, DrawingStyle::DSPion, DrawingStyle::DSMuon};
    
    string var_unit[3] = {" (MeV/#it{c})", " (MeV/#it{c})", " (MeV)" };
    string var_symb[3] = {" #it{p}", " |#it{p}_{T}|", " E" };
    
    string pdg_cut[3] = { "CC1P1Pi_mu_PDG == 13", "CC1P1Pi_pr_PDG == 2212", "TMath::Abs(CC1P1Pi_pi_PDG) == 211"};
    
    int var_bin[3] = { 20, 20, 20};
    double var_range[6] = {0., 30., 0., 2., 0., 1.2};
    double truemom_range[6] = {0., 40., 0., 20., 0., 30.};

    
    string rec_var[3] = {"mom", "pTMag", "E"};
    string rec_name[3] = {"Momentum", "|#it{p}_{T}|", "Energy"};
    
    string true_var[3];
    string true_name[3];
    
    for(int i = 0; i < 3; i++){
        true_var[i] = "true" + rec_var[i];
        true_name[i] = "True " + rec_name[i];
    }
    
    TFile * outfile = new TFile(savename, "RECREATE");
    outfile->cd();
    
    DrawingTools * plot = new DrawingTools(file);
    
    for(int k = 0; k < 2; k++){
        
        string target = "mc_target == ";
        string tar_name;
        if(k == 0){
            target += "1";
            tar_name = "scint";
        }
        else{
            target += "2";
            tar_name = "carbon";
        }
        
        for(int i=0; i<3; i++){
            
            double mom_low = var_range[ i*2 ];
            double mom_hig = var_range[ i*2 + 1 ];
            int mom_bin = var_bin[i];
            
            double truemom_low = truemom_range[i*2];
            double truemom_hig = truemom_range[i*2 + 1 ];
            
            //DrawingTools * plot = new DrawingTools(testing_mc);
            
            TString common_cuts_p = target + " && accum_level > 4";
            TString tmp_part_name = part_name[i];
            TString tmp_part_snam = part_snam[i];
            TString tmp_part_symb = part_symb[i];
            TString tmp_pdg_cut = pdg_cut[i];
            
            //-----------------------------------------------------------------------------------------------------------------------------------------------------------------//
            
            TH1D * h_mom_p0 = plot->GetRecoHisto(Form("CC1P1Pi_%s_mom/1000",tmp_part_snam.Data()),mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s",common_cuts_p.Data()));
            
            int color = part_colo[i];
            plot->ColFill(h_mom_p0, color);
            
            double h_mom_p0_i = h_mom_p0->Integral();
            cout << " h_mom_p0_i = " << h_mom_p0_i << endl;
            
            TLegend * mom_bare = plot->Legend(0.25,0.2);
            mom_bare->AddEntry((TObject*)0, Form("Total Counts %d", (int)h_mom_p0_i),"");
            
            //Form("%s_%s_mom", tar_name.c_str(), tmp_part_name.Data())
            
            TCanvas * c_bare = new TCanvas( ( tar_name + "_" + tmp_part_name ), "", 800, 800);
            c_bare->cd();
            h_mom_p0->Draw("HIST");
            TLegend * pot_bare = plot-> GetPOT(0.7,0.8);
            pot_bare->Draw();
            mom_bare->Draw();
            outfile->cd();
            c_bare->Write();
            
            continue;
            //-----------------------------------------------------------------------------------------------------------------------------------------------------------------//
            
            TH1D * h_mom_p1 = plot->GetRecoHisto(Form("CC1P1Pi_%s_mom/1000",tmp_part_snam.Data()),mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s && CC1P1Pi_%s_PDG == 2212", common_cuts_p.Data(), tmp_part_snam.Data()));
            TH1D * h_mom_p2 = plot->GetRecoHisto(Form("CC1P1Pi_%s_mom/1000",tmp_part_snam.Data()),mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s && TMath::Abs(CC1P1Pi_%s_PDG) == 211", common_cuts_p.Data(), tmp_part_snam.Data()));
            TH1D * h_mom_p3 = plot->GetRecoHisto(Form("CC1P1Pi_%s_mom/1000",tmp_part_snam.Data()),mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s && CC1P1Pi_%s_PDG == 13", common_cuts_p.Data(), tmp_part_snam.Data()));
            TH1D * h_mom_p4 = plot->GetRecoHisto(Form("CC1P1Pi_%s_mom/1000",tmp_part_snam.Data()),mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s && CC1P1Pi_%s_PDG != 13 && TMath::Abs(CC1P1Pi_%s_PDG) != 211 && CC1P1Pi_%s_PDG != 2212", common_cuts_p.Data(), tmp_part_snam.Data(), tmp_part_snam.Data(), tmp_part_snam.Data()));
            
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
            
            TLegend * leg_part = plot->Legend(0.25, 0.4); //new TLegend(leg_ints_x, leg_ints_y, leg_ints_x + leg_ints_size_x, leg_ints_y + leg_ints_size_y);
            leg_part->AddEntry((TObject*)0, Form("Total Counts %d",(int)h_mom_p_tot), "");
            leg_part->AddEntry(h_mom_p1, Form("Proton (%.2f%%)",h_mom_p1_i), "f");
            leg_part->AddEntry(h_mom_p2, Form("#pi^{#pm} (%.2f%%)",h_mom_p2_i), "f");
            leg_part->AddEntry(h_mom_p3, Form("#mu^{-} (%.2f%%)",h_mom_p3_i), "f");
            leg_part->AddEntry(h_mom_p4, Form("Other (%.2f%%)",h_mom_p4_i), "f");
            
            TCanvas * c_parts = new TCanvas(Form("%s_mom_part_breakdown",tmp_part_name.Data()), "", 1000, 1000);
            c_parts->cd();
            hs_part->Draw();
            TLegend * pot_parts = plot->GetPOT(0.7,0.8);
            pot_parts->Draw();
            leg_part->Draw();
            c_parts->Write();
            
            //-----------------------------------------------------------------------------------------------------------------------------------------------------------------//
            
            TString common_cuts_i = common_cuts_p + " && mc_current == 1 && mc_intType";
            TH1D * h_mom_i1 = plot->GetRecoHisto(Form("CC1P1Pi_%s_mom/1000",tmp_part_snam.Data()), mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s%s", common_cuts_i.Data(), " == 1"));//QE
            TH1D * h_mom_i2 = plot->GetRecoHisto(Form("CC1P1Pi_%s_mom/1000",tmp_part_snam.Data()), mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s%s", common_cuts_i.Data(), " == 2"));//Res
            TH1D * h_mom_i3 = plot->GetRecoHisto(Form("CC1P1Pi_%s_mom/1000",tmp_part_snam.Data()), mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s%s", common_cuts_i.Data(), " == 3"));//Dis
            TH1D * h_mom_i4 = plot->GetRecoHisto(Form("CC1P1Pi_%s_mom/1000",tmp_part_snam.Data()), mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s%s", common_cuts_i.Data(), " >= 4"));//Other
            TH1D * h_mom_i5 = plot->GetRecoHisto(Form("CC1P1Pi_%s_mom/1000",tmp_part_snam.Data()), mom_bin, mom_low, mom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s && mc_current != 1", common_cuts_p.Data()));//NC/no current
            h_mom_i4->Add(h_mom_i5);//Add NC ints to other.
            
            plot->ColFill(h_mom_i1, DrawingStyle::DSQes);
            plot->ColFill(h_mom_i2, DrawingStyle::DSRes);
            plot->ColFill(h_mom_i3, DrawingStyle::DSDis);
            plot->ColFill(h_mom_i4, DrawingStyle::DSOther);
            
            THStack * hs_ints = new THStack(Form("hs%s_ints", tmp_part_snam.Data()), Form(";#it{p}_{%s} (GeV/#it{c});Counts", tmp_part_symb.Data()));
            hs_ints->Add(h_mom_i1);
            hs_ints->Add(h_mom_i2);
            hs_ints->Add(h_mom_i3);
            hs_ints->Add(h_mom_i4);
            
            double h_mom_i1_i = h_mom_i1->Integral();
            double h_mom_i2_i = h_mom_i2->Integral();
            double h_mom_i3_i = h_mom_i3->Integral();
            double h_mom_i4_i = h_mom_i4->Integral();
            double h_mom_i_tot = h_mom_i1_i + h_mom_i2_i + h_mom_i3_i + h_mom_i4_i;
            
            h_mom_i1_i *= 100/h_mom_i_tot;
            h_mom_i2_i *= 100/h_mom_i_tot;
            h_mom_i3_i *= 100/h_mom_i_tot;
            h_mom_i4_i *= 100/h_mom_i_tot;
            
            TLegend * leg_ints = plot->Legend(0.3, 0.45); //new TLegend(leg_ints_x, leg_ints_y, leg_ints_x + leg_ints_size_x, leg_ints_y + leg_ints_size_y);
            leg_ints->AddEntry((TObject*)0, Form("Total Counts %d",(int)h_mom_i_tot), "");
            leg_ints->AddEntry(h_mom_i1, Form("CC Quasi El. (%.2f %%)",h_mom_i1_i), "f");
            leg_ints->AddEntry(h_mom_i2, Form("CC Resonance (%.2f %%)",h_mom_i2_i), "f");
            leg_ints->AddEntry(h_mom_i3, Form("CC DIS (%.2f%%)",h_mom_i3_i), "f");
            leg_ints->AddEntry(h_mom_i4, Form("Other (inc. NC) (%.2f %%)",h_mom_i4_i), "f");
            
            TCanvas * c_ints = new TCanvas(Form("%s_mom_ints_breakdown",tmp_part_name.Data()), "", 1000, 1000);
            c_ints->cd();
            hs_ints->Draw();
            TLegend * pot_ints = plot->GetPOT(0.7,0.8);
            pot_ints->Draw();
            leg_ints->Draw();
            c_ints->Write();
            
            //-----------------------------------------------------------------------------------------------------------------------------------------------------------------//
            
            
            TH2D * h_smear_nc = plot->SmearMatrix(Form("CC1P1Pi_%s_truemom/1000:CC1P1Pi_%s_mom/1000", tmp_part_snam.Data(), tmp_part_snam.Data()), mom_bin, mom_low, mom_hig, Form("Reco #it{p}_{%s} (GeV/#it{c}); True #it{p}_{%s} (GeV/#it{c})", tmp_part_symb.Data(), tmp_part_symb.Data()), "accum_level > 4");
            //SmearMatrix(const TString vars_yx, int nbins, const double low, const double high, const TString xy_title = "", const TString cuts = "");
            
            TCanvas * c_smear_nc = new TCanvas(Form("%s_mom_smear_nocuts",tmp_part_name.Data()), "", 1000, 1000);
            c_smear_nc->cd();
            h_smear_nc->Draw("COLZ");
            c_smear_nc->Write();
            
            //-----------------------------------------------------------------------------------------------------------------------------------------------------------------//
            
            TH2D * h_smear_pdgc = plot->SmearMatrix(Form("CC1P1Pi_%s_truemom/1000:CC1P1Pi_%s_mom/1000", tmp_part_snam.Data(), tmp_part_snam.Data()), mom_bin, mom_low, mom_hig, Form("Reco #it{p}_{%s} (GeV/#it{c}); True #it{p}_{%s} (GeV/#it{c})", tmp_part_symb.Data(), tmp_part_symb.Data()), Form("accum_level > 4 && %s", tmp_pdg_cut.Data()));
            TCanvas * c_smear_pdgc = new TCanvas(Form("%s_mom_smear_PDGcuts",tmp_part_name.Data()), "", 1000, 1000);
            c_smear_pdgc->cd();
            h_smear_pdgc->Draw("COLZ");
            c_smear_pdgc->Write();
            
            if(!tmp_part_snam.EqualTo("mu", TString::kExact)){
                TString xyz_cut = Form("TMath::Abs(CC1P1Pi_%s_endpos_xyz[0]) < 1000 && TMath::Abs(CC1P1Pi_%s_endpos_xyz[1]) < 1000 && CC1P1Pi_%s_endpos_xyz[2] < 9950", tmp_part_snam.Data(), tmp_part_snam.Data(), tmp_part_snam.Data());
                TH2D * h_smear_xyzc = plot->SmearMatrix(Form("CC1P1Pi_%s_truemom/1000:CC1P1Pi_%s_mom/1000", tmp_part_snam.Data(), tmp_part_snam.Data()), mom_bin, mom_low, mom_hig, Form("Reco #it{p}_{%s} (GeV/#it{c}); True #it{p}_{%s} (GeV/#it{c})", tmp_part_symb.Data(), tmp_part_symb.Data()), Form("accum_level > 4 && %s", xyz_cut.Data()));
                TCanvas * c_smear_xyzc = new TCanvas(Form("%s_mom_smear_XYZcuts",tmp_part_name.Data()), "", 1000, 1000);
                c_smear_xyzc->cd();
                h_smear_xyzc->Draw("COLZ");
                c_smear_xyzc->Write();
            }
            
            //-----------------------------------------------------------------------------------------------------------------------------------------------------------------//
            
            TString common_cuts_i_true = "accum_level > 4 && mc_current == 1 && mc_intType";
            TH1D * h_mom_i1_true = plot->GetRecoHisto(Form("CC1P1Pi_%s_truemom/1000",tmp_part_snam.Data()), mom_bin, truemom_low, truemom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s%s", common_cuts_i_true.Data(), " == 1"));//QE
            TH1D * h_mom_i2_true = plot->GetRecoHisto(Form("CC1P1Pi_%s_truemom/1000",tmp_part_snam.Data()), mom_bin, truemom_low, truemom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s%s", common_cuts_i_true.Data(), " == 2"));//Res
            TH1D * h_mom_i3_true = plot->GetRecoHisto(Form("CC1P1Pi_%s_truemom/1000",tmp_part_snam.Data()), mom_bin, truemom_low, truemom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s%s", common_cuts_i_true.Data(), " == 3"));//Dis
            TH1D * h_mom_i4_true = plot->GetRecoHisto(Form("CC1P1Pi_%s_truemom/1000",tmp_part_snam.Data()), mom_bin, truemom_low, truemom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s%s", common_cuts_i_true.Data(), " >= 4"));//Other
            TH1D * h_mom_i5_true = plot->GetRecoHisto(Form("CC1P1Pi_%s_truemom/1000",tmp_part_snam.Data()), mom_bin, truemom_low, truemom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s && mc_current != 1", common_cuts_p.Data()));//NC/no current
            h_mom_i4_true->Add(h_mom_i5_true);//Add NC ints to other.
            
            plot->ColFill(h_mom_i1_true, DrawingStyle::DSQes);
            plot->ColFill(h_mom_i2_true, DrawingStyle::DSRes);
            plot->ColFill(h_mom_i3_true, DrawingStyle::DSDis);
            plot->ColFill(h_mom_i4_true, DrawingStyle::DSOther);
            
            THStack * hs_ints_true = new THStack(Form("hs%s_ints_true", tmp_part_snam.Data()), Form(";#it{p}_{%s} (GeV/#it{c});Counts", tmp_part_symb.Data()));
            hs_ints_true->Add(h_mom_i1_true);
            hs_ints_true->Add(h_mom_i2_true);
            hs_ints_true->Add(h_mom_i3_true);
            hs_ints_true->Add(h_mom_i4_true);
            
            double h_mom_i1_i_true = h_mom_i1_true->Integral();
            double h_mom_i2_i_true = h_mom_i2_true->Integral();
            double h_mom_i3_i_true = h_mom_i3_true->Integral();
            double h_mom_i4_i_true = h_mom_i4_true->Integral();
            double h_mom_i_tot_true = h_mom_i1_i_true + h_mom_i2_i_true + h_mom_i3_i_true + h_mom_i4_i_true;
            
            h_mom_i1_i_true *= 100/h_mom_i_tot_true;
            h_mom_i2_i_true *= 100/h_mom_i_tot_true;
            h_mom_i3_i_true *= 100/h_mom_i_tot_true;
            h_mom_i4_i_true *= 100/h_mom_i_tot_true;
            
            TLegend * leg_ints_true = plot->Legend(0.3, 0.45); //new TLegend(leg_ints_x, leg_ints_y, leg_ints_x + leg_ints_size_x, leg_ints_y + leg_ints_size_y);
            leg_ints_true->AddEntry((TObject*)0, Form("Total Counts %d",(int)h_mom_i_tot_true), "");
            leg_ints_true->AddEntry(h_mom_i1_true, Form("CC Quasi El. (%.2f %%)",h_mom_i1_i_true), "f");
            leg_ints_true->AddEntry(h_mom_i2_true, Form("CC Resonance (%.2f %%)",h_mom_i2_i_true), "f");
            leg_ints_true->AddEntry(h_mom_i3_true, Form("CC DIS (%.2f%%)",h_mom_i3_i_true), "f");
            leg_ints_true->AddEntry(h_mom_i4_true, Form("Other (inc. NC) (%.2f %%)",h_mom_i4_i_true), "f");
            
            TCanvas * c_ints_true = new TCanvas(Form("%s_truemom_ints_breakdown",tmp_part_name.Data()), "", 1000, 1000);
            c_ints_true->cd();
            hs_ints_true->Draw();
            TLegend * pot_ints_true = plot->GetPOT(0.7,0.8);
            pot_ints_true->Draw();
            leg_ints_true->Draw();
            c_ints_true->Write();
            
            //-----------------------------------------------------------------------------------------------------------------------------------------------------------------//
            
            TH1D * h_mom_p1_true = plot->GetRecoHisto(Form("CC1P1Pi_%s_truemom/1000",tmp_part_snam.Data()),mom_bin, truemom_low, truemom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s && CC1P1Pi_%s_PDG == 2212", common_cuts_p.Data(), tmp_part_snam.Data()));
            TH1D * h_mom_p2_true = plot->GetRecoHisto(Form("CC1P1Pi_%s_truemom/1000",tmp_part_snam.Data()),mom_bin, truemom_low, truemom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s && TMath::Abs(CC1P1Pi_%s_PDG) == 211", common_cuts_p.Data(), tmp_part_snam.Data()));
            TH1D * h_mom_p3_true = plot->GetRecoHisto(Form("CC1P1Pi_%s_truemom/1000",tmp_part_snam.Data()),mom_bin, truemom_low, truemom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s && CC1P1Pi_%s_PDG == 13", common_cuts_p.Data(), tmp_part_snam.Data()));
            TH1D * h_mom_p4_true = plot->GetRecoHisto(Form("CC1P1Pi_%s_truemom/1000",tmp_part_snam.Data()),mom_bin, truemom_low, truemom_hig, Form("#it{p}_{%s} (GeV/#it{c})",tmp_part_symb.Data()), Form("%s && CC1P1Pi_%s_PDG != 13 && TMath::Abs(CC1P1Pi_%s_PDG) != 211 && CC1P1Pi_%s_PDG != 2212", common_cuts_p.Data(), tmp_part_snam.Data(), tmp_part_snam.Data(), tmp_part_snam.Data()));
            
            plot->ColFill(h_mom_p1_true, DrawingStyle::DSProton);
            plot->ColFill(h_mom_p2_true, DrawingStyle::DSPion);
            plot->ColFill(h_mom_p3_true, DrawingStyle::DSMuon);
            plot->ColFill(h_mom_p4_true, DrawingStyle::DSOther);
            
            THStack * hs_part_true = new THStack(Form("hs%s_part_true", tmp_part_snam.Data()),Form(";#it{p}_{%s} (GeV/#it{c});Counts", tmp_part_symb.Data()));
            hs_part_true->Add(h_mom_p1_true);
            hs_part_true->Add(h_mom_p2_true);
            hs_part_true->Add(h_mom_p3_true);
            hs_part_true->Add(h_mom_p4_true);
            
            double h_mom_p1_i_true = h_mom_p1_true->Integral();
            double h_mom_p2_i_true = h_mom_p2_true->Integral();
            double h_mom_p3_i_true = h_mom_p3_true->Integral();
            double h_mom_p4_i_true = h_mom_p4_true->Integral();
            double h_mom_p_tot_true = h_mom_p1_i_true + h_mom_p2_i_true + h_mom_p3_i_true + h_mom_p4_i_true;
            
            cout << " s_h_mom_p_tot_true = " << h_mom_p_tot_true << endl;
            
            h_mom_p1_i_true *= 100/h_mom_p_tot_true;
            h_mom_p2_i_true *= 100/h_mom_p_tot_true;
            h_mom_p3_i_true *= 100/h_mom_p_tot_true;
            h_mom_p4_i_true *= 100/h_mom_p_tot_true;
            
            TLegend * leg_part_true = plot->Legend(0.25, 0.4); //new TLegend(leg_ints_x, leg_ints_y, leg_ints_x + leg_ints_size_x, leg_ints_y + leg_ints_size_y);
            leg_part_true->AddEntry((TObject*)0, Form("Total Counts %d",(int)h_mom_p_tot_true), "");
            leg_part_true->AddEntry(h_mom_p1_true, Form("Proton (%.2f%%)",h_mom_p1_i_true), "f");
            leg_part_true->AddEntry(h_mom_p2_true, Form("#pi^{#pm} (%.2f%%)",h_mom_p2_i_true), "f");
            leg_part_true->AddEntry(h_mom_p3_true, Form("#mu^{-} (%.2f%%)",h_mom_p3_i_true), "f");
            leg_part_true->AddEntry(h_mom_p4_true, Form("Other (%.2f%%)",h_mom_p4_i_true), "f");
            
            TCanvas * c_parts_true = new TCanvas(Form("%s_truemom_part_breakdown",tmp_part_name.Data()), "", 1000, 1000);
            c_parts_true->cd();
            hs_part_true->Draw();
            TLegend * pot_parts_true = plot->GetPOT(0.7,0.8);
            pot_parts_true->Draw();
            leg_part_true->Draw();
            c_parts_true->Write();
            
            //-----------------------------------------------------------------------------------------------------------------------------------------------------------------//
        }
        
        std::vector<TString> cut_names;
        cut_names.push_back("Vertex");
        cut_names.push_back("3 Tracks");
        cut_names.push_back("Muon Track");
        cut_names.push_back("Contained Vtx");
        cut_names.push_back("PID: p/#pi^{+}");
        
        EffPurTools * m_ep = new EffPurTools(file, cut_names);
        
        TH1D * purcut = m_ep->PurVSCuts("mc_intType == 2 && mc_current == 1");
        
        if(purcut){
            TCanvas * pur_can = new TCanvas("pur_can", "", 500,500);
            pur_can->cd();
            purcut->Draw();
            outfile->cd();
            pur_can->Write();
            //pur_can->Print("first_pur_plot.eps");
        }
        
        TH1D * effcut = m_ep->EffVSCuts("mc_intType == 2 && mc_current == 1");
        
        if(effcut){
            TCanvas * eff_can = new TCanvas("eff_can", "", 500,500);
            eff_can->cd();
            effcut->Draw();
            outfile->cd();
            eff_can->Write();
            //eff_can->Print("first_eff_plot.eps");
        }
    
    }
    
  /*  TH1D * effvar = m_ep->PurVSVar("CC1P1Pi_mu_mom/1000", 20, 0, 30., "mc_current == 1","#it{p}_{#mu} (GeV/#it{c})");
    
    if(effvar){
        TCanvas * eff_vac_can = new TCanvas("eff_vac_can", "", 500,500);
        eff_vac_can->cd();
        effvar->Draw();
        outfile->cd();
        eff_vac_can->Write();
        //eff_vac_can->Print("first_eff_var_plot.eps");
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
    
    TLegend * leg_part = plot->Legend(0.25, 0.4); //new TLegend(leg_ints_x, leg_ints_y, leg_ints_x + leg_ints_size_x, leg_ints_y + leg_ints_size_y);
    leg_part->AddEntry((TObject*)0, Form("Total Counts %d",(int)h_mom_p_tot), "");
    leg_part->AddEntry(h_mom_p1, Form("Proton (%.2f%%)",h_mom_p1_i), "f");
    leg_part->AddEntry(h_mom_p2, Form("#pi^{#pm} (%.2f%%)",h_mom_p2_i), "f");
    leg_part->AddEntry(h_mom_p3, Form("#mu^{-} (%.2f%%)",h_mom_p3_i), "f");
    leg_part->AddEntry(h_mom_p4, Form("Other (%.2f%%)",h_mom_p4_i), "f");
    
    TCanvas * c_parts = new TCanvas(Form("%s_mom_part_breakdown",tmp_part_name.Data()), "", 1000, 1000);
    c_parts->cd();
    hs_part->Draw();
    TLegend * pot_parts = plot->GetPOT(0.7,0.8);
    pot_parts->Draw();
    leg_part->Draw();
    return c_parts;
}*/


int main(int argc, char *argv[])
{
    string filename = testing_mc;
    string savename = "CC1P1Pi_Plots.root";
    
    char cc;
    while((cc = getopt(argc, argv, "i:o:")) != -1){
        switch (cc){
            case 'i': filename = optarg; break;
            case 'o': savename = optarg; break;
            default: return 1;
        }
    }
    
    MomentumDists(filename, savename);
    
        return 0;
}
