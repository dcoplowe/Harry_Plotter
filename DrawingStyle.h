#ifndef DRAWING_STYLE_H
#define DRAWING_STYLE_H

#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <TROOT.h>
#include <TStyle.h>
//#include <TGaxis.h>
//#include <TColor.h> TColor

using namespace std;

//namespace DrawingStyle {
//    declarations
//}

class DrawingStyle {

public:
    DrawingStyle(){
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
        //gStyle->SetLegendTextSize(0.15);
        //TGaxis::SetMaxDigits(2);
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
        Pion = kAzure+2,
        PionP = kAzure+2,
        PionM = kWhite,
        Muon = kCyan+2,
        MuonM = kCyan+2,
        MuonP = kOrange+7,
        Pi0 = kRed+1,
        Kaon = kBlue-5,
        Ka0 = kGreen-3,
        //Topologies:
        T2Pr = kOrange-2,
        T1P1Pi = kAzure+2,
        T1P1PiM = kGreen-3,
        T2Pi = kRed+1,
        T1P1Pi0 = kCyan+2,
        T1Pi1Pi0 = kOrange+7,
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
        HYDROGEN = kOrange-2,
        CARBON = kRed+1,
        OXYGEN = kAzure+2,
        ARGON = kGray
    };
    
    
};

#endif
