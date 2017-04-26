#include "DataClasses.h"

#include <iostream>//cout/endl headers
#include "DrawingStyle.h"
#include "TMath.h"

using std::cout;
using std::endl;

#ifndef _PRINT_LEVEL_CXX
#define _PRINT_LEVEL_CXX

namespace print_level {
    bool debug = true;// = true;// = true;
    bool quiet = false;
    bool effpurtools = false;
    bool drawingtools = false;
}

#endif


#ifndef _EXPERIMENT_CXX
#define _EXPERIMENT_CXX

Experiment::Experiment(Name exp) : m_type(exp), m_name( ToString(m_type) )
{
    m_topologies = new Topologies();
    m_intmodes.clear();
    m_cuts.clear();

    if(exp == Experiment::MIN){

        m_logo = m_name + " Preliminary";

        m_reconame = "sel";
        m_truename = "Truth";

        m_signal  = "truth_n_pro == 1 && truth_n_piP == 1 && truth_n_muo == 1 && mc_nFSPart == 3 && mc_targetZ == 1";
        m_signal += " && mc_current == 1 && TMath::RadToDeg()*truth_mu_Theta < 25. && TMath::RadToDeg()*truth_mu_Theta >= 0.";
        m_signal += " && truth_true_target_region == 1 && truth_mu_E < 20000. && truth_mu_E > 0.";

        m_signaldef = "H-CC1p1#pi^{+} (1.5 < E_{#mu} GeV < 20,  #theta_{#mu} < 25 Deg.)"; 

        m_tag = "sel";

        m_tarvarname = "mc_targetZ";

        m_weight = "1";
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

        m_weight = "1";
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

        if(print_level::quiet) cout << " Adding Topology = " << Topology::ToString(Topology::CC1P1PiPlus) << endl;

    	m_topologies->AddTopology(		  Topology::CC1P1PiPlus, "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1");
    	m_topologies->AddSignalToTopology(Topology::CC1P1PiPlus, "truth_n_ntn == 0 && truth_n_pho == 0 && truth_n_pi0 == 0 && truth_n_piM == 0");
    	m_topologies->AddSignalToTopology(Topology::CC1P1PiPlus, "truth_n_piP == 1 && truth_n_pro == 1 && truth_n_tau == 0 && mc_nFSPart == 3");

        if(print_level::quiet) cout << " Adding Topology = " << Topology::ToString(Topology::HCC1P1PiPlus) << endl;

    	m_topologies->AddTopology(		  Topology::HCC1P1PiPlus,  m_topologies->GetTopology(Topology::CC1P1PiPlus).GetSignal() );
    	m_topologies->AddSignalToTopology(Topology::HCC1P1PiPlus, "mc_targetZ == 1 && true_target_region == 1 && truth_mu_E < 20000.");
    	m_topologies->AddSignalToTopology(Topology::HCC1P1PiPlus, "truth_mu_E > 0.");

        if(print_level::quiet) cout << " Adding Topology = " << Topology::ToString(Topology::CCNPNPiMinus) << endl;

    	m_topologies->AddTopology(		  Topology::CCNPNPiMinus, "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1");
    	m_topologies->AddSignalToTopology(Topology::CCNPNPiMinus, "truth_n_ntn == 0 && truth_n_pho == 0 && truth_n_pi0 == 0 && truth_n_piM > 0");
    	m_topologies->AddSignalToTopology(Topology::CCNPNPiMinus, "truth_n_piP == 0 && truth_n_pro > 0 && truth_n_tau == 0 &&  mc_nFSPart > 0");

        if(print_level::quiet) cout << " Adding Topology = " << Topology::ToString(Topology::CCNP) << endl;

    	m_topologies->AddTopology(		  Topology::CCNP, "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1 ");
    	m_topologies->AddSignalToTopology(Topology::CCNP, "truth_n_ntn == 0 && truth_n_pho == 0 && truth_n_pi0 == 0 && truth_n_piM == 0 ");
    	m_topologies->AddSignalToTopology(Topology::CCNP, "truth_n_piP == 0 && truth_n_pro > 0 && truth_n_tau == 0 && mc_nFSPart > 0");

        if(print_level::quiet) cout << " Adding Topology = " << Topology::ToString(Topology::CCNPiPlus) << endl;

    	m_topologies->AddTopology(		  Topology::CCNPiPlus, "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1");
    	m_topologies->AddSignalToTopology(Topology::CCNPiPlus, "truth_n_ntn == 0 && truth_n_pho == 0 && truth_n_pi0 == 0 && truth_n_piM == 0");
    	m_topologies->AddSignalToTopology(Topology::CCNPiPlus, "truth_n_piP > 0 && truth_n_pro == 0 && truth_n_tau == 0 && mc_nFSPart > 0");

        if(print_level::quiet) cout << " Adding Topology = " << Topology::ToString(Topology::CCNPNPiZero) << endl;

		m_topologies->AddTopology(		  Topology::CCNPNPiZero, "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1");
    	m_topologies->AddSignalToTopology(Topology::CCNPNPiZero, "truth_n_ntn == 0 && truth_n_pho == 0 && truth_n_pi0 > 0 && truth_n_piM == 0");
    	m_topologies->AddSignalToTopology(Topology::CCNPNPiZero, "truth_n_piP == 0 && truth_n_pro > 0 && truth_n_tau == 0 && mc_nFSPart > 0");

        if(print_level::quiet) cout << " Adding Topology = " << Topology::ToString(Topology::CCNPiZeroNPiPlus) << endl;

		m_topologies->AddTopology(		  Topology::CCNPiZeroNPiPlus, "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1");
    	m_topologies->AddSignalToTopology(Topology::CCNPiZeroNPiPlus, "truth_n_ntn == 0 && truth_n_pho == 0 && truth_n_pi0 > 0 && truth_n_piM == 0");
    	m_topologies->AddSignalToTopology(Topology::CCNPiZeroNPiPlus, "truth_n_piP > 0 && truth_n_pro == 0 && truth_n_tau == 0 && mc_nFSPart > 0");

        if(print_level::quiet) cout << " Adding Topology = " << Topology::ToString(Topology::CCKaonsOth) << endl;

		m_topologies->AddTopology(		  Topology::CCKaonsOth, "truth_n_ele == 0 && (truth_n_kPM > 0 || truth_n_kaO > 0) && truth_n_muo == 1");
    	m_topologies->AddSignalToTopology(Topology::CCKaonsOth, "(truth_n_ntn > 0 || truth_n_pho > 0 || truth_n_pi0 > 0 || truth_n_piM > 0 ");
    	m_topologies->AddSignalToTopology(Topology::CCKaonsOth, "truth_n_piP > 0 && truth_n_pro == 0 && truth_n_tau == 0 && mc_nFSPart > 0 || truth_n_piP > 0 || truth_n_pro > 0)");
    	m_topologies->AddSignalToTopology(Topology::CCKaonsOth, "truth_n_tau == 0 && mc_nFSPart > 0");

        if(print_level::quiet) cout << " Adding Topology = " << Topology::ToString(Topology::CCNN) << endl;

    	m_topologies->AddTopology(        Topology::CCNN, "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1");
    	m_topologies->AddSignalToTopology(Topology::CCNN, "truth_n_ntn > 0 && truth_n_pho == 0 && truth_n_pi0 == 0 && truth_n_piM == 0");
    	m_topologies->AddSignalToTopology(Topology::CCNN, "truth_n_piP == 0 && truth_n_pro == 0 && truth_n_tau == 0 && mc_nFSPart > 0");

        if(print_level::quiet) cout << " Adding Topology = " << Topology::ToString(Topology::CCNPNN) << endl;

    	m_topologies->AddTopology(        Topology::CCNPNN, "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1");
    	m_topologies->AddSignalToTopology(Topology::CCNPNN, "truth_n_ntn > 0 && truth_n_pho == 0 && truth_n_pi0 == 0 && truth_n_piM == 0");
    	m_topologies->AddSignalToTopology(Topology::CCNPNN, "truth_n_piP == 0 && truth_n_pro > 0 && truth_n_tau == 0 && mc_nFSPart > 0");

        if(print_level::quiet) cout << " Adding Topology = " << Topology::ToString(Topology::CCNPiNN) << endl;

    	m_topologies->AddTopology(        Topology::CCNPiNN, "truth_n_ele == 0 && truth_n_kPM == 0 && truth_n_kaO == 0 && truth_n_muo == 1");
    	m_topologies->AddSignalToTopology(Topology::CCNPiNN, "truth_n_ntn > 0 && truth_n_pho == 0");
    	m_topologies->AddSignalToTopology(Topology::CCNPiNN, "(truth_n_pi0 > 0 || truth_n_piM > 0 || truth_n_piP > 0)");
    	m_topologies->AddSignalToTopology(Topology::CCNPiNN, "truth_n_pro == 0 && truth_n_tau == 0 && mc_nFSPart > 0");

        if(print_level::quiet) cout << " Adding Topology = " << Topology::ToString(Topology::CCNPiNPNN) << endl;

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

        if(print_level::quiet) cout << " Adding Topology = " << Topology::ToString(Topology::CC1P1PiPlus) << endl;

        m_topologies->AddTopology(        Topology::CC1P1PiPlus, "true_ntracks == 3 && truemu_ntracks == 1 && truep_ntracks == 1 && truepi_ntracks == 1");

        if(print_level::quiet) cout << " Adding Topology = " << Topology::ToString(Topology::HCC1P1PiPlus) << endl;

        m_topologies->AddTopology(        Topology::HCC1P1PiPlus,  m_topologies->GetTopology(Topology::CC1P1PiPlus).GetSignal() );
        m_topologies->AddSignalToTopology(Topology::HCC1P1PiPlus, "target == 1 && selmu_mom > 250 && selp_mom > 450 && selpi_mom > 250");

        // if(print_level::quiet) cout << " Adding Topology = " << Topology::ToString(Topology::HCC1P1PiPlusOOPS) << endl;
        // m_topologies->AddTopology(        Topology::HCC1P1PiPlusOOPS,  m_topologies->GetTopology(Topology::HCC1P1PiPlusOOPS).GetSignal() );
        // m_topologies->AddSignalToTopology(Topology::HCC1P1PiPlusOOPS, "target == 1 && (selmu_mom < 250 || selp_mom < 450 || selpi_mom < 250)");

        if(print_level::quiet) cout << " Adding Topology = " << Topology::ToString(Topology::HCC1P1PiPlus) << endl;
        m_topologies->AddTopology(        Topology::CCNP, "true_ntracks > 0 && truemu_ntracks == 1 && truep_ntracks > 0 && truepi_ntracks == 0" );

        if(print_level::quiet) cout << " Adding Topology = " << Topology::ToString(Topology::HCC1P1PiPlus) << endl;
        m_topologies->AddTopology(   Topology::CCNPiPlus, "true_ntracks > 0 && truemu_ntracks == 1 && truep_ntracks == 0 && truepi_ntracks > 0" );

        // CC1P1PiPlus,
        // CCNPNPiMinus,
        // CCNP,
        // CCNPiPlus,
        // CCNPNPiZero,
        // CCNPiZeroNPiPlus,
        // //Added topos (200217):
        // CCKaonsOth,
        // CCNN,//N Neutrons
        // CCNPNN,//N Protons, N Neutrons
        // CCNPiNN, //N pions, N Neutrons
        // CCNPiNPNN, //N pions, N Protons N Neutrons
        // //CCNucBreakUp, -- may be nice in the future
        // Other,

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

    std::string basecuts = "accum_level";
    basecuts += (m_type == Experiment::T2K ? "[0]" : "");
    basecuts += "[" + br.str() + "] > " + ac.str(); 

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

        m_pdg = Particle::MuonM;
    }
    else if(name.find("pi") != std::string::npos){ 
        m_name = "Pion"; 
        m_symbol = "#pi^{+}";
        m_pdg = Particle::PionP;
    }
    else if(name.find("p") != std::string::npos){ 
        m_name = "Proton"; 
        m_symbol = "p";
        m_pdg = Particle::Proton;

    }
    else{ 
        m_name = "Unknown";
        m_symbol = "UN";
    }

    m_label = "";

    if(exp == Experiment::T2K){
        //Reco vars: Common:
        if(m_pdg == Particle::Proton || m_pdg == Particle::PionP){            
            P           = Variable(m_tag + name + "_mom", 20, 0., 2000.);
                // Varaible ranges/binning need improving:
            pT          = Variable(m_tag + name + "_pT", 20, 0., 2000.);
            pTMag       = Variable(m_tag + name + "_pTMag", 20, 0., 2000.);
            startdir    = Variable(m_tag + name + "_dir", 20, 0., 2000.);
            endpos      = Variable(m_tag + name + "_endpos", 20, 0., 2000.);

                //True vars: Common:
            trueP           = Variable(m_tag + name + "_truemom", 20, 0., 2000.);
            truepT          = Variable(m_tag + name + "_truepT", 20, 0., 2000.);
            truepTMag       = Variable(m_tag + name + "_truepTMag", 20, 0., 2000.);
            truestartdir    = Variable(m_tag + name + "_truedir", 20, 0., 2000.);
            trueendpos      = Variable(m_tag + name + "_endpos", 20, 0., 2000.);
        }
        else if(m_pdg == Particle::MuonM){
            P           = Variable(m_tag + name + "_mom", 20, 0., 2000.);
                // Varaible ranges/binning need improving:
            pT          = Variable(m_tag + name + "_pT", 20, 0., 2000.);
            pTMag       = Variable(m_tag + name + "_pTMag", 20, 0., 2000.);
            startdir    = Variable(m_tag + name + "_dir", 20, 0., 2000.);
            endpos      = Variable(m_tag + name + "_endpos", 20, 0., 2000.);

                //True vars: Common:
            trueP           = Variable(m_tag + name + "_truemom", 20, 0., 2000.);
            truepT          = Variable(m_tag + name + "_truepT", 20, 0., 2000.);
            truepTMag       = Variable(m_tag + name + "_truepTMag", 20, 0., 2000.);
            truestartdir    = Variable(m_tag + name + "_truedir", 20, 0., 2000.);
            trueendpos      = Variable(m_tag + name + "_endpos", 20, 0., 2000.);


        }

        pdg = Variable(m_tag + name + "_pdg", 20, 0., 2000.);
        
        // Varaible ranges/binning need improving:

        //MIN: Reco vars:
        E = Variable();
        P4 = Variable();
        T = Variable();//KE
        pTT = Variable();
        theta = Variable();
        startpos = Variable();
        phi = Variable();
        
        michel = Variable();
        score = Variable();
        
        //MIN: True vars:
        trueE = Variable();
        trueP4 = Variable();
        trueT = Variable();//KE
        truepTT = Variable();
        truetheta = Variable();
        truestartpos = Variable();
        truephi = Variable();
        
        //T2K: Reco vars:
        ctheta       =  Variable(m_tag + name + "_costheta", 20, -1., 1.);
        ctheta_nudir =  Variable(m_tag + name + "_nudir_costheta", 20, -1., 1);
        
        //T2K: Reco vars:
        truectheta =    Variable(m_tag + name + "_truecostheta", 20, -1., 1.);
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

        if(m_pdg == Particle::Proton || m_pdg == Particle::PionP){        
            P = Variable(m_tag + name + "_mom", 40,  0., 2000.);
            pT =        Variable(m_tag + name + "_pT", 40, 0., 2000.);
            pTMag =     Variable(m_tag + name + "_pTMag", 40, 0., 2000.);

                // True vars: Common:
            trueP =         Variable(m_tag + tmpname + "_truemom", 40, 0., 2000.);
            truepT =        Variable(m_tag + tmpname + "_truepT", 40, 0., 2000.);
            truepTMag =     Variable(m_tag + tmpname + "_truepTMag", 40, 0., 2000.);

            startdir =  Variable(m_tag + tmpname + "_startdir", 40, 0., 2000.);
            endpos =    Variable(m_tag + tmpname + "_endpos", 40, 0., 2000.);
            
            truestartdir =  Variable(m_tag + tmpname + "_truestartdir", 40, 0., 2000.);
            trueendpos =    Variable(m_tag + tmpname + "_endpos", 40, 0., 2000.);
            
                //MIN: Reco vars:
            E =     Variable(m_tag + name + "_E", 40, 0., 2000.);
            P4 =    Variable(m_tag + name + "_4mom", 40, 0., 2000.);
            T =     Variable(m_tag + name + "_KE", 40, 0., 2000.);//KE
            pTT =   Variable(m_tag + name + "_pTT", 40, 0., 2000.);
            startpos = Variable(m_tag + tmpname + "_startpos", 40, 0., 2000.);
            truestartpos =  Variable(m_tag + tmpname + "_truestartpos", 40, 0., 2000.);

                //MIN: True vars:
            trueE =         Variable(m_tag + tmpname + "_trueE", 40, 0., 2000.);
            trueP4 =        Variable(m_tag + tmpname + "_true4mom", 40, 0., 2000.);
            trueT =         Variable(m_tag + tmpname + "_trueKE", 40, 0., 2000.);//KE
            truepTT =       Variable(m_tag + tmpname + "_truepTT", 40, 0., 2000.);
        }
        else if(m_pdg == Particle::MuonM){        
            P = Variable(m_tag + name + "_mom", 40,  0., 20000.);
            pT =        Variable(m_tag + name + "_pT", 40, 0., 2000.);
            pTMag =     Variable(m_tag + name + "_pTMag", 40, 0., 2000.);

                // True vars: Common:
            trueP =         Variable(m_tag + tmpname + "_truemom", 40, 0., 2000.);
            truepT =        Variable(m_tag + tmpname + "_truepT", 40, 0., 2000.);
            truepTMag =     Variable(m_tag + tmpname + "_truepTMag", 40, 0., 2000.);

            startdir =  Variable(m_tag + tmpname + "_startdir", 40, 0., 2000.);
            endpos =    Variable(m_tag + tmpname + "_endpos", 40, 0., 2000.);
            
            truestartdir =  Variable(m_tag + tmpname + "_truestartdir", 40, 0., 2000.);
            trueendpos =    Variable(m_tag + tmpname + "_endpos", 40, 0., 2000.);
            
                //MIN: Reco vars:
            E =     Variable(m_tag + name + "_E", 40, 0., 2000.);
            P4 =    Variable(m_tag + name + "_4mom", 40, 0., 2000.);
            T =     Variable(m_tag + name + "_KE", 40, 0., 2000.);//KE
            pTT =   Variable(m_tag + name + "_pTT", 40, 0., 2000.);
            startpos = Variable(m_tag + tmpname + "_startpos", 40, 0., 2000.);
            truestartpos =  Variable(m_tag + tmpname + "_truestartpos", 40, 0., 2000.);

                //MIN: True vars:
            trueE =         Variable(m_tag + tmpname + "_trueE", 40, 0., 2000.);
            trueP4 =        Variable(m_tag + tmpname + "_true4mom", 40, 0., 2000.);
            trueT =         Variable(m_tag + tmpname + "_trueKE", 40, 0., 2000.);//KE
            truepTT =       Variable(m_tag + tmpname + "_truepTT", 40, 0., 2000.);
        }

        pdg =           Variable(m_tag + tmpname + "_PDG", 40, 0., 2000.);
        theta =         Variable(m_tag + name + "_Theta", 40, 0, TMath::TwoPi());
        phi =           Variable(m_tag + name + "_Phi", 40, -TMath::Pi(), TMath::Pi());
        truetheta =     Variable(m_tag + tmpname + "_trueTheta", 40, 0, TMath::TwoPi());
        truephi =       Variable(m_tag + tmpname + "_truePhi", 40, -TMath::Pi(), TMath::Pi());
        
        michel =    Variable(m_tag + name + "_michel", 2, 0., 2000.);
        score =     Variable(m_tag + name + "_score", 40, -100., -100.);

        //T2K: Reco vars:
        ctheta = Variable();
        ctheta_nudir = Variable();
        
        //T2K: Reco vars:
        truectheta = Variable();
    }
    else{
        //Reco vars: Common:
        P = Variable();
        pT = Variable();
        pTMag = Variable();
        startdir = Variable();
        endpos = Variable();

        //True vars: Common:
        trueP = Variable();
        truepT = Variable();
        truepTMag = Variable();
        truestartdir = Variable();
        trueendpos = Variable();
        pdg = Variable();

        //MIN: Reco vars:
        E = Variable();
        P4 = Variable();
        T = Variable();//KE
        pTT = Variable();
        theta = Variable();
        startpos = Variable();
        phi = Variable();

        michel = Variable();
        score = Variable();

        //MIN: True vars:
        trueE = Variable();
        trueP4 = Variable();
        trueT = Variable();//KE
        truepTT = Variable();
        truetheta = Variable();
        truestartpos = Variable();
        truephi = Variable();

        //T2K: Reco vars:
        ctheta = Variable();
        ctheta_nudir = Variable();

        //T2K: Reco vars:
        truectheta = Variable();
    }
    
}

#endif


#ifndef _KINEMATICVARS_CXX
#define _KINEMATICVARS_CXX

KinematicVars::KinematicVars(Experiment::Name exp){
    
    if(exp == Experiment::T2K){
        muon = new Particle(    exp, "mu", "sel");
        proton = new Particle(  exp, "p",  "sel");
        pion = new Particle(    exp, "pi", "sel");
        
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
m_symbol(ToString(m_type ,1)), m_fill_colour(0), m_fill_style(1001), m_line_colour(1), m_line_style(1)
{
	m_signal = definition;
    // cout << "Post : " << "FC = " << m_fill_colour << " LC = " << m_line_colour << " FS = " << m_fill_style << " LS = " << m_line_style << endl;

	if(m_type == HCC1P1PiPlus){
		m_line_style = 2;
		m_line_colour = 1;
		m_fill_style = 0;
	}
    else if(m_type == CC1P1PiPlus) m_fill_colour = (Int_t)DrawingStyle::T1P1PiP;//Proton
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
    // cout << "Post : " << "FC = " << m_fill_colour << " LC = " << m_line_colour << " FS = " << m_fill_style << " LS = " << m_line_style << endl;
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
