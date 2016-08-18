#ifndef DRAWING_STYLE_H
#define DRAWING_STYLE_H

#include <TROOT.h>
#include <TStyle.h>

class DrawingStyle {

public:
    DrawingStyle(){
        gStyle->SetLegendBorderSize(0);
    }
    
    ~DrawingStyle(){;}
}

#endif
