#include "BreakdownTools.h"
#include "DrawingStyle.h"

#include <string>
#include <iostream>

#include "TH1D.h"
#include "TH2D.h"
#include "TLegend.h"
#include "THStack.h"
#include "TCanvas.h"

#ifndef _PARTPDGS_CXX
#define _PARTPDGS_CXX

PDGs::PDGs(Int_t part_pdg, std::string part_name, std::string part_symbol) : pdg(part_pdg), name(part_name), symbol(part_symbol){
    stringstream ss;
    ss << pdg;
    pdg_s = ss.str();
    
    if(name.find("proton") != std::string::npos)        colour = (Int_t)DrawingStyle::Proton;//Proton
    else if(name.find("piplus") != std::string::npos)   colour = (Int_t)DrawingStyle::PionP;//PionP
    else if(name.find("piminus") != std::string::npos)  colour = (Int_t)DrawingStyle::PionM;
    else if(name.find("pion") != std::string::npos)     colour = (Int_t)DrawingStyle::Pion;
    else if(name.find("amuon") != std::string::npos)    colour = (Int_t)DrawingStyle::MuonP;//PionP
    else if(name.find("muon") != std::string::npos)     colour = (Int_t)DrawingStyle::MuonM;
    else if(name.find("pizero") != std::string::npos)   colour = (Int_t)DrawingStyle::Pi0;
    else if(name.find("kapm") != std::string::npos)     colour = (Int_t)DrawingStyle::Kaon;
    else if(name.find("kazero") != std::string::npos)   colour = (Int_t)DrawingStyle::Ka0;
    else colour = (Int_t)DrawingStyle::Other;

}
#endif

BreakdownTools::BreakdownTools(std::string filename, std::string treename) : DrawingTools(filename, treename, ("BD" + treename).c_str()), m_printPOT(false), m_fullbreakdown(true){

//    PDGs proton;
    m_pdglist.push_back( PDGs(2212, "proton",   "p"));
    m_pdglist.push_back( PDGs(211,  "piplus",   "#pi^{+}") );
    m_pdglist.push_back( PDGs(-211, "piminus", "#pi^{-}") );
    m_pdglist.push_back( PDGs(13, "muon", "#mu^{-}") );
    m_pdglist.push_back( PDGs(-13, "amuon", "#mu^{-}") );
    m_pdglist.push_back( PDGs(111, "pizero", "#pi^{0}") );
    m_pdglist.push_back( PDGs(321, "kapm", "K^{#pm}") );
    m_pdglist.push_back( PDGs(311, "kazero", "K^{0}") );
    //Miminum particles to define in breakdown:
    ResetBDlist();
}

BreakdownTools::~BreakdownTools(){
    m_pdglist.clear();
    ClearBDlist();
}

