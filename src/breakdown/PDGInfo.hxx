#ifndef _PARTPDGInfo_
#define _PARTPDGInfo_

#include <string>

class PDGInfo {
public:

    enum Type {
        Proton = 2212,
        PiPlus = 211,
        PiMinus = -211,
        Neutron = 2112,
        Electron = 11,
        Positron = -11,
        Muon = 13,
        AMuon = -13,
        PiZero = 111,
        Kaon = 321,
        KaZero = 311
    };

    PDGInfo() : m_pdg(-999), m_name("unknown"), m_symbol(m_name), m_pdg_s(m_name), m_particle_antiparticle(false), m_colour(1), m_line_colour(1), m_line_style(1) {;}
    PDGInfo(int part_pdg, std::string part_name, std::string part_symbol, bool is_p_ap = false);
    ~PDGInfo(){};

    int GetPDGNo(){ return m_pdg; }
    std::string GetName(){ return m_name;}
    std::string GetSymbol(){ return m_symbol;}
    std::string GetPDGStr(){ return m_pdg_s;}
    int  GetColor(){ return m_colour; }//Fill colour}
    int  GetLineColor(){ return m_line_colour;}
    int  GetLineStyle(){ return m_line_style; }
    bool IsBoth(){ return m_particle_antiparticle; }

private:    
    int m_pdg;
    std::string m_name;
    std::string m_symbol;
    std::string m_pdg_s;
    bool m_particle_antiparticle;
    int  m_colour;//Fill colour
    int  m_line_colour;
    int  m_line_style;
};

#endif