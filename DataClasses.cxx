#include "DataClasses.h"

#include <iostream>//cout/endl headers
#include "DrawingStyle.h"

using std::cout;
using std::endl;

#ifndef _EXPERIMENT_CXX
#define _EXPERIMENT_CXX

Experiment::Experiment(Name exp)
{
    m_topologies = new Topologies();
    m_intmodes.clear();
    m_cuts.clear();

    if(exp == Experiment::MIN){

        m_name = "MINERvA";
        m_logo = m_name + " Preliminary";

        m_reconame = "sel";
        m_truename = "Truth";

        m_signal  = "truth_n_pro == 1 && truth_n_piP == 1 && truth_n_muo == 1 && mc_nFSPart == 3 && mc_targetZ == 1";
        m_signal += " && mc_current == 1 && TMath::RadToDeg()*truth_mu_Theta < 25. && TMath::RadToDeg()*truth_mu_Theta >= 0.";
        m_signal += " && truth_true_target_region == 1 && truth_mu_E < 20000. && truth_mu_E > 0.";

        m_signaldef = "H-CC1p1#pi^{+} (1.5 < E_{#mu} GeV < 20,  #theta_{#mu} < 25 Deg.)"; 

        m_tag = "sel";

        m_tarvarname = "mc_targetZ";
    }
    else if(exp == Experiment::T2K){

        m_name = "T2K";
        m_logo = m_name + " Work In Progress";

        m_reconame = "default";
        m_truename = "truth";

        m_signal  = "";

        m_signaldef = "H-CC1p1#pi^{+}"; 

        m_tag = "";

        m_tarvarname = "target";
    }

    SetTopologies(exp);
    SetIntModes(exp);
    SetCutNames(exp);
}

Experiment::~Experiment()
{
    delete m_topologies;
    m_intmodes.clear();
    m_cuts.clear();
}

