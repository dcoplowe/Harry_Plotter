#include <Harrry_Plotter.hxx>

#include <string>     // std::string, std::stoi
#include <cctype> // no idea.
#include <stdlib.h> //std::atoi ?
#include <fstream> //ifstream header

// const string testing("nuwro_carbon_p14_1GeV_noFSI_rawformat.txt");
const std::string testing("lists/neut_carbon_p14_1GeV_RES_noFSI.txt");

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

	Harrry_Plotter * run = new Harrry_Plotter();

    return 1;
}