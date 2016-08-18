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
        gROOT->SetStyle("myStyle");
        gStyle->SetOptStat(0);
        gStyle->SetHistLineWidth(2.5);
        gStyle->SetHistLineColor(DSYellow);
        gStyle->SetMarkerStyle(20);
        gStyle->SetPadTickX(1);
        gStyle->SetPadTickY(1);
        gStyle->SetLegendBorderSize(0);
        gStyle->SetLegendFillColor(0);
        
        gROOT->cd(0);
    }
    
    ~DrawingStyle(){;}
    
    enum DSColors {
        DSYellow = kOrange-2,
        DSBlue = kAzure+2
    };
};

#endif
