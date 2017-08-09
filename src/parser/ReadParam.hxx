#include <string>
#include <iostream>
#include <vector> 
#include <cstdlib> //exit(X)

using std::cout;
using std::endl;

#ifndef __BINPAR__HXX__
#define __BINPAR__HXX__

class BinPar
{
public:
    BinPar() : var(""), title(""), units(""), nbins(-999), good(false) { bvals.clear(); bins = 0x0; }
    ~BinPar(){ bvals.clear(); if(bins) delete [] bins; }

    std::string var;
    std::string title;
    std::string units;
    int nbins;
    std::vector<double> bvals;
    double * bins;
    bool good;
    // std::string opts;

    inline void Print(){
        cout << "Par: " << var << " : Title: " << title << " (" << (units.empty() ? "NONE" : units) << ")";
        cout << "NBins: " << nbins << " :";
        if(bins){
            for(int i = 0; i < nbins + 1; i++){
                cout << " " << bins[i];
            } 
            cout << endl;
        }
        else cout << "Problem Bins not set!!! " << endl;
    }

    inline double * SetBinning(int tmp_nbins, double low, double high){
        double * tmp_bins = new double[ tmp_nbins + 1 ];
        double range = high - low;
        double binwidth = range/(double)tmp_nbins;

        for (int i=0; i < tmp_nbins + 1; i++) {
            tmp_bins[i] = low + binwidth*i;
        // if(m_verbose) cout << "Array Element: " << i << " : " << tmp_bins[i] << endl;
        }
        return tmp_bins;
    }

    inline void DetermineBins(){
        if(bvals.size() == 2) bins = SetBinning(nbins, bvals[0], bvals[1]);
        else if( (int)bvals.size() == nbins + 1){
            bins = new double [ nbins + 1 ];
            for(int i = 0; i < nbins + 1; i++) bins[i] = bvals[i]; 
        }
        else{
            cout << __FILE__ << ":" << __LINE__ << " : ERROR : Could not make array: nbins = " << nbins << ", array size = " << bvals.size() << endl;
            exit(0);       
        }
    }

};

#endif

#ifndef __READPARAM__HXX__
#define __READPARAM__HXX__

// Want this class to read in parameters files for the following:
// Read in parameter for plotting w/ symbol, units, binning etc. + options
// Read in general parameters file for things like signal 

const std::string left_brace = "<|";
const std::string right_brace = "|>";

class ReadParam
{
public:

    enum Type { kBinning = 0, kReweight, kPlot, kUnknown };

    // Fix this to include axis titles in parser.
    ReadParam(const std::string instring, Type type, string left_arrow = left_brace, string right_arrow = right_brace);
    ~ReadParam();

    std::string GetInString(){ return m_instring; }

    // Binning:
    // 1D:
    std::string GetVar1(){ return m_varOne.var; }
    std::string GetVar1Title(){ return (m_varOne.title + (m_varOne.units.empty() ? "" : " (" + m_varOne.units + ")") ); }
    int GetVar1NBins(){ return m_varOne.nbins; }
    double * GetVar1Bins(){ return m_varOne.bins; }

    // 2D:
    std::string GetVar2(){ return m_varTwo.var; }
    std::string GetVar2Title(){ return (m_varTwo.title + (m_varTwo.units.empty() ? "" : " (" + m_varTwo.units + ")") ); }
    int GetVar2NBins(){ return m_varTwo.nbins; }
    double * GetVar2Bins(){ return m_varTwo.bins; }

    bool Run2D(){ return m_Is_TwoD; }

    // RW:
    std::string GetVar2RW(){ return m_var2rw.var; }
    std::string GetVar2RWTitle(){ return (m_var2rw.title + (m_var2rw.units.empty() ? "" : " (" + m_var2rw.units + ")") ); }
    int GetVar2RWNBins(){ return m_var2rw.nbins; }
    double * GetVar2RWBins(){ return m_var2rw.bins; }

    

    static std::vector<std::string> ReadFile(std::string infile);

    static std::string GetParameterS(std::string name, std::string infile, std::string left_arrow = left_brace, std::string right_arrow = right_brace);
    static int GetParameterI(std::string name, std::string infile, std::string left_arrow = left_brace, std::string right_arrow = right_brace);
    static double GetParameterD(std::string name, std::string infile, std::string left_arrow = left_brace, std::string right_arrow = right_brace);

private:
    std::string m_instring;

    BinPar m_var2rw;
    BinPar m_varOne;
    BinPar m_varTwo;

    bool m_Is_TwoD;

    static void RemoveArrows(std::string word, std::string left_arrow = left_brace, std::string right_arrow = right_brace);
    static bool IsNumber(std::string);

    double * DetermineBins(int nbins, std::vector<double> binning);
    double * SetBinning(int nbins, double low, double high);

};

#endif