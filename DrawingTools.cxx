#include "DrawingTools.h"

#include "assert.h"
//ROOT Includes:

//Forward declarations;
#include "TFile.h"
#include "TTree.h"
#include "TLegend.h"
#include "TLeaf.h"
#include "TH1D.h"
#include "TH2D.h"

using namespace std;

DrawingTools::DrawingTools(TString filename, bool debug, TString reconame, TString truename) {
    
    cout << "DrawingTools::DrawingTools(TString filename, TString reconame, TString truename)" << endl;
    
    SetFileName(filename);
    SetFile();

    SetRecoBranch(reconame);
    SetTrueBranch(truename);

    cout << "    Filename: " << _filename.Data() << endl;
    cout << "Truth branch: " << _truename.Data() << endl;
    cout << "Recon branch: " << _reconame.Data() << endl;
    
    SetTrees();
    
    _POT = -999.;
    SetPOT();
    
    //Include counter to make sure hists have unique names:
    _1Dcounter = -1;
    _2Dcounter = -1;
    
    _DEBUG_ = debug;
    
    _ratiorange = 3.0;
    _ratiobins = 61;
}

DrawingTools::DrawingTools(){
    _1Dcounter = -1;
    _2Dcounter = -1;
    _POT = -999.;
    _DEBUG_ = false;
    _ratiorange = 3.0;
    _ratiobins = 10;
}

void DrawingTools::SetFile(){
    _file = new TFile(_filename, "READ");
    
    if(_file->IsZombie()){
        cout << "DrawingTools::SetFile() : Couldn't set file." << endl;
        exit(0);
    }
}

void DrawingTools::SetTrees(){
    
    bool should_exit = false;
    
    if(_reconame.EqualTo("",TString::kExact)){
        cout << "Reco Tree name not defined" << endl;
        should_exit = true;
    }
    else{
        if(_recotree){
            cout << "Probably do something here" << endl;
        }
        _recotree = (TTree*)_file->Get(_reconame.Data());
    }
    
    if(_truename.EqualTo("",TString::kExact)){
        cout << "True Tree name not defined"<< endl;
        should_exit = true;
    }
    else{
        if(_truetree){
            cout << "Probably do something here" << endl;
        }
        
        _truetree = (TTree*)_file->Get(_truename.Data());
    }
    
    _metatree = (TTree*)_file->Get("Meta");
    
    if(should_exit){
        cout << "Missing Tree names... Exiting" << endl;
        exit(0);
    }
    
}

TH1D * DrawingTools::GetHisto(TTree * intree, const TString var, int nbins, const double x_low, const double x_high, const TString xy_title, const TString cuts){
    if(_DEBUG_) cout << "DrawingTools::GetHisto(TTree * intree, const TString var, int nbins, const double x_low, const double x_high, const TString xy_title, const TString cuts)" << endl;

    Double_t * xbins = new Double_t[ nbins + 1 ];
    
    Double_t range = x_high - x_low;
    Double_t binwidth = range/(Double_t)nbins;
    
    for (int i=0; i < nbins + 1; i++) {
        xbins[i] = x_low + binwidth*i;
        //cout << "Array Element: " << i << " : " << xbins[i] << endl;
    }
    
    return GetHisto(intree, var, nbins, xbins, xy_title, cuts);
}

