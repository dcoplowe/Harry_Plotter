#include <CompareMC.hxx>

#include <string>     // std::string, std::stoi
#include <cctype> // no idea.
#include <stdlib.h> //std::atoi ?
#include <fstream> //ifstream header

// const string testing("nuwro_carbon_p14_1GeV_noFSI_rawformat.txt");
const std::string testing("lists/neut_carbon_p14_1GeV_RES_noFSI.txt");

// class GeneratorInfo;
int ReWeight::OneDCounter = 0;
int ReWeight::TwoDCounter = 0;

Int_t DrawingTools::s_1Dcounter = 0;
Int_t DrawingTools::s_2Dcounter = 0;

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

	CompareMC * run = new CompareMC(ifile);

	if(make_flattree) run->BuildAnalysisTrees();
	
	if(analyse_mc) run->AnalyseMCs();

	if(make_plots){
		if(!plot_range.empty()){
			
			std::string first_s = plot_range.substr(0, 1);
			if(plot_range.find("-") != std::string::npos){
				
				std::string last_s = plot_range.substr(2, 1);
				first = std::atoi(first_s.c_str());
				last = std::atoi(last_s.c_str());
			}
			else{
				first = std::atoi(first_s.c_str());
				last = first;
			}
		}

		if(first > last){
			cout << "Plot range is not correct: (first < last) we have " << first << " > " << last << endl;
			exit(0);
		}
		run->RunComparisons(first, last);
	}
	
	if(run_reweight) run->RunReweight();
	
	delete run;

    return 1;
}