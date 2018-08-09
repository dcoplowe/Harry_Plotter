#ifndef __DRAWINGTOOLS__CXX
#define __DRAWINGTOOLS__CXX

#include <DrawingTools.hxx>

#include <assert.h>
//ROOT Includes:
#include <TString.h>
#include <TArrow.h>

//Forward declarations;
#include <TFile.h>
#include <TTree.h>
#include <TLegend.h>
#include <TLeaf.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TMath.h>
// #inclu<de DataClasses.h>
#include <THStack.h>
#include <TList.h>
#include <TCanvas.h>
#include <TObject.h>

using namespace std;

DrawingTools::DrawingTools(std::string filename, std::string treename, std::string uniquename, bool check_pot) : DrawingStyle(), m_filename(filename), m_treename(treename), m_uniquename(uniquename), m_weight("1"),
    m_verbose(false), m_checkPOT(check_pot), m_bad_POT(true), m_linewarning(true) {
    
    m_file = new TFile(m_filename.c_str(), "READ");
    
    if(m_file->IsZombie()){
        cout << "DrawingTools::DrawingTools() : Couldn't set file." << endl;
        exit(0);
    }
    
    m_tree = (TTree*)m_file->Get(m_treename.c_str());

    assert(m_tree);
    
    // From root static constexpr Long64_t   TVirtualTreePlayer::kMaxEntries:
    if(MaxEntries == 80085) MaxEntries = std::numeric_limits<Long64_t>::max();

    m_ratiorange = 3;
    m_ratiobins = 59;
    
    empty_kinmap.recon = 0x0;
    empty_kinmap.truth = 0x0;
    empty_kinmap.ratio = 0x0;
    empty_kinmap.smear = 0x0;
    
    if(m_checkPOT) SetPOT();
}

DrawingTools::~DrawingTools(){
    
    if(m_file->IsOpen()) m_file->Close();
    if(m_tree) delete m_tree;
    if(m_file) delete m_file;
}

TH1D * DrawingTools::GetHisto(std::string var, int nbins, double * bins, std::string xy_title, std::string cuts){
    TString host_name = Form("h1Dvar%s%.5d", m_uniquename.c_str(), s_1Dcounter++);
    
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

    string selection = m_weight;
    if(!cuts.empty()){ 
        selection += "*(";
        selection += cuts;
        selection += ")";
    }

    m_tree->Project(host_name.Data(), var.c_str(), selection.c_str(), "", MaxEntries);
    
    return hist;
}

TH1D * DrawingTools::GetHisto(std::string var, int nbins, double low, double high, std::string xy_title, std::string cuts){
    return GetHisto(var, nbins, SetBinning(nbins, low, high), xy_title, cuts);
}

TH2D * DrawingTools::GetHisto(std::string var_yx, int x_nbins, double * x_bins, int y_nbins, double * y_bins, std::string xy_title, std::string cuts){
    
    s_2Dcounter++;
    TString host_name = Form("h2Dvar%s%.5d", m_uniquename.c_str(), s_2Dcounter);
    
    TH2D * hist = new TH2D(host_name.Data(), Form(";%s", xy_title.c_str()), x_nbins, x_bins, y_nbins, y_bins);

    string selection = m_weight;
    if(!cuts.empty()){ 
        selection += "*(";
        selection += cuts;
        selection += ")";
    }

    // cout << __FILE__ << " : " << selection << endl;
    
    m_tree->Project(host_name.Data(), var_yx.c_str(), selection.c_str());    
    return hist;
}

TH2D * DrawingTools::GetHisto(std::string var_yx, int x_nbins, double x_low, double x_high, int y_nbins, double y_low, double y_high, std::string xy_title, std::string cuts){
    return GetHisto(var_yx, x_nbins, SetBinning(x_nbins, x_low, x_high), y_nbins, SetBinning(y_nbins, y_low, y_high), xy_title, cuts);
}

