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
        gStyle->SetHistLineWidth(kOrange+2);
        gROOT->cd(0);
        //gStyle->SetLegendBorderSize(0);
        //gStyle->SetLegendFillColor(0);
    }
    
    ~DrawingStyle(){;}
};

#endif