void Experiment::SetTopologies(Experiment::Name exp)
{
    if(exp == Experiment::MIN){

        cout << " Adding Topology = " << Topology::ToString(Topology::CC1P1PiPlus) << endl;

    	m_topologies->AddTopology(		  Topology::CC1P1PiPlus, "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1");
    	m_topologies->AddSignalToTopology(Topology::CC1P1PiPlus, "truth_n_ntn == 0 && truth_n_pho == 0 && truth_n_pi0 == 0 && truth_n_piM == 0");
    	m_topologies->AddSignalToTopology(Topology::CC1P1PiPlus, "truth_n_piP == 1 && truth_n_pro == 1 && truth_n_tau == 0 && mc_nFSPart == 3");

        cout << " Adding Topology = " << Topology::ToString(Topology::HCC1P1PiPlus) << endl;

    	m_topologies->AddTopology(		  Topology::HCC1P1PiPlus,  m_topologies->GetTopology(Topology::CC1P1PiPlus).GetSignal() );
    	m_topologies->AddSignalToTopology(Topology::HCC1P1PiPlus, "mc_targetZ == 1 && true_target_region == 1 && truth_mu_E < 20000.");
    	m_topologies->AddSignalToTopology(Topology::HCC1P1PiPlus, "truth_mu_E > 0.");

        cout << " Adding Topology = " << Topology::ToString(Topology::CCNPNPiMinus) << endl;

    	m_topologies->AddTopology(		  Topology::CCNPNPiMinus, "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1");
    	m_topologies->AddSignalToTopology(Topology::CCNPNPiMinus, "truth_n_ntn == 0 && truth_n_pho == 0 && truth_n_pi0 == 0 && truth_n_piM > 0");
    	m_topologies->AddSignalToTopology(Topology::CCNPNPiMinus, "truth_n_piP == 0 && truth_n_pro > 0 && truth_n_tau == 0 &&  mc_nFSPart > 0");

        cout << " Adding Topology = " << Topology::ToString(Topology::CCNP) << endl;

    	m_topologies->AddTopology(		  Topology::CCNP, "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1 ");
    	m_topologies->AddSignalToTopology(Topology::CCNP, "truth_n_ntn == 0 && truth_n_pho == 0 && truth_n_pi0 == 0 && truth_n_piM == 0 ");
    	m_topologies->AddSignalToTopology(Topology::CCNP, "truth_n_piP == 0 && truth_n_pro > 0 && truth_n_tau == 0 && mc_nFSPart > 0");

        cout << " Adding Topology = " << Topology::ToString(Topology::CCNPiPlus) << endl;

    	m_topologies->AddTopology(		  Topology::CCNPiPlus, "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1");
    	m_topologies->AddSignalToTopology(Topology::CCNPiPlus, "truth_n_ntn == 0 && truth_n_pho == 0 && truth_n_pi0 == 0 && truth_n_piM == 0");
    	m_topologies->AddSignalToTopology(Topology::CCNPiPlus, "truth_n_piP > 0 && truth_n_pro == 0 && truth_n_tau == 0 && mc_nFSPart > 0");

        cout << " Adding Topology = " << Topology::ToString(Topology::CCNPNPiZero) << endl;

		m_topologies->AddTopology(		  Topology::CCNPNPiZero, "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1");
    	m_topologies->AddSignalToTopology(Topology::CCNPNPiZero, "truth_n_ntn == 0 && truth_n_pho == 0 && truth_n_pi0 > 0 && truth_n_piM == 0");
    	m_topologies->AddSignalToTopology(Topology::CCNPNPiZero, "truth_n_piP == 0 && truth_n_pro > 0 && truth_n_tau == 0 && mc_nFSPart > 0");

        cout << " Adding Topology = " << Topology::ToString(Topology::CCNPiZeroNPiPlus) << endl;

		m_topologies->AddTopology(		  Topology::CCNPiZeroNPiPlus, "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1");
    	m_topologies->AddSignalToTopology(Topology::CCNPiZeroNPiPlus, "truth_n_ntn == 0 && truth_n_pho == 0 && truth_n_pi0 > 0 && truth_n_piM == 0");
    	m_topologies->AddSignalToTopology(Topology::CCNPiZeroNPiPlus, "truth_n_piP > 0 && truth_n_pro == 0 && truth_n_tau == 0 && mc_nFSPart > 0");

        cout << " Adding Topology = " << Topology::ToString(Topology::CCKaonsOth) << endl;

		m_topologies->AddTopology(		  Topology::CCKaonsOth, "truth_n_ele == 0 && (truth_n_kPM > 0 || truth_n_kaO > 0) && truth_n_muo == 1");
    	m_topologies->AddSignalToTopology(Topology::CCKaonsOth, "(truth_n_ntn > 0 || truth_n_pho > 0 || truth_n_pi0 > 0 || truth_n_piM > 0 ");
    	m_topologies->AddSignalToTopology(Topology::CCKaonsOth, "truth_n_piP > 0 && truth_n_pro == 0 && truth_n_tau == 0 && mc_nFSPart > 0 || truth_n_piP > 0 || truth_n_pro > 0)");
    	m_topologies->AddSignalToTopology(Topology::CCKaonsOth, "truth_n_tau == 0 && mc_nFSPart > 0");

        cout << " Adding Topology = " << Topology::ToString(Topology::CCNN) << endl;

    	m_topologies->AddTopology(        Topology::CCNN, "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1");
    	m_topologies->AddSignalToTopology(Topology::CCNN, "truth_n_ntn > 0 && truth_n_pho == 0 && truth_n_pi0 == 0 && truth_n_piM == 0");
    	m_topologies->AddSignalToTopology(Topology::CCNN, "truth_n_piP == 0 && truth_n_pro == 0 && truth_n_tau == 0 && mc_nFSPart > 0");

        cout << " Adding Topology = " << Topology::ToString(Topology::CCNPNN) << endl;

    	m_topologies->AddTopology(        Topology::CCNPNN, "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1");
    	m_topologies->AddSignalToTopology(Topology::CCNPNN, "truth_n_ntn > 0 && truth_n_pho == 0 && truth_n_pi0 == 0 && truth_n_piM == 0");
    	m_topologies->AddSignalToTopology(Topology::CCNPNN, "truth_n_piP == 0 && truth_n_pro > 0 && truth_n_tau == 0 && mc_nFSPart > 0");

        cout << " Adding Topology = " << Topology::ToString(Topology::CCNPiNN) << endl;

    	m_topologies->AddTopology(        Topology::CCNPiNN, "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1");
    	m_topologies->AddSignalToTopology(Topology::CCNPiNN, "truth_n_ntn > 0 && truth_n_pho == 0");
    	m_topologies->AddSignalToTopology(Topology::CCNPiNN, "(truth_n_pi0 > 0 || truth_n_piM > 0 || truth_n_piP > 0)");
    	m_topologies->AddSignalToTopology(Topology::CCNPiNN, "truth_n_pro == 0 && truth_n_tau == 0 && mc_nFSPart > 0");

        cout << " Adding Topology = " << Topology::ToString(Topology::CCNPiNPNN) << endl;

    	m_topologies->AddTopology(        Topology::CCNPiNPNN, "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1");
    	m_topologies->AddSignalToTopology(Topology::CCNPiNPNN, "truth_n_ntn > 0 && truth_n_pho == 0");
    	m_topologies->AddSignalToTopology(Topology::CCNPiNPNN, "(truth_n_pi0 > 0 || truth_n_piM > 0 || truth_n_piP > 0)");
    	m_topologies->AddSignalToTopology(Topology::CCNPiNPNN, "truth_n_pro > 0 && truth_n_tau == 0 && mc_nFSPart > 0");

    	// m_topologies->AddTopology(        Topology::CC1Pi1PNN, "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1");
    	// m_topologies->AddSignalToTopology(Topology::CC1Pi1PNN, "truth_n_ntn > 0 && truth_n_pho == 0");
    	// m_topologies->AddSignalToTopology(Topology::CC1Pi1PNN, "(truth_n_pi0 > 0 || truth_n_piM > 0 || truth_n_piP > 0)");
    	// m_topologies->AddSignalToTopology(Topology::CC1Pi1PNN, "truth_n_pro > 0 && truth_n_tau == 0 && mc_nFSPart > 0");

    	//Set the list to draw: 
    	m_topologies->ResetList2Draw();
    	m_topologies->Add2DrawList( Topology::CC1P1PiPlus        );
    	m_topologies->Add2DrawList( Topology::CCNPNPiMinus       );
    	m_topologies->Add2DrawList( Topology::CCNP               );
    	m_topologies->Add2DrawList( Topology::CCNPiPlus          );
    	m_topologies->Add2DrawList( Topology::CCNPNPiZero        );
    	m_topologies->Add2DrawList( Topology::CCNPiZeroNPiPlus   );
    	m_topologies->Add2DrawList( Topology::Other              );

    }
    else if(exp == Experiment::T2K){

    }
    else{
        cout << "Experiment::SetTopologies(Experiment::Name exp) : Warning : Topologies not set!" << endl;
    }
}

