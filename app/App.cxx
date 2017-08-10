#include <Harry_Plotter.hxx>

#include <string>     // std::string, std::stoi
#include <cctype> // no idea.
#include <stdlib.h> //std::atoi ?
#include <fstream> //ifstream header
#include <TROOT.h>
#include <DrawingTools.hxx>
#include <EffPurTools.hxx>

const std::string testing("/data/t2k/coplowe/numuCC1P1PiOutput/march17/neutP6BWA/FGD1/nall_data_FGD1_type2_wome_fboff_minaclev0_270717/allTree.root");

Int_t DrawingTools::s_1Dcounter = -1;
Int_t DrawingTools::s_2Dcounter = -1;
Long64_t DrawingTools::MaxEntries = 100;// Change to 80085
int EffPurTools::Counter = -1;

int main(int argc, char *argv[])
{   
	bool make_flattree = false;
	bool analyse_mc = false;
	bool run_reweight = false;
	bool make_plots = false;
	std::string plot_range = "";
	std::string ifile = testing;
	std::string ofile = "";

	int first = 1;
	int last = 7;

	char cc;
    while((cc = getopt(argc, argv, "i:o:c::a::r::p::")) != -1){
        switch (cc){
        	case 'i': ifile = std::string(optarg); break;
        	case 'o': ofile = std::string(optarg); break;
            case 'c': make_flattree = true; break;
            case 'a': analyse_mc = true; break;
            case 'r': run_reweight = true; break;
            case 'p': make_plots = true; 
            			if(!(optarg == 0x0)) plot_range = std::string(optarg);
            			break;
            default: return 0;
        }
    }

	Harry_Plotter * run = new Harry_Plotter(ifile);
	run->Run();
	delete run;

    return 1;
}