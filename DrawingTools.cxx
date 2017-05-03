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
#include "TMath.h"
#include "DataClasses.h"
#include "THStack.h"
#include "TList.h"
#include "TCanvas.h"
#include "TObject.h"

using namespace std;

DrawingTools::DrawingTools(std::string filename, std::string treename, std::string uniquename) : m_filename(filename), m_treename(treename), m_uniquename(uniquename), m_weight("1"), m_verbose(print_level::drawingtools), m_bad_POT(true) {
    
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
    
    empty_kinmap.recon = 0x0;
    empty_kinmap.truth = 0x0;
    empty_kinmap.ratio = 0x0;
    empty_kinmap.smear = 0x0;
    
    SetPOT();
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
    m_tree->Project(host_name.Data(), var.c_str(), (m_weight + "*(" + cuts + ")").c_str());
    
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
    
    m_tree->Project(host_name.Data(), var_yx.c_str(), (m_weight + "*(" + cuts + ")").c_str());
    
    return hist;
}

TH2D * DrawingTools::GetHisto(std::string var_yx, Int_t x_nbins, Double_t x_low, Double_t x_high, Int_t y_nbins, Double_t y_low, Double_t y_high, std::string xy_title, std::string cuts){
    return GetHisto(var_yx, x_nbins, SetBinning(x_nbins, x_low, x_high), y_nbins, SetBinning(y_nbins, y_low, y_high), xy_title, cuts);
}

TH2D * DrawingTools::SmearMatrix(std::string vars_yx, Int_t nbins, Double_t * bins, std::string xy_title, std::string cuts){
//    cout << "Pre: xy_title = " << xy_title << endl;
    if(xy_title.find(";") == std::string::npos){
        xy_title = Form("%s Reco.;%s Truth", xy_title.c_str(), xy_title.c_str());
    }
//    cout << "Post: xy_title = " << xy_title << endl;
    return GetHisto(vars_yx, nbins, bins, nbins, bins, xy_title, cuts);
}

TH2D * DrawingTools::SmearMatrix(std::string vars_yx, Int_t nbins, Double_t low, Double_t high, std::string xy_title, std::string cuts){
    return GetHisto(vars_yx, nbins, SetBinning(nbins, low, high), nbins, SetBinning(nbins, low, high), xy_title, cuts);
}

DrawingTools::KinMap DrawingTools::KinArray(std::string vars_tr, Int_t nbins, Double_t * bins, std::string rt_title, std::string cuts){
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
        true_title = TString(rt_title) + "^{true}";
        reco_title = TString(rt_title) + "^{reco}";
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
    
    map.ratio = GetRTRatio(vars_tr, std::string(reco_title), std::string(tmp_cuts));
    
    return map;
}

DrawingTools::KinMap DrawingTools::KinArray(std::string vars_tr, Int_t nbins, Double_t low, Double_t high, std::string rt_title, std::string cuts){
    return KinArray(vars_tr, nbins, SetBinning(nbins, low, high), rt_title, cuts);
}

TH1D * DrawingTools::GetRTRatio(std::string vars_tr, std::string x_title, std::string cuts){
    
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
    
    if(TString(x_title).Contains(";",TString::kExact)){
        tmp_title += " (" + TString(x_title) + ")";
    }
    else tmp_title += " (" + TString(x_title) + ");Counts";
    
    TH1D * ratio = new TH1D(host_name.Data(), Form(";%s", tmp_title.Data()), m_ratiobins, -m_ratiorange, m_ratiorange);
    
    TString var = "1 - ( (" + recon_var + ") / (" + truth_var + ") )";
    //cout << var.Data() << endl;
    //cout << cuts.Data() << endl;
    
    m_tree->Project(host_name.Data(), var.Data(), (m_weight + "*(" + cuts + ")").c_str());//(m_weight + "*(" + cuts + ")").c_str() <-- Don't know if this is needed.
    
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
        // if(m_verbose) cout << "Array Element: " << i << " : " << bins[i] << endl;
    }
    
    return bins;
}

TLegend * DrawingTools::Legend(Double_t x_size, Double_t y_size, Double_t x_start, Double_t y_start){
    TLegend * leg = new TLegend(x_start, y_start, x_start + x_size, y_start + y_size);
    leg->SetFillStyle(0);
    //leg->SetTextSize(0.042);
    return leg;
}

void DrawingTools::SetColors(TH1D *&h1, Int_t fill_color, Int_t line_color, Int_t fill_style, Int_t line_style){
    
//    cout << "FillColor = " << fill_color << " FillSyle = " << fill_style << " LineColor = " << line_color << " LineStyle = " << line_style << endl;
//    cout << "H1 = FillColor = " << h1->GetFillColor() << " FillSyle = " << h1->GetFillStyle() << " LineColor = " << h1->GetLineColor() << " LineStyle = " << h1->GetLineStyle() << endl;
    
    h1->SetFillColor(fill_color);
    h1->SetLineColor(line_color);
    
    if(fill_style != -999) h1->SetFillStyle(fill_style);
    if(line_style != -999) h1->SetLineStyle(line_style);
}

