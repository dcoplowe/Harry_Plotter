#ifndef DRAWING_STYLE_H
#define DRAWING_STYLE_H

#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <TROOT.h>
#include <TStyle.h>
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
        gStyle->SetLegendTextSize(0.1);
        gROOT->cd(0);
    }
    
    ~DrawingStyle(){;}
    
    enum DSColors {//Add nice colors here and ones for ints/particle types
        DSYellow = kOrange-2,
        DSBlue = kAzure+2,
        DSProton = kOrange-2,
        DSPion = kAzure+2,
        DSMuon = kCyan+2,
        DSQES = kOrange-2,
        DSRes = kAzure+2,
        DSDIS = kCyan+2,
        DSOther = kGray+2
    };
};

#endif
