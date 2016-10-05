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

class DrawingStyle {

public:
    DrawingStyle(){
        gStyle->SetOptStat(0);
        gStyle->SetHistLineWidth(2.5);
        gStyle->SetHistLineColor(DSYellow);
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
        Muon = kCyan+2,
        MuonM = kCyan+2,
        MuonP = kOrange+7,
        Pi0 = kRed+1,
        Kaon = kBlue-5,
        Ka0 = kGreen-3,
        //Topologies:
        T2Pr = kOrange-2,
        T1P1Pi = kAzure+2,
        T2Pi = kCyan+2,
        TOther = kGray
    };
};

#endif