TH2D * DrawingTools::GetSmear(std::string vars_yx, int nbins, double * bins, std::string xy_title, std::string cuts){
//    cout << "Pre: xy_title = " << xy_title << endl;
    if(xy_title.find(";") == std::string::npos){
        xy_title = Form("Recon %s;Truth %s", xy_title.c_str(), xy_title.c_str());
    }
//    cout << "Post: xy_title = " << xy_title << endl;
    return GetHisto(vars_yx, nbins, bins, nbins, bins, xy_title, cuts);
}

TH2D * DrawingTools::GetSmear(std::string vars_yx, int nbins, double low, double high, std::string xy_title, std::string cuts){
    return GetSmear(vars_yx, nbins, SetBinning(nbins, low, high), xy_title, cuts);
}

TH2D * DrawingTools::GetSmearSN(std::string vars_yx, int nbins, double * bins, std::string xy_title, std::string cuts, double thres, bool kmax){
    TH2D * smear = GetSmear(vars_yx, nbins, bins, xy_title, cuts);
    return NormalHist(smear, thres, kmax);
}

TH2D * DrawingTools::GetSmearSN(std::string vars_yx, int nbins, double low, double high, std::string xy_title, std::string cuts, double thres, bool kmax){
    return GetSmearSN(vars_yx, nbins, SetBinning(nbins, low, high), xy_title, cuts, thres, kmax);
}