void DrawingTools::SetColors(KinMap &map, Int_t fill_color, Int_t line_color, Int_t fill_style, Int_t line_style){
    SetColors(map.recon, fill_color, line_color, fill_style, line_style);
    SetColors(map.truth, fill_color, line_color, fill_style, line_style);
    SetColors(map.ratio, fill_color, line_color, fill_style, line_style);
}

std::vector<double> DrawingTools::GetPercentage(std::vector<TH1D*> histos){
    
    std::vector<double> pers;
    double norm = 0;
    for(int i = 0; i < (int)histos.size(); i++){
        double integral = histos[i]->Integral();
        if(m_verbose) cout << "Histogram: " << histos[i]->GetName() << " Integtral = " << integral << endl;
        norm += integral;
        pers.push_back( integral );
    }
    
    if(m_verbose) cout << "Norm = " << norm << endl;
    
    for(int i = 0; i < (int)pers.size(); i++) pers[i] *= 100/norm;
    
    return pers;
}

std::vector<double> DrawingTools::GetPercentage(std::vector<DrawingTools::KinMap> histos, Int_t type){
//    DrawingTools::KinMap empty_kinmap;
    return GetPercentage(histos, type, empty_kinmap);
}

std::vector<double> DrawingTools::GetPercentage(std::vector<DrawingTools::KinMap> histos, Int_t type, KinMap other){
    
    std::vector<TH1D*> list;
    if(type == 0){//Reco;
        if(m_verbose) cout << "GetPercentage : Recon" << endl;
        for (int i = 0; i < (int)histos.size(); i++) list.push_back( histos[i].recon );
        if(other.recon) list.push_back(other.recon);
    }
    else if(type == 1){//Truth;
        if(m_verbose) cout << "GetPercentage : Truth" << endl;
        for (int i = 0; i < (int)histos.size(); i++) list.push_back( histos[i].truth );
        if(other.truth) list.push_back(other.truth);
    }
    else if(type == 2){//ratio;
        if(m_verbose) cout << "GetPercentage : Ratio" << endl;
        for (int i = 0; i < (int)histos.size(); i++) list.push_back( histos[i].ratio );
        if(other.ratio) list.push_back(other.ratio);
    }
    
    std::vector<double> out_list = GetPercentage(list);
    list.clear();
    return out_list;
}

TH1D * DrawingTools::ToPDF(TH1D *hraw, TString hn){
    const Int_t x0 = 0;
    const Int_t x1 = hraw->GetNbinsX()+1;
    const Double_t tmpnt = hraw->Integral(x0, x1);
    
    TH1D * hist = (TH1D*) hraw->Clone((hn+hraw->GetName())+"pdf");
    hist->Scale(0);
    
    for(Int_t ib=x0; ib<=x1; ib++){
        const Double_t bw = hraw->GetBinWidth(ib);
        const Double_t cont = hraw->GetBinContent(ib);
        if(cont<1e-12)
            continue;
        
        //in case of finit number of bins (i.e. eff not always small), Binomial error is more accurate than Poisson error
        const Double_t eff = cont/tmpnt;
        const Double_t pdf = eff/bw;
        
        const Double_t dpdf = TMath::Sqrt(eff*(1-eff)/tmpnt) / bw;
        hist->SetBinContent(ib, pdf);
        hist->SetBinError(ib, dpdf);
    }
    
    hist->SetEntries(tmpnt);
    
    return hist;
}


