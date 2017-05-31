#include "BreakdownTools.h"
#include "DrawingStyle.h"
#include "DataClasses.h"

#include <string>
#include <iostream>

#include "TH1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLegend.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TList.h"
#include "TF1.h"
#include <TLine.h>

BreakdownTools::BreakdownTools(std::string filename, std::string treename, Topologies * topologies, std::string target_name) : 
    DrawingTools(filename, treename, ("BD" + treename).c_str()), m_printPOT(false), m_fullbreakdown(true) {

//    PDGInfo proton;
    m_pdglist.push_back( PDGInfo(2212, "proton",        "p"));
    m_pdglist.push_back( PDGInfo(211,  "piplus",        "#pi^{+}") );
    m_pdglist.push_back( PDGInfo(-211, "piminus",       "#pi^{-}") );
    m_pdglist.push_back( PDGInfo(2112, "neutron",       "n") );
    m_pdglist.push_back( PDGInfo(11,   "electronpm",    "e^{#pm}", true) );
    m_pdglist.push_back( PDGInfo(13,   "muon",          "#mu^{-}") );
    m_pdglist.push_back( PDGInfo(-13,  "amuon",         "#mu^{+}") );
    m_pdglist.push_back( PDGInfo(111,  "pizero",        "#pi^{0}") );
    m_pdglist.push_back( PDGInfo(321,  "kapm",          "K^{#pm}", true) );
    m_pdglist.push_back( PDGInfo(311,  "kazero",        "K^{0}") );
    //Miminum particles to define in breakdown:
    ResetPDGBDlist();

    m_topologies = new Topologies( topologies );

    if(!m_topologies->GetList2Draw().empty()){
        ClearTOPBDlist();
        m_toplist_minBD = m_topologies->GetList2Draw();
    }

    m_target = target_name;
    
    m_toplist = m_topologies->GetList();
    ResetTOPBDlist();

    m_statcounter = -1;

    m_signal = m_topologies->GetSignal();
    m_sig_top = m_topologies->GetTopology(m_signal);
}

BreakdownTools::~BreakdownTools(){
    m_pdglist.clear();
    m_toplist.clear();

    ClearPDGBDlist();
    ClearTOPBDlist();

    delete m_topologies;
}

void BreakdownTools::ResetPDGBDlist(){
    ClearPDGBDlist();
    SetMinPDGBDlist(2212);
    SetMinPDGBDlist(2112);
    SetMinPDGBDlist(211);
    SetMinPDGBDlist(-211);
    SetMinPDGBDlist(13);
}

void BreakdownTools::ResetTOPBDlist(){
    ClearTOPBDlist();
    SetMinTOPBDlist( Topology::CC1P1PiPlus        );
    SetMinTOPBDlist( Topology::CCNPNPiMinus       );
    SetMinTOPBDlist( Topology::CCNP               );
    SetMinTOPBDlist( Topology::CCNPiPlus          );
    SetMinTOPBDlist( Topology::CCNPNPiZero        );
    SetMinTOPBDlist( Topology::CCNPiZeroNPiPlus   );
    SetMinTOPBDlist( Topology::Other              );
}

BDCans BreakdownTools::PID(Variable var, Int_t nbins, Double_t * bins, std::string pdgvar, std::string cuts, bool check){

    std::vector<Breakdown> list;
    list.clear();

    std::string other_cut = cuts;
    
    std::string tmp_cuts_1 = cuts;
    if(!cuts.empty()){
        tmp_cuts_1 += " && ";
        other_cut += " && (";
    }
    
    tmp_cuts_1 += pdgvar;
    tmp_cuts_1 += " != -999 ";
    tmp_cuts_1 += " && ";

    other_cut += pdgvar;
    other_cut += " != -999";

    for(int i = 0; i < (int)m_pdglist.size(); i++){
        PDGInfo particle = m_pdglist[i];

        std::string tmp_cuts = tmp_cuts_1;
        if(particle.IsBoth()) tmp_cuts += "TMath::Abs(";
        tmp_cuts += pdgvar;
        if(particle.IsBoth()) tmp_cuts += ")";
        tmp_cuts += " == ";
        tmp_cuts += particle.GetPDGStr();
                
        if(other_cut.empty()){
            if(particle.IsBoth()) other_cut += "TMath::Abs(";
            other_cut = pdgvar;
            if(particle.IsBoth()) other_cut += ")";
            other_cut += " != ";
            other_cut += particle.GetPDGStr();
        }
        else{
            other_cut += " && ";
            if(particle.IsBoth()) other_cut += "TMath::Abs(";
            other_cut += pdgvar;
            if(particle.IsBoth()) other_cut += ")";
            other_cut += " != ";
            other_cut += particle.GetPDGStr();
        }

        Breakdown tmp(particle.GetSymbol(), tmp_cuts, particle.GetColor());
        list.push_back(tmp);
    }

    Breakdown other("Other", other_cut, DrawingStyle::Other);
    list.push_back(other);

    return BaseBreakdown(var, nbins, bins, list, "PID", cuts, check);
}

BDCans BreakdownTools::PID(Variable var, Int_t nbins, Double_t low, Double_t high, std::string pdgvar, std::string cuts, bool check){
    return PID(var, nbins, SetBinning(nbins, low, high), pdgvar, cuts, check);
}

