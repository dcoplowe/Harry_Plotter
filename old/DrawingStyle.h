#ifndef DRAWING_STYLE_H
#define DRAWING_STYLE_H

#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <TROOT.h>
#include <TStyle.h>
#include <TGaxis.h>
#include <TColor.h>

using namespace std;

//namespace DrawingStyle {
//    declarations
//}

class DrawingStyle {

public:
    DrawingStyle(){
        // gStyle->Reset();

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

        // gROOT->ForceStyle();
        gROOT->cd(0);
    }
    
    ~DrawingStyle(){;}
    
    enum DSColors {//Add nice colors here and ones for ints/particle types
        //Colors:
        Yellow = kOrange-2,
        Blue = kAzure+2,
        Red = kRed+1,
        Gray = kGray,
        Green = kGreen-3,
        LBlue = kBlue-5,
        //Particles:
        Proton = kOrange-2,
        Neutron = kWhite,
        Pion = kAzure+2,
        PionP = kAzure+2,
        PionM = kMagenta-10,
        Muon = kCyan+2,
        MuonM = kCyan+2,
        MuonP = kOrange+7,
        Pi0 = kRed+1,
        Kaon = kBlue-5,
        Ka0 = kGreen-3,
        Electron = kAzure+8,
        //Topologies:
        T2Pr = kOrange-2,
        T1P1PiP = kAzure+2,
        T1P1PiM = kGreen-3,
        T2PiP = kRed+1,
        T1P1Pi0 = kCyan+2,
        T1Pi1Pi0 = kOrange+7,
        OOPS = kMagenta-10,
        Other = kGray,
        //Initial States:
        LFG = kOrange-2,
        GFG = kAzure+2,
        SF  = kCyan+2,
        BRFG = kRed+1,
        //Final state interactions:
        FSION = kOrange-2,
        FSIOFF = kAzure+2,
        //Generators:
        GENIE = kOrange-2,
        NUWRO = kAzure+2,
        NEUT = kCyan+2,
        GIBUU = kGreen-3,
        //Nuclei:
        HYDROGEN = kWhite,
        CARBON = kOrange-2,
        OXYGEN = kAzure+2,
        ARGON = kGray
    };
    
    static void TastyPalette(Float_t alpha = 1.0){
        int nconts = 255;
        const UInt_t Number = 9;
        Double_t stops[Number] = { 0.0000, 0.1250, 0.2500, 0.3750, 0.5000, 0.6250, 0.7500, 0.8750, 1.0000};
        Double_t red[Number]   = { 0.2082, 0.0592, 0.0780, 0.0232, 0.1802, 0.5301, 0.8186, 0.9956, 0.9764};
        Double_t green[Number] = { 0.1664, 0.3599, 0.5041, 0.6419, 0.7178, 0.7492, 0.7328, 0.7862, 0.9832};
        Double_t blue[Number]  = { 0.5293, 0.8684, 0.8385, 0.7914, 0.6425, 0.4662, 0.3499, 0.1968, 0.0539};
        TColor::CreateGradientColorTable(Number, stops, red, green, blue, nconts, alpha);
        gStyle->SetNumberContours(nconts);
    }

    //  const UInt_t Number = 5;
    // Double_t Red[Number]    = { 0.00, 0.25, 1.00, 1.00, 0.50 };
    // Double_t Green[Number]  = { 0.00, 0.25, 1.00, 0.25, 0.00 };
    // Double_t Blue[Number]   = { 0.50, 1.00, 1.00, 0.25, 0.00 };
    // Double_t Length[Number] = { 0.00, 0.25, 0.50, 0.75, 1.00 };
    // Int_t nb=255;
    // TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,nb);
    // gStyle->SetNumberContours(nb);
};

#endif
