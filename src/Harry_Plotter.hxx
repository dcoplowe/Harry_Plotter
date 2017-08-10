#ifndef __HARRY_PLOTTER__HXX
#define __HARRY_PLOTTER__HXX

#include <string>
#include <vector>

class ReadParam;
class TH1D;
class TH2D;
class DrawingTools;

class Harry_Plotter
{
public:

	Harry_Plotter(std::string infile);
	~Harry_Plotter();
	
	void Run();

private: 
	std::vector<ReadParam*> m_plots;

	std::string m_filename;
	std::string m_recontree; 
	std::string m_truthtree;

	std::string CheckCuts(ReadParam * par, bool is_recon = true);

	DrawingTools * m_recon;
	DrawingTools * m_truth;

	TH1D * Get1D(ReadParam * par);
	TH2D * Get2D(ReadParam * par);
};
#endif

// <| dpTT #deltap_{TT} MeV/#it{c} 49 -300 300 |>
// <| truemu_mom #it{p}_{#mu} GeV/#it{c} 4 0 10 45 75 99 |>
// <| truemu_mom #it{p}_{#mu} GeV/#it{c} 4 0. 10. 45. 75. 99. : dpTT #deltap_{TT} MeV/#it{c} 4 0. .10 4.5 .75 993. |>
// <| truemu_cTheta #it{p}_{#mu} 5 -1 1 : truemu_mom 4 0 10 45 75 99 |>