TH1D * DrawingTools::GetHisto(TTree * intree, const TString var, int nbins, const Double_t * xbins, const TString xy_title, const TString cuts){
    if(_DEBUG_) cout << "DrawingTools::GetHisto(TTree * intree, const TString var, int nbins, const Double_t * xbins, const TString xy_title, const TString cuts)" << endl;
    
    _1Dcounter++;
    TString host_name = Form("h1Dvar%.3d", _1Dcounter);
    
    TString tmp_title;
    if(xy_title.Contains(";",TString::kExact)){
        tmp_title = xy_title;
    }
    else tmp_title = xy_title + ";Counts";
    
    TH1D * hist = new TH1D(host_name.Data(), Form(";%s", tmp_title.Data()), nbins, xbins);
    //hist->GetYaxis()->Setmaxdigits(2);
    //hist->GetXaxis()->Setmaxdigits(2);
    
    TString tmp_cuts = cuts.Data();
    if(!tmp_cuts.EqualTo("", TString::kExact)) tmp_cuts.Append(" && ");
    
    tmp_cuts.Append(Form("(%s != -999)", var.Data()));//Might get rid of this.
    
    //cout << tmp_cuts.Data() << endl;
    
    intree->Project(host_name.Data(), var.Data(), tmp_cuts.Data());
    
    //cout << "Histo: " << hist->GetName() << " : Entries = " << hist->Integral() << endl;
    return hist;
}

TH2D * DrawingTools::GetHisto(TTree * intree, const TString vars_yx, int x_nbins, const double x_low, const double x_high, int y_nbins, const double y_low, const double y_high, const TString xy_title, const TString cuts){
    
    Double_t * xbins = new Double_t[ x_nbins + 1 ];
    Double_t x_range = x_high - x_low;
    Double_t x_binwidth = x_range/(Double_t)x_nbins;
    
    Double_t * ybins = new Double_t[ y_nbins + 1 ];
    Double_t y_range = y_high - y_low;
    Double_t y_binwidth = y_range/(Double_t)y_nbins;
    
    int loop = x_nbins;
    if(loop < y_nbins) loop = y_nbins;
    
    for (int i=0; i < loop + 1; i++){
        
        if(_DEBUG_) cout << "Array Element: " << i;
        if(i < x_nbins + 1 ){
            xbins[i] = x_low + x_binwidth*i;
            if(_DEBUG_) cout << " X[" << i << "] = "<< xbins[i];
        }
        else{
            if(_DEBUG_) cout << " X[" << i << "] = NULL";
        }
        
        if(i < y_nbins + 1 ){
            ybins[i] = y_low + y_binwidth*i;
            if(_DEBUG_)cout << " Y[" << i << "] = " << ybins[i];
        }
        else{
            if(_DEBUG_) cout << " Y[" << i << "] = NULL";
        }
        if(_DEBUG_) cout << endl;
    }
    
    return GetHisto(intree, vars_yx, x_nbins, xbins, y_nbins, ybins, xy_title, cuts);
}

TH2D * DrawingTools::GetHisto(TTree * intree, const TString vars_yx, int x_nbins, const Double_t * xbins, int y_nbins, const Double_t * ybins, const TString xy_title, const TString cuts){
    
    _2Dcounter++;
    TString host_name = Form("h2Dvar%.3d", _2Dcounter);
    
    TH2D * hist = new TH2D(host_name.Data(), Form(";%s", xy_title.Data()), x_nbins, xbins, y_nbins, ybins);
    //hist->GetYaxis()->Setmaxdigits(2);
    //hist->GetXaxis()->Setmaxdigits(2);
    
    //Separate true and rec vars:
    TString y_var( vars_yx(0, vars_yx.First(":")) );
    TString x_var( vars_yx(vars_yx.First(":") + 1, vars_yx.Length()) );
    
    TString tmp_cuts = cuts.Data();
    if(!tmp_cuts.EqualTo("", TString::kExact)) tmp_cuts.Append(" && ");
    tmp_cuts.Append(Form("(%s != -999) && (%s != -999)", y_var.Data(), x_var.Data()));
    if(_DEBUG_) cout << tmp_cuts.Data() << endl;
    
    intree->Project(host_name.Data(), vars_yx.Data(), tmp_cuts.Data());
    
    //cout << "Histo: " << hist->GetName() << " : Entries = " << hist->Integral() << endl;
    return hist;
}