TH2D * DrawingTools::NormalHist(TH2D *hraw, Double_t thres, bool kmax){
    TH2D *hh=(TH2D*)hraw->Clone(Form("%sSN",hraw->GetName()));
    hh->Scale(0);
    
    const Int_t x0 = hh->GetXaxis()->GetFirst();
    const Int_t x1 = hh->GetXaxis()->GetLast();
    const Int_t y0 = hh->GetYaxis()->GetFirst();
    const Int_t y1 = hh->GetYaxis()->GetLast();
    
    Double_t hmax = -1e10;
    Double_t hmin = 1e10;
    Double_t nent = 0;
    for(Int_t ix=x0; ix<=x1; ix++){
        
        //if option "e" is specified, the errors are computed. if option "o" original axis range of the taget axes will be kept, but only bins inside the selected range will be filled.
        
        TH1D * sliceh = hraw->ProjectionY(Form("tmpnormalhist%sx%d", hh->GetName(), ix), ix, ix, "oe");
        const Double_t tot = sliceh->GetEntries();
        
        TH1D * pdfh=0x0;
        
        if(tot>1e-12){
            nent += tot;
            
            Double_t imax = -999;
            
            if(!kmax){
                pdfh = ToPDF(sliceh,"tmp");
            }
            else{
                imax = sliceh->GetBinContent(sliceh->GetMaximumBin());
            }
            
            for(Int_t iy=y0; iy<=y1; iy++){
                const Double_t cont = kmax ? sliceh->GetBinContent(iy)/imax : pdfh->GetBinContent(iy);
                const Double_t ierr = kmax ? sliceh->GetBinError(iy)/imax   : pdfh->GetBinError(iy);
                if(tot>thres && cont>0){
                    hh->SetBinContent(ix, iy, cont);
                    hh->SetBinError(ix,iy, ierr);
                    if(cont>hmax) hmax = cont;
                    if(cont<hmin) hmin = cont;
                }
            }
        }
        
        delete pdfh;
        delete sliceh;
    }
    
    hh->SetEntries(nent);
    hh->SetMinimum(0.99*hmin);
    hh->SetMaximum(1.1*hmax);
    
    TString xtit(hraw->GetXaxis()->GetTitle()); 
    if(xtit.Contains("(")){
        xtit=xtit(0, xtit.First('('));
    }
    
    TString ytit(hraw->GetYaxis()->GetTitle()); 
    if(ytit.Contains("(")){
        ytit=ytit(0, ytit.First('('));
    }
    
    //hh->SetTitle(Form("f(%s|%s) %s", ytit.Data(), xtit.Data(), hraw->GetTitle()));
    return hh;
}

TLegend * DrawingTools::GetPOT(double x_pos, double y_pos){
    TLegend * pot = Legend(x_pos, y_pos);
    pot->AddEntry((TObject*)0, ( m_bad_POT ? "7.0x10^{21} POT" : Form(" %.2e POT", m_POT) ),"");
    pot->SetTextSize(0.042);
    return pot;
}

void DrawingTools::SetPOT(){
    
    m_bad_POT = true;

    TTree * tmp_POT_tree = (TTree*)m_file->Get("Meta");
    if(tmp_POT_tree){
        assert(tmp_POT_tree->GetEntries()==1);
        tmp_POT_tree->GetEntry(0);
        TLeaf * lpot= tmp_POT_tree->GetLeaf("POT_Used");
        if(lpot) m_POT = lpot->GetValue();
        m_bad_POT = false;
    }

    if(!m_bad_POT){
        tmp_POT_tree = (TTree*)m_file->Get("header");
        if(tmp_POT_tree){
            if(!tmp_POT_tree->FindLeaf("Spill_GoodBeamGoodND280")) m_bad_POT = true;
            else{
                m_POT = 0.;
                double tmp_POT = 0;
                tmp_POT_tree->SetBranchAddress("Spill_GoodBeamGoodND280", &tmp_POT);
                int entries = tmp_POT_tree->GetEntries();

                for(int i = 0; i < entries; i++){
                    tmp_POT_tree->GetEntry(i);
                    m_POT += tmp_POT;
                }
                cout << "POT counted : " << m_POT << endl;
                m_bad_POT = false;
            } 
        }
    }
    
    if(m_bad_POT) cout << "POT is bad" << endl;
}

void DrawingTools::DrawLine(TH1 * histo, double pos, int color){
    TLine * z_line = new TLine(pos, histo->GetMinimum(), pos, histo->GetMaximum());
    z_line->SetLineColor(color);
    z_line->SetLineStyle(2);
    z_line->SetLineWidth(2);
    z_line->Draw(); 
}

TH1D * DrawingTools::GetHistFromStack(THStack * stack)
{

    TList * slist = stack->GetHists();
    TH1D * hfirst = (TH1D*)slist->First()->Clone( (string(slist->First()->GetName()) + "_clone").c_str() );

    TIter next(slist);
    m_1Dcounter++;
    TH1D * sum = new TH1D(Form("%s_sum%.3d", hfirst->GetName(), m_1Dcounter), "", hfirst->GetXaxis()->GetNbins(), hfirst->GetXaxis()->GetXbins()->GetArray());
    TH1D * shist_tmp;
    while ( (shist_tmp = (TH1D*)next()) ) {
        sum->Add(shist_tmp);
    }
    delete hfirst;
    delete shist_tmp;
    return sum;
}

TObject * DrawingTools::GetObjectFromCanvas(TCanvas * can, std::string name)
{
    TObject *obj;
    TIter next(can->GetListOfPrimitives());
    while ((obj=next())) {
        if(print_level::drawingtools) cout << "Reading: " << obj->GetName() << endl;
        if ( obj->InheritsFrom(name.c_str()) ) {
          if(print_level::drawingtools) cout << "Found Object of type " << name << ": " << obj->GetName() << endl;
          return obj->Clone();
        }
    }
    cout << "DrawingTools::GetObjectFromCanvas : Warning could not find any objects of type" << name << endl;
    return 0x0;
}

