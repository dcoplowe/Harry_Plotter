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
    
    DrawingStyle::DSColors varssss ;
    
    
//    if(name.find("proton") != std::string::npos) colour =  (Int_t)DrawingStyle::DSColors::Proton;//Proton
//    else if(name.find("pionP") != std::string::npos) colour =  static_cast<Int_t>(  DrawingStyle::DSColors::PionP );//PionP
//    else if(name.find("pionM") != std::string::npos) colour =  static_cast<Int_t>(  DrawingStyle::DSColors::PionM );
//    else if(name.find("pion") != std::string::npos) colour =  static_cast<Int_t>(   DrawingStyle::DSColors::Pion );
//    else if(name.find("muonP") != std::string::npos) colour =  static_cast<Int_t>(  DrawingStyle::DSColors::MuonP );//PionP
//    else if(name.find("muonM") != std::string::npos) colour =  static_cast<Int_t>(  DrawingStyle::DSColors::MuonM );
//    else if(name.find("muon") != std::string::npos) colour =  static_cast<Int_t>(   DrawingStyle::DSColors::Muon );
//    else if(name.find("pizero") != std::string::npos) colour =  static_cast<Int_t>( DrawingStyle::DSColors::Pi0 );
//    else if(name.find("kapm") != std::string::npos) colour =  static_cast<Int_t>(   DrawingStyle::DSColors::Kaon );
//    else if(name.find("kazero") != std::string::npos) colour =  static_cast<Int_t>( DrawingStyle::DSColors::Ka0 );
//    else colour = static_cast<Int_t>( DrawingStyle::DSColors::Other );
    
}
#endif

BreakdownTools::BreakdownTools(std::string filename, std::string treename) : DrawingTools(filename, treename, ("BD" + treename).c_str()), m_printPOT(false), m_fullbreakdown(false){

//    PDGs proton;
    m_pdglist.push_back( PDGs(2212, "proton",   "p"));
    m_pdglist.push_back( PDGs(211,  "piplus",   "#pi^{+}") );
    m_pdglist.push_back( PDGs(-211, "piminus", "#pi^{-}") );
    m_pdglist.push_back( PDGs(13, "muon", "#muon^{-}") );
    m_pdglist.push_back( PDGs(-13, "amuon", "#muon^{-}") );
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

BreakdownTools::ResetBDlist(){
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
            other_cut += " || ";
            other_cut += pdgvar;
            other_cut += " != ";
            other_cut += particle.pdg_s;
        }
            
        DrawingTools::KinMap tmp_kinmap = KinArray(var.name, nbins, bins, (var.symbol + " (" + var.units + ")" ).c_str(), tmp_cuts);
        kinmap_list.push_back(tmp_kinmap);
    }
    
    if(!cuts.empty()) other_cut += ")";
    
    cout << "other_cut: " << other_cut << endl;
    
    DrawingTools::KinMap other_kinmap = KinArray(var.name, nbins, bins, (var.symbol + " (" + var.units + ")" ).c_str(), other_cut);
    kinmap_list.push_back(other_kinmap);
//    particle_symbs.push_back("Other");
    
//    THStack ;
    
    
    if(m_fullbreakdown){
        
        for(int i = 0; i < (int)kinmap_list.size(); i++){
            
        }
        
        
    }
    else{
        
    }
    
    
    
    
    BDCans cans;
    
    return cans;
}

BDCans BreakdownTools::PID(Variable var, Int_t nbins, Double_t low, Double_t high, std::string pdgvar, std::string cuts){
    return PID(var, nbins, SetBinning(nbins, low, high), pdgvar, cuts);
}