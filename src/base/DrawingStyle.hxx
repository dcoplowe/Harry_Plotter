#ifndef DRAWING_STYLE_H
#define DRAWING_STYLE_H

#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <TROOT.h>
#include <TStyle.h>
#include <TGaxis.h>
#include <TColor.h>

using namespace std;

class DrawingStyle {

public:
    DrawingStyle();
    
    ~DrawingStyle(){;}
    
    static void TastyPalette(Float_t alpha = 1.0);
    
    enum DSColors {//Add nice colors here and ones for ints/particle types
        //Colors:
        // kOrange (Yellow) = 800,
        // kAzure (Blue) = 860,
        // kRed = 632,
        // kGray = 920,
        // kGreen = 416,
        // kBlue = 600,
        // kMagenta = 616,
        // kWhite = 0,
        // kCyan = 431
        Yellow = kOrange-2, 
        Blue = kAzure+2,
        Red = kRed+1,
        Gray = kGray,
        Green = kGreen-3,
        LBlue = kBlue-5,
        //Particles:
        Proton = kOrange-2,
        Neutron = kWhite,
        Pion = kAzure+2,
        PionP = kAzure+2,
        PionM = kMagenta-10,
        Muon = kCyan+2,
        MuonM = kCyan+2,
        MuonP = kOrange+7,
        Pi0 = kRed+1,
        Kaon = kBlue-5,
        Ka0 = kGreen-3,
        Electron = kAzure+8,
        //Topologies:
        T2Pr = kOrange-2,
        T1P1PiP = kAzure+2,
        T1P1PiM = kGreen-3,
        T2PiP = kRed+1,
        T1P1Pi0 = kCyan+2,
        T1Pi1Pi0 = kOrange+7,
        Other = kGray,
        //Initial States:
        LFG = kOrange-2,
        GFG = kAzure+2,
        SF  = kCyan+2,
        BRFG = kRed+1,
        //Final state interactions:
        FSION = kOrange-2,
        FSIOFF = kAzure+2,
        //Generators:
        GENIE = kOrange-2,
        NUWRO = kAzure+2,
        NEUT = kCyan+2,
        GIBUU = kGreen-3,
        //Nuclei:
        HYDROGEN = kWhite,
        CARBON = kOrange-2,
        OXYGEN = kAzure+2,
        ARGON = kGray,
        //Int Mode(l)s:
        QE = kOrange-2,
        RES = kRed+1,
        MECTEM = kAzure+2,
        MECNIEVES = kGreen-3,
        MECMARTEAU = kBlue-5,
        COH = kCyan+2,
        DIS = kAzure+8,
        T2KFlux = kOrange-2,
        OneGeVFlux = kAzure+2,
        UNKNOWN = kPink-9
    };
};

#endif
