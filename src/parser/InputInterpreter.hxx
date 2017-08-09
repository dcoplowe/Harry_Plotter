#ifndef __INPUTINTERPRETER__HXX__
#define __INPUTINTERPRETER__HXX__

#include <string>
#include <TROOT.h>

class InputInterpreter
{
public:
    // Fix this to include axis titles in parser.
    InputInterpreter(const std::string instring, bool run_rw = true);
    ~InputInterpreter();

    std::string GetInString(){ return m_instring; }

    std::string GetVar1(){ return m_rwvar1; }
    std::string GetVar1Title(){ return m_rwvar1_title; }
    int GetVar1NBins(){ return m_rwvar1_nbins; }
    double * GetVar1Bins(){ return m_rwvar1_bins; }

    std::string GetVar2(){ return m_rwvar2; }
    std::string GetVar2Title(){ return m_rwvar2_title; }
    int GetVar2NBins(){ return m_rwvar2_nbins; }
    double * GetVar2Bins(){ return m_rwvar2_bins; }

    bool Run2D(){ return m_Is_TwoD; }

    std::string GetVar2RW(){ return m_var2rw; }
    std::string GetVar2RWTitle(){ return m_var2rw_title; }
    int GetVar2RWNBins(){ return m_var2rw_nbins; }
    double * GetVar2RWBins(){ return m_var2rw_bins; }

    static std::vector<std::string> ReadFile(std::string infile);

private:

    std::string m_instring;

    std::string m_var2rw;
    std::string m_var2rw_title;
    int m_var2rw_nbins;
    std::vector<double> m_var2rw_vals;
    double * m_var2rw_bins;

    std::string m_rwvar1;
    std::string m_rwvar1_title;
    int m_rwvar1_nbins;
    std::vector<double> m_rwvar1_vals;
    double * m_rwvar1_bins;

    std::string m_rwvar2; 
    std::string m_rwvar2_title;
    int m_rwvar2_nbins;
    std::vector<double> m_rwvar2_vals;
    double * m_rwvar2_bins;

    bool m_Is_TwoD;

    double * DetermineBins(int nbins, std::vector<double> binning);
    double * SetBinning(int nbins, Double_t low, Double_t high);

    bool IsNumber(std::string);
};

#endif