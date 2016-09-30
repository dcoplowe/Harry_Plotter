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
        DSYellow = kOrange-2,
        DSBlue = kAzure+2,
        DSRed = kRed+1,
        DSGray = kGray,
        DSGreen = kSpring-3,
        DSLBlue = kBlue-5,
        //Particles:
        DSProton = kOrange-2,
        DSPion = kAzure+2,
        DSMuon = kCyan+2,
        DSPi0 = kRed+1,
        DSKaon = kBlue-5,
        DSKa0 = kSpring-3,
        //Topologies:
        DS2Pr = kOrange-2,
        DS1P1Pi = kAzure+2,
        DS2Pi = kCyan+2,
        DSOther = kGray
    };
};

#endif
