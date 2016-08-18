#ifndef DRAWING_STYLE_H
#define DRAWING_STYLE_H

#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <TROOT.h>
#include <TStyle.h>

using namespace std;

class DrawingStyle {

public:
    DrawingStyle(){
        gROOT->SetStyle("myStyle");
        gStyle->SetOptStat(0);
        gStyle->SetHistLineWidth(2.5);
        gStyle->SetHistLineColor(kOrange+2);
        gStyle->SetMarkerStyle(20);
        gROOT->cd(0);
        //gStyle->SetLegendBorderSize(0);
        //gStyle->SetLegendFillColor(0);
    }
    
    ~DrawingStyle(){;}
};

#endif
