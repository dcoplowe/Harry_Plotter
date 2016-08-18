#ifndef DRAWING_STYLE_H
#define DRAWING_STYLE_H

#include <TROOT.h>
#include <TStyle.h>

class DrawingStyle {

public:
    DrawingStyle(){
        gROOT->SetStyle("myStyle");
        gStyle->SetOptStat(0);
        gROOT->cd(0);
        //gStyle->SetLegendBorderSize(0);
        //gStyle->SetLegendFillColor(0);
        //gStyle->SetOptStat(0);
    }
    
    ~DrawingStyle(){;}
}

#endif