BDCans BreakdownTools::TOPO(Variable var, Int_t nbins, Double_t * bins, std::string cuts, bool check){
    // cout << "BreakdownTools::TOPO(Variable var, Int_t nbins, Double_t * bins, std::string cuts)" << endl;
    std::vector<DrawingTools::KinMap> kinmap_list;
    std::vector<Int_t> top2draw;
    
    std::string tmp_cuts_1 = cuts;
    if(!cuts.empty()) tmp_cuts_1 += " && ";
    
    // cout << Topology::ToString(m_signal) << endl;

    bool total_CC1p1pi = true;

    for(int i = 0; i < (int)m_toplist.size(); i++){
        if(m_toplist[i].GetType() == Topology::OCC1P1PiPlus){
           total_CC1p1pi = false;
           continue;
       }
    }

    for(int i = 0; i < (int)m_toplist.size(); i++){
        
//        cout << i << ":" << (int)m_toplist.size() << endl;
        
        Topology topology = m_toplist[i];

        if(print_level::quiet){ 
            cout << "Topology = " << topology.GetName() << endl;
            cout << "           " << topology.GetSignal() << endl;
        }

        // if(topology.GetType() == Topology::Other){
        //     cout << "Signal = " << topology.GetSignal() << endl;
        // }

        if(topology.GetType() == m_signal){ 
            if(print_level::quiet) cout << " (Is signal) -- Skipping." << endl;
            continue;
        }

        if( (!total_CC1p1pi) && topology.GetType() == Topology::CC1P1PiPlus){
            cout << "Found Topology::CC1P1PiPlus -- Skipping" << endl;
            continue;
        }

        top2draw.push_back(i);

        //Check that other doesn't include signal not signal: -- this does matter! -- So do we need to remove it from others signal? NO? 

        if(print_level::quiet) cout << "FC = " << topology.GetFillColor() << " LC = " << topology.GetLineColor();
        if(print_level::quiet) cout << "FS = " << topology.GetFillStyle() << " LS = " << topology.GetLineStyle() << endl;
        
        std::string tmp_cuts = tmp_cuts_1;
        tmp_cuts += topology.GetSignal();
        
        DrawingTools::KinMap tmp_kinmap = KinArray(var.GetName(), nbins, bins, var.GetSymbol(), tmp_cuts);
        // cout << "Map Made for Topology = " << topology.GetName() << endl;

        SetColors(tmp_kinmap, topology.GetFillColor(), topology.GetLineColor(), topology.GetFillStyle(), topology.GetLineStyle());
        
        kinmap_list.push_back(tmp_kinmap);
    }
    
    THStack * recon_tot = new THStack( (var.GetSName() + "_TOP_recon").c_str(), Form(";%s (%s);%s", kinmap_list[0].recon->GetXaxis()->GetTitle(), var.GetUnits().c_str(),
                                                                                        kinmap_list[0].recon->GetYaxis()->GetTitle() ) );
    
    THStack * truth_tot = new THStack( (var.GetSName() + "_TOP_truth").c_str(), Form(";%s (%s);%s", kinmap_list[0].truth->GetXaxis()->GetTitle(), var.GetUnits().c_str(),
                                                                                        kinmap_list[0].truth->GetYaxis()->GetTitle() ) );
    
    THStack * ratio_tot = new THStack( (var.GetSName() + "_TOP_ratio").c_str(), Form(";%s (%s);%s", kinmap_list[0].ratio->GetXaxis()->GetTitle(), var.GetUnits().c_str(),
                                                                                        kinmap_list[0].ratio->GetYaxis()->GetTitle() ) );
    
    TH2D * smear_tot = (TH2D*)kinmap_list[0].smear->Clone( (var.GetSName() + "_TOP_smear").c_str() );//Just add all of these histos.

    TLegend * recon_leg = Legend(0.25, 0.4, 0.551, 0.362);
    TLegend * truth_leg = Legend(0.25, 0.4, 0.551, 0.362);
    TLegend * ratio_leg = Legend(0.25, 0.4, 0.551, 0.362);
    
    //The signal bit:
    // cout << "Signal Topology" << endl;

    std::string hsignal = tmp_cuts_1;// + " && ";
    Topology sig_top = m_topologies->GetTopology(m_signal);
    hsignal += sig_top.GetSignal();//    cout << "hsignal: " << hsignal << endl;
    cout << "               " << sig_top.GetSignal() << endl;

    DrawingTools::KinMap signal_kinmap = KinArray(var.GetName(), nbins, bins, var.GetSymbol(), hsignal);
    SetColors(signal_kinmap, sig_top.GetFillColor(), sig_top.GetLineColor(), sig_top.GetFillStyle(), sig_top.GetLineStyle());
    // cout << "Map Made for Signal Topology" << endl;

//    signal_kinmap.recon->SetLineColor(1);
//    signal_kinmap.truth->SetLineColor(1);
//    signal_kinmap.recon->SetLineStyle(2);
//    signal_kinmap.truth->SetLineStyle(2);
    signal_kinmap.recon->SetLineWidth(2);
    signal_kinmap.truth->SetLineWidth(2);
    signal_kinmap.ratio->SetLineWidth(2);
    
    recon_leg->AddEntry(signal_kinmap.recon, sig_top.GetSymbol().c_str(), "l");
    truth_leg->AddEntry(signal_kinmap.truth, sig_top.GetSymbol().c_str(), "l");
    ratio_leg->AddEntry(signal_kinmap.ratio, sig_top.GetSymbol().c_str(), "l");
    
    std::vector<double> recon_percent = GetPercentage(kinmap_list, 0);
    std::vector<double> truth_percent = GetPercentage(kinmap_list, 1);
    std::vector<double> ratio_percent = GetPercentage(kinmap_list, 2);

    if(m_fullbreakdown){
        
        for(int i = 1; i < (int)(kinmap_list.size() + 1); i++){

            recon_tot->Add(kinmap_list[ (int)(kinmap_list.size() - i) ].recon);
            recon_leg->AddEntry(kinmap_list[ (i - 1) ].recon, Form("%s (%.2f%%)", m_toplist[ top2draw[(i - 1)] ].GetSymbol().c_str(), recon_percent[ i - 1 ]), "f");
            
            truth_tot->Add(kinmap_list[ (int)(kinmap_list.size() - i) ].truth);
            truth_leg->AddEntry(kinmap_list[ (i - 1) ].truth, Form("%s (%.2f%%)", m_toplist[ top2draw[(i - 1)] ].GetSymbol().c_str(), truth_percent[ i - 1 ]), "f");
            
            ratio_tot->Add(kinmap_list[ (int)(kinmap_list.size() - i) ].ratio);
            ratio_leg->AddEntry(kinmap_list[ (i - 1) ].ratio, Form("%s (%.2f%%)", m_toplist[ top2draw[(i - 1)] ].GetSymbol().c_str(), ratio_percent[ i - 1 ]), "f");
            
            if(i < (int)kinmap_list.size()) smear_tot->Add(kinmap_list[ i ].smear);
        }
    }
    else{

        double recon_other_percent = 0.;
        double truth_other_percent = 0.;
        double ratio_other_percent = 0.;
        
        std::vector<int> plot_by_self;
        
        for(int i = 1; i < (int)(kinmap_list.size() + 1); i++){
    
            bool in_other = true;
            for(int j = 0; j < (int)m_pdglist_minBD.size(); j++){
                if(m_toplist_minBD[j] == m_toplist[ top2draw[(i - 1)] ].GetType()){
                    if((!total_CC1p1pi) && m_toplist[ top2draw[(i - 1)] ].GetType() == Topology::CC1P1PiPlus) continue;
                    in_other = false;
                    plot_by_self.push_back((i - 1));
                    break;
                }
            }
            
            if(in_other){
                recon_other_percent += recon_percent[ i - 1 ];
                truth_other_percent += truth_percent[ i - 1 ];
                ratio_other_percent += ratio_percent[ i - 1 ];
                
                if( (i - 1) != (int)(kinmap_list.size() - 1)){
                    kinmap_list.back().recon->Add(kinmap_list[ (i - 1) ].recon);
                    kinmap_list.back().truth->Add(kinmap_list[ (i - 1) ].truth);
                    kinmap_list.back().ratio->Add(kinmap_list[ (i - 1) ].ratio);
                }//                other_kinmap.smear->Add();
            }
            
            //This is common to both:
            if(i < (int)kinmap_list.size()) smear_tot->Add(kinmap_list[ i ].smear);
        }
        
        recon_tot->Add(kinmap_list.back().recon);
        truth_tot->Add(kinmap_list.back().truth);
        ratio_tot->Add(kinmap_list.back().ratio);
        smear_tot->Add(kinmap_list.back().smear);
        
        for(int i = 0; i < (int)plot_by_self.size(); i++){
            recon_tot->Add(kinmap_list[ plot_by_self[i] ].recon);
            recon_leg->AddEntry(kinmap_list[ plot_by_self[i] ].recon, Form("%s (%.2f%%)", m_toplist[ plot_by_self[i] ].GetSymbol().c_str(), recon_percent[ plot_by_self[i] ]), "f");
            
            truth_tot->Add(kinmap_list[ plot_by_self[i] ].truth);
            truth_leg->AddEntry(kinmap_list[ plot_by_self[i] ].truth, Form("%s (%.2f%%)", m_toplist[ plot_by_self[i] ].GetSymbol().c_str(), truth_percent[ plot_by_self[i] ]), "f");
            
            ratio_tot->Add(kinmap_list[ plot_by_self[i] ].ratio);
            ratio_leg->AddEntry(kinmap_list[ plot_by_self[i] ].ratio, Form("%s (%.2f%%)", m_toplist[ plot_by_self[i] ].GetSymbol().c_str(), ratio_percent[ plot_by_self[i] ]), "f");
        }
        
        recon_leg->AddEntry(kinmap_list.back().recon, Form("Other (%.2f%%)", recon_other_percent), "f");
        truth_leg->AddEntry(kinmap_list.back().truth, Form("Other (%.2f%%)", truth_other_percent), "f");
        ratio_leg->AddEntry(kinmap_list.back().ratio, Form("Other (%.2f%%)", ratio_other_percent), "f");
    }
    
    TLegend * ratio_stats = RatioStats(ratio_tot);

    BDCans cans;
    
    cans.recon = new TCanvas( recon_tot->GetName(), "", 500, 500);
    cans.recon->cd();
    recon_tot->Draw();
    signal_kinmap.recon->Draw("SAME");
    recon_leg->Draw();
    TLegend * recon_pot = GetPOT(0.1,0.1);
    recon_pot->Draw();
    //    mom_recon_pot->Draw();
    
    cans.truth = new TCanvas( truth_tot->GetName(), "", 500, 500);
    cans.truth->cd();
    truth_tot->Draw();
    signal_kinmap.truth->Draw("SAME");
    truth_leg->Draw();
    TLegend * truth_pot = GetPOT(0.1,0.1);
    truth_pot->Draw();
    
    cans.ratio = new TCanvas( ratio_tot->GetName(), "", 500, 500);
    cans.ratio->cd();
    ratio_tot->Draw();
    TLine * z_line = new TLine(0.0, ratio_tot->GetYaxis()->GetXmin(), 0.0, ratio_tot->GetYaxis()->GetXmax());
    z_line->Draw(); 
    ratio_leg->Draw();
    ratio_stats->Draw();
    signal_kinmap.ratio->Draw("SAME");
    TLegend * ratio_pot = GetPOT(0.1,0.1);
    ratio_pot->Draw();
    
    cans.smear = new TCanvas( smear_tot->GetName(), "", 500, 500);
    cans.smear->cd();
    smear_tot->Draw("COLZ");
    TLegend * smear_pot = GetPOT(0.1,0.1);
    smear_pot->Draw();
    
    TH2D * smear_totSN = NormalHist(smear_tot, 0., true);
    cans.smearSN = new TCanvas( (std::string(smear_tot->GetName()) + "SN").c_str(), "", 500, 500);
    cans.smearSN->cd();
    smear_totSN->Draw("COLZ");
    TLegend * smearSN_pot = GetPOT(0.1,0.1);
    smearSN_pot->Draw();
    
    return cans;
}