void Experiment::SetIntModes(Experiment::Name exp)
{
    m_intmodes.clear();

    if(exp == Experiment::MIN){
        m_intmodes.push_back("SomeMode");
    }
    else if(exp == Experiment::T2K){//T2K has lots of different possible selections -- need in include this here. 
        m_intmodes.push_back("SomeMode");
    }
    else{
        cout << "Experiment::SetIntModes(Experiment::Name exp) : Warning : Interaction modes not set!" << endl;
    }

}

void Experiment::SetCutNames(Experiment::Name exp)
{
    m_cuts.clear();
    if(exp == Experiment::MIN){
        m_cuts.push_back("Vertex");
        m_cuts.push_back("3 Tracks");
        m_cuts.push_back("Muon Track");
        m_cuts.push_back("Contained Vtx");
        m_cuts.push_back("PID: p/#pi^{+}");
        m_cuts.push_back("Michel Sense");
    }
    else if(exp == Experiment::T2K){//T2K has lots of different possible selections -- need in include this here. 
        m_cuts.push_back("Event quality");
        m_cuts.push_back("> 0 tracks");
        m_cuts.push_back("Quality and fiducial");
        m_cuts.push_back("Three TPC tracks");
        m_cuts.push_back("TPC only tracks angular cut");
        m_cuts.push_back("+ve TPC tracks PID");
    }
    else{
        cout << "Experiment::SetCutNames(Experiment::Name exp) : Warning : Cut names not set!" << endl;
    }
}

