#include "BreakdownTools.h"
#include "DrawingTools.h"
#include "TString.h"
#include <string>
#include <iostream>

BreakdownTools::BreakdownTools(TString filename, bool debug, TString reconame, TString truename){
    //m_plot = new DrawingTools(filename, debug, "BD", reconame, truename);
}

BreakdownTools::~BreakdownTools(){
    //delete m_plot;
}

/*BDCans BreakdownTools::PIDVar(const char * mom_name, const int mom_nbins, const double mom_low, const double mom_high, const char pname, const char * can_title, const char * mom_title, const char * cuts){

 /*   //Was taken from momentum plots in original;
    std::string internal_cuts = std::string(cuts);
    if(!internal_cuts.empty()) internal_cuts += " && ";
    
    KinMap mom_pr_map =  m_plot->KinArray(mom_name, mom_nbins, mom_low, mom_high, mom_title,  (internal_cuts + pname.str() + "_PDG == 2212").c_str());
    //KinMap mom_pi_map = m_plot->KinArray(mom_name), mom_nbins, mom_low, mom_high, mom_title,  Form("%s && TMath::Abs(%s_PDG) == 211", common_cuts_mom.c_str(), part_name_.c_str())));
    KinMap mom_piP_map = m_plot->KinArray(mom_name, mom_nbins, mom_low, mom_high, mom_title,  (internal_cuts + pname.str() + "_PDG == 211").c_str());
    KinMap mom_piM_map = m_plot->KinArray(mom_name, mom_nbins, mom_low, mom_high, mom_title,  (internal_cuts + pname.str() + "_PDG == -211").c_str());
    KinMap mom_mum_map = m_plot->KinArray(mom_name, mom_nbins, mom_low, mom_high, mom_title,  (internal_cuts + pname.str() + "_PDG == 13").c_str());
    KinMap mom_mup_map = m_plot->KinArray(mom_name, mom_nbins, mom_low, mom_high, mom_title,  (internal_cuts + pname.str() + "_PDG == -13").c_str());
    KinMap mom_p0_map =  m_plot->KinArray(mom_name, mom_nbins, mom_low, mom_high, mom_title,  (internal_cuts + pname.str() + "_PDG == 111").c_str());
    KinMap mom_ka_map =  m_plot->KinArray(mom_name, mom_nbins, mom_low, mom_high, mom_title,  (internal_cuts + "TMath::Abs(" + pname.str() + "_PDG) == 321").c_str());
    //KinMap mom_kz_map = m_plot->KinArray(mom_name, mom_nbins, mom_low, mom_high, mom_title,  (internal_cuts + pname.str() + "_PDG == 311").c_str());
    std::string mom_other = pname.str() + "_PDG != 2212 && TMath::Abs(" + pname.str() + "_PDG) != 211 && TMath::Abs(" + pname.str() + "_PDG) != 13 && ";
    mom_other += pname.str() + "_PDG != 111 && TMath::Abs(" + pname.str() + "_PDG) != 321 && " + pname.str() + "_PDG != 311 &&" + pname.str() + "_PDG != -999";
    KinMap mom_ot_map = m_plot->KinArray(mom_name, mom_nbins, mom_low, mom_high, mom_title, (internal_cuts + mom_other).c_str());
    
    std::vector<KinMap> mom_map;
    std::vector<std::string> mom_map_names;
    std::vector<int> mom_map_cols;
    
    mom_map.push_back( mom_pr_map );    mom_map_names.push_back("p");           mom_map_cols.push_back( DrawingStyle::Proton );
    mom_map.push_back( mom_piP_map );   mom_map_names.push_back("#pi^{+}");     mom_map_cols.push_back( DrawingStyle::Pion   );
    mom_map.push_back( mom_piM_map );   mom_map_names.push_back("#pi^{-}");     mom_map_cols.push_back( DrawingStyle::Ka0    );
    mom_map.push_back( mom_mum_map);    mom_map_names.push_back("#mu^{-}");     mom_map_cols.push_back( DrawingStyle::MuonM  );
    mom_map.push_back( mom_mup_map);    mom_map_names.push_back("#mu^{+}");     mom_map_cols.push_back( DrawingStyle::MuonP  );
    mom_map.push_back( mom_p0_map );    mom_map_names.push_back("#pi^{0}");     mom_map_cols.push_back( DrawingStyle::Pi0    );
    mom_map.push_back( mom_ka_map );    mom_map_names.push_back("K^{#pm}");     mom_map_cols.push_back( DrawingStyle::Kaon   );
    //mom_map.push_back( mom_kz_map );    mom_map_names.push_back("K^{0}");       mom_map_cols.push_back( DrawingStyle::Ka0    );
    mom_map.push_back( mom_ot_map );    mom_map_names.push_back("Other");       mom_map_cols.push_back( DrawingStyle::Other  );
    
    std::vector<TH1D*> mom_recon;
    std::vector<TH1D*> mom_truth;
    std::vector<TH2D*> mom_smear;
    std::vector<TH1D*> mom_ratio;
    
    int mom_map_size = (int)mom_map.size();
    
    for(int mpc = 0; mpc < mom_map_size; mpc++){
        m_plot->ColFill( mom_map[mpc].recon, mom_map_cols[mpc]);
        m_plot->ColFill( mom_map[mpc].truth, mom_map_cols[mpc]);
        m_plot->ColFill( mom_map[mpc].ratio, mom_map_cols[mpc]);
        
        mom_recon.push_back( mom_map[mpc].recon );
        mom_truth.push_back( mom_map[mpc].truth );
        mom_smear.push_back( mom_map[mpc].smear );
        mom_ratio.push_back( mom_map[mpc].ratio );
    }
    
    std::vector<double> mom_recon_per = m_plot->GetPercentage( mom_recon );
    std::vector<double> mom_truth_per = m_plot->GetPercentage( mom_truth );
    std::vector<double> mom_ratio_per = m_plot->GetPercentage( mom_ratio );
    
    string mom_recon_title = Form(";%s;%s", mom_recon[0]->GetXaxis()->GetTitle(), mom_recon[0]->GetYaxis()->GetTitle());
    string mom_truth_title = Form(";%s;%s", mom_truth[0]->GetXaxis()->GetTitle(), mom_truth[0]->GetYaxis()->GetTitle());
    string mom_smear_title = Form(";%s;%s", mom_smear[0]->GetXaxis()->GetTitle(), mom_smear[0]->GetYaxis()->GetTitle());
    string mom_ratio_title = Form(";%s;%s", mom_ratio[0]->GetXaxis()->GetTitle(), mom_ratio[0]->GetYaxis()->GetTitle());
    
    THStack * mom_recon_tot = new THStack( (pname.str() + tmp_hyp + "_recon").c_str() , mom_recon_title.c_str());
    THStack * mom_truth_tot = new THStack( (pname.str() + tmp_hyp + "_truth").c_str(), mom_truth_title.c_str());
    THStack * mom_ratio_tot = new THStack( (pname.str() + tmp_hyp + "_ratio").c_str(),  mom_ratio_title.c_str());
    TH2D * mom_smear_tot = (TH2D*)mom_smear[0]->Clone( (part_name_ + tmp_hyp + "_smear").c_str() );//Just add all of these histos.
    
    TLegend * mom_recon_leg = m_plot->Legend(0.25, 0.4, 0.551, 0.362);
    TLegend * mom_truth_leg = m_plot->Legend(0.25, 0.4, 0.551, 0.362);
    TLegend * mom_ratio_leg = m_plot->Legend(0.25, 0.4, 0.551, 0.362);
    
    for(int mpc = 1; mpc < mom_map_size + 1; mpc++){
        
        mom_recon_tot->Add( mom_recon[mom_map_size - mpc] );
        mom_truth_tot->Add( mom_truth[mom_map_size - mpc] );
        mom_ratio_tot->Add( mom_ratio[mom_map_size - mpc] );
        
        if( (mpc - 1) < mom_map_size){
            string mom_part_names = mom_map_names[mpc - 1];
            mom_recon_leg->AddEntry(mom_recon[ mpc - 1 ], Form("%s (%.2f%%)", mom_part_names.c_str(), mom_recon_per[ mpc - 1 ]), "f");
            mom_truth_leg->AddEntry(mom_truth[ mpc - 1 ], Form("%s (%.2f%%)", mom_part_names.c_str(), mom_truth_per[ mpc - 1 ]), "f");
            mom_ratio_leg->AddEntry(mom_ratio[ mpc - 1 ], Form("%s (%.2f%%)", mom_part_names.c_str(), mom_ratio_per[ mpc - 1 ]), "f");
        }
        
        if(mpc < mom_map_size) mom_smear_tot->Add( mom_smear[mpc] );
    }
    //-------------------------//
    
    TLegend * mom_recon_pot = m_plot->GetPOT(0.521,0.781);
    TLegend * mom_truth_pot = m_plot->GetPOT(0.521,0.781);
    TLegend * mom_ratio_pot = m_plot->GetPOT(0.521,0.781);
    */
    //BDCans canvases;
  
   /* canvases.recon = new TCanvas( (can_title.str() + "_" + pname.str() + "_recon").c_str(), "", 500, 500);
    canvases.recon->cd();
    mom_recon_tot->Draw();
    mom_recon_leg->Draw();
    mom_recon_pot->Draw();
    
    canvases.truth = new TCanvas( (can_title.str() + "_" + pname.str() + "_truth").c_str(), "", 500, 500);
    canvases.truth->cd();
    mom_truth_tot->Draw();
    mom_truth_leg->Draw();
    mom_truth_pot->Draw();
    
    canvases.smear = new TCanvas( (can_title.str() + "_" + pname.str() + "_smear").c_str(), "", 500, 500);
    canvases.smear->cd();
    mom_smear_tot->Draw("COLZ");
    //mom_smear_pot->Draw();
    
    canvases.ratio = new TCanvas( (can_title.str() + "_" + pname.str() + "_ratio").c_str(), "", 500, 500);
    canvases.ratio->cd();
    mom_ratio_tot->Draw();
    mom_ratio_leg->Draw();
    mom_ratio_pot->Draw();
   */
    //return canvases;
//}

/*
void BreakdownTools::TopVar(){
    
}

void BreakdownTools::MisPIDVar(){
    
}

*/