TH2D * DrawingTools::SmearMatrix(TTree * intree, const TString vars_yx, int nbins, const double low, const double high, const TString xy_title, const TString cuts){
   
    Double_t * bins = new Double_t[ nbins + 1 ];
    Double_t range = high - low;
    Double_t binwidth = range/(Double_t)nbins;

    for (int i=0; i < nbins + 1; i++){
        
        if(_DEBUG_)cout << "Array Element: " << i;
        if(i < nbins + 1 ){
            bins[i] = low + binwidth*i;
            if(_DEBUG_) cout << " X[" << i << "] = "<< bins[i];
        }
        if(_DEBUG_) cout << endl;
    }
    
    return SmearMatrix(intree, vars_yx, nbins, bins, xy_title, cuts);
}

TH2D * DrawingTools::SmearMatrix(TTree * intree, const TString vars_yx, int nbins, const Double_t * bins, const TString xy_title, const TString cuts){
    return GetHisto(intree, vars_yx, nbins, bins, nbins, bins, xy_title, cuts);
}

KinMap DrawingTools::KinArray(TTree * intree, const TString vars_tr, int nbins, const double low, const double high, const TString rt_title, const TString cuts, bool cor){
    if(_DEBUG_) cout << "DrawingTools::KinArray(TTree * intree, const TString vars_yx, int nbins, const double low, const double high, const TString xy_title, const TString cuts)" << endl;
    return KinArray(intree, vars_tr, nbins, low, high, nbins, low, high, rt_title, cuts, cor);
}

KinMap DrawingTools::KinArray(TTree * intree, const TString vars_tr, int nbins, const Double_t * bins, const TString rt_title, const TString cuts, bool cor){
    if(_DEBUG_) cout << "DrawingTools::KinArray(TTree * intree, const TString vars_tr, int nbins, const Double_t * bins, const TString xy_title, const TString cuts)" << endl;
    return KinArray(intree, vars_tr, nbins, bins, nbins, bins, rt_title, cuts, cor);
}

KinMap DrawingTools::KinArray(TTree * intree, const TString vars_tr, int re_nbins, const double re_low, const double re_high, int tr_nbins, const double tr_low, const double tr_high, const TString rt_title, const TString cuts, bool cor){
    if(_DEBUG_) cout << "DrawingTools::KinArray(TTree * intree, const TString vars_tr, int re_nbins, const double re_low, const double re_high, int tr_nbins, const double tr_low, const double tr_high, const TString rt_title, const TString cuts, bool cor)" << endl;
    
    Double_t * re_bins = new Double_t[ re_nbins + 1 ];
    Double_t re_range = re_high - re_low;
    Double_t re_binwidth = re_range/(Double_t)re_nbins;
    
    Double_t * tr_bins = new Double_t[ tr_nbins + 1 ];
    Double_t tr_range = tr_high - tr_low;
    Double_t tr_binwidth = tr_range/(Double_t)tr_nbins;
    
    int loop = re_nbins;
    if(loop < tr_nbins) loop = tr_nbins;
    
    for (int i=0; i < loop + 1; i++){
        
        if(_DEBUG_) cout << "Array Element: " << i;
        if(i < re_nbins + 1 ){
            re_bins[i] = re_low + re_binwidth*i;
            if(_DEBUG_) cout << " X[" << i << "] = "<< re_bins[i];
        }
        else{
            if(_DEBUG_) cout << " X[" << i << "] = NULL";
        }
        
        if(i < tr_nbins + 1 ){
            tr_bins[i] = tr_low + tr_binwidth*i;
            if(_DEBUG_)cout << " Y[" << i << "] = " << tr_bins[i];
        }
        else{
            if(_DEBUG_) cout << " Y[" << i << "] = NULL";
        }
        if(_DEBUG_) cout << endl;
    }
    
    return KinArray(intree, vars_tr, re_nbins, re_bins, tr_nbins, tr_bins, rt_title, cuts, cor);
}