std::string Experiment::GetBaseCuts(int accum_level, int branch, std::string cuts)
{
    stringstream br, ac;
    br << branch;
    ac << accum_level;

    std::string basecuts = "accum_level[" + br.str() + "] > " + ac.str(); 
    if(!cuts.empty()){
        basecuts += " && ";
        basecuts += cuts;
    }
    return basecuts;
}

#endif


#ifndef _PARTICLE_CXX
#define _PARTICLE_CXX

Particle::Particle(Experiment::Name exp, std::string name, std::string tag) : m_tag(tag) {
    
    if(name.find("mu") != std::string::npos){ 
        m_name = "Muon";
        m_symbol = "#mu^{-}";
    }
    else if(name.find("pi") != std::string::npos){ 
        m_name = "Pion"; 
        m_symbol = "#pi^{+}";
    }
    else if(name.find("p") != std::string::npos){ 
        m_name = "Proton"; 
        m_symbol = "p";
    }
    else{ 
        m_name = "Unknown";
        m_symbol = "UN";
    }

    m_label = "";

    if(exp == Experiment::T2K){
        //Reco vars: Common:
        P =         m_tag + name + "_mom";
        pT =        m_tag + name + "_pT";
        pTMag =     m_tag + name + "_pTMag";
        startdir =  m_tag + name + "_dir";
        endpos =    m_tag + name + "_endpos";
        
        //True vars: Common:
        trueP =         name + "_truemom";
        truepT =        name + "_truepT";
        truepTMag =     name + "_truepTMag";
        truestartdir =  name + "_truedir";
        trueendpos =    name + "_endpos";
        pdg =           name + "_pdg";
        
        //MIN: Reco vars:
        E = "";
        P4 = "";
        T = "";//KE
        pTT = "";
        theta = "";
        startpos = "";
        phi = "";
        
        michel = "";
        score = "";
        
        //MIN: True vars:
        trueE = "";
        trueP4 = "";
        trueT = "";//KE
        truepTT = "";
        truetheta = "";
        truestartpos = "";
        truephi = "";
        
        //T2K: Reco vars:
        ctheta =        m_tag + name + "_costheta";
        ctheta_nudir =  m_tag + name + "_nudir_costheta";
        
        //T2K: Reco vars:
        truectheta =    name + "_truecostheta";
    }
    else if(exp == Experiment::MIN){
        
        std::string tmpname = name;
        if(name.find("LL") != std::string::npos){
            std::string tmpstring = "_LL";
            tmpname.erase( tmpname.find(tmpstring), tmpname.length() );
            m_label = "LL";
        }
        else if(name.find("EX") != std::string::npos){
            std::string tmpstring = "_EX";
            tmpname.erase( tmpname.find(tmpstring), tmpname.length() );
            m_label = "dEdX";
        }
        
        //Reco vars: Common:
        P =         m_tag + name + "_mom";
        pT =        m_tag + name + "_pT";
        pTMag =     m_tag + name + "_pTMag";
        startdir =  m_tag + tmpname + "_startdir";
        endpos =    m_tag + tmpname + "_endpos";
        
        //True vars: Common:
        trueP =         m_tag + tmpname + "_truemom";
        truepT =        m_tag + tmpname + "_truepT";
        truepTMag =     m_tag + tmpname + "_truepTMag";
        truestartdir =  m_tag + tmpname + "_truestartdir";
        trueendpos =    m_tag + tmpname + "_endpos";
        pdg =           m_tag + tmpname + "_PDG";
        
        //MIN: Reco vars:
        E =     m_tag + name + "_E";
        P4 =    m_tag + name + "_4mom";
        T =     m_tag + name + "_KE";//KE
        pTT =   m_tag + name + "_pTT";
        theta = m_tag + name + "_Theta";
        phi =   m_tag + name + "_Phi";
        startpos = m_tag + tmpname + "_startpos";
        
        michel =    m_tag + name + "_michel";
        score =     m_tag + name + "_score";
        
        //MIN: True vars:
        trueE =         m_tag + tmpname + "_trueE";
        trueP4 =        m_tag + tmpname + "_true4mom";
        trueT =         m_tag + tmpname + "_trueKE";//KE
        truepTT =       m_tag + tmpname + "_truepTT";
        truetheta =     m_tag + tmpname + "_trueTheta";
        truephi =       m_tag + tmpname + "_truePhi";
        truestartpos =  m_tag + tmpname + "_truestartpos";
        
        //T2K: Reco vars:
        ctheta = "";
        ctheta_nudir = "";
        
        //T2K: Reco vars:
        truectheta = "";
    }
    else{
        //Reco vars: Common:
        P = "";
        pT = "";
        pTMag = "";
        startdir = "";
        endpos = "";
        
        //True vars: Common:
        trueP = "";
        truepT = "";
        truepTMag = "";
        truestartdir = "";
        trueendpos = "";
        pdg = "";
        
        //MIN: Reco vars:
        E = "";
        P4 = "";
        T = "";//KE
        pTT = "";
        theta = "";
        startpos = "";
        phi = "";
        
        michel = "";
        score = "";
        
        //MIN: True vars:
        trueE = "";
        trueP4 = "";
        trueT = "";//KE
        truepTT = "";
        truetheta = "";
        truestartpos = "";
        truephi = "";
        
        //T2K: Reco vars:
        ctheta = "";
        ctheta_nudir = "";
        
        //T2K: Reco vars:
        truectheta = "";
    }
    
}

