#ifndef __TOPOLOGY__HXX
#define __TOPOLOGY__HXX

#include <string>

class Topology {
public:
    
    enum Name{
        HCC1P1PiPlus = 0,
        HCC1P1PiPlusPS,
        HCC1P1PiPlusOOPS,
        CCC1P1PiPlus,
        CC1P1PiPlus,
        CCNPNPiMinus,
        CCNP,
        CCNPiPlus,
        CCNPNPiZero,
        CCNPiZeroNPiPlus,
        //Added topos (200217):
        CCKaonsOth,
        CCNN,//N Neutrons
        CCNPNN,//N Protons, N Neutrons
        CCNPiNN, //N pions, N Neutrons
        CCNPiNPNN, //N pions, N Protons N Neutrons
        //CCNucBreakUp, -- may be nice in the future
        // Inclusive final state:
        CC1P1PiPlusInc,
        CCNPInc,
        CCNPiPlusInc,
        OCC1P1PiPlus,
        Other,
        Unknown = -999
    };

    Topology();
    Topology(Name name, std::string definition);
    ~Topology(){;}

    void AddToSignal(std::string add2def, bool reset = false);

    void Reset();

    Name GetType(){ return m_type; }
    std::string GetName(){ return m_name; }
    std::string GetSymbol(){ return m_symbol; }
    std::string GetSignal(){ return m_signal; }
    int GetFillColor(){ return m_fill_colour; }
    int GetFillStyle(){ return m_fill_style; }
    int GetLineColor(){ return m_line_colour; }
    int GetLineStyle(){ return m_line_style; }

    static std::string ToString(Name name, int form = 0);
    static Name GetEnum(std::string name);

    void Print();

private:
    Name m_type;
    std::string m_name;
    std::string m_symbol;
    std::string m_signal;
    int  m_fill_colour;
    int  m_fill_style;
    int  m_line_colour;
    int  m_line_style;
};

#endif