KinMap DrawingTools::KinArray(TTree * intree, const TString vars_tr, int re_nbins, const Double_t * re_bins, int tr_nbins, const Double_t * tr_bins, const TString rt_title, const TString cuts, bool cor){
    //In this map we can to keep all the entries preserved from one plot to another, i.e. all integrals are the same. Therefore want a common cut for all plots.
    if(_DEBUG_) cout << "DrawingTools::KinArray(TTree * intree, const TString vars_tr, int re_nbins, const Double_t * re_bins, int tr_nbins, const Double_t * tr_bins, const TString xy_title, const TString cuts)" << endl;
    
    KinMap map;
    map.smear = SmearMatrix(intree, vars_tr, re_nbins, re_bins, tr_nbins, tr_bins, rt_title, cuts);//the xy projections with preserve the entries are required.
    
    //Replace : with / in vars_yx Split up the vars. using tstring and then
    TString tmp_cuts = cuts.Data();
    
    TString true_title;
    TString reco_title;
    
    if(!rt_title.Contains(";",TString::kExact)){
        true_title = "true " + rt_title;
        reco_title = "reco " + rt_title;
    }
    else{
        true_title = TString( rt_title(rt_title.First(";") + 1, rt_title.Length()) );
        reco_title = TString( rt_title(0, rt_title.First(";")) );
    }
    
    //Separate true and rec vars:
    TString truth_var( vars_tr(0, vars_tr.First(":")) );
    TString recon_var( vars_tr(vars_tr.First(":") + 1, vars_tr.Length()) );
    
    if(cor){//Default is to preserve the number of events in each kinematic histo.
        string name = map.smear->GetName();
        map.recon = map.smear->ProjectionX( (name + "_reco").c_str(), 1, map.smear->GetNbinsY());
        map.truth = map.smear->ProjectionY( (name + "_true").c_str(), 1, map.smear->GetNbinsX());
        
        map.recon->GetXaxis()->SetTitle(reco_title.Data());
        map.recon->GetYaxis()->SetTitle("Counts");
        
        map.truth->GetXaxis()->SetTitle(true_title.Data());
        map.truth->GetYaxis()->SetTitle("Counts");
        
        if(!tmp_cuts.EqualTo("", TString::kExact)) tmp_cuts.Append(" && ");
        tmp_cuts.Append(Form("%f < %s && %s < %f && %f < %s && %s < %f ", re_bins[0], recon_var.Data(), recon_var.Data(), re_bins[re_nbins], tr_bins[0],
                             truth_var.Data(), truth_var.Data(), tr_bins[tr_nbins]));
        
        //cout << tmp_cuts << endl;
    }
    else{
        map.truth = GetHisto(intree, truth_var, tr_nbins, tr_bins, true_title, cuts);
        map.recon = GetHisto(intree, recon_var, re_nbins, re_bins, reco_title, cuts);
    }
    
    map.ratio = GetRTRatio(intree, vars_tr, reco_title, tmp_cuts);
    
    return map;
}

TH1D * DrawingTools::GetRTRatio(TTree * intree, const TString vars_tr, const TString x_title, const TString cuts){
    if(_DEBUG_) cout << "DrawingTools::GetRTRatio(TTree * intree, const TString vars_tr, int nbins, const Double_t * bins, const TString xy_title, const TString cuts)" << endl;

    if(!vars_tr.Contains(":")){
        cout << "DrawingTools::GetRTRatio : Error: Could not differentiate between truth and recon." << endl;
        return NULL;
    }

    //Separate true and rec vars:
    TString truth_var( vars_tr(0, vars_tr.First(":")) );
    TString recon_var( vars_tr(vars_tr.First(":") + 1, vars_tr.Length()) );
    
    _1Dcounter++;
    TString host_name = Form("h1Dvar%.3d", _1Dcounter);
    
    TString tmp_title = "1 - reco/true";
    
    if(x_title.Contains(";",TString::kExact)){
            tmp_title += " (" + x_title + ")";
    }
    else tmp_title += " (" + x_title + ");Counts";
    
    TH1D * ratio = new TH1D(host_name.Data(), Form(";%s", tmp_title.Data()), _ratiobins, -_ratiorange, _ratiorange);
    
    cout << "DrawingTools::GetRTRatio" << endl;
    TString var = "1 - ( (" + recon_var + ") / (" + truth_var + ") )";
    //cout << var.Data() << endl;
    //cout << cuts.Data() << endl;
    
    intree->Project(host_name.Data(), var.Data(), cuts.Data());
    
    if(_DEBUG_) cout << "RTRatio : Integral = " << ratio->Integral() << endl;
    
    return ratio;
}

