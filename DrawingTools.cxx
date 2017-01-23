#include "DrawingTools.h"

#include "assert.h"
//ROOT Includes:
#include "TString.h"

//Forward declarations;
#include "TFile.h"
#include "TTree.h"
#include "TLegend.h"
#include "TLeaf.h"
#include "TH1D.h"
#include "TH2D.h"

using namespace std;

DrawingTools::DrawingTools(std::string filename, std::string treename, std::string uniquename) : m_filename(filename), m_treename(treename), m_uniquename(uniquename), m_verbose(false) {
    
    m_file = new TFile(m_filename.c_str(), "READ");
    
    if(m_file->IsZombie()){
        cout << "DrawingTools::DrawingTools() : Couldn't set file." << endl;
        exit(0);
    }
    
    m_tree = (TTree*)m_file->Get(m_treename.c_str());;
    
    m_1Dcounter = -1;
    m_2Dcounter = -1;
    
    m_ratiorange = 3;
    m_ratiobins = 59;

}

DrawingTools::~DrawingTools(){
    
    if(m_file->IsOpen()) m_file->Close();
    if(m_tree) delete m_tree;
    if(m_file) delete m_file;
}

TH1D * DrawingTools::GetHisto(std::string var, Int_t nbins, Double_t * bins, std::string xy_title, std::string cuts){
    
    m_1Dcounter++;
    TString host_name = Form("h1Dvar%s%.5d", m_uniquename.c_str(), m_1Dcounter);
    
    TString title;
    TString tmp_title(xy_title.c_str());
    if(tmp_title.Contains(";",TString::kExact)){
        title = xy_title;
    }
    else title = tmp_title + ";Counts";
    
    TH1D * hist = new TH1D(host_name.Data(), Form(";%s", title.Data()), nbins, bins);
    //hist->GetYaxis()->Setmaxdigits(2);
    //hist->GetXaxis()->Setmaxdigits(2);
    
//    TString tmp_cuts(cuts);
//    if(!tmp_cuts.EqualTo("", TString::kExact)) tmp_cuts.Append(" && ");
    
//    tmp_cuts.Append(Form("(%s != -999)", var.Data()));//Might get rid of this.
    
    //cout << tmp_cuts.Data() << endl;
    
    //    m_tree->Project(host_name.Data(), var.Data(), tmp_cuts.Data());
    m_tree->Project(host_name.Data(), var.c_str(), cuts.c_str());
    
    return hist;
}

TH1D * DrawingTools::GetHisto(std::string var, Int_t nbins, Double_t low, Double_t high, std::string xy_title, std::string cuts){
    return GetHisto(var, nbins, SetBinning(nbins, low, high), xy_title, cuts);
}

TH2D * DrawingTools::GetHisto(std::string var_yx, Int_t x_nbins, Double_t * x_bins, Int_t y_nbins, Double_t * y_bins, std::string xy_title, std::string cuts){
    
    m_2Dcounter++;
    TString host_name = Form("h2Dvar%s%.5d", m_uniquename.c_str(), m_2Dcounter);
    
    TH2D * hist = new TH2D(host_name.Data(), Form(";%s", xy_title.c_str()), x_nbins, x_bins, y_nbins, y_bins);
    //hist->GetYaxis()->Setmaxdigits(2);
    //hist->GetXaxis()->Setmaxdigits(2);
    
    //Separate true and rec vars:
//    TString y_var( vars_yx(0, vars_yx.First(":")) );
//    TString x_var( vars_yx(vars_yx.First(":") + 1, vars_yx.Length()) );
//    
//    TString tmp_cuts = cuts.Data();
//    if(!tmp_cuts.EqualTo("", TString::kExact)) tmp_cuts.Append(" && ");
//    tmp_cuts.Append(Form("(%s != -999) && (%s != -999)", y_var.Data(), x_var.Data()));
//    if(_DEBUG_) cout << tmp_cuts.Data() << endl;
    
    m_tree->Project(host_name.Data(), var_yx.c_str(), cuts.c_str());
    
    return hist;
}

TH2D * DrawingTools::GetHisto(std::string var_yx, Int_t x_nbins, Double_t x_low, Double_t x_high, Int_t y_nbins, Double_t y_low, Double_t y_high, std::string xy_title, std::string cuts){
    return GetHisto(var_yx, x_nbins, SetBinning(x_nbins, x_low, x_high), y_nbins, SetBinning(y_nbins, y_low, y_high), xy_title, cuts);
}

TH2D * DrawingTools::SmearMatrix(std::string vars_yx, Int_t nbins, Double_t * bins, std::string xy_title, std::string cuts){
    return GetHisto(vars_yx, nbins, bins, nbins, bins, xy_title, cuts);
}

TH2D * DrawingTools::SmearMatrix(std::string vars_yx, Int_t nbins, Double_t low, Double_t high, std::string xy_title, std::string cuts){
    return GetHisto(vars_yx, nbins, SetBinning(nbins, low, high), nbins, SetBinning(nbins, low, high), xy_title, cuts);
}

