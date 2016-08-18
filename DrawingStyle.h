#ifndef DRAWING_STYLE_H
#define DRAWING_STYLE_H

#include <TROOT.h>
#include <TStyle.h>

class DrawingStyle {

    DrawingStyle(){
        gStyle->SetLegendBorderSize(0);
    }
    
    ~DrawingStyle(){;}
}

#endif
