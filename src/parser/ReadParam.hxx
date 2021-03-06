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
    BinPar();
    ~BinPar();

    std::string var;
    std::string title;
    std::string units;
    std::string extra;
    int nbins;
    std::vector<double> bvals;
    double * bins;
    bool good;

    void Print();
    double * SetBinning(int tmp_nbins, double low, double high);
    void DetermineBins();

    BinPar& operator =(const BinPar& mp)
    {
        var = mp.var;
        title = mp.title;
        units = mp.units;
        extra = mp.extra;
        nbins = mp.nbins;
        bvals = mp.bvals;
        bins = mp.bins;
        good = mp.good;
        return *this;
    }

};

#endif

#ifndef __TYPE_STYLE__NS_HXX
#define __TYPE_STYLE__NS_HXX

#include <map>

namespace Type {
    enum Type { kStandard = 0, kEff, kPur, kEP, kPID, kTop, kTar, 
        kEffVSCuts, kPurVSCuts, kEffPurVSCuts, kNCutsMinusOne, kRes, kSmear, kSmearSN };
    
    // static const std::map<Type::Type, std::string> Names;
    // static std::map<Type::Type, std::string> create_Names()
    // {
    //     std::map<Type::Type, std::string> m;
    //     m[ kStandard ] = "";
    //     m[ kEff ] = "kEff";
    //     m[ kPur ] = "Pur";
    //     m[ kEP ] = "EP";
    //     m[ kPID ] = "PID";
    //     m[ kTop ] = "Top";
    //     m[ kTar ] = "Tar";
    //     m[ kEffVSCuts ] = "EffVSCuts";
    //     m[ kPurVSCuts ] = "PurVSCuts";
    //     m[ kEffPurVSCuts ] = "EffPurVSCuts";
    //     m[ kNCutsMinusOne ] = "NCutsMinusOne";
    //     m[ kRes ] = "Residual";
    //     m[ kSmear ] = "Smear";
    //     m[ kSmearSN ] = "SmearSN";
    //     return m;
    // }

    // static const std::map<Type::Type, std::string> EffPur;
    // static std::map<Type::Type, std::string> create_EffPur()
    // {
    //     std::map<Type::Type, std::string> m;
    //     m[ Type::kEffVSCuts ] = "EffVSCuts";
    //     m[ Type::kPurVSCuts ] = "PurVSCuts";
    //     m[ Type::kEffPurVSCuts ] = "EffPurVSCuts";
    //     m[ Type::kNCutsMinusOne ] = "NCutsMinusOne";
    //     return m;
    // }

    static std::string ToString(Type::Type tt)
    {
        std::map<Type::Type, std::string> Names;
        Names[ kStandard ] = "";
        Names[ kEff ] = "kEff";
        Names[ kPur ] = "Pur";
        Names[ kEP ] = "EP";
        Names[ kPID ] = "PID";
        Names[ kTop ] = "Top";
        Names[ kTar ] = "Tar";
        Names[ kEffVSCuts ] = "EffVSCuts";
        Names[ kPurVSCuts ] = "PurVSCuts";
        Names[ kEffPurVSCuts ] = "EffPurVSCuts";
        Names[ kNCutsMinusOne ] = "NCutsMinusOne";
        Names[ kRes ] = "Residual";
        Names[ kSmear ] = "Smear";
        Names[ kSmearSN ] = "SmearSN";
        std::map<Type::Type, std::string>::iterator found = Names.find(tt);
        if(found != Names.end()) return found->second; 
        else return "Unknown";
    }
}

namespace Style {
    enum Style { kLC = 0, kLS, kLW, kFC, kFS, kNotSet };
}

#endif

#ifndef __READPARAM__HXX__
#define __READPARAM__HXX__

#include <map>

// Want this class to read in parameters files for the following:
// Read in parameter for plotting w/ symbol, units, binning etc. + options
// Read in general parameters file for things like signal 

const std::string left_brace = "<|";
const std::string right_brace = "|>";
const std::string ignore_sign = "//";

class ReadParam
{
public:
    enum Dim { kZero = 0, kOne, kTwo, kThree };
    enum Opts { kType = 0, kCuts, kStyle, kInput, kOption };

    // Fix this to include axis titles in parser.
    ReadParam(const std::string instring, std::string left_arrow = left_brace, std::string right_arrow = right_brace);
    ~ReadParam();

    std::string GetInString(){ return m_instring; }

    // Binning:
    // 1D:  
    std::string GetVar1() const { return m_varOne.var; }
    std::string GetVar1Title() const { return (m_varOne.title + (m_varOne.units.empty() ? "" : " (" + m_varOne.units + ")") ); }
    int GetVar1NBins() const { return m_varOne.nbins; }
    double * GetVar1Bins() const { return m_varOne.bins; }

    // 2D:
    std::string GetVar2(){ return m_varTwo.var; }
    std::string GetVar2Title(){ return (m_varTwo.title + (m_varTwo.units.empty() ? "" : " (" + m_varTwo.units + ")") ); }
    int GetVar2NBins(){ return m_varTwo.nbins; }
    double * GetVar2Bins(){ return m_varTwo.bins; }

    int GetDim(){ return static_cast<int>(m_Dim); }
    Dim GetDimEnum(){ return m_Dim; }

    // 3D:
    std::string GetVar3(){ return m_varThree.var; }
    std::string GetVar3Title(){ return (m_varThree.title + (m_varThree.units.empty() ? "" : " (" + m_varThree.units + ")") ); }
    int GetVar3NBins(){ return m_varThree.nbins; }
    double * GetVar3Bins(){ return m_varThree.bins; }
   
    void Print();

    static std::vector<std::string> ReadFile(std::string infile, std::string left_arrow = left_brace, std::string right_arrow = right_brace,
        std::string ignore = ignore_sign);

    static std::string GetParameterS(std::string name, std::string infile, std::string left_arrow = left_brace, std::string right_arrow = right_brace);
    static int GetParameterI(std::string name, std::string infile, std::string left_arrow = left_brace, std::string right_arrow = right_brace);
    static double GetParameterD(std::string name, std::string infile, std::string left_arrow = left_brace, std::string right_arrow = right_brace);
    static bool GetParameterB(std::string name, std::string infile, std::string left_arrow = left_brace, std::string right_arrow = right_brace);

    std::string GetOpt(Opts option);
    bool OptSet(Opts option);

    Type::Type GetType() const { return m_type; } 
    int GetStyle(Style::Style st);

    bool Truth() const { return m_truthtree; }
    bool ResetCuts() const { return m_reset_cuts; }

    static int GetInt(std::string val);
    static double GetDouble(std::string val);

    static void RemoveArrows(std::string &word, std::string left_arrow = left_brace, std::string right_arrow = right_brace);
    static bool IsNumber(std::string);

private:
    std::string m_instring;

    std::map<ReadParam::Opts, std::string> m_options;
    Type::Type m_type;
    std::map<Style::Style, int> m_style;
    bool m_truthtree;

    bool m_NoOptions;
    bool m_reset_cuts;

    BinPar m_varOne;
    BinPar m_varTwo;
    BinPar m_varThree;

    Dim m_Dim;

    static std::string GetParameterN(std::string name, std::string infile, std::string left_arrow = left_brace, std::string right_arrow = right_brace);
    std::string FindOpt(std::string name, std::string in);

    void ExtractOptions();

    // double * DetermineBins(int nbins, std::vector<double> binning);
    // double * SetBinning(int nbins, double low, double high);

};

#endif