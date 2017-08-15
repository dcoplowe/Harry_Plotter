#ifndef __TOPOLOGY__HXX
#define __TOPOLOGY__HXX

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
    Int_t GetFillColor(){ return m_fill_colour; }
    Int_t GetFillStyle(){ return m_fill_style; }
    Int_t GetLineColor(){ return m_line_colour; }
    Int_t GetLineStyle(){ return m_line_style; }

    static std::string ToString(Name name, int form = 0);
    static Name GetEnum(std::string name);

private:
    Name m_type;
    std::string m_name;
    std::string m_symbol;
    std::string m_signal;
    Int_t  m_fill_colour;
    Int_t  m_fill_style;
    Int_t  m_line_colour;
    Int_t  m_line_style;
};

#endif