#ifndef DRAWING_STYLE_H
#define DRAWING_STYLE_H

#include <iostream>
#include <cstdio>
#include <cstdlib>

//#include <TROOT.h>
//#include <TStyle.h>
using namespace std;

class DrawingStyle {

public:
    DrawingStyle(){
        cout << "DrawingStyle()" << endl;
        /*gROOT->SetStyle("myStyle");
        gStyle->SetOptStat(0);
        gROOT->cd(0);*/
        //gStyle->SetLegendBorderSize(0);
        //gStyle->SetLegendFillColor(0);
        //gStyle->SetOptStat(0);
    }
    
    ~DrawingStyle(){;}
};

#endif
