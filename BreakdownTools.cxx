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

PDGs::PDGs(Int_t part_pdg, std::string part_name, std::string part_symbol) : pdg(part_pdg), name(part_name), symbol(part_symbol), line_colour(1), line_style(1) {
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

#ifndef _TOPOLOGY_CXX
#define _TOPOLOGY_CXX

TOPS::TOPS(TOPSTYPE::TOPSTYPE topo_type) : type(topo_type), name(TOPSTYPE::ToString(type ,0)), symbol(TOPSTYPE::ToString(type ,1)), fill_style(1001), line_colour(1), line_style(1), is_Other(false) {
    
    if(type == TOPSTYPE::CC1P1PiPlus || type == TOPSTYPE::HCC1P1PiPlus){
        
        signal = "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1 ";
        signal += "&& truth_n_ntn == 0 && truth_n_pho == 0 && truth_n_pi0 == 0 && truth_n_piM == 0 ";
        signal += "&& truth_n_piP == 1 && truth_n_pro == 1 && truth_n_tau == 0";
        signal += "&& mc_nFSPart == 3";
        
        if(type == TOPSTYPE::HCC1P1PiPlus){
            signal += " && mc_targetZ == 1";
            line_style = 2;
            line_colour = 1;
            fill_style = 0;
        }
        else fill_colour = (Int_t)DrawingStyle::T1P1PiP;//Proton
    }
    else if(type == TOPSTYPE::CCNPNPiMinus){
        fill_colour = (Int_t)DrawingStyle::T1P1PiM;
        signal = "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1 ";
        signal += "&& truth_n_ntn == 0 && truth_n_pho == 0 && truth_n_pi0 == 0 && truth_n_piM > 0 ";
        signal += "&& truth_n_piP == 0 && truth_n_pro > 0 && truth_n_tau == 0";
        signal += "&& mc_nFSPart > 0";
    }
    else if(type == TOPSTYPE::CCNP){
        fill_colour = (Int_t)DrawingStyle::T2Pr;
        signal = "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1 ";
        signal += "&& truth_n_ntn == 0 && truth_n_pho == 0 && truth_n_pi0 == 0 && truth_n_piM == 0 ";
        signal += "&& truth_n_piP == 0 && truth_n_pro > 0 && truth_n_tau == 0";
        signal += "&& mc_nFSPart > 0";
    }
    else if(type == TOPSTYPE::CCNPiPlus){
        fill_colour = (Int_t)DrawingStyle::T2PiP;
        signal = "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1 ";
        signal += "&& truth_n_ntn == 0 && truth_n_pho == 0 && truth_n_pi0 == 0 && truth_n_piM == 0 ";
        signal += "&& truth_n_piP > 0 && truth_n_pro == 0 && truth_n_tau == 0";
        signal += "&& mc_nFSPart > 0";
    }
    else if(type == TOPSTYPE::CCNPNPiZero){
        fill_colour = (Int_t)DrawingStyle::T1P1Pi0;
        signal = "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1 ";
        signal += "&& truth_n_ntn == 0 && truth_n_pho == 0 && truth_n_pi0 > 0 && truth_n_piM == 0 ";
        signal += "&& truth_n_piP == 0 && truth_n_pro > 0 && truth_n_tau == 0";
        signal += "&& mc_nFSPart > 0";
    }
    else if(type == TOPSTYPE::CCNPiZeroNPiPlus){
        fill_colour = (Int_t)DrawingStyle::T1Pi1Pi0;
        signal = "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1 ";
        signal += "&& truth_n_ntn == 0 && truth_n_pho == 0 && truth_n_pi0 > 0 && truth_n_piM == 0 ";
        signal += "&& truth_n_piP > 0 && truth_n_pro == 0 && truth_n_tau == 0";
        signal += "&& mc_nFSPart > 0";
    }
    else if(type == TOPSTYPE::CCKaonsOth){
        fill_colour = (Int_t)DrawingStyle::T1P1PiM;
        fill_style = 3008;
        signal = "truth_n_ele == 0 && (truth_n_kPM > 0 || truth_n_kaO > 0) && truth_n_muo == 1 ";
        signal += "&& (truth_n_ntn > 0 || truth_n_pho > 0 || truth_n_pi0 > 0 || truth_n_piM > 0 ";
        signal += "|| truth_n_piP > 0 || truth_n_pro > 0) && truth_n_tau == 0";
        signal += "&& mc_nFSPart > 0";
    }
    else if(type == TOPSTYPE::CCNN){
        fill_style = 3009;
        fill_colour = (Int_t)DrawingStyle::T2Pr;
        signal = "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1 ";
        signal += "&& truth_n_ntn > 0 && truth_n_pho == 0 && truth_n_pi0 == 0 && truth_n_piM == 0 ";
        signal += "&& truth_n_piP == 0 && truth_n_pro == 0 && truth_n_tau == 0";
        signal += "&& mc_nFSPart > 0";
    }
    else if(type == TOPSTYPE::CCNPNN){
        fill_colour = (Int_t)DrawingStyle::T2PiP;
        fill_style = 3019;
        signal = "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1 ";
        signal += "&& truth_n_ntn > 0 && truth_n_pho == 0 && truth_n_pi0 == 0 && truth_n_piM == 0 ";
        signal += "&& truth_n_piP == 0 && truth_n_pro > 0 && truth_n_tau == 0";
        signal += "&& mc_nFSPart > 0";
    }
    else if(type == TOPSTYPE::CCNPiNN){
        fill_colour = (Int_t)DrawingStyle::T1P1Pi0;
        fill_style = 3025;
        signal = "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1 ";
        signal += "&& truth_n_ntn > 0 && truth_n_pho == 0 && (truth_n_pi0 > 0 || truth_n_piM > 0 ";
        signal += "|| truth_n_piP > 0) && truth_n_pro == 0 && truth_n_tau == 0";
        signal += "&& mc_nFSPart > 0";
    }
    else if(type == TOPSTYPE::CCNPiNPNN){
        fill_colour = (Int_t)DrawingStyle::T1Pi1Pi0;
        fill_style = 3024;
        signal = "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1 ";
        signal += "&& truth_n_ntn > 0 && truth_n_pho == 0 && (truth_n_pi0 > 0 || truth_n_piM > 0 ";
        signal += "|| truth_n_piP > 0) && truth_n_pro > 0 && truth_n_tau == 0";
        signal += "&& mc_nFSPart > 0";
    }
//    else if(type == TOPSTYPE::CC1Pi1PNN){
//        fill_colour = (Int_t)DrawingStyle::CC1Pi1PNN;
//        signal = "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1 ";
//        signal += "&& truth_n_ntn > 0 && truth_n_pho == 0 && (truth_n_pi0 > 0 || truth_n_piM > 0 ";
//        signal += "|| truth_n_piP > 0) && truth_n_pro > 0 && truth_n_tau == 0";
//        signal += "&& mc_nFSPart > 0";
//    }
    else if(type == TOPSTYPE::Other){
        signal = "";
        is_Other = true;
        fill_colour = (Int_t)DrawingStyle::Other;
    }
    else std::cout << "Warning Toplogoly definition not set!! This will make no cuts on " << TOPSTYPE::ToString(type, 0) << " topology." << std::endl;
}

void TOPS::AddToOther(TOPSTYPE::TOPSTYPE topo_type){
//    std::cout << " signal (pre) = " << signal << std::endl;
    if(is_Other){
        if(!signal.empty()) signal += " && ";
        signal += "!(";
        TOPS tmp_top = TOPS(topo_type);
        signal += tmp_top.signal;
        signal += ")";
    }
//    std::cout << " signal (post) = " << signal << std::endl;
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
    ResetPDGBDlist();
    
    std::vector<TOPSTYPE::TOPSTYPE> topo_list;
    topo_list.push_back( TOPSTYPE::CC1P1PiPlus      );
    topo_list.push_back( TOPSTYPE::CCNPNPiMinus     );
    topo_list.push_back( TOPSTYPE::CCNP             );
    topo_list.push_back( TOPSTYPE::CCNPiPlus        );
    topo_list.push_back( TOPSTYPE::CCNPNPiZero      );
    topo_list.push_back( TOPSTYPE::CCNPiZeroNPiPlus );
    topo_list.push_back( TOPSTYPE::CCKaonsOth );
    topo_list.push_back( TOPSTYPE::CCNN );
    topo_list.push_back( TOPSTYPE::CCNPNN );
    topo_list.push_back( TOPSTYPE::CCNPiNN );
    topo_list.push_back( TOPSTYPE::CCNPiNPNN );
    
    TOPS Other_Tops(TOPSTYPE::Other);//This has to be added to the list last.
    
    for(int i = 0; i < (int)topo_list.size(); i++){
        m_toplist.push_back( TOPS(topo_list[i]) );
        Other_Tops.AddToOther( topo_list[i] );
    }
    m_toplist.push_back( Other_Tops );
    ResetTOPBDlist();
}

BreakdownTools::~BreakdownTools(){
    m_pdglist.clear();
    m_toplist.clear();

    ClearPDGBDlist();
    ClearTOPBDlist();
}

void BreakdownTools::ResetPDGBDlist(){
    ClearPDGBDlist();
    SetMinPDGBDlist(2212);
    SetMinPDGBDlist(211);
    SetMinPDGBDlist(-211);
    SetMinPDGBDlist(13);
}

void BreakdownTools::ResetTOPBDlist(){
    ClearTOPBDlist();
    SetMinTOPBDlist( TOPSTYPE::CC1P1PiPlus        );
    SetMinTOPBDlist( TOPSTYPE::CCNPNPiMinus       );
    SetMinTOPBDlist( TOPSTYPE::CCNP               );
    SetMinTOPBDlist( TOPSTYPE::CCNPiPlus          );
    SetMinTOPBDlist( TOPSTYPE::CCNPNPiZero        );
    SetMinTOPBDlist( TOPSTYPE::CCNPiZeroNPiPlus   );
    SetMinTOPBDlist( TOPSTYPE::Other              );
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
        
//        cout << "tmp_cuts: " << tmp_cuts << endl;
        
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
        
        SetColors(tmp_kinmap, particle.colour);
        
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
            recon_leg->AddEntry(kinmap_list[ (i - 1) ].recon, Form("%s (%.2f%%)", m_pdglist[(i - 1)].symbol.c_str(), recon_percent[ i - 1 ]), "f");
            
            truth_tot->Add(kinmap_list[ (int)(kinmap_list.size() - i) ].truth);
            truth_leg->AddEntry(kinmap_list[ (i - 1) ].truth, Form("%s (%.2f%%)", m_pdglist[(i - 1)].symbol.c_str(), truth_percent[ i - 1 ]), "f");
            
            ratio_tot->Add(kinmap_list[ (int)(kinmap_list.size() - i) ].ratio);
            ratio_leg->AddEntry(kinmap_list[ (i - 1) ].ratio, Form("%s (%.2f%%)", m_pdglist[(i - 1)].symbol.c_str(), ratio_percent[ i - 1 ]), "f");
            
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
                if(m_pdglist_minBD[j] == m_pdglist[(i - 1)].pdg){
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
                truth_leg->AddEntry(kinmap_list[ plot_by_self[i] ].truth, Form("%s (%.2f%%)", m_pdglist[ plot_by_self[i] ].symbol.c_str(), truth_percent[ plot_by_self[i] ]), "f");
                
                ratio_tot->Add(kinmap_list[ plot_by_self[i] ].ratio);
                ratio_leg->AddEntry(kinmap_list[ plot_by_self[i] ].ratio, Form("%s (%.2f%%)", m_pdglist[ plot_by_self[i] ].symbol.c_str(), ratio_percent[ plot_by_self[i] ]), "f");
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
    ratio_leg->Draw();
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
    
    std::string tmp_cuts_1 = cuts;
    if(!cuts.empty()){
        tmp_cuts_1 += " && ";
    }
    
    for(int i = 0; i < (int)m_toplist.size(); i++){
        
//        cout << i << ":" << (int)m_toplist.size() << endl;
        
        TOPS topology = m_toplist[i];
        
        std::string tmp_cuts = tmp_cuts_1;
        tmp_cuts += topology.signal;
//        cout << "tmp_cuts: " << tmp_cuts << endl;
        
//        cout << "Working -2" << endl;
        
        DrawingTools::KinMap tmp_kinmap = KinArray(var.name, nbins, bins, var.symbol, tmp_cuts);
        
//        cout << "Working -1" << endl;
        
        SetColors(tmp_kinmap, topology.fill_colour, topology.line_colour, topology.fill_style, topology.line_style);
        
//        cout << "Working 0" << endl;

        kinmap_list.push_back(tmp_kinmap);
    }
    
//    cout << "Working 1" << endl;
    
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
    TOPS sig_top(TOPSTYPE::HCC1P1PiPlus);
    hsignal += sig_top.signal;//    cout << "hsignal: " << hsignal << endl;
    DrawingTools::KinMap signal_kinmap = KinArray(var.name, nbins, bins, var.symbol, hsignal);
    SetColors(signal_kinmap, sig_top.fill_colour, sig_top.line_colour, sig_top.fill_style, sig_top.line_style);
//    signal_kinmap.recon->SetLineColor(1);
//    signal_kinmap.truth->SetLineColor(1);
//    signal_kinmap.recon->SetLineStyle(2);
//    signal_kinmap.truth->SetLineStyle(2);
    signal_kinmap.recon->SetLineWidth(2);
    signal_kinmap.truth->SetLineWidth(2);
    signal_kinmap.ratio->SetLineWidth(2);
    
    recon_leg->AddEntry(signal_kinmap.recon, ("H-" + m_toplist[0].symbol).c_str(), "l");
    truth_leg->AddEntry(signal_kinmap.truth, ("H-" + m_toplist[0].symbol).c_str(), "l");
    ratio_leg->AddEntry(signal_kinmap.ratio, ("H-" + m_toplist[0].symbol).c_str(), "l");
    
    std::vector<double> recon_percent = GetPercentage(kinmap_list, 0);
    std::vector<double> truth_percent = GetPercentage(kinmap_list, 1);
    std::vector<double> ratio_percent = GetPercentage(kinmap_list, 2);

//    cout << "Working 2" << endl;

    if(m_fullbreakdown){
        
//        cout << "Working 3a" << endl;
        
        for(int i = 1; i < (int)(kinmap_list.size() + 1); i++){
            //            cout << i << ":" << (int)kinmap_list.size() << " : " << (int)(kinmap_list.size() - i) << endl;
            
            recon_tot->Add(kinmap_list[ (int)(kinmap_list.size() - i) ].recon);
            recon_leg->AddEntry(kinmap_list[ (i - 1) ].recon, Form("%s (%.2f%%)", m_toplist[(i - 1)].symbol.c_str(), recon_percent[ i - 1 ]), "f");
            
            truth_tot->Add(kinmap_list[ (int)(kinmap_list.size() - i) ].truth);
            truth_leg->AddEntry(kinmap_list[ (i - 1) ].truth, Form("%s (%.2f%%)", m_toplist[(i - 1)].symbol.c_str(), truth_percent[ i - 1 ]), "f");
            
            ratio_tot->Add(kinmap_list[ (int)(kinmap_list.size() - i) ].ratio);
            ratio_leg->AddEntry(kinmap_list[ (i - 1) ].ratio, Form("%s (%.2f%%)", m_toplist[(i - 1)].symbol.c_str(), ratio_percent[ i - 1 ]), "f");
            
            if(i < (int)kinmap_list.size()) smear_tot->Add(kinmap_list[ i ].smear);
        }
    }
    else{
        
//        cout << "Working 3b" << endl;

        double recon_other_percent = 0.;
        double truth_other_percent = 0.;
        double ratio_other_percent = 0.;
        
        std::vector<int> plot_by_self;
        
        for(int i = 1; i < (int)(kinmap_list.size() + 1); i++){
//            cout << i << ":" << (int)kinmap_list.size() << " : " << (int)(kinmap_list.size() - i) << endl;
            
            bool in_other = true;
            for(int j = 0; j < (int)m_pdglist_minBD.size(); j++){
                if(m_toplist_minBD[j] == m_toplist[(i - 1)].type){
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
            recon_leg->AddEntry(kinmap_list[ plot_by_self[i] ].recon, Form("%s (%.2f%%)", m_toplist[ plot_by_self[i] ].symbol.c_str(), recon_percent[ plot_by_self[i] ]), "f");
            
            truth_tot->Add(kinmap_list[ plot_by_self[i] ].truth);
            truth_leg->AddEntry(kinmap_list[ plot_by_self[i] ].truth, Form("%s (%.2f%%)", m_toplist[ plot_by_self[i] ].symbol.c_str(), truth_percent[ plot_by_self[i] ]), "f");
            
            ratio_tot->Add(kinmap_list[ plot_by_self[i] ].ratio);
            ratio_leg->AddEntry(kinmap_list[ plot_by_self[i] ].ratio, Form("%s (%.2f%%)", m_toplist[ plot_by_self[i] ].symbol.c_str(), ratio_percent[ plot_by_self[i] ]), "f");
        }
        
        recon_leg->AddEntry(kinmap_list.back().recon, Form("Other (%.2f%%)", recon_other_percent), "f");
        truth_leg->AddEntry(kinmap_list.back().truth, Form("Other (%.2f%%)", truth_other_percent), "f");
        ratio_leg->AddEntry(kinmap_list.back().ratio, Form("Other (%.2f%%)", ratio_other_percent), "f");
    }
    
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
    ratio_leg->Draw();
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
    std::string hsig = tmp_cuts_1 + "mc_targetZ == 1";
    DrawingTools::KinMap hydrogen_kinmap = KinArray(var.name, nbins, bins, var.symbol, hsig);
    SetColors(hydrogen_kinmap, DrawingStyle::HYDROGEN);
    
    //Carbon:
    std::string csig = tmp_cuts_1 + "mc_targetZ == 6";
    DrawingTools::KinMap carbon_kinmap = KinArray(var.name, nbins, bins, var.symbol, csig);
    SetColors(carbon_kinmap, DrawingStyle::CARBON);

    //Other:
    std::string osig = tmp_cuts_1 + "mc_targetZ != 1 && mc_targetZ != 6";
    DrawingTools::KinMap other_kinmap = KinArray(var.name, nbins, bins, var.symbol, osig);
    SetColors(other_kinmap, DrawingStyle::Other);

    //The signal bit:
    std::string hsignal = tmp_cuts_1;
    TOPS sig_top(TOPSTYPE::HCC1P1PiPlus);
    hsignal += sig_top.signal;
    DrawingTools::KinMap signal_kinmap = KinArray(var.name, nbins, bins, var.symbol, hsignal);
    SetColors(signal_kinmap, sig_top.fill_colour, sig_top.line_colour, sig_top.fill_style, sig_top.line_style);
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
    
    recon_leg->AddEntry(signal_kinmap.recon, ("H-" + m_toplist[0].symbol).c_str(), "l");
    truth_leg->AddEntry(signal_kinmap.truth, ("H-" + m_toplist[0].symbol).c_str(), "l");
    ratio_leg->AddEntry(signal_kinmap.ratio, ("H-" + m_toplist[0].symbol).c_str(), "l");
    
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
    ratio_leg->Draw();
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
    std::string hsig = tmp_cuts_1 + "mc_targetZ == 1";
    TH1D * hydrogen_kinmap = GetHisto(var.name, nbins, bins, var.symbol, hsig);
    SetColors(hydrogen_kinmap, DrawingStyle::HYDROGEN);
    
    //Carbon:
    std::string csig = tmp_cuts_1 + "mc_targetZ == 6";
    TH1D * carbon_kinmap = GetHisto(var.name, nbins, bins, var.symbol, csig);
    SetColors(carbon_kinmap, DrawingStyle::CARBON);
    
    //Other:
    std::string osig = tmp_cuts_1 + "mc_targetZ != 1 && mc_targetZ != 6";
    TH1D * other_kinmap = GetHisto(var.name, nbins, bins, var.symbol, osig);
    SetColors(other_kinmap, DrawingStyle::Other);
    
    //The signal bit:
    std::string hsignal = tmp_cuts_1;
    TOPS sig_top(TOPSTYPE::HCC1P1PiPlus);
    hsignal += sig_top.signal;
    TH1D * signal_kinmap = GetHisto(var.name, nbins, bins, var.symbol, hsignal);
    SetColors(signal_kinmap, sig_top.fill_colour, sig_top.line_colour, sig_top.fill_style, sig_top.line_style);
    signal_kinmap->SetLineWidth(2);
    
    std::vector<TH1D*> kinmap_list;
    kinmap_list.push_back( other_kinmap );
    kinmap_list.push_back( carbon_kinmap );
    kinmap_list.push_back( hydrogen_kinmap );
    
    THStack * recon_tot = new THStack( (var.savename + "_TAR").c_str(), Form(";%s (%s);%s", kinmap_list[0]->GetXaxis()->GetTitle(), var.units.c_str(),
                                                                                   kinmap_list[0]->GetYaxis()->GetTitle() ) );
    
    TLegend * recon_leg = Legend(0.25, 0.4, 0.551, 0.362);
    
    std::vector<double> recon_percent = GetPercentage(kinmap_list);
    
    recon_leg->AddEntry(signal_kinmap.recon, ("H-" + m_toplist[0].symbol).c_str(), "l");
    
    recon_leg->AddEntry(kinmap_list[2].recon, Form("Hydrogen (%.2f%%)", recon_percent[2]), "f");
    
    recon_leg->AddEntry(kinmap_list[1].recon, Form("Carbon (%.2f%%)", recon_percent[1]), "f");
    
    recon_leg->AddEntry(kinmap_list[0].recon, Form("Other (%.2f%%)", recon_percent[0]), "f");
    
    for(int i = 0; i < 3; i++) recon_tot->Add( kinmap_list[i].recon);
    
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