TH1D * DrawingTools::GetTruthHisto(const TString var, int nbins, const double x_low, const double x_high, const TString xy_title, const TString cuts){
    return GetHisto(_truetree, var, nbins, x_low, x_high, xy_title, cuts);
}

TH1D * DrawingTools::GetTruthHisto(const TString var, int nbins, const Double_t * xbins, const TString xy_title, const TString cuts){
    return GetHisto(_truetree, var, nbins, xbins, xy_title, cuts);
}

TH1D * DrawingTools::GetRecoHisto(const TString var, int nbins, const double x_low, const double x_high, const TString xy_title, const TString cuts){
    if(_DEBUG_) cout << "DrawingTools::GetRecoHisto(const TString var, int nbins, const double x_low, const double x_high, const TString xy_title, const TString cuts)" << endl;
    return GetHisto(_recotree, var, nbins, x_low, x_high, xy_title, cuts);
}

TH1D * DrawingTools::GetRecoHisto(const TString var, int nbins, const Double_t * xbins, const TString xy_title, const TString cuts){
    return GetHisto(_recotree, var, nbins, xbins, xy_title, cuts);
}

TH2D * DrawingTools::SmearMatrix(const TString vars_yx, int nbins, const double low, const double high, const TString xy_title, const TString cuts){
    return SmearMatrix(_recotree, vars_yx, nbins, low, high, xy_title, cuts);
}

TH2D * DrawingTools::SmearMatrix(const TString vars_yx, int nbins, const Double_t * bins, const TString xy_title, const TString cuts){
    return SmearMatrix(_recotree, vars_yx, nbins, bins, xy_title, cuts);
}

TH2D * DrawingTools::SmearMatrix(TTree * intree, const TString vars_yx, int re_nbins, const double re_low, const double re_high, int tr_nbins, const double tr_low, const double tr_high, const TString xy_title, const TString cuts){
    return GetHisto(intree, vars_yx, re_nbins, re_low, re_high, tr_nbins, tr_low, tr_high, xy_title, cuts);
}

TH2D * DrawingTools::SmearMatrix(TTree * intree, const TString vars_yx, int re_nbins, const Double_t * re_bins, int tr_nbins, const Double_t * tr_bins, const TString xy_title, const TString cuts){
    return GetHisto(intree, vars_yx, re_nbins, re_bins, tr_nbins, tr_bins, xy_title, cuts);
}

TH2D * DrawingTools::SmearMatrix(const TString vars_yx, int re_nbins, const double re_low, const double re_high, int tr_nbins, const double tr_low, const double tr_high, const TString xy_title, const TString cuts){
    return GetHisto(_recotree, vars_yx, re_nbins, re_low, re_high, tr_nbins, tr_low, tr_high, xy_title, cuts);
}

TH2D * DrawingTools::SmearMatrix(const TString vars_yx, int re_nbins, const Double_t * re_bins, int tr_nbins, const Double_t * tr_bins, const TString xy_title, const TString cuts){
    return GetHisto(_recotree, vars_yx, re_nbins, re_bins, tr_nbins, tr_bins, xy_title, cuts);
}

TH1D * DrawingTools::GetRTRatio(const TString vars_tr, const TString x_title, const TString cuts){
    return GetRTRatio(_recotree, vars_tr, x_title, cuts);
}

KinMap DrawingTools::KinArray(const TString vars_tr, int nbins, const double low, const double high, const TString rt_title, const TString cuts, bool cor){
    return KinArray(_recotree, vars_tr, nbins, low, high, rt_title, cuts, cor);
}

