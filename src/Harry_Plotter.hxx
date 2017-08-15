#ifndef __HARRY_PLOTTER__HXX
#define __HARRY_PLOTTER__HXX

#include <string>
#include <vector>
#include <map>

class ReadParam;
class TH1D;
class TH2D;
// class DrawingTools;
class EffPurTools;
class BreakdownTools;
class TFile;

class Harry_Plotter
{
public:

	Harry_Plotter(std::string infile, std::string ofile);
	~Harry_Plotter();
	
	void Run();

private: 
	std::vector<ReadParam*> m_plots;

	std::string m_filename;
	std::string m_recontree; 
	std::string m_truthtree;

	std::string m_signal;
	std::string m_Rcuts;
	std::string m_Tcuts;
	std::string	m_epcut;
	int m_branch;

	std::map<std::string,int> m_particles;
	int GetParticleColor(std::string var);

	std::string m_ofilename;
	TFile * m_outfile;

	std::string CheckCuts(ReadParam * par, int type = 0);

	BreakdownTools * m_recon;
	BreakdownTools * m_truth;
	EffPurTools * m_effpur;

	TH1D * Get1D(ReadParam * par);
	TH2D * Get2D(ReadParam * par);
};
#endif