BDCans BreakdownTools::TOPO(Variable var, Int_t nbins, Double_t low, Double_t high, std::string cuts, bool check){
    return TOPO(var, nbins, SetBinning(nbins, low, high), cuts, check);
}

BDCans BreakdownTools::TARGET(Variable var, Int_t nbins, Double_t * bins, std::string cuts, bool check){

    std::string tmp_cuts_1 = cuts;
    if(!cuts.empty()){
        tmp_cuts_1 += " && ";
    }
    
    //Hydrogen:
    std::string hsig = tmp_cuts_1 + m_target + " == 1";
    DrawingTools::KinMap hydrogen_kinmap = KinArray(var.GetName(), nbins, bins, var.GetSymbol(), hsig);
    SetColors(hydrogen_kinmap, DrawingStyle::HYDROGEN);
    
    //Carbon:
    std::string csig = tmp_cuts_1 + m_target + " == 6";
    DrawingTools::KinMap carbon_kinmap = KinArray(var.GetName(), nbins, bins, var.GetSymbol(), csig);
    SetColors(carbon_kinmap, DrawingStyle::CARBON);

    //Other:
    std::string osig = tmp_cuts_1 + m_target + " != 1 && " + m_target + " != 6";
    DrawingTools::KinMap other_kinmap = KinArray(var.GetName(), nbins, bins, var.GetSymbol(), osig);
    SetColors(other_kinmap, DrawingStyle::Other);

    //The signal bit:
    // DrawingTools::KinMap GetSignalMap(Variable var, Int_t nbins, Double_t * bins, std::string cuts){
    std::string hsignal = tmp_cuts_1;
    Topology sig_top = m_topologies->GetTopology(m_signal);
    hsignal += sig_top.GetSignal();
    DrawingTools::KinMap signal_kinmap = KinArray(var.GetName(), nbins, bins, var.GetSymbol(), hsignal);
    SetColors(signal_kinmap, sig_top.GetFillColor(), sig_top.GetLineColor(), sig_top.GetFillStyle(), sig_top.GetLineStyle());
    signal_kinmap.recon->SetLineWidth(2);
    signal_kinmap.truth->SetLineWidth(2);
    signal_kinmap.ratio->SetLineWidth(2);
    // }

    std::vector<KinMap> kinmap_list;
    kinmap_list.push_back( other_kinmap );
    kinmap_list.push_back( carbon_kinmap );
    kinmap_list.push_back( hydrogen_kinmap );
    
    THStack * recon_tot = new THStack( (var.GetSName() + "_TAR_recon").c_str(), Form(";%s (%s);%s", kinmap_list[0].recon->GetXaxis()->GetTitle(), var.GetUnits().c_str(),
                                                                                   kinmap_list[0].recon->GetYaxis()->GetTitle() ) );
    
    THStack * truth_tot = new THStack( (var.GetSName() + "_TAR_truth").c_str(), Form(";%s (%s);%s", kinmap_list[0].truth->GetXaxis()->GetTitle(), var.GetUnits().c_str(),
                                                                                   kinmap_list[0].truth->GetYaxis()->GetTitle() ) );
    
    THStack * ratio_tot = new THStack( (var.GetSName() + "_TAR_ratio").c_str(), Form(";%s (%s);%s", kinmap_list[0].ratio->GetXaxis()->GetTitle(), var.GetUnits().c_str(),
                                                                                   kinmap_list[0].ratio->GetYaxis()->GetTitle() ) );
    
    TH2D * smear_tot = (TH2D*)kinmap_list[0].smear->Clone( (var.GetSName() + "_TAR_smear").c_str() );//Just add all of these histos.
    
    TLegend * recon_leg = Legend(0.25, 0.4, 0.551, 0.362);
    TLegend * truth_leg = Legend(0.25, 0.4, 0.551, 0.362);
    TLegend * ratio_leg = Legend(0.25, 0.4, 0.551, 0.362);
    
    std::vector<double> recon_percent = GetPercentage(kinmap_list, 0);
    std::vector<double> truth_percent = GetPercentage(kinmap_list, 1);
    std::vector<double> ratio_percent = GetPercentage(kinmap_list, 2);
    
    recon_leg->AddEntry(signal_kinmap.recon, sig_top.GetSymbol().c_str(), "l");
    truth_leg->AddEntry(signal_kinmap.truth, sig_top.GetSymbol().c_str(), "l");
    ratio_leg->AddEntry(signal_kinmap.ratio, sig_top.GetSymbol().c_str(), "l");
    
    recon_leg->AddEntry(kinmap_list[2].recon, Form("Hydrogen (%.2f%%)", recon_percent[2]), "f");
    truth_leg->AddEntry(kinmap_list[2].truth, Form("Hydrogen (%.2f%%)", truth_percent[2]), "f");
    ratio_leg->AddEntry(kinmap_list[2].ratio, Form("Hydrogen (%.2f%%)", recon_percent[2]), "f");

    recon_leg->AddEntry(kinmap_list[1].recon, Form("Carbon (%.2f%%)", recon_percent[1]), "f");
    truth_leg->AddEntry(kinmap_list[1].truth, Form("Carbon (%.2f%%)", truth_percent[1]), "f");
    ratio_leg->AddEntry(kinmap_list[1].ratio, Form("Carbon (%.2f%%)", ratio_percent[1]), "f");

    recon_leg->AddEntry(kinmap_list[0].recon, Form("Other (%.2f%%)", recon_percent[0]), "f");
    truth_leg->AddEntry(kinmap_list[0].truth, Form("Other (%.2f%%)", truth_percent[0]), "f");
    ratio_leg->AddEntry(kinmap_list[0].ratio, Form("Other (%.2f%%)", ratio_percent[0]), "f");
    
    for(int i = 0; i < 3; i++){
        
        recon_tot->Add( kinmap_list[i].recon);
        truth_tot->Add( kinmap_list[i].truth);
        ratio_tot->Add( kinmap_list[i].ratio);
        
        if(i > 0) smear_tot->Add(kinmap_list[i].smear);
    }
    
    TLegend * ratio_stats = RatioStats(ratio_tot);

    BDCans cans;
    
    cans.recon = new TCanvas( recon_tot->GetName(), "", 500, 500);
    cans.recon->cd();
    recon_tot->Draw();
    signal_kinmap.recon->Draw("SAME");
    recon_leg->Draw();
    TLegend * recon_pot = GetPOT(0.1,0.1);
    recon_pot->Draw();
    //    mom_recon_pot->Draw();
    
    cans.truth = new TCanvas( truth_tot->GetName(), "", 500, 500);
    cans.truth->cd();
    truth_tot->Draw();
    signal_kinmap.truth->Draw("SAME");
    truth_leg->Draw();
    TLegend * truth_pot = GetPOT(0.1,0.1);
    truth_pot->Draw();
    
    cans.ratio = new TCanvas( ratio_tot->GetName(), "", 500, 500);
    cans.ratio->cd();
    ratio_tot->Draw();
    DrawZeroPointLine(ratio_tot);
    // TLine * z_line = new TLine(0.0, ratio_tot->GetYaxis()->GetXmin(), 0.0, ratio_tot->GetYaxis()->GetXmax());
    // // TLine * z_line = new TLine(0.0, ratio_tot->GetYaxis()->GetXmin(), 0.0, ratio_tot->GetYaxis()->GetXmax());
    // z_line->SetLineColor(1);
    // z_line->SetLineStyle(2);
    // z_line->Draw(); 
    ratio_leg->Draw();
    ratio_stats->Draw();
    signal_kinmap.ratio->Draw("SAME");
    TLegend * ratio_pot = GetPOT(0.1,0.1);
    ratio_pot->Draw();
    
    cans.smear = new TCanvas( smear_tot->GetName(), "", 500, 500);
    cans.smear->cd();
    smear_tot->Draw("COLZ");
    TLegend * smear_pot = GetPOT(0.1,0.1);
    smear_pot->Draw();
    
    TH2D * smear_totSN = NormalHist(smear_tot, 0., true);
    cans.smearSN = new TCanvas( (std::string(smear_tot->GetName()) + "SN").c_str(), "", 500, 500);
    cans.smearSN->cd();
    smear_totSN->Draw("COLZ");
    TLegend * smearSN_pot = GetPOT(0.1,0.1);
    smearSN_pot->Draw();

    return cans;
}

