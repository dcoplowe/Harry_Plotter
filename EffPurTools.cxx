#include "EffPurTools.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

EffPurTools::EffPurTools(TString filename, TString truename, TString reconame){
    
    cout << "EffPurTools::EffPurTools(TString, TString, TString)" << endl;
    SetFile(filename);
    SetRecoBranch(reconame);
    SetTrueBranch(truename);
    
    cout << "    Filename: " << _filename.Data() << endl;
    cout << "Truth branch: " << _truename.Data() << endl;
    cout << "Recon branch: " << _reconame.Data() << endl;
    
}

EffPurTools::EffPurTools() {
    cout << "EffPurTools::EffPurTools()" << endl;
    EffPurTools("UNKNOWN");
    //SetRecoBranch("CC1P1Pi");
    //SetTrueBranch("Truth");
}

//These may not be void functions:
void EffPurTools::EffVSCuts(TString signal, TString cuts){
    cout << "EffPurTools::EffVSCuts(TString, TString)" << endl;
    
    cout << "    Signal: " << signal.Data() << endl;
    cout << "    Cut(s): " << cuts.Data() << endl;


}

void EffPurTools::EffVSVar(TString var, TString signal, TString cuts){
    cout << "EffPurTools::EffVSVar(TString, TString, TString)" << endl;
    cout << "    Signal: " << signal.Data() << endl;
    cout << "    Cut(s): " << cuts.Data() << endl;

}

void EffPurTools::PurVSCuts(TString signal, TString cuts){
    cout << "EffPurTools::PurVSCuts(TString, TString)" << endl;
    cout << "    Signal: " << signal.Data() << endl;
    cout << "    Cut(s): " << cuts.Data() << endl;

}