#endif


#ifndef _KINEMATICVARS_CXX
#define _KINEMATICVARS_CXX

KinematicVars::KinematicVars(Experiment::Name exp){
    
    if(exp == Experiment::T2K){
        muon = new Particle(    exp, "mu");
        proton = new Particle(  exp, "p");
        pion = new Particle(    exp, "pi");
        
        // muon_alt = 0x0;
        proton_alt = 0x0;
        pion_alt = 0x0;
        
        //Transverse Variables:
        dpTT = "dpTT";
        dpT = "dpT";
        dalphaT = "dalphaT";
        dphiT = "dphiT";
        
        truedpTT = "truedpTT";
        truedpT = "truedpT";
        truedalphaT = "truedalphaT";
        truedphiT = "truedphiT";
    }
    else if(exp == Experiment::MIN){
        
        muon = new Particle(    exp, "mu", "sel_");
        proton = new Particle(  exp, "pr_EX", "sel_");
        pion = new Particle(    exp, "pi_EX", "sel_");
        
        proton_alt = new Particle(  exp, "pr_LL", "sel_");
        pion_alt = new Particle(    exp, "pi_LL", "sel_");
        
        //Transverse Variables:
        dpTT = "sel_dpTT_EX";
        dpTT_tmumom = "sel_dpTT_EX_tmumom";
        dpTT_tpimom = "sel_dpTT_EX_tpimom";
        dpTT_tprmom = "sel_dpTT_EX_tprmom";
        dpTT_tnudir = "sel_dpTT_EX_tnudir";
        
        dpTT_pi = "sel_dpTT_pi_EX";
        dpTT_pr = "sel_dpTT_pr_EX";
        
        dpTT_pidir = "sel_dpTT_pi_dir_EX";
        dpTT_prdir = "sel_dpTT_pr_dir_EX";
        
        dpT = "sel_dpT_EX";
        dalphaT = "sel_dalphaT_EX";
        dphiT = "sel_dphiT_EX";
        
        truedpTT = "sel_truedpTT";
        truedpT = "sel_truedpT";
        truedalphaT = "sel_truedalphaT";
        truedphiT = "sel_truedphiT";
        
        dpTT_alt = "sel_dpTT_LL";
        dpTT_tmumom_alt = "sel_dpTT_LL_tmumom";
        dpTT_tpimom_alt = "sel_dpTT_LL_tpimom";
        dpTT_tprmom_alt = "sel_dpTT_LL_tprmom";
        dpTT_tnudir_alt = "sel_dpTT_LL_tnudir";
        
        dpTT_pi_alt = "sel_dpTT_pi_LL";
        dpTT_pr_alt = "sel_dpTT_pr_LL";
        
        dpTT_pidir_alt = "sel_dpTT_pi_dir_LL";
        dpTT_prdir_alt = "sel_dpTT_pr_dir_LL";
        
        dpT_alt = "sel_dpT_LL";
        dalphaT_alt = "sel_dalphaT_LL";
        dphiT_alt = "sel_dphiT_LL";
        
    }
    else{
        //Transverse Variables:
        muon = 0x0;
        proton = 0x0;
        pion = 0x0;
        proton_alt = 0x0;
        pion_alt = 0x0;

        dpTT = "";
        dpT = "";
        dalphaT = "";
        dphiT = "";
        
        truedpTT = "";
        truedpT = "";
        truedalphaT = "";
        truedphiT = "";
        
        dpTT_alt = "";
        dpT_alt = "";
        dalphaT_alt = "";
        dphiT_alt = "";
    }
}