BDCans BreakdownTools::TARGET(Variable var, Int_t nbins, Double_t low, Double_t high, std::string cuts, bool check){
    return TARGET(var, nbins, SetBinning(nbins, low, high), cuts, check);
}

TCanvas * BreakdownTools::TARGETSingle(Variable var, Int_t nbins, Double_t * bins, std::string cuts){
    
    std::string tmp_cuts_1 = cuts;
    if(!cuts.empty()){
        tmp_cuts_1 += " && ";
    }
    
    //Hydrogen:
    std::string hsig = tmp_cuts_1 + m_target + " == 1";
    TH1D * hydrogen_kinmap = GetHisto(var.GetName(), nbins, bins, var.GetSymbol(), hsig);
    SetColors(hydrogen_kinmap, DrawingStyle::HYDROGEN);
    
    //Carbon:
    std::string csig = tmp_cuts_1 + m_target + " == 6";
    TH1D * carbon_kinmap = GetHisto(var.GetName(), nbins, bins, var.GetSymbol(), csig);
    SetColors(carbon_kinmap, DrawingStyle::CARBON);
    
    //Other:
    std::string osig = tmp_cuts_1 + m_target + " != 1 && " + m_target + " != 6";
    TH1D * other_kinmap = GetHisto(var.GetName(), nbins, bins, var.GetSymbol(), osig);
    SetColors(other_kinmap, DrawingStyle::Other);
    
    //The GetSignal() bit:
    std::string hsignal = tmp_cuts_1;
    Topology sig_top = m_topologies->GetTopology(m_signal);
    hsignal += sig_top.GetSignal();
    TH1D * signal_kinmap = GetHisto(var.GetName(), nbins, bins, var.GetSymbol(), hsignal);
    SetColors(signal_kinmap, sig_top.GetFillColor(), sig_top.GetLineColor(), sig_top.GetFillStyle(), sig_top.GetLineStyle());
    signal_kinmap->SetLineWidth(2);
    
    std::vector<TH1D*> kinmap_list;
    kinmap_list.push_back( other_kinmap );
    kinmap_list.push_back( carbon_kinmap );
    kinmap_list.push_back( hydrogen_kinmap );
    
    THStack * recon_tot = new THStack( (var.GetSName() + "_TAR").c_str(), Form(";%s (%s);%s", kinmap_list[0]->GetXaxis()->GetTitle(), var.GetUnits().c_str(),
                                                                                   kinmap_list[0]->GetYaxis()->GetTitle() ) );
    
    TLegend * recon_leg = Legend(0.25, 0.4, 0.551, 0.362);
    
    std::vector<double> recon_percent = GetPercentage(kinmap_list);
    
    recon_leg->AddEntry(signal_kinmap, sig_top.GetSymbol().c_str(), "l");
    
    recon_leg->AddEntry(kinmap_list[2], Form("Hydrogen (%.2f%%)", recon_percent[2]), "f");
    
    recon_leg->AddEntry(kinmap_list[1], Form("Carbon (%.2f%%)", recon_percent[1]), "f");
    
    recon_leg->AddEntry(kinmap_list[0], Form("Other (%.2f%%)", recon_percent[0]), "f");
    
    for(int i = 0; i < 3; i++) recon_tot->Add( kinmap_list[i]);
    
    TCanvas * cans = new TCanvas( recon_tot->GetName(), "", 500, 500);
    cans->cd();
    recon_tot->Draw();
    signal_kinmap->Draw("SAME");
    recon_leg->Draw();
    TLegend * recon_pot = GetPOT(0.1,0.1);
    recon_pot->Draw();
    
    return cans;
}

