#include "DrawingTools.h"

//ROOT Includes:

//Forward declarations;
#include "TFile.h"
#include "TTree.h"
#include "TLegend.h"
#include "TLeaf.h"

using namespace std;

DrawingTools::DrawingTools(TString filename, TString reconame, TString truename) {
    
    cout << "DrawingTools::DrawingTools(TString filename, TString reconame, TString truename)" << endl;
    
    SetFileName(filename);
    SetRecoBranch(reconame);
    SetTrueBranch(truename);
    SetFile();
    
    cout << "    Filename: " << _filename.Data() << endl;
    cout << "Truth branch: " << _truename.Data() << endl;
    cout << "Recon branch: " << _reconame.Data() << endl;
    
    //Include counter to make sure hists have unique names:
    _1Dcounter = -1;
    _2Dcounter = -1;
}

DrawingTools::DrawingTools(){
    _1Dcounter = -1;
    _2Dcounter = -1;
}

void DrawingTools::SetFile(){
    _file = new TFile(_filename, "READ");
    
    if(_file->IsZombie()){
        cout << "DrawingTools::SetFile() : Couldn't set file." << endl;
        exit(0);
    }
}

MnvH1D * DrawingTools::GetHisto(TTree * intree, const TString var, int nbins, const double x_low, const double x_high, const TString xy_title, const TString cuts){
    
    Double_t * xbins = new Double_t[ nbins + 1 ];
    
    Double_t range = x_high - x_low;
    Double_t binwidth = range/(Double_t)nbins;
    
    for (int i=0; i < nbins + 1; i++) {
        xbins[i] = x_low + binwidth*i;
        //cout << "Array Element: " << i << " : " << xbins[i] << endl;
    }
    
    return GetHisto(intree, var, nbins, xbins, xy_title, cuts);
}

MnvH1D * DrawingTools::GetHisto(TTree * intree, const TString var, int nbins, const Double_t * xbins, const TString xy_title, const TString cuts){
    _1Dcounter++;
    TString host_name = Form("h1Dvar%.3d", _1Dcounter);
    
    TString tmp_title;
    if(xy_title.Contains(";",TString::kExact)){
        tmp_title = xy_title;
    }
    else tmp_title = xy_title + ";Counts";
    
    MnvH1D * hist = new MnvH1D(host_name.Data(), Form(";%s", tmp_title.Data()), nbins, xbins);
    
    TString tmp_cuts = cuts.Data();
    if(!tmp_cuts.EqualTo("", TString::kExact)) tmp_cuts.Append(" && ");
    
    tmp_cuts.Append(Form("(%s != -999)", var.Data()));//Might get rid of this.
    
    cout << tmp_cuts.Data() << endl;
    
    intree->Project(host_name.Data(), var.Data(), tmp_cuts.Data());
    
    //cout << "Histo: " << hist->GetName() << " : Entries = " << hist->Integral() << endl;
    return hist;
}

MnvH2D * DrawingTools::GetHisto(TTree * intree, const TString vars_yx, int x_nbins, const double x_low, const double x_high, int y_nbins, const double y_low, const double y_high, const TString xy_title, const TString cuts){
    
    Double_t * xbins = new Double_t[ x_nbins + 1 ];
    Double_t x_range = x_high - x_low;
    Double_t x_binwidth = x_range/(Double_t)x_nbins;
    
    Double_t * ybins = new Double_t[ y_nbins + 1 ];
    Double_t y_range = y_high - y_low;
    Double_t y_binwidth = y_range/(Double_t)y_nbins;
    
    int loop = x_nbins;
    if(loop < y_nbins) loop = y_nbins;
    
    for (int i=0; i < loop + 1; i++){
        
        cout << "Array Element: " << i;
        if(i < x_nbins + 1 ){
            xbins[i] = x_low + x_binwidth*i;
            cout << " X[" << i << "] = "<< xbins[i];
        }
        else cout << " X[" << i << "] = NULL";
        
        if(i < y_nbins + 1 ){
            ybins[i] = y_low + y_binwidth*i;
            cout << " Y[" << i << "] = " << ybins[i];
        }
        else cout << " Y[" << i << "] = NULL";
        
        cout << endl;
    }
    
    return GetHisto(intree, vars_yx, x_nbins, xbins, y_nbins, ybins, xy_title, cuts);
}

MnvH2D * DrawingTools::GetHisto(TTree * intree, const TString vars_yx, int x_nbins, const Double_t * xbins, int y_nbins, const Double_t * ybins, const TString xy_title, const TString cuts){
    _2Dcounter++;
    TString host_name = Form("h2Dvar%.3d", _2Dcounter);
    
    MnvH2D * hist = new MnvH2D(host_name.Data(), Form(";%s", xy_title.Data()), x_nbins, xbins, y_nbins, ybins);
    
    TString tmp_cuts = cuts.Data();
    //if(!tmp_cuts.EqualTo("", TString::kExact)) tmp_cuts.Append(" && ");
    //tmp_cuts.Append(Form("(%s != -999)", var.Data()));
    //cout << tmp_cuts.Data() << endl;
    
    intree->Project(host_name.Data(), vars_yx.Data(), tmp_cuts.Data());
    
    //cout << "Histo: " << hist->GetName() << " : Entries = " << hist->Integral() << endl;
    return hist;
}

MnvH2D * DrawingTools::SmearMatrix(TTree * intree, const TString vars_yx, int nbins, const double low, const double high, const TString xy_title, const TString cuts){
   
    Double_t * bins = new Double_t[ nbins + 1 ];
    Double_t range = high - low;
    Double_t binwidth = range/(Double_t)nbins;

    for (int i=0; i < nbins + 1; i++){
        
        cout << "Array Element: " << i;
        if(i < nbins + 1 ){
            bins[i] = low + binwidth*i;
            cout << " X[" << i << "] = "<< bins[i];
        }
        cout << endl;
    }
    
    return SmearMatrix(intree, vars_yx, nbins, bins, xy_title, cuts);
}

MnvH2D * DrawingTools::SmearMatrix(TTree * intree, const TString vars_yx, int nbins, const Double_t * bins, const TString xy_title, const TString cuts){
    return GetHisto(intree, vars_yx, nbins, bins, nbins, bins, xy_title, cuts);
}

MnvH1D * DrawingTools::GetTruthHisto(const TString var, int nbins, const double x_low, const double x_high, const TString xy_title, const TString cuts){
    TTree * intree = (TTree*)_file->Get(_truename.Data());
    return GetHisto(intree, var, nbins, x_low, x_high, xy_title, cuts);
}

MnvH1D * DrawingTools::GetRecoHisto(const TString var, int nbins, const Double_t * xbins, const TString xy_title, const TString cuts){
    TTree * intree = (TTree*)_file->Get(_reconame.Data());
    return GetHisto(intree, var, nbins, xbins, xy_title, cuts);
}

MnvH2D * DrawingTools::SmearMatrix(const TString vars_yx, int nbins, const double low, const double high, const TString xy_title, const TString cuts){
    TTree * intree = (TTree*)_file->Get(_reconame.Data());
    return SmearMatrix(intree, vars_yx, nbins, low, high, xy_title, cuts);
}

MnvH2D * DrawingTools::SmearMatrix(const TString vars_yx, int nbins, const Double_t * bins, const TString xy_title, const TString cuts){
    TTree * intree = (TTree*)_file->Get(_reconame.Data());
    return SmearMatrix(intree, vars_yx, nbins, bins, xy_title, cuts);
}

TLegend * DrawingTools::DrawPOT(double x_pos, double y_pos, TString filename){
    
    TFile * tmp_file;
    if(!filename.EqualTo("",TString::kExact)){
        tmp_file = new TFile(filename.Data());
    }
    else tmp_file = _file;
    
    if(tmp_file->IsZombie()){
        cout << "DrawingTools::DrawPOT : Could not read file." << endl;
    }
    
    TTree * meta_tree = (TTree*)tmp_file->Get("Meta");
    
    double x_size = 0.2;
    double y_size = 0.2;
    TLegend * pot = new TLegend(x_pos, y_pos, x_pos + x_size, y_pos + y_size);
    
    if(!meta_tree){
        cout << "DrawingTools::DrawPOT : Meta tree not found." << endl;
        pot->AddEntry((TObject*)0, "No POT Found","");
    }
    else{
        double POT_Used = 0;
        assert(meta_tree->GetEntries()==1);
        meta_tree->GetEntry(0);
        TLeaf * lpot= meta_tree->GetLeaf("POT_Used");
        if(lpot) POT_Used = lpot->GetValue();
        pot->AddEntry((TObject*)0, Form(" %.4f POT", POT_Used),"");
        delete lpot;
    }
    
    delete meta_tree;
    delete tmp_file;
    
    return pot;
}