KinematicVars::~KinematicVars(){
    if(muon) delete muon;
    if(proton) delete proton;
    if(pion) delete pion;
    
    if(proton_alt)  delete proton_alt;
    if(pion_alt)    delete pion_alt;
}

#endif

#ifndef _TOPOLOGY_CXX
#define _TOPOLOGY_CXX

Topology::Topology() : m_type(Unknown), m_name(ToString(m_type ,0)), m_symbol(ToString(m_type ,1)), 
m_fill_style(1001), m_line_colour(1), m_line_style(1)
{
}

Topology::Topology(Name topo_type, std::string definition) : m_type(topo_type), m_name(ToString(m_type ,0)),
m_symbol(ToString(m_type ,1)), m_fill_colour(40), m_fill_style(1001), m_line_colour(1), m_line_style(1)
{
	m_signal = definition;

	if(m_type == HCC1P1PiPlus){
		m_line_style = 2;
		m_line_colour = 1;
		m_fill_style = 0;
	}
    else if(CC1P1PiPlus) m_fill_colour = (Int_t)DrawingStyle::T1P1PiP;//Proton
    else if(m_type == CCNPNPiMinus) m_fill_colour = (Int_t)DrawingStyle::T1P1PiM;
	else if(m_type == CCNP) m_fill_colour = (Int_t)DrawingStyle::T2Pr;
    else if(m_type == CCNPiPlus) m_fill_colour = (Int_t)DrawingStyle::T2PiP;
    else if(m_type == CCNPNPiZero) m_fill_colour = (Int_t)DrawingStyle::T1P1Pi0;
    else if(m_type == CCNPiZeroNPiPlus) m_fill_colour = (Int_t)DrawingStyle::T1Pi1Pi0;
    else if(m_type == CCKaonsOth){
    	m_fill_colour = (Int_t)DrawingStyle::T1P1PiM;
    	m_fill_style = 3008;
    }
    else if(m_type == CCNN){
    	m_fill_style = 3009;
    	m_fill_colour = (Int_t)DrawingStyle::T2Pr;
    }
    else if(m_type == CCNPNN){
    	m_fill_colour = (Int_t)DrawingStyle::T2PiP;
    	m_fill_style = 3019;
    }
    else if(m_type == CCNPiNN){
    	m_fill_colour = (Int_t)DrawingStyle::T1P1Pi0;
    	m_fill_style = 3025;
    }
    else if(m_type == CCNPiNPNN){
    	m_fill_colour = (Int_t)DrawingStyle::T1Pi1Pi0;
    	m_fill_style = 3024;
    }

    cout << "Post : " << "FC = " << m_fill_colour << " LC = " << m_line_colour << " FS = " << m_fill_style << " LS = " << m_line_style << endl;
}