TCanvas * BreakdownTools::TARGETSingle(Variable var, Int_t nbins, Double_t low, Double_t high, std::string cuts){
    return TARGETSingle(var, nbins, SetBinning(nbins, low, high), cuts);
}

TLegend * BreakdownTools::RatioStats(const THStack * ratio_tot)
{
//Get the ratio histograms, make them into one histogram and determine the rms and mean:
    TList * rlist = ratio_tot->GetHists();
    TH1D * hfirst = (TH1D*)rlist->First()->Clone( (string(rlist->First()->GetName()) + "_clone").c_str() );
    // cout << "FOund hfirst" << endl;
    Int_t ratio_nbins = hfirst->GetNbinsX();
    Double_t ratio_low = hfirst->GetXaxis()->GetXmin();
    Double_t ratio_high = hfirst->GetXaxis()->GetXmax();
    // cout << "Range : Bins = " << ratio_nbins << " low = " << ratio_low << " high = " << ratio_high << endl;
    
    TIter next(rlist);
    m_statcounter++;
    TH1D ratio_sum( Form("%s_ratio_sum%.3d", hfirst->GetName(), m_statcounter), "", ratio_nbins, ratio_low, ratio_high);
    TH1D * rhist_tmp;
    while ( (rhist_tmp = (TH1D*)next()) ) {
        ratio_sum.Add(rhist_tmp);
    }
    
    TLegend * ratio_stats = Legend(0.25, 0.8, 0.05, 0.1);
    ratio_stats->AddEntry((TObject*)0, Form("Mean = %.3f", (double)ratio_sum.GetMean()), "");
    ratio_stats->AddEntry((TObject*)0, Form(" RMS = %.3f", (double)ratio_sum.GetRMS() ), "");

    TF1 * cauchy = new TF1("cauchy","([2]*[1])/(TMath::Pi()*([1]*[1] + (x-[0])*(x-[0]) ) )", ratio_low, ratio_high);
    delete hfirst;

    cauchy->SetParameter(0, (double)ratio_sum.GetXaxis()->GetBinCenter(ratio_sum.GetMaximumBin() ) );
    cauchy->SetParameter(1, (double)ratio_sum.GetRMS()  );
    cauchy->SetParameter(2, (double)ratio_sum.Integral());
    TFitResultPtr r = ratio_sum.Fit(cauchy,"RLNQ");
    Int_t fitStatus = r;
    // cout << "(double)cauchy->GetParameter(0) = " << (double)cauchy->GetParameter(0) << endl;
    // cout << "(double)cauchy->GetParameter(1) = " << (double)cauchy->GetParameter(1) << endl;

    if(fitStatus == 0){//Fit status successful add the parameters:
        ratio_stats->AddEntry((TObject*)0, Form("Cauchy Mean = %.3f", (double)cauchy->GetParameter(0)), "");
        ratio_stats->AddEntry((TObject*)0, Form("Cauchy #sigma = %.3f", (double)cauchy->GetParameter(1)), "");
    }

    return ratio_stats;
}

