#ifndef __HARRY_PLOTTER__HXX
#define __HARRY_PLOTTER__HXX

#include <string>
#include <vector>

class ReadParam;

class Harry_Plotter
{
public:

	Harry_Plotter();
	~Harry_Plotter();
	
private: 
	std::vector<ReadParam*> m_plots;
};
#endif

// <| dpTT #deltap_{TT} MeV/#it{c} 49 -300 300 |>
// <| truemu_mom #it{p}_{#mu} GeV/#it{c} 4 0 10 45 75 99 |>
// <| truemu_mom #it{p}_{#mu} GeV/#it{c} 4 0. 10. 45. 75. 99. : dpTT #deltap_{TT} MeV/#it{c} 4 0. .10 4.5 .75 993. |>
// <| truemu_cTheta #it{p}_{#mu} 5 -1 1 : truemu_mom 4 0 10 45 75 99 |>