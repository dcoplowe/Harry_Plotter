#ifndef _TOPOLOGY_CXX
#define _TOPOLOGY_CXX

#include <Topology.hxx>
#include <DrawingStyle.hxx>

Topology::Topology() : m_type(Unknown), m_name(ToString(m_type ,0)), m_symbol(ToString(m_type ,1)), 
m_fill_style(1001), m_line_colour(1), m_line_style(1)
{
}

Topology::Topology(Name topo_type, std::string definition) : m_type(topo_type), m_name(ToString(m_type ,0)),
m_symbol(ToString(m_type ,1)), m_fill_colour(kGray), m_fill_style(1001), m_line_colour(1), m_line_style(1)
{
	m_signal = definition;
    int inclusive = 3144;

	if(m_type == HCC1P1PiPlus){
		m_line_style = 2;
		m_line_colour = 1;
		m_fill_style = 0;
	}
    else if(m_type == HCC1P1PiPlusPS){
        m_line_style = 2;
        m_line_colour = 1;
        m_fill_style = 0;
    }
    else if(m_type == HCC1P1PiPlusOOPS){
        m_fill_colour = (int)DrawingStyle::TOOPS;
    }
    else if(m_type == CC1P1PiPlus)  m_fill_colour = (int)DrawingStyle::T1P1PiP;//Proton
    else if(m_type == OCC1P1PiPlus) m_fill_colour = (int)DrawingStyle::T1P1PiP;//Proton
    else if(m_type == CC1P1PiPlusInc){
        m_fill_colour = (int)DrawingStyle::T1P1PiP;
        m_fill_style = inclusive;
    }
    else if(m_type == CCNPNPiMinus) m_fill_colour = (int)DrawingStyle::T1P1PiM;
	else if(m_type == CCNP) m_fill_colour = (int)DrawingStyle::T2Pr;
    else if(m_type == CCNPInc){
        m_fill_colour = (int)DrawingStyle::T2Pr;
        m_fill_style = inclusive;
    }
    else if(m_type == CCNPiPlus) m_fill_colour = (int)DrawingStyle::T2PiP;
    else if(m_type == CCNPiPlusInc){ 
        m_fill_colour = (int)DrawingStyle::T2PiP;
        m_fill_style = inclusive;
    }
    else if(m_type == CCNPNPiZero) m_fill_colour = (int)DrawingStyle::T1P1Pi0;
    else if(m_type == CCNPiZeroNPiPlus) m_fill_colour = (int)DrawingStyle::T1Pi1Pi0;
    else if(m_type == CCKaonsOth){
    	m_fill_colour = (int)DrawingStyle::T1P1PiM;
    	m_fill_style = 3008;
    }
    else if(m_type == CCNN){
    	m_fill_style = 3009;
    	m_fill_colour = (int)DrawingStyle::T2Pr;
    }
    else if(m_type == CCNPNN){
    	m_fill_colour = (int)DrawingStyle::T2PiP;
    	m_fill_style = 3019;
    }
    else if(m_type == CCNPiNN){
    	m_fill_colour = (int)DrawingStyle::T1P1Pi0;
    	m_fill_style = 3025;
    }
    else if(m_type == CCNPiNPNN){
    	m_fill_colour = (int)DrawingStyle::T1Pi1Pi0;
    	m_fill_style = 3024;
    }
    // cout << "Post : " << "FC = " << m_fill_colour << " LC = " << m_line_colour << " FS = " << m_fill_style << " LS = " << m_line_style << endl;
}