TCanvas * BreakdownTools::PID(Variable var, std::string pdgvar, std::string cuts, std::vector<PDGInfo> pdglist)
{

    std::vector<TH1D*> kinmap_list;
    std::vector<std::string> name_list;

    std::string other_cut = cuts;
    
    std::string tmp_cuts_1 = cuts;
    if(!cuts.empty()){
        tmp_cuts_1 += " && ";
        other_cut += " && (";
    }
    
    other_cut += pdgvar;
    other_cut += " != -999";
    
    for(int i = 0; i < (int)pdglist.size(); i++){

        PDGInfo particle = pdglist[i];
        
        std::string tmp_cuts = tmp_cuts_1;
        if(particle.IsBoth()) tmp_cuts += "TMath::Abs(";
        tmp_cuts += pdgvar;
        if(particle.IsBoth()) tmp_cuts += ")";
        tmp_cuts += " == ";
        tmp_cuts += particle.GetPDGStr();

        if(!other_cut.empty()) other_cut += " && ";
        
        if(particle.IsBoth()) other_cut += "TMath::Abs(";
        other_cut += pdgvar;
        if(particle.IsBoth()) other_cut += ")";
        other_cut += " != ";
        other_cut += particle.GetPDGStr();

        TH1D * tmp_kinmap = GetHisto(var.GetName(), var.GetNBins(), var.GetBinning(), var.GetAxisTitle(), tmp_cuts);
        
        SetColors(tmp_kinmap, particle.GetColor());
        
        kinmap_list.push_back(tmp_kinmap);
        name_list.push_back(particle.GetSymbol());
    }
    
    if(!cuts.empty()) other_cut += ")";

    TH1D * other_kinmap = GetHisto(var.GetName(), var.GetNBins(), var.GetBinning(), var.GetSymbol(), other_cut);
    SetColors(other_kinmap, DrawingStyle::Other);

    kinmap_list.push_back(other_kinmap);
    name_list.push_back("Other");

    //Make outputs:
    
    THStack * recon_tot = new THStack( (var.GetSName() + "_PID").c_str(), Form(";%s (%s);%s", var.GetAxisTitle().c_str(), var.GetUnits().c_str(),
        kinmap_list[0]->GetYaxis()->GetTitle() ) );
    
    TLegend * recon_leg = Legend(0.25, 0.4, 0.551, 0.362);
    
    std::vector<double> recon_percent = GetPercentage(kinmap_list);
    
    int list_size = (int)kinmap_list.size();

    for(int forwards = 0; forwards < list_size; forwards++){
        int backwards = list_size - 1 - forwards;
        //Add other first:
        recon_tot->Add( kinmap_list[backwards] );
        recon_leg->AddEntry( kinmap_list[forwards], name_list[forwards].c_str(), "f");
    }

    TCanvas * cans = new TCanvas( recon_tot->GetName(), "", 500, 500);
    cans->cd();
    recon_tot->Draw();
    recon_leg->Draw();
    GetPOT(0.1,0.1)->Draw();

    return cans;
}


