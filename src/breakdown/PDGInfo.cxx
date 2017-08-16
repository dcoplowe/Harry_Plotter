#ifndef _PARTPDGInfo_CXX
#define _PARTPDGInfo_CXX

#include <PDGInfo.hxx>
#include <TMath.h>
#include <DrawingStyle.hxx>
#include <sstream>
// #include <>

using std::cout;
using std::endl;

PDGInfo::PDGInfo(int part_pdg, bool is_p_ap) : m_pdg(part_pdg), 
    m_particle_antiparticle(is_p_ap), m_line_colour(1), m_line_style(1) 
{
    stringstream ss;
    ss << m_pdg;
    m_pdg_s = ss.str();
    
    if(m_pdg == (int)PDGInfo::Proton)          m_colour = (int)DrawingStyle::Proton;//Proton
    else if(m_pdg == (int)PDGInfo::PiPlus)     m_colour = (int)DrawingStyle::PionP;//PionP
    else if(m_pdg == (int)PDGInfo::PiMinus)    m_colour = (int)DrawingStyle::PionM;
    else if(m_pdg == (int)PDGInfo::PiPlus)       m_colour = (int)DrawingStyle::Pion;
    else if(m_pdg == (int)PDGInfo::AMuon)      m_colour = (int)DrawingStyle::MuonP;//PionP
    else if(m_pdg == (int)PDGInfo::Muon)       m_colour = (int)DrawingStyle::MuonM;
    else if(m_pdg == (int)PDGInfo::PiZero)     m_colour = (int)DrawingStyle::Pi0;
    else if(m_pdg == (int)PDGInfo::Kaon)       m_colour = (int)DrawingStyle::Kaon;
    else if(m_pdg == (int)PDGInfo::KaZero)     m_colour = (int)DrawingStyle::Ka0;
    else if(m_pdg == (int)PDGInfo::Neutron)    m_colour = (int)DrawingStyle::Neutron;
    else if(m_pdg == (int)PDGInfo::Electron)   m_colour = (int)DrawingStyle::Electron;
    else m_colour = (int)DrawingStyle::Other; 

    if(m_particle_antiparticle) m_pdg = TMath::Abs(m_pdg);

    switch(TMath::Abs(m_pdg)){
        case 2212: m_name = "proton";   m_symbol = "p"; break;
        case 211:  m_name =  "piplus";  m_symbol = "#pi"; break;
        // case -211: m_name = "piminus";  m_symbol = "#pi^{-}"; break;
        case 2112: m_name = "neutron";  m_symbol = "n"; break;
        case 11:   m_name = "electron"; m_symbol = "e"; break;
        case 13:   m_name =   "muon";   m_symbol = "#mu"; break;
        // case -13:  m_name =  "amuon";   m_symbol = "#mu^{+}"; break;
        case 111:  m_name =  "pizero";  m_symbol = "#pi"; break;
        case 321:  m_name =  "ka";      m_symbol = "K"; break;
        case 311:  m_name =  "kazero";  m_symbol = "K"; break;
        default: m_name = "unknown"; m_symbol = m_name; break;
    }

    // Determine particle/antiparticle:
    if(m_particle_antiparticle) m_symbol += "^{#pm}";
    else if(m_pdg == 111 || m_pdg == 321) m_symbol += "^{0}";
    else if(10 < m_pdg && m_pdg < 20 && m_pdg % 2 != 0) m_symbol += "^{-}";
    else if(-10 > m_pdg && m_pdg > -20 && m_pdg % 2 != 0) m_symbol += "^{+}";
    else if( 100 < m_pdg) m_symbol += "^{+}";
    else if(-100 > m_pdg) m_symbol += "^{-}";

    if(m_particle_antiparticle){
        m_name += "pm";
    }
}

PDGInfo::PDGInfo(int part_pdg, std::string part_name, std::string part_symbol, bool is_p_ap) : m_pdg(part_pdg), m_name(part_name), m_symbol(part_symbol), 
    m_particle_antiparticle(is_p_ap), m_line_colour(1), m_line_style(1) 
{
    stringstream ss;
    ss << m_pdg;
    m_pdg_s = ss.str();
    
    if(m_pdg == (int)PDGInfo::Proton)          m_colour = (int)DrawingStyle::Proton;//Proton
    else if(m_pdg == (int)PDGInfo::PiPlus)     m_colour = (int)DrawingStyle::PionP;//PionP
    else if(m_pdg == (int)PDGInfo::PiMinus)    m_colour = (int)DrawingStyle::PionM;
    else if(m_pdg == (int)PDGInfo::PiPlus)       m_colour = (int)DrawingStyle::Pion;
    else if(m_pdg == (int)PDGInfo::AMuon)      m_colour = (int)DrawingStyle::MuonP;//PionP
    else if(m_pdg == (int)PDGInfo::Muon)       m_colour = (int)DrawingStyle::MuonM;
    else if(m_pdg == (int)PDGInfo::PiZero)     m_colour = (int)DrawingStyle::Pi0;
    else if(m_pdg == (int)PDGInfo::Kaon)       m_colour = (int)DrawingStyle::Kaon;
    else if(m_pdg == (int)PDGInfo::KaZero)     m_colour = (int)DrawingStyle::Ka0;
    else if(m_pdg == (int)PDGInfo::Neutron)    m_colour = (int)DrawingStyle::Neutron;
    else if(m_pdg == (int)PDGInfo::Electron)   m_colour = (int)DrawingStyle::Electron;
    else m_colour = (int)DrawingStyle::Other; 

}

void PDGInfo::Print()
{
    cout << "___________PDG Info___________" << endl;
    cout << "    PDG Code: " << m_pdg << endl;
    cout << "        Name: " << m_name << endl;
    cout << "      Symbol: " << m_symbol << endl;
    cout << "___________________________________" << endl;
}

#endif