KinMap DrawingTools::KinArray(std::string vars_tr, Int_t nbins, Double_t * bins, std::string rt_title, std::string cuts){
    //In this map we can to keep all the entries preserved from one plot to another, i.e. all integrals are the same. Therefore want a common cut for all plots.
    
    KinMap map;
    
    TString tr_vars(vars_tr.c_str());
    if(!tr_vars.Contains(":")){
        cout << "DrawingTools::GetRTRatio : Error: Could not differentiate between truth and recon (Missing ':')." << endl;
        return map;
    }
    
    map.smear = SmearMatrix(vars_tr, nbins, bins, rt_title, cuts);//the xy projections with preserve the entries are required.
    
    TString tmp_title(rt_title.c_str());
    TString true_title;
    TString reco_title;
    
    if(!tmp_title.Contains(";",TString::kExact)){
        true_title = rt_title.c_str() + "^{true}";
        reco_title = rt_title.c_str() + "^{reco}";
    }
    else{
        true_title = TString( tmp_title(tmp_title.First(";") + 1, tmp_title.Length()) );
        reco_title = TString( tmp_title(0, tmp_title.First(";")) );
    }
    
    //Separate true and rec vars:
    
    TString truth_var( tr_vars(0, tr_vars.First(":")) );
    TString recon_var( tr_vars(tr_vars.First(":") + 1, tr_vars.Length()) );
    
    std::string name = map.smear->GetName();
    map.recon = map.smear->ProjectionX( (name + "_reco").c_str(), 1, map.smear->GetNbinsY());
    map.truth = map.smear->ProjectionY( (name + "_true").c_str(), 1, map.smear->GetNbinsX());
    
    map.recon->GetXaxis()->SetTitle(reco_title.Data());
    map.recon->GetYaxis()->SetTitle("Counts");
    
    map.truth->GetXaxis()->SetTitle(true_title.Data());
    map.truth->GetYaxis()->SetTitle("Counts");
    
    TString tmp_cuts(cuts);
    if(!tmp_cuts.EqualTo("", TString::kExact)) tmp_cuts.Append(" && ");
    
    tmp_cuts.Append(Form("%f < %s && %s < %f && %f < %s && %s < %f ", bins[0], recon_var.Data(), recon_var.Data(), bins[nbins], bins[0], truth_var.Data(), truth_var.Data(), bins[nbins]));
    
    map.ratio = GetRTRatio(vars_tr, reco_title, std::string(tmp_cuts));
    
    return map;
}

KinMap DrawingTools::KinArray(std::string vars_tr, Int_t nbins, Double_t low, Double_t high, std::string rt_title, std::string cuts){
    return KinArray(std::string vars_tr, nbins, SetBinning(nbins, low, high), rt_title, cuts);
}

    TH1D * DrawingTools::GetRTRatio(std::string vars_tr, std::string x_title, const TString cuts){
    
    TString tr_vars(vars_tr.c_str());
    if(!tr_vars.Contains(":")){
        cout << "DrawingTools::GetRTRatio : Error: Could not differentiate between truth and recon (Missing ':')." << endl;
        return 0x0;
    }
    
    //Separate true and rec vars:
    TString truth_var( tr_vars(0, tr_vars.First(":")) );
    TString recon_var( tr_vars(tr_vars.First(":") + 1, tr_vars.Length()) );
    
    m_1Dcounter++;
    TString host_name = Form("h1Dvar%s%.5d", m_uniquename.c_str(), m_1Dcounter);
    
    TString tmp_title = "1 - reco/true";
    
    if(x_title.Contains(";",TString::kExact)){
        tmp_title += " (" + TString(x_title) + ")";
    }
    else tmp_title += " (" + TString(x_title) + ");Counts";
    
    TH1D * ratio = new TH1D(host_name.Data(), Form(";%s", tmp_title.Data()), m_ratiobins, -m_ratiorange, m_ratiorange);
    
    TString var = "1 - ( (" + recon_var + ") / (" + truth_var + ") )";
    //cout << var.Data() << endl;
    //cout << cuts.Data() << endl;
    
    m_tree->Project(host_name.Data(), var.Data(), cuts.c_str());
    
    if(m_verbose) cout << "RTRatio : Integral = " << ratio->Integral() << endl;
    
    return ratio;
}

//Private functions:
Double_t * DrawingTools::SetBinning(int nbins, Double_t low, Double_t high){

    Double_t * bins = new Double_t[ nbins + 1 ];
    
    Double_t range = high - low;
    Double_t binwidth = range/(Double_t)nbins;
    
    for (int i=0; i < nbins + 1; i++) {
        bins[i] = low + binwidth*i;
        if(m_verbose) cout << "Array Element: " << i << " : " << bins[i] << endl;
    }
    
    return bins;
}