// void BreakdownTools::DrawZeroPointLine(TH1 * histo){

//     TLine * z_line = new TLine(0.0, histo->GetMinimum(), 0.0, histo->GetMaximum());
//     z_line->SetLineColor(1);
//     z_line->SetLineStyle(2);
//     z_line->Draw(); 
// }

void BreakdownTools::DrawZeroPointLine(THStack * histo){

    TList * rlist = histo->GetHists();
    TH1D * hfirst = (TH1D*)rlist->First()->Clone( (string(rlist->First()->GetName()) + "_clone").c_str() );
    DrawZeroPointLine(hfirst);
}
           
BDCans BreakdownTools::BaseBreakdown(Variable var, Int_t nbins, Double_t * bins, std::vector<Breakdown> list, 
    std::string basename, std::string cuts, bool check)
{
    //Make outputs:
    if(list.empty()){ 
        BDCans bad;
        cout << __FILE__ << ":" << __LINE__ << " : List is empty, not building breakdown plot for " << basename << endl;
        return bad;
    }

    for(unsigned int i = 0; i < list.size(); i++){
        Breakdown tmp = list[i];
        tmp.SetMap( KinArray(var.GetName(), nbins, bins, var.GetSymbol(), tmp.GetSignal()) );
    }

    string units = "";
    if(!var.GetUnits().empty()){
        units += "(";
        units += var.GetUnits();
        units += ")";
    }

    THStack * recon_tot = new THStack( (var.GetSName() + "_" + basename + "_recon").c_str(), (var.GetSymbol() + units + ";Count").c_str());
    
    THStack * truth_tot = new THStack( (var.GetSName() + "_" + basename + "_truth").c_str(), (var.GetSymbol() + units + ";Count").c_str());
    
    THStack * ratio_tot = new THStack( (var.GetSName() + "_" + basename + "_ratio").c_str(), 
        Form(";%s;%s", list[0].GetMap().ratio->GetXaxis()->GetTitle(), list[0].GetMap().ratio->GetYaxis()->GetTitle() ) );
    
    TH2D * smear_tot = (TH2D*)list[0].GetMap().smear->Clone( (var.GetSName() + "_PID_smear").c_str() );//Just add all of these histos.
    
    TLegend * recon_leg = Legend(0.25, 0.4, 0.551, 0.362);
    TLegend * truth_leg = Legend(0.25, 0.4, 0.551, 0.362);
    TLegend * ratio_leg = Legend(0.25, 0.4, 0.551, 0.362);
    
    std::vector<double> recon_percent = GetPercentages(list, 0);
    std::vector<double> truth_percent = GetPercentages(list, 1);
    std::vector<double> ratio_percent = GetPercentages(list, 2);
    
    DrawingTools::KinMap signal_kinmap = GetSignalMap(var, nbins, bins, cuts);

    DrawingTools::KinMap tmp_check;
    if(check){
        tmp_check = KinArray(var.GetName(), nbins, bins, var.GetSymbol(), cuts);
        SetColors(tmp_check, 0, kPink-9, 0, 4);
        // TODO: Check first 0 - Fill color. 
    }

    BDCans cans;

    for(int type = 0; type < 3; type++){
        THStack * tmp_stack;
        TLegend * tmp_leg;
        TH1D * tmp_sig;
        std::vector<double> percent;
        switch(type){
            case 0: 
                cans.recon = new TCanvas( recon_tot->GetName(), "", 500, 500); 
                cans.recon->cd();
                tmp_sig = signal_kinmap.recon;
                tmp_stack = recon_tot; 
                tmp_leg = recon_leg; 
                percent = recon_percent;
                break;
            case 1: 
                cans.truth = new TCanvas( truth_tot->GetName(), "", 500, 500); 
                cans.truth->cd();
                tmp_sig = signal_kinmap.truth;
                tmp_stack = truth_tot; 
                tmp_leg = truth_leg; 
                percent = truth_percent;
                break;
            case 2: 
                cans.ratio = new TCanvas( ratio_tot->GetName(), "", 500, 500); 
                cans.ratio->cd();
                tmp_sig = signal_kinmap.ratio;
                tmp_stack = ratio_tot; 
                tmp_leg = ratio_leg; 
                percent = ratio_percent;
                break;
        }

        for(unsigned int i = 1; i < list.size() + 1; i++){
            Breakdown tmp = list[ list.size() - i ];
            TH1D * tmp_hist;
            switch(type){
                case 0: tmp_hist = tmp.GetMap().recon; break;
                case 1: tmp_hist = tmp.GetMap().truth; break;
                case 2: tmp_hist = tmp.GetMap().ratio; break;
            }
            tmp_stack->Add( tmp_hist );
        }

        tmp_leg->AddEntry(tmp_sig, m_sig_top.GetSymbol().c_str(), "l");

        for(unsigned int i = 0; i < list.size(); i++){
            Breakdown tmp = list[ i ];
            TH1D * tmp_hist;
            switch(type){
                case 0: tmp_hist = tmp.GetMap().recon; break;
                case 1: tmp_hist = tmp.GetMap().truth; break;
                case 2: tmp_hist = tmp.GetMap().ratio; break;
            }
            tmp_leg->AddEntry( tmp_hist, (tmp.GetName() + string(Form(" (%.2f)", recon_percent[ i ] )) ).c_str(), tmp.GetLegOpt() );
        }

        tmp_stack->Draw();
        tmp_sig->Draw("SAME");
        tmp_leg->Draw();
        GetPOT(0.1,0.1)->Draw();
        percent.clear();

        if(check){
            TH1D * check_hist;
            switch(type){
                case 0: check_hist = tmp_check.recon; break;
                case 1: check_hist = tmp_check.truth; break;
                case 2: check_hist = tmp_check.ratio; break;
            }
            check_hist->Draw("SAME");
            cout << "Checking if integral is the same:";
            double difference = GetHistFromStack(tmp_stack)->Integral() - check_hist->Integral();
            if(difference == 0.) cout << " Integrals consistent between stack and hist.";
            else cout << " Inconsistent integrals between stack and hist (Diff. = " << difference << ")";
            cout << endl;
        }
    }

    for(unsigned int i = 1; i < list.size(); i++) smear_tot->Add(list[i].GetMap().smear);

    cans.smear = new TCanvas( smear_tot->GetName(), "", 500, 500);
    cans.smear->cd();
    smear_tot->Draw("COLZ");
    GetPOT(0.1,0.1)->Draw();
    
    TH2D * smear_totSN = NormalHist(smear_tot, 0., true);
    cans.smearSN = new TCanvas( (std::string(smear_tot->GetName()) + "SN").c_str(), "", 500, 500);
    cans.smearSN->cd();
    smear_totSN->Draw("COLZ");
    GetPOT(0.1,0.1)->Draw();
    
    return cans;
}