DrawingTools::KinMap DrawingTools::KinArray(std::string vars_tr, int nbins, double * bins, std::string rt_title, std::string cuts){
    //In this map we can to keep all the entries preserved from one plot to another, i.e. all integrals are the same. Therefore want a common cut for all plots.
    
    KinMap map;
    
    TString tr_vars(vars_tr.c_str());
    if(!tr_vars.Contains(":")){
        cout << "DrawingTools::GetRTRatio : Error: Could not differentiate between truth and recon (Missing ':')." << endl;
        return map;
    }
    
    map.smear = GetSmear(vars_tr, nbins, bins, rt_title, cuts);//the xy projections with preserve the entries are required.
    map.smearSN = NormalHist(map.smear);

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

DrawingTools::KinMap DrawingTools::KinArray(std::string vars_tr, int nbins, double low, double high, std::string rt_title, std::string cuts){
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
    
    TString host_name = Form("h1Dvar%s%.5d", m_uniquename.c_str(), s_1Dcounter++);
    
    TString tmp_title = "1 - reco/true";
    
    if(TString(x_title).Contains(";",TString::kExact)){
        tmp_title += " (" + TString(x_title) + ")";
    }
    else tmp_title += " (" + TString(x_title) + ");Counts";
    
    TH1D * ratio = new TH1D(host_name.Data(), Form(";%s", tmp_title.Data()), m_ratiobins, -m_ratiorange, m_ratiorange);
    
    TString var = "1 - ( (" + recon_var + ") / (" + truth_var + ") )";
    //cout << var.Data() << endl;
    //cout << cuts.Data() << endl;
    
    m_tree->Project(host_name.Data(), var.Data(), (m_weight + "*(" + cuts + ")").c_str(), "", MaxEntries);//(m_weight + "*(" + cuts + ")").c_str() <-- Don't know if this is needed.
    
    if(m_verbose) cout << "RTRatio : Integral = " << ratio->Integral() << endl;
    
    return ratio;
}

//Private functions:
double * DrawingTools::SetBinning(int nbins, double low, double high){

    double * bins = new double[ nbins + 1 ];
    
    double range = high - low;
    double binwidth = range/(double)nbins;
    
    for (int i=0; i < nbins + 1; i++) {
        bins[i] = low + binwidth*i;
        // if(m_verbose) cout << "Array Element: " << i << " : " << bins[i] << endl;
    }
    
    return bins;
}

TLegend * DrawingTools::Legend(double x_size, double y_size, double x_start, double y_start){
    TLegend * leg = new TLegend(x_start, y_start, x_start + x_size, y_start + y_size);
    leg->SetFillStyle(0);
    //leg->SetTextSize(0.042);
    return leg;
}

void DrawingTools::SetColors(TH1D *&h1, int fill_color, int line_color, int fill_style, int line_style, int line_width)
{
    if(fill_color != -999) h1->SetFillColor(fill_color);
    if(line_color != -999) h1->SetLineColor(line_color);
    if(fill_style != -999) h1->SetFillStyle(fill_style);
    if(line_style != -999) h1->SetLineStyle(line_style);
    if(line_width != -999) h1->SetLineWidth(line_width);
}

void DrawingTools::SetColors(KinMap &map, int fill_color, int line_color, int fill_style, int line_style, int line_width){
    SetColors(map.recon, fill_color, line_color, fill_style, line_style, line_width);
    SetColors(map.truth, fill_color, line_color, fill_style, line_style, line_width);
    SetColors(map.ratio, fill_color, line_color, fill_style, line_style, line_width);
}

std::vector<double> DrawingTools::GetPercentage(std::vector<TH1D*> histos){
    
    std::vector<double> pers;
    double norm = 0;
    for(int i = 0; i < (int)histos.size(); i++){
        double integral = histos[i]->Integral();
        // if(m_verbose) cout << "Histogram: " << histos[i]->GetName() << " Integtral = " << integral << endl;
        norm += integral;
        pers.push_back( integral );
    }    
    // if(m_verbose) cout << "Norm = " << norm << endl;
    for(int i = 0; i < (int)pers.size(); i++) pers[i] *= 100/norm;
    
    return pers;
}

std::vector<double> DrawingTools::GetPercentage(std::vector<DrawingTools::KinMap> histos, int type){
//    DrawingTools::KinMap empty_kinmap;
     std::vector<TH1D*> list;
    if(type == 0){//Reco;
        for (int i = 0; i < (int)histos.size(); i++) list.push_back( histos[i].recon );
    }
    else if(type == 1){//Truth;
        for (int i = 0; i < (int)histos.size(); i++) list.push_back( histos[i].truth );
    }
    else if(type == 2){//ratio;
        for (int i = 0; i < (int)histos.size(); i++) list.push_back( histos[i].ratio );
    }
    
    std::vector<double> out_list = GetPercentage(list);
    list.clear();
    return out_list;
}

std::vector<double> DrawingTools::GetPercentage(std::vector<DrawingTools::KinMap> histos, int type, KinMap other){
    
    std::vector<TH1D*> list;
    if(type == 0){//Reco;
        // if(m_verbose) cout << "GetPercentage : Recon" << endl;
        for (int i = 0; i < (int)histos.size(); i++) list.push_back( histos[i].recon );
        if(other.recon) list.push_back(other.recon);
    }
    else if(type == 1){//Truth;
        // if(m_verbose) cout << "GetPercentage : Truth" << endl;
        for (int i = 0; i < (int)histos.size(); i++) list.push_back( histos[i].truth );
        if(other.truth) list.push_back(other.truth);
    }
    else if(type == 2){//ratio;
        // if(m_verbose) cout << "GetPercentage : Ratio" << endl;
        for (int i = 0; i < (int)histos.size(); i++) list.push_back( histos[i].ratio );
        if(other.ratio) list.push_back(other.ratio);
    }
    
    std::vector<double> out_list = GetPercentage(list);
    list.clear();
    return out_list;
}

TH1D * DrawingTools::ToPDF(TH1D *hraw, TString hn){
    const int x0 = 0;
    const int x1 = hraw->GetNbinsX()+1;
    const double tmpnt = hraw->Integral(x0, x1);
    
    TH1D * hist = (TH1D*) hraw->Clone((hn+hraw->GetName())+"pdf");
    hist->Scale(0);
    
    for(int ib=x0; ib<=x1; ib++){
        const double bw = hraw->GetBinWidth(ib);
        const double cont = hraw->GetBinContent(ib);
        if(cont<1e-12)
            continue;
        
        //in case of finit number of bins (i.e. eff not always small), Binomial error is more accurate than Poisson error
        const double eff = cont/tmpnt;
        const double pdf = eff/bw;
        
        const double dpdf = TMath::Sqrt(eff*(1-eff)/tmpnt) / bw;
        hist->SetBinContent(ib, pdf);
        hist->SetBinError(ib, dpdf);
    }
    
    hist->SetEntries(tmpnt);
    
    return hist;
}


TH2D * DrawingTools::NormalHist(TH2D *hraw, double thres, bool kmax){
    TH2D *hh=(TH2D*)hraw->Clone(Form("%sSN",hraw->GetName()));
    hh->Scale(0);
    
    const int x0 = hh->GetXaxis()->GetFirst();
    const int x1 = hh->GetXaxis()->GetLast();
    const int y0 = hh->GetYaxis()->GetFirst();
    const int y1 = hh->GetYaxis()->GetLast();
    
    double hmax = -1e10;
    double hmin = 1e10;
    double nent = 0;
    for(int ix=x0; ix<=x1; ix++){
        
        //if option "e" is specified, the errors are computed. if option "o" original axis range of the taget axes will be kept, but only bins inside the selected range will be filled.
        
        TH1D * sliceh = hraw->ProjectionY(Form("tmpnormalhist%sx%d", hh->GetName(), ix), ix, ix, "oe");
        const double tot = sliceh->GetEntries();
        
        TH1D * pdfh=0x0;
        
        if(tot>1e-12){
            nent += tot;
            
            double imax = -999;
            
            if(!kmax){
                pdfh = ToPDF(sliceh,"tmp");
            }
            else{
                imax = sliceh->GetBinContent(sliceh->GetMaximumBin());
            }
            
            for(int iy=y0; iy<=y1; iy++){
                const double cont = kmax ? sliceh->GetBinContent(iy)/imax : pdfh->GetBinContent(iy);
                const double ierr = kmax ? sliceh->GetBinError(iy)/imax   : pdfh->GetBinError(iy);
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

void DrawingTools::DrawLine(THStack * stack, double pos, int color, int style, int width)
{
    TH1D * hfirst = GetFirstHistFromStack(stack);
    DrawLine(hfirst, pos, color, style, width);
    delete hfirst;
}

void DrawingTools::DrawLine(TH1 * histo, double pos, int color, int style, int width){
    if(pos > histo->GetXaxis()->GetXmin() && pos < histo->GetXaxis()->GetXmax()) DrawLine(pos, histo->GetMinimum(), pos, histo->GetMaximum(), color, style, width);
    else if(m_linewarning){ 
        m_linewarning = false;
        cout << "DrawingTools::DrawLine : Warning : X position of line out of range : Switching off" << endl; 
    }
}

void DrawingTools::DrawLine(TCanvas * can, double pos, int color, int style, int width)
{
    can->cd();
    cout << "pos = " << pos << endl;
    TLine * z_line = new TLine(pos, 0., pos, 100.);
    z_line->SetLineColor(color);
    z_line->SetLineStyle(style);
    z_line->SetLineWidth(width);
    z_line->Draw();
}

void DrawingTools::DrawLine(double x_low, double y_low, double x_high, double y_high, int color, int style, int width)
{
    TLine * line = new TLine(x_low, y_low, x_high, y_high);
    line->SetLineColor(color);
    line->SetLineStyle(style);
    line->SetLineWidth(width);
    line->Draw();
}

void DrawingTools::DrawBox(double * low, double * high, int color, int style, int width)
{
    DrawLine( low[0],  low[1],  low[0], high[1], color, style, width);
    DrawLine( low[0], high[1], high[0], high[1], color, style, width);
    DrawLine(high[0], high[1], high[0],  low[1], color, style, width);
    DrawLine(high[0],  low[1],  low[0],  low[1], color, style, width);
}

TH1D * DrawingTools::GetHistFromStack(THStack * stack)
{
    // TODO: Fix problem with rebinning hists -3 - 3 -> 0 - 1
    TH1D * hfirst = GetFirstHistFromStack(stack);
    // cout << "hfirst hfirst hfirst: Bins = " << hfirst->GetXaxis()->GetNbins() << " Low " <<
        // hfirst->GetXaxis()->GetXmin() << " High " << hfirst->GetXaxis()->GetXmax() << endl;

    TList * slist = stack->GetHists();
    TIter next(slist);
    TH1D * sum = new TH1D(Form("%s_sum%.3d", hfirst->GetName(), s_1Dcounter++), "", 
        hfirst->GetXaxis()->GetNbins(), hfirst->GetXaxis()->GetXbins()->GetArray());

    // cout << "sum sum sum: Bins = " << sum->GetXaxis()->GetNbins() << " Low " << sum->GetXaxis()->GetXmin() << " High " << sum->GetXaxis()->GetXmax() << endl;

    TH1D * shist_tmp;
    while ( (shist_tmp = (TH1D*)next()) ) {
        // cout << "DrawingTools::GetHistFromStack : Adding Hist" << endl;
        // cout << "shist_tmp: Low " << shist_tmp->GetXaxis()->GetXmin() << " High " << shist_tmp->GetXaxis()->GetXmax() << endl;
        sum->Add(shist_tmp);
    }
    delete hfirst;
    delete shist_tmp;
    return sum;
}

TH1D * DrawingTools::GetFirstHistFromStack(THStack * stack)
{
    TList * slist = stack->GetHists();
    return (TH1D*)slist->First()->Clone( (string(slist->First()->GetName()) + "_clone").c_str() );
}

TObject * DrawingTools::GetObjectFromCanvas(TCanvas * can, std::string name)
{
    TObject *obj;
    TIter next(can->GetListOfPrimitives());
    while ((obj=next())) {
        if(m_verbose) cout << "Reading: " << obj->GetName() << endl;
        if ( obj->InheritsFrom(name.c_str()) ) {
          if(m_verbose) cout << "Found Object of type " << name << ": " << obj->GetName() << endl;
          return obj->Clone();
        }
    }
    cout << "DrawingTools::GetObjectFromCanvas : Warning could not find any objects of type" << name << endl;
    return 0x0;
}

void DrawingTools::DrawCutLine(double xpos, DrawingTools::LineDirection dir, double arrowpos, int color, int style, int width)
{
    DrawLine(xpos, (double)gPad->PadtoY(gPad->GetUymin()), xpos, (double)gPad->PadtoY(gPad->GetUymax()), color, style, width);

    if (dir != DrawingTools::Unknown) {
        double deltax = xpos-gPad->PadtoX( gPad->XtoPad(xpos)-(gPad->GetUxmax()-gPad->GetUxmin())/20.);
        if (dir == DrawingTools::Left) deltax *= -1;

            // It needs this to work in LogY scale
        double yarr = gPad->PadtoY((gPad->GetUymax()-gPad->GetUymin())*arrowpos+gPad->GetUymin());
        double xarrmax = xpos + deltax;
        DrawCutArrow(xpos, yarr, xarrmax, yarr, color, style, width);
    }
}

void DrawingTools::DrawCutArrow(double xmin, double ymin, double xmax, double ymax, int color, int style, int width)
{
    TArrow* arr = new TArrow(xmin, ymin, xmax, ymax, 0.02, "|>");
    arr->SetLineColor(color);
    arr->SetFillColor(color);
    arr->SetLineWidth(width);
    arr->Draw();
}

void DrawingTools::Normalise(TH1D *& hist)
{
    hist->Scale( (1./(double)hist->Integral(1, hist->GetNbinsX() ) ) ); 
}

void DrawingTools::Normalise(TH2D *& hist)
{
    hist->Scale( (1./(double)hist->Integral(1, hist->GetNbinsX(), 1, hist->GetNbinsY() ) ) ); 
}

// If -999 is the presision use error as stat error.
double * DrawingTools::GetOptBinning(const std::string &var_name, const int x_nbins, const double x_min, const double x_max,
    const std::string &cuts = "", const double precision = -999.)
{
    return GetOptBinning(m_tree, var_name, x_nbins, x_min, x_max, cuts, precision);
}

int DrawingTools::GetEntriesInRange(const std::string &var_name, const double x_min, const double x_max,  const std::string &cuts)
{
    return GetEntriesInRange(m_tree, cvar_name, x_min, x_max, cuts);
}

double * DrawingTools::GetOptBinning(TTree * intree, const std::string &var_name, const int x_nbins, const double x_min, const double x_max,
    const string &cuts, const double precision)
{
    // Default presision should be within sqrt( N(x_high - x_low)/nbins )

    double * binning = new double [ x_nbins + 1 ];
    binning[ 0 ] = x_min;
    binning[ x_nbins ] = x_max;

    int integral = GetEntriesInRange(intree, var_name, x_min, x_max, cuts);
    int dentry = (int)integral/x_nbins;
    double ave_bin = (double)(x_max - x_min)/x_nbins;

    double in_precision = precision;
    if(precision == -999.) in_precision = TMath::Sqrt((double)dentry)/(double)dentry;

    double * tot_bin = new double [ x_nbins ];

    for(int i = 1; i < x_nbins; i++){
        // first using starting point:
        double start = x_min + i*ave_bin;
        // cout << "Finding element " << i << ": Using " << start << " as initial value." << endl;
        double low = binning[ (i - 1) ];

        int entries = GetEntriesInRange(intree, var_name, low, start, cuts);
        double delta = (double)(1. - (double)entries/(double)dentry);
        // cout << "entries/dentry = " << entries << "/" << dentry << " Starting delta = " << delta << endl;
        double value = start;
        double sign = (delta < 0.) ? -1. : 1.;
        if(TMath::Abs(delta) > in_precision){
            // This could/should be made more efficient.
            for(int m = 0; m < 999; m++){
                        // This is good up to in stats of 1e6.
                value = start*(1. + sign*m*0.001);
                entries = GetEntriesInRange(intree, var_name, low, value, cuts);
                double delta = (double)(1. - (double)entries/(double)dentry);
                // cout << "For " << low << " <= " << var_name << " <= " << value << " : " << entries << " (" << dentry << ") Delta = " << delta << endl;
                // Add a factor of two for the fact that we
                if(TMath::Abs(delta) < (in_precision/2.) ) break;
            }
        }
        // cout << "Best bin value found: " << value << endl;
        binning[i] = value;
        tot_bin[(i -1)] = entries;
    }

    tot_bin[ (x_nbins - 1 ) ] = GetEntriesInRange(intree, var_name, binning[ x_nbins - 1 ], x_max, cuts); 

    cout << "********* Finished Binning *********" << endl;
    cout << "   For " << x_nbins << "bins : Ave. Binning = " << dentry << " p/m " << (int)(dentry*in_precision) << endl;

    for(int i = 0; i < x_nbins + 1; i++){
        cout << var_name << "[" << i << "] = " << binning[i] << " ";
        if(i < x_nbins) cout << tot_bin[i];
        cout << endl;
    }
    cout << "************************" << endl;

    return binning;
}

int DrawingTools::GetEntriesInRange(TTree * tree, const std::string &var_name, const double x_min, const double x_max, const std::string &cuts)
{
    string basecuts = cuts; 
    if(!cuts.empty()) basecuts += " && ";
    else basecuts = "";
    string sel = Form("%s%f <= %s && %s <= %f", basecuts.c_str(), x_min, var_name.c_str(), var_name.c_str(), x_max);
    return (int)tree->Draw(var_name.c_str(), sel.c_str() , "goff");
}

#endif