void Topology::AddToSignal(std::string add2def, bool reset)
{
	if(reset) m_signal = add2def;
	else if(m_signal.empty()) m_signal = add2def;
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

#ifndef _TOPOLOGIES_CXX
#define _TOPOLOGIES_CXX

Topologies::Topologies(Topology::Name signal) : m_signal(signal), m_other(Topology::Other, "")
{
    m_original.clear();
    m_list2draw.clear();
}

Topologies::Topologies(Topologies * topologies) : m_signal( topologies->GetSignal() ), m_other(Topology::Other, "")
{
    m_original = topologies->GetOriginal();
    m_list2draw = topologies->GetList2Draw();
}

Topologies::~Topologies()
{
    m_original.clear();
    m_list2draw.clear();
}

void Topologies::AddTopology(Topology::Name name, std::string definition)
{
	bool add = true;
	for(unsigned int i = 0; i < m_original.size(); i++){
		if(m_original[i].GetType() == name){ 
			add = false; 
			break; 
		}
	}

	if(add)	m_original.push_back( Topology(name, definition) );
	else cout << "Topologies::AddTopology : Already added " << Topology::ToString(name) << ". Not adding to list." << endl;
}

void Topologies::AddSignalToTopology(Topology::Name name, std::string add2def, bool reset)
{
	for(unsigned int i = 0; i < m_original.size(); i++){
		if(m_original[i].GetType() == name){ 
			m_original[i].AddToSignal(add2def, reset); 
			break; 
		}
	}
}

std::vector<Topology> Topologies::GetList()
{ 
	std::vector<Topology> list;
	m_other.Reset();
	for(unsigned int i = 0; i < m_original.size(); i++){ 
		list.push_back(m_original[i]);
        if(m_original[i].GetType() != m_signal) m_other.AddToSignal( ("!(" + m_original[i].GetSignal() + ")") ); 
	}
	list.push_back( m_other );
	return list;
}


Topology Topologies::GetTopology(Topology::Name name)
{
	Topology top;

	if(name == Topology::Other) m_other.Reset();

	for(unsigned int i = 0; i < m_original.size(); i++){
		
		if(name == Topology::Other && m_original[i].GetType() != m_signal) m_other.AddToSignal( "!(" + m_original[i].GetSignal() + ")" );
		else if(m_original[i].GetType() == name){ 
			top = m_original[i];
			break; 
		}
	}

	if(name == Topology::Other) top = m_other;
    // if(name == Topology::Unknown) cout << "!!!!!! ------------- Top not changes for initialisation ------------- !!!!!!" << endl;
    // cout << " top = " << top.GetName() << " signal = " << top.GetSignal() << endl;

	return top;
}

#endif

#ifndef _PARTPDGInfo_CXX
#define _PARTPDGInfo_CXX

PDGInfo::PDGInfo(Int_t part_pdg, std::string part_name, std::string part_symbol) : m_pdg(part_pdg), m_name(part_name), m_symbol(part_symbol), m_line_colour(1), m_line_style(1) {
    stringstream ss;
    ss << m_pdg;
    m_pdg_s = ss.str();
    
    if(m_name.find("proton") != std::string::npos)        m_colour = (Int_t)DrawingStyle::Proton;//Proton
    else if(m_name.find("piplus") != std::string::npos)   m_colour = (Int_t)DrawingStyle::PionP;//PionP
    else if(m_name.find("piminus") != std::string::npos)  m_colour = (Int_t)DrawingStyle::PionM;
    else if(m_name.find("pion") != std::string::npos)     m_colour = (Int_t)DrawingStyle::Pion;
    else if(m_name.find("amuon") != std::string::npos)    m_colour = (Int_t)DrawingStyle::MuonP;//PionP
    else if(m_name.find("muon") != std::string::npos)     m_colour = (Int_t)DrawingStyle::MuonM;
    else if(m_name.find("pizero") != std::string::npos)   m_colour = (Int_t)DrawingStyle::Pi0;
    else if(m_name.find("kapm") != std::string::npos)     m_colour = (Int_t)DrawingStyle::Kaon;
    else if(m_name.find("kazero") != std::string::npos)   m_colour = (Int_t)DrawingStyle::Ka0;
    else m_colour = (Int_t)DrawingStyle::Other; 
}
#endif