DrawingTools::KinMap GetSignalMap(Variable var, Int_t nbins, Double_t * bins, std::string cuts){
    std::string hsignal = cuts;
    if(!hsignal.empty()){
        hsignal += " && ";
    }
    Topology sig_top = m_topologies->GetTopology(m_signal);
    hsignal += sig_top.GetSignal();
    DrawingTools::KinMap signal_kinmap = KinArray(var.GetName(), nbins, bins, var.GetSymbol(), hsignal);
    SetColors(signal_kinmap, sig_top.GetFillColor(), sig_top.GetLineColor(), sig_top.GetFillStyle(), sig_top.GetLineStyle());
    signal_kinmap.recon->SetLineWidth(2);
    signal_kinmap.truth->SetLineWidth(2);
    signal_kinmap.ratio->SetLineWidth(2);
    return signal_kinmap;
}

std::vector<double> BreakdownTools::GetPercentages(std::vector<Breakdown> list, int type)
{
    std::vector<TH1D*> histos;
    for(unsigned int i = 0; i < list.size(); i++){
        switch(type) {
            case 0 : histos.push_back( list[i].GetMap().recon ); break;
            case 1 : histos.push_back( list[i].GetMap().truth ); break;
            case 2 : histos.push_back( list[i].GetMap().ratio ); break;
            default: cout << __FILE__ << ":" << __LINE__ << ": Warning could not determine type" << endl; break;
        }
    }
    return DrawingTools::GetPercentage(histos);
}

// BreakdownTools::SetPerentages(std::vector<Breakdown> list, int type)
// {
//     std::vector<double> pers = DrawingTools::GetPercentage(histos, type);

//     for(unsigned int i = 0; i < list.size(); i++){
//         switch(type) {
//             case 0 : list[i].GetMap().recon ); break;
//             case 1 : list[i].GetMap().truth ); break;
//             case 2 : list[i].GetMap().ratio ); break;
//             default: cout << __FILE__ << ":" << __LINE__ << ": Warning could not determine type" << endl; break;
//         }
//     }
// }












