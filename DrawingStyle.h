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
        DSYellow = kOrange-2,
        DSBlue = kAzure+2,
        DSProton = kOrange-2,
        DSPion = kAzure+2,
        DSMuon = kCyan+2,
        DSQes = kOrange-2,
        DSRes = kAzure+2,
        DSDis = kCyan+2,
        DSOther = kGray
    };
};

#endif
