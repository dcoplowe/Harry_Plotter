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

BreakdownTools::BreakdownTools(std::string filename, std::string treename, Topologies * topologies, std::string target_name) : DrawingTools(filename, treename, ("BD" + treename).c_str()),
m_printPOT(false), m_fullbreakdown(true) {

//    PDGInfo proton;
    m_pdglist.push_back( PDGInfo(2212, "proton",   "p"));
    m_pdglist.push_back( PDGInfo(211,  "piplus",   "#pi^{+}") );
    m_pdglist.push_back( PDGInfo(-211, "piminus",  "#pi^{-}") );
    m_pdglist.push_back( PDGInfo(2112, "neutron",  "n") );
    m_pdglist.push_back( PDGInfo(13,   "muon",     "#mu^{-}") );
    m_pdglist.push_back( PDGInfo(-13,  "amuon",    "#mu^{+}") );
    m_pdglist.push_back( PDGInfo(111,  "pizero",   "#pi^{0}") );
    m_pdglist.push_back( PDGInfo(321,  "kapm",     "K^{#pm}") );
    m_pdglist.push_back( PDGInfo(311,  "kazero",   "K^{0}") );
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

BDCans BreakdownTools::PID(Variable var, Int_t nbins, Double_t * bins, std::string pdgvar, std::string cuts){

    std::vector<DrawingTools::KinMap> kinmap_list;
    
    std::string other_cut = cuts;
    
    std::string tmp_cuts_1 = cuts;
    if(!cuts.empty()){
        tmp_cuts_1 += " && ";
        other_cut += " && (";
    }
    
    other_cut += pdgvar;
    other_cut += " != -999";
    
    for(int i = 0; i < (int)m_pdglist.size(); i++){
    
        PDGInfo particle = m_pdglist[i];
        
        std::string tmp_cuts = tmp_cuts_1;
        tmp_cuts += pdgvar;
        tmp_cuts += " == ";
        tmp_cuts += particle.GetPDGStr();
                
        if(other_cut.empty()){
            other_cut = pdgvar;
            other_cut += " != ";
            other_cut += particle.GetPDGStr();
        }
        else{
            other_cut += " && ";
            other_cut += pdgvar;
            other_cut += " != ";
            other_cut += particle.GetPDGStr();
        }
            
        DrawingTools::KinMap tmp_kinmap = KinArray(var.name, nbins, bins, var.symbol, tmp_cuts);
        
        SetColors(tmp_kinmap, particle.GetColor());
        
        kinmap_list.push_back(tmp_kinmap);
    }
    
    if(!cuts.empty()) other_cut += ")";
    
//    cout << "other_cut: " << other_cut << endl;
    
    DrawingTools::KinMap other_kinmap = KinArray(var.name, nbins, bins, var.symbol, other_cut);
    SetColors(other_kinmap, DrawingStyle::Other);

    //Make outputs:
    
    THStack * recon_tot = new THStack( (var.savename + "_PID_recon").c_str(), Form(";%s (%s);%s", kinmap_list[0].recon->GetXaxis()->GetTitle(), var.units.c_str(),
                                                                                kinmap_list[0].recon->GetYaxis()->GetTitle() ) );
    
    THStack * truth_tot = new THStack( (var.savename + "_PID_truth").c_str(), Form(";%s (%s);%s", kinmap_list[0].truth->GetXaxis()->GetTitle(), var.units.c_str(),
                                                                               kinmap_list[0].truth->GetYaxis()->GetTitle() ) );
    
    THStack * ratio_tot = new THStack( (var.savename + "_PID_ratio").c_str(), Form(";%s (%s);%s", kinmap_list[0].ratio->GetXaxis()->GetTitle(), var.units.c_str(),
                                                                               kinmap_list[0].ratio->GetYaxis()->GetTitle() ) );
    
    TH2D * smear_tot = (TH2D*)kinmap_list[0].smear->Clone( (var.savename + "_PID_smear").c_str() );//Just add all of these histos.
    
    TLegend * recon_leg = Legend(0.25, 0.4, 0.551, 0.362);
    TLegend * truth_leg = Legend(0.25, 0.4, 0.551, 0.362);
    TLegend * ratio_leg = Legend(0.25, 0.4, 0.551, 0.362);
    
    std::vector<double> recon_percent = GetPercentage(kinmap_list, 0, other_kinmap);
    std::vector<double> truth_percent = GetPercentage(kinmap_list, 1, other_kinmap);
    std::vector<double> ratio_percent = GetPercentage(kinmap_list, 2, other_kinmap);
    
    if(m_fullbreakdown){
        
//        std::vector<double> recon_percent = GetPercentage(kinmap_list, 0, other_kinmap);
//        std::vector<double> truth_percent = GetPercentage(kinmap_list, 1, other_kinmap);
//        std::vector<double> ratio_percent = GetPercentage(kinmap_list, 2, other_kinmap);
//        
        recon_tot->Add(other_kinmap.recon);
        truth_tot->Add(other_kinmap.truth);
        ratio_tot->Add(other_kinmap.ratio);
        smear_tot->Add(other_kinmap.smear);
        
        for(int i = 1; i < (int)(kinmap_list.size() + 1); i++){
//            cout << i << ":" << (int)kinmap_list.size() << " : " << (int)(kinmap_list.size() - i) << endl;
            
            recon_tot->Add(kinmap_list[ (int)(kinmap_list.size() - i) ].recon);
            recon_leg->AddEntry(kinmap_list[ (i - 1) ].recon, Form("%s (%.2f%%)", m_pdglist[(i - 1)].GetSymbol().c_str(), recon_percent[ i - 1 ]), "f");
            
            truth_tot->Add(kinmap_list[ (int)(kinmap_list.size() - i) ].truth);
            truth_leg->AddEntry(kinmap_list[ (i - 1) ].truth, Form("%s (%.2f%%)", m_pdglist[(i - 1)].GetSymbol().c_str(), truth_percent[ i - 1 ]), "f");
            
            ratio_tot->Add(kinmap_list[ (int)(kinmap_list.size() - i) ].ratio);
            ratio_leg->AddEntry(kinmap_list[ (i - 1) ].ratio, Form("%s (%.2f%%)", m_pdglist[(i - 1)].GetSymbol().c_str(), ratio_percent[ i - 1 ]), "f");
            
            if(i < (int)kinmap_list.size()) smear_tot->Add(kinmap_list[ i ].smear);
        }
               
        recon_leg->AddEntry(other_kinmap.recon, Form("Other (%.2f%%)", recon_percent.back()), "f");
        truth_leg->AddEntry(other_kinmap.truth, Form("Other (%.2f%%)", truth_percent.back()), "f");
        ratio_leg->AddEntry(other_kinmap.ratio, Form("Other (%.2f%%)", ratio_percent.back()), "f");
    }
    else{
        double recon_other_percent = 0.;
        double truth_other_percent = 0.;
        double ratio_other_percent = 0.;
        
        std::vector<int> plot_by_self;

        for(int i = 1; i < (int)(kinmap_list.size() + 1); i++){
//            cout << i << ":" << (int)kinmap_list.size() << " : " << (int)(kinmap_list.size() - i) << endl;
            
            bool in_other = true;
            for(int j = 0; j < (int)m_pdglist_minBD.size(); j++){
                if(m_pdglist_minBD[j] == m_pdglist[(i - 1)].GetPDGNo()){
                    in_other = false;
                    plot_by_self.push_back((i - 1));
                    break;
                }
            }
            
            if(in_other){
                recon_other_percent += recon_percent[ i - 1 ];
                truth_other_percent += truth_percent[ i - 1 ];
                ratio_other_percent += ratio_percent[ i - 1 ];
                
                other_kinmap.recon->Add(kinmap_list[ (i - 1) ].recon);
                other_kinmap.truth->Add(kinmap_list[ (i - 1) ].truth);
                other_kinmap.ratio->Add(kinmap_list[ (i - 1) ].ratio);
//                other_kinmap.smear->Add();
            }
            
            //This is common to both:
            if(i < (int)kinmap_list.size()) smear_tot->Add(kinmap_list[ i ].smear);
        }
        
        recon_tot->Add(other_kinmap.recon);
        truth_tot->Add(other_kinmap.truth);
        ratio_tot->Add(other_kinmap.ratio);
        smear_tot->Add(other_kinmap.smear);
        
        for(int i = 0; i < (int)plot_by_self.size(); i++){
                recon_tot->Add(kinmap_list[ plot_by_self[i] ].recon);
                recon_leg->AddEntry(kinmap_list[ plot_by_self[i] ].recon, Form("%s (%.2f%%)", m_pdglist[ plot_by_self[i] ].GetSymbol().c_str(), recon_percent[ plot_by_self[i] ]), "f");
                
                truth_tot->Add(kinmap_list[ plot_by_self[i] ].truth);
                truth_leg->AddEntry(kinmap_list[ plot_by_self[i] ].truth, Form("%s (%.2f%%)", m_pdglist[ plot_by_self[i] ].GetSymbol().c_str(), truth_percent[ plot_by_self[i] ]), "f");
                
                ratio_tot->Add(kinmap_list[ plot_by_self[i] ].ratio);
                ratio_leg->AddEntry(kinmap_list[ plot_by_self[i] ].ratio, Form("%s (%.2f%%)", m_pdglist[ plot_by_self[i] ].GetSymbol().c_str(), ratio_percent[ plot_by_self[i] ]), "f");
        }
        
        recon_leg->AddEntry(other_kinmap.recon, Form("Other (%.2f%%)", recon_other_percent), "f");
        truth_leg->AddEntry(other_kinmap.truth, Form("Other (%.2f%%)", truth_other_percent), "f");
        ratio_leg->AddEntry(other_kinmap.ratio, Form("Other (%.2f%%)", ratio_other_percent), "f");
    }
    
    TLegend * ratio_stats = RatioStats(ratio_tot);
    // cout << "TLegend * ratio_stats = RatioStats(ratio_tot)" << endl;


    BDCans cans;
    
    cans.recon = new TCanvas( recon_tot->GetName(), "", 500, 500);
    cans.recon->cd();
    recon_tot->Draw();
    recon_leg->Draw();
    TLegend * recon_pot = GetPOT(0.1,0.1);
    recon_pot->Draw();
//    mom_recon_pot->Draw();
    
    cans.truth = new TCanvas( truth_tot->GetName(), "", 500, 500);
    cans.truth->cd();
    truth_tot->Draw();
    truth_leg->Draw();
    TLegend * truth_pot = GetPOT(0.1,0.1);
    truth_pot->Draw();
    
    cans.ratio = new TCanvas( ratio_tot->GetName(), "", 500, 500);
    cans.ratio->cd();
    ratio_tot->Draw();
    TLine * z_line = new TLine(0.0, ratio_tot->GetYaxis()->GetXmin(), 0.0, ratio_tot->GetYaxis()->GetXmax());
    z_line->Draw(); 
    ratio_leg->Draw();
    // cout << "Drawing ratio stats" << endl;
    ratio_stats->Draw();
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

BDCans BreakdownTools::PID(Variable var, Int_t nbins, Double_t low, Double_t high, std::string pdgvar, std::string cuts){
    return PID(var, nbins, SetBinning(nbins, low, high), pdgvar, cuts);
}

BDCans BreakdownTools::TOPO(Variable var, Int_t nbins, Double_t * bins, std::string cuts){
    
    std::vector<DrawingTools::KinMap> kinmap_list;
    std::vector<Int_t> top2draw;
    
    std::string tmp_cuts_1 = cuts;
    if(!cuts.empty()) tmp_cuts_1 += " && ";
    
    for(int i = 0; i < (int)m_toplist.size(); i++){
        
//        cout << i << ":" << (int)m_toplist.size() << endl;
        
        Topology topology = m_toplist[i];

        // if(print_level::quiet) 
        cout << "Topology = " << topology.GetName() << endl;

        // if(topology.GetType() == Topology::Other){
        //     cout << "Signal = " << topology.GetSignal() << endl;
        // }

        if(topology.GetType() == m_signal){ 
            if(print_level::quiet)cout << " (Is signal) -- Skipping." << endl;
            continue;
        }

        top2draw.push_back(i);

        //Check that other doesn't include signal not signal: -- this does matter! -- So do we need to remove it from others signal? NO? 

        if(print_level::quiet) cout << "FC = " << topology.GetFillColor() << " LC = " << topology.GetLineColor();
        if(print_level::quiet) cout << "FS = " << topology.GetFillStyle() << " LS = " << topology.GetLineStyle() << endl;
        
        std::string tmp_cuts = tmp_cuts_1;
        tmp_cuts += topology.GetSignal();
        
        DrawingTools::KinMap tmp_kinmap = KinArray(var.name, nbins, bins, var.symbol, tmp_cuts);
        
        SetColors(tmp_kinmap, topology.GetFillColor(), topology.GetLineColor(), topology.GetFillStyle(), topology.GetLineStyle());
        
        kinmap_list.push_back(tmp_kinmap);
    }
    
    THStack * recon_tot = new THStack( (var.savename + "_TOP_recon").c_str(), Form(";%s (%s);%s", kinmap_list[0].recon->GetXaxis()->GetTitle(), var.units.c_str(),
                                                                                        kinmap_list[0].recon->GetYaxis()->GetTitle() ) );
    
    THStack * truth_tot = new THStack( (var.savename + "_TOP_truth").c_str(), Form(";%s (%s);%s", kinmap_list[0].truth->GetXaxis()->GetTitle(), var.units.c_str(),
                                                                                        kinmap_list[0].truth->GetYaxis()->GetTitle() ) );
    
    THStack * ratio_tot = new THStack( (var.savename + "_TOP_ratio").c_str(), Form(";%s (%s);%s", kinmap_list[0].ratio->GetXaxis()->GetTitle(), var.units.c_str(),
                                                                                        kinmap_list[0].ratio->GetYaxis()->GetTitle() ) );
    
    TH2D * smear_tot = (TH2D*)kinmap_list[0].smear->Clone( (var.savename + "_TOP_smear").c_str() );//Just add all of these histos.

    TLegend * recon_leg = Legend(0.25, 0.4, 0.551, 0.362);
    TLegend * truth_leg = Legend(0.25, 0.4, 0.551, 0.362);
    TLegend * ratio_leg = Legend(0.25, 0.4, 0.551, 0.362);
    
    //The signal bit:
    std::string hsignal = tmp_cuts_1;// + " && ";
    Topology sig_top = m_topologies->GetTopology(m_signal);
    hsignal += sig_top.GetSignal();//    cout << "hsignal: " << hsignal << endl;
    DrawingTools::KinMap signal_kinmap = KinArray(var.name, nbins, bins, var.symbol, hsignal);
    SetColors(signal_kinmap, sig_top.GetFillColor(), sig_top.GetLineColor(), sig_top.GetFillStyle(), sig_top.GetLineStyle());
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
            //            cout << i << ":" << (int)kinmap_list.size() << " : " << (int)(kinmap_list.size() - i) << endl;
            
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

BDCans BreakdownTools::TOPO(Variable var, Int_t nbins, Double_t low, Double_t high, std::string cuts){
    return TOPO(var, nbins, SetBinning(nbins, low, high), cuts);
}

BDCans BreakdownTools::TARGET(Variable var, Int_t nbins, Double_t * bins, std::string cuts){

    std::string tmp_cuts_1 = cuts;
    if(!cuts.empty()){
        tmp_cuts_1 += " && ";
    }
    
    //Hydrogen:
    std::string hsig = tmp_cuts_1 + m_target + " == 1";
    DrawingTools::KinMap hydrogen_kinmap = KinArray(var.name, nbins, bins, var.symbol, hsig);
    SetColors(hydrogen_kinmap, DrawingStyle::HYDROGEN);
    
    //Carbon:
    std::string csig = tmp_cuts_1 + m_target + " == 6";
    DrawingTools::KinMap carbon_kinmap = KinArray(var.name, nbins, bins, var.symbol, csig);
    SetColors(carbon_kinmap, DrawingStyle::CARBON);

    //Other:
    std::string osig = tmp_cuts_1 + m_target + " != 1 && " + m_target + " != 6";
    DrawingTools::KinMap other_kinmap = KinArray(var.name, nbins, bins, var.symbol, osig);
    SetColors(other_kinmap, DrawingStyle::Other);

    //The signal bit:
    std::string hsignal = tmp_cuts_1;
    Topology sig_top = m_topologies->GetTopology(m_signal);
    hsignal += sig_top.GetSignal();
    DrawingTools::KinMap signal_kinmap = KinArray(var.name, nbins, bins, var.symbol, hsignal);
    SetColors(signal_kinmap, sig_top.GetFillColor(), sig_top.GetLineColor(), sig_top.GetFillStyle(), sig_top.GetLineStyle());
    signal_kinmap.recon->SetLineWidth(2);
    signal_kinmap.truth->SetLineWidth(2);
    signal_kinmap.ratio->SetLineWidth(2);

    std::vector<KinMap> kinmap_list;
    kinmap_list.push_back( other_kinmap );
    kinmap_list.push_back( carbon_kinmap );
    kinmap_list.push_back( hydrogen_kinmap );
    
    THStack * recon_tot = new THStack( (var.savename + "_TAR_recon").c_str(), Form(";%s (%s);%s", kinmap_list[0].recon->GetXaxis()->GetTitle(), var.units.c_str(),
                                                                                   kinmap_list[0].recon->GetYaxis()->GetTitle() ) );
    
    THStack * truth_tot = new THStack( (var.savename + "_TAR_truth").c_str(), Form(";%s (%s);%s", kinmap_list[0].truth->GetXaxis()->GetTitle(), var.units.c_str(),
                                                                                   kinmap_list[0].truth->GetYaxis()->GetTitle() ) );
    
    THStack * ratio_tot = new THStack( (var.savename + "_TAR_ratio").c_str(), Form(";%s (%s);%s", kinmap_list[0].ratio->GetXaxis()->GetTitle(), var.units.c_str(),
                                                                                   kinmap_list[0].ratio->GetYaxis()->GetTitle() ) );
    
    TH2D * smear_tot = (TH2D*)kinmap_list[0].smear->Clone( (var.savename + "_TAR_smear").c_str() );//Just add all of these histos.
    
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

BDCans BreakdownTools::TARGET(Variable var, Int_t nbins, Double_t low, Double_t high, std::string cuts){
    return TARGET(var, nbins, SetBinning(nbins, low, high), cuts);
}

TCanvas * BreakdownTools::TARGETSingle(Variable var, Int_t nbins, Double_t * bins, std::string cuts){
    
    std::string tmp_cuts_1 = cuts;
    if(!cuts.empty()){
        tmp_cuts_1 += " && ";
    }
    
    //Hydrogen:
    std::string hsig = tmp_cuts_1 + m_target + " == 1";
    TH1D * hydrogen_kinmap = GetHisto(var.name, nbins, bins, var.symbol, hsig);
    SetColors(hydrogen_kinmap, DrawingStyle::HYDROGEN);
    
    //Carbon:
    std::string csig = tmp_cuts_1 + m_target + " == 6";
    TH1D * carbon_kinmap = GetHisto(var.name, nbins, bins, var.symbol, csig);
    SetColors(carbon_kinmap, DrawingStyle::CARBON);
    
    //Other:
    std::string osig = tmp_cuts_1 + m_target + " != 1 && " + m_target + " != 6";
    TH1D * other_kinmap = GetHisto(var.name, nbins, bins, var.symbol, osig);
    SetColors(other_kinmap, DrawingStyle::Other);
    
    //The GetSignal() bit:
    std::string hsignal = tmp_cuts_1;
    Topology sig_top = m_topologies->GetTopology(m_signal);
    hsignal += sig_top.GetSignal();
    TH1D * signal_kinmap = GetHisto(var.name, nbins, bins, var.symbol, hsignal);
    SetColors(signal_kinmap, sig_top.GetFillColor(), sig_top.GetLineColor(), sig_top.GetFillStyle(), sig_top.GetLineStyle());
    signal_kinmap->SetLineWidth(2);
    
    std::vector<TH1D*> kinmap_list;
    kinmap_list.push_back( other_kinmap );
    kinmap_list.push_back( carbon_kinmap );
    kinmap_list.push_back( hydrogen_kinmap );
    
    THStack * recon_tot = new THStack( (var.savename + "_TAR").c_str(), Form(";%s (%s);%s", kinmap_list[0]->GetXaxis()->GetTitle(), var.units.c_str(),
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

void BreakdownTools::DrawZeroPointLine(TH1 * histo){

    TLine * z_line = new TLine(0.0, histo->GetMinimum(), 0.0, histo->GetMaximum());
    z_line->SetLineColor(1);
    z_line->SetLineStyle(2);
    z_line->Draw(); 
}

void BreakdownTools::DrawZeroPointLine(THStack * histo){

    TList * rlist = histo->GetHists();
    TH1D * hfirst = (TH1D*)rlist->First()->Clone( (string(rlist->First()->GetName()) + "_clone").c_str() );
    DrawZeroPointLine(hfirst);
}