std::string Topology::ToString(Topology::Name name, int form){
        std::string sname = "Unknown";
        if(name == Topology::HCC1P1PiPlus)            sname = (form == 0) ? "HCC1P1PiPlus"        : "CC1p1#pi^{+} on H";
        else if(name == Topology::HCC1P1PiPlusPS)     sname = (form == 0) ? "HCC1P1PiPlusPS"      : "CC1p1#pi^{+} on H (PS)";
        else if(name == Topology::HCC1P1PiPlusOOPS)   sname = (form == 0) ? "HCC1P1PiPlusOOPS"    : "CC1p1#pi^{+} on H (OOPS)";
        else if(name == Topology::OCC1P1PiPlus)       sname = (form == 0) ? "OCC1P1PiPlus"        : "CC1p1#pi^{+} on Other";
        else if(name == Topology::CC1P1PiPlus)        sname = (form == 0) ? "CC1P1PiPlus"         : "CC1p1#pi^{+}";
        else if(name == Topology::CC1P1PiPlusInc)     sname = (form == 0) ? "CC1P1PiPlusInc"      : "Inclusive CC1p1#pi^{+}";
        else if(name == Topology::CCNPNPiMinus)       sname = (form == 0) ? "CCNPNPiMinus"        : "CCNpN#pi^{-}";
        else if(name == Topology::CCNP)               sname = (form == 0) ? "CCNP"                : "CCNp";
        else if(name == Topology::CCNPInc)            sname = (form == 0) ? "CCNPInc"             : "Inclusive CCNp";
        else if(name == Topology::CCNPiPlus)          sname = (form == 0) ? "CCNPiPlus"           : "CCN#pi^{+}";
        else if(name == Topology::CCNPiPlusInc)       sname = (form == 0) ? "CCNPiPlusInc"        : "Inclusive CCN#pi^{+}";
        else if(name == Topology::CCNPNPiZero)        sname = (form == 0) ? "CCNPNPiZero"         : "CCNPN#pi^{0}";
        else if(name == Topology::CCNPiZeroNPiPlus)   sname = (form == 0) ? "CCNPiZeroNPiPlus"    : "CCN#pi^{0}N#pi^{+}";
        else if(name == Topology::CCKaonsOth)         sname = (form == 0) ? "CCKaonsOth"          : "CC Kaons Inc.";
        else if(name == Topology::CCNN)               sname = (form == 0) ? "CCNN"                : "CCNn";
        else if(name == Topology::CCNPNN)             sname = (form == 0) ? "CCNPNN"              : "CCNpNn";
        else if(name == Topology::CCNPiNN)            sname = (form == 0) ? "CCNPiNN"             : "CCN#piNn";
        else if(name == Topology::CCNPiNPNN)          sname = (form == 0) ? "CCNPiPNNN"           : "CCN#piNpNn";
        else if(name == Topology::Other)              sname = "Other";
        
        return sname;
    }

Topology::Name Topology::GetEnum(std::string name)
{
    Topology::Name sname = Unknown;
    if(name.find("HCC1P1PiPlus") != string::npos)            sname = HCC1P1PiPlus;
    else if(name.find("HCC1P1PiPlusPS") != string::npos)     sname = HCC1P1PiPlusPS;
    else if(name.find("HCC1P1PiPlusOOPS") != string::npos)   sname = HCC1P1PiPlusOOPS;
    else if(name.find("OCC1P1PiPlus") != string::npos)       sname = OCC1P1PiPlus;
    else if(name.find("CC1P1PiPlus") != string::npos)        sname = CC1P1PiPlus;
    else if(name.find("CC1P1PiPlusInc") != string::npos)     sname = CC1P1PiPlusInc;
    else if(name.find("CCNPNPiMinus") != string::npos)       sname = CCNPNPiMinus;
    else if(name.find("CCNP") != string::npos)               sname = CCNP;
    else if(name.find("CCNPInc") != string::npos)            sname = CCNPInc;
    else if(name.find("CCNPiPlus") != string::npos)          sname = CCNPiPlus;
    else if(name.find("CCNPiPlusInc") != string::npos)       sname = CCNPiPlusInc;
    else if(name.find("CCNPNPiZero") != string::npos)        sname = CCNPNPiZero;
    else if(name.find("CCNPiZeroNPiPlus") != string::npos)   sname = CCNPiZeroNPiPlus;
    else if(name.find("CCKaonsOth") != string::npos)         sname = CCKaonsOth;
    else if(name.find("CCNN") != string::npos)               sname = CCNN;
    else if(name.find("CCNPNN") != string::npos)             sname = CCNPNN;
    else if(name.find("CCNPiNN") != string::npos)            sname = CCNPiNN;
    else if(name.find("CCNPiNPNN") != string::npos)          sname = CCNPiPNNN;
    else if(name.find("Other") != string::npos)              sname = Other;
    return sname;
}

void Topology::AddToSignal(std::string add2def, bool reset)
{
	if(reset) m_signal = add2def;
	else if(!add2def.empty() && m_signal.empty()) m_signal = add2def;
    else{
        m_signal += " && ";
        m_signal += add2def;
    }
}

void Topology::Reset()
{
	m_signal = "";
}

#endif