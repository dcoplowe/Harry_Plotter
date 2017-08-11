#ifndef DRAWING_STYLE_CXX
#define DRAWING_STYLE_CXX

#include <DrawingStyle.hxx>

#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <TROOT.h>
#include <TStyle.h>
#include <TGaxis.h>
#include <TColor.h>
#include <TMath.h>

DrawingStyle::DrawingStyle()
{
    gStyle->SetOptStat(0);
    gStyle->SetHistLineWidth(2.5);
    gStyle->SetHistLineColor(Blue);
    gStyle->SetMarkerStyle(20);
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
    gStyle->SetLegendBorderSize(0);
    gStyle->SetLegendFillColor(0);
    gStyle->SetLegendFont(62);

    gStyle->SetPadTopMargin(0.10);
    gStyle->SetPadRightMargin(0.10);
    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetPadLeftMargin(0.15);

    gStyle->SetFrameBorderMode(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetPadBorderMode(0);
    gStyle->SetPadColor(0);
    gStyle->SetCanvasColor(0);
    gStyle->SetStatColor(0);

        //gStyle->SetLegendTextSize(0.15);
    TGaxis::SetMaxDigits(2);

    TastyPalette();
        // gROOT->ForceStyle(); <-- Never use this!!
    gROOT->cd(0);
}

void DrawingStyle::TastyPalette(Float_t alpha){
    int nconts = 255;
    const UInt_t Number = 9;
    Double_t stops[Number] = { 0.0000, 0.1250, 0.2500, 0.3750, 0.5000, 0.6250, 0.7500, 0.8750, 1.0000};
    Double_t red[Number]   = { 0.2082, 0.0592, 0.0780, 0.0232, 0.1802, 0.5301, 0.8186, 0.9956, 0.9764};
    Double_t green[Number] = { 0.1664, 0.3599, 0.5041, 0.6419, 0.7178, 0.7492, 0.7328, 0.7862, 0.9832};
    Double_t blue[Number]  = { 0.5293, 0.8684, 0.8385, 0.7914, 0.6425, 0.4662, 0.3499, 0.1968, 0.0539};
    TColor::CreateGradientColorTable(Number, stops, red, green, blue, nconts, alpha);
    gStyle->SetNumberContours(nconts);
}

int DrawingStyle::GetParticleCol(int pdg)
{
        int colour = static_cast<int>(UNKNOWN);
        if(pdg == 2212) colour = static_cast<int>(Proton);
        else if(pdg == 2112) colour = static_cast<int>(Neutron);
        else if(TMath::Abs(pdg) == 211) colour = static_cast<int>(Pion);
        else if(pdg == 211) colour = static_cast<int>(PionP);
        else if(pdg == -211) colour = static_cast<int>(PionM);
        else if(TMath::Abs(pdg) == 13) colour = static_cast<int>(Muon);
        else if(pdg == 13) colour = static_cast<int>(MuonM);
        else if(pdg == -13) colour = static_cast<int>(MuonP);
        else if(pdg == 111) colour = static_cast<int>(Pi0);
        else if(TMath::Abs(pdg) == 321) colour = static_cast<int>(Kaon);
        else if(pdg == 311) colour = static_cast<int>(Ka0);
        else if(pdg == 11) colour = static_cast<int>(Electron);
        return colour;
}

#endif