void BreakdownTools::ResetBDlist(){
    ClearBDlist();
    m_MinBDlist.push_back(2212);
    m_MinBDlist.push_back(211);
    m_MinBDlist.push_back(-211);
    m_MinBDlist.push_back(13);
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
    
        PDGs particle = m_pdglist[i];
        
        std::string tmp_cuts = tmp_cuts_1;
        tmp_cuts += pdgvar;
        tmp_cuts += " == ";
        tmp_cuts += particle.pdg_s;
        
        cout << "tmp_cuts: " << tmp_cuts << endl;
        
        if(other_cut.empty()){
            other_cut = pdgvar;
            other_cut += " != ";
            other_cut += particle.pdg_s;
        }
        else{
            other_cut += " && ";
            other_cut += pdgvar;
            other_cut += " != ";
            other_cut += particle.pdg_s;
        }
            
        DrawingTools::KinMap tmp_kinmap = KinArray(var.name, nbins, bins, var.symbol, tmp_cuts);
        
        ColFill(tmp_kinmap, particle.colour);
        
        kinmap_list.push_back(tmp_kinmap);
    }
    
    if(!cuts.empty()) other_cut += ")";
    
    cout << "other_cut: " << other_cut << endl;
    
    DrawingTools::KinMap other_kinmap = KinArray(var.name, nbins, bins, var.symbol, other_cut);
    ColFill(other_kinmap, DrawingStyle::Other);

    //Make outputs:
    
    THStack * recon_tot = new THStack( (var.savename + "_recon").c_str(), Form(";%s (%s);%s", kinmap_list[0].recon->GetXaxis()->GetTitle(), var.units.c_str(),
                                                                                kinmap_list[0].recon->GetYaxis()->GetTitle() ) );
    
    THStack * truth_tot = new THStack( (var.savename + "_truth").c_str(), Form(";%s (%s);%s", kinmap_list[0].truth->GetXaxis()->GetTitle(), var.units.c_str(),
                                                                               kinmap_list[0].truth->GetYaxis()->GetTitle() ) );
    
    THStack * ratio_tot = new THStack( (var.savename + "_ratio").c_str(), Form(";%s (%s);%s", kinmap_list[0].ratio->GetXaxis()->GetTitle(), var.units.c_str(),
                                                                               kinmap_list[0].ratio->GetYaxis()->GetTitle() ) );
    
    TH2D * smear_tot = (TH2D*)kinmap_list[0].smear->Clone( (var.savename + "_smear").c_str() );//Just add all of these histos.
    
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
            recon_leg->AddEntry(kinmap_list[ (i - 1) ].recon, Form("%s (%.2f%%)", m_pdglist[(i - 1)].symbol.c_str(), recon_percent[ i - 1 ]), "f");
            
            truth_tot->Add(kinmap_list[ (int)(kinmap_list.size() - i) ].truth);
            truth_leg->AddEntry(kinmap_list[ (i - 1) ].truth, Form("%s (%.2f%%)", m_pdglist[(i - 1)].symbol.c_str(), recon_percent[ i - 1 ]), "f");
            
            ratio_tot->Add(kinmap_list[ (int)(kinmap_list.size() - i) ].ratio);
            ratio_leg->AddEntry(kinmap_list[ (i - 1) ].ratio, Form("%s (%.2f%%)", m_pdglist[(i - 1)].symbol.c_str(), recon_percent[ i - 1 ]), "f");
            
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
            cout << i << ":" << (int)kinmap_list.size() << " : " << (int)(kinmap_list.size() - i) << endl;
            
            bool in_other = true;
            for(int j = 0; j < (int)m_MinBDlist.size(); j++){
                if(m_MinBDlist[j] == m_pdglist[(i - 1)].pdg){
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
                recon_leg->AddEntry(kinmap_list[ plot_by_self[i] ].recon, Form("%s (%.2f%%)", m_pdglist[ plot_by_self[i] ].symbol.c_str(), recon_percent[ plot_by_self[i] ]), "f");
                
                truth_tot->Add(kinmap_list[ plot_by_self[i] ].truth);
                truth_leg->AddEntry(kinmap_list[ plot_by_self[i] ].truth, Form("%s (%.2f%%)", m_pdglist[ plot_by_self[i] ].symbol.c_str(), recon_percent[ plot_by_self[i] ]), "f");
                
                ratio_tot->Add(kinmap_list[ plot_by_self[i] ].ratio);
                ratio_leg->AddEntry(kinmap_list[ plot_by_self[i] ].ratio, Form("%s (%.2f%%)", m_pdglist[ plot_by_self[i] ].symbol.c_str(), recon_percent[ plot_by_self[i] ]), "f");
        }
        
        recon_leg->AddEntry(other_kinmap.recon, Form("Other (%.2f%%)", recon_other_percent), "f");
        truth_leg->AddEntry(other_kinmap.truth, Form("Other (%.2f%%)", truth_other_percent), "f");
        ratio_leg->AddEntry(other_kinmap.ratio, Form("Other (%.2f%%)", ratio_other_percent), "f");
    }
    
    BDCans cans;
    
    cans.recon = new TCanvas( recon_tot->GetName(), "", 500, 500);
    cans.recon->cd();
    recon_tot->Draw();
    recon_leg->Draw();
//    mom_recon_pot->Draw();
    
    cans.truth = new TCanvas( truth_tot->GetName(), "", 500, 500);
    cans.truth->cd();
    truth_tot->Draw();
    truth_leg->Draw();
    
    cans.ratio = new TCanvas( ratio_tot->GetName(), "", 500, 500);
    cans.ratio->cd();
    ratio_tot->Draw();
    ratio_leg->Draw();
    
    cans.smear = new TCanvas( smear_tot->GetName(), "", 500, 500);
    cans.smear->cd();
    smear_tot->Draw();
    
    return cans;
}

BDCans BreakdownTools::PID(Variable var, Int_t nbins, Double_t low, Double_t high, std::string pdgvar, std::string cuts){
    return PID(var, nbins, SetBinning(nbins, low, high), pdgvar, cuts);
}