KinMap DrawingTools::KinArray(const TString vars_tr, int nbins, const Double_t * bins, const TString rt_title, const TString cuts, bool cor){
    return KinArray(_recotree, vars_tr, nbins, bins, rt_title, cuts, cor);
}

KinMap DrawingTools::KinArray(const TString vars_tr, int re_nbins, const double re_low, const double re_high, int tr_nbins, const double tr_low, const double tr_high, const TString rt_title, const TString cuts, bool cor){
    return KinArray(_recotree, vars_tr, re_nbins, re_low, re_high, tr_nbins, tr_low, tr_high, rt_title, cuts, cor);
}
KinMap DrawingTools::KinArray(const TString vars_tr, int re_nbins, const Double_t * re_bins, int tr_nbins, const Double_t * tr_bins, const TString rt_title, const TString cuts, bool cor){
    return KinArray(_recotree, vars_tr, re_nbins, re_bins, tr_nbins, tr_bins, rt_title, cuts, cor);
}

TLegend * DrawingTools::GetPOT(double x_pos, double y_pos, TString filename){
    
    SetPOT(filename);
    
    double x_size = 0.3;
    double y_size = 0.1;
    //Legend(double x_size, double y_size, double x_start, double y_start)
    TLegend * pot = Legend(x_size, y_size, x_pos, y_pos);
    pot->AddEntry((TObject*)0, Form(" %.2e POT", _POT),"");
    //pot->SetFillStyle(0);
    pot->SetTextSize(0.042);
    return pot;
}

/*void DrawingTools::ColFill(MnvH1D_ *&h1, int fill_color, int line_color){
    h1->SetFillColor(fill_color);
    h1->SetLineColor(line_color);
}*/

void DrawingTools::ColFill(TH1D *&h1, int fill_color, int line_color){
    h1->SetFillColor(fill_color);
    h1->SetLineColor(line_color);
}

void DrawingTools::SetPOT(TString filename){
    
    if(!filename.EqualTo("",TString::kExact)){
        if(_POT == -999.0){
            TFile * tmp_file = new TFile(filename.Data(), "READ");
            if(!tmp_file->IsZombie()){
                TTree * tmp_meta_tree = (TTree*)tmp_file->Get("Meta");
                double POT_Used = 0;
                assert(tmp_meta_tree->GetEntries()==1);
                tmp_meta_tree->GetEntry(0);
                TLeaf * lpot= tmp_meta_tree->GetLeaf("POT_Used");
                if(lpot) POT_Used = lpot->GetValue();
                _POT = POT_Used;
            }
        }
        else{
            if(_DEBUG_) cout << "POT already set " << _POT << endl;
        }
    }
    else{
        if(_POT == -999.0){
            double POT_Used = 0;
            assert(_metatree->GetEntries()==1);
            _metatree->GetEntry(0);
            TLeaf * lpot= _metatree->GetLeaf("POT_Used");
            if(lpot) POT_Used = lpot->GetValue();
            _POT = POT_Used;
        }
        else{
            if(_DEBUG_) cout << "POT already set as " << _POT << endl;
        }
    }
}

std::vector<double> DrawingTools::GetPercentage(std::vector<TH1D*> histos){

    std::vector<double> pers;
    double norm = 0;
    for(int i = 0; i < (int)histos.size(); i++){
        double integral = histos[i]->Integral();
        if(_DEBUG_) cout << "Histogram: " << histos[i]->GetName() << " Integtral = " << integral << endl;
        norm += integral;
        pers.push_back( integral );
    }
    
    if(_DEBUG_) cout << "Norm = " << norm << endl;
    
    for(int i = 0; i < (int)pers.size(); i++) pers[i] *= 100/norm;
    
    return pers;
}

TLegend * DrawingTools::Legend(double x_size, double y_size, double x_start, double y_start){
    TLegend * leg = new TLegend(x_start, y_start, x_start + x_size, y_start + y_size);
    leg->SetFillStyle(0);
    //leg->SetTextSize(0.042);
    return leg;
}


