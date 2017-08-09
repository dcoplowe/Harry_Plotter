#ifndef __HARRY_PLOTTER__CXX
#define __HARRY_PLOTTER__CXX

#include <Harry_Plotter.hxx>

#include <ReadParam.hxx>
#include <iostream>

using std::string;
using std::endl;
using std::cout;

Harry_Plotter::Harry_Plotter()
{

	string opts_file = string( getenv("HP_ROOT") );
    opts_file += "/parameters/run_options.txt";

    cout << "opts_file = " << opts_file <<endl;
    int dcool = ReadParam::GetParameterI("david is cool", opts_file);
    string cuts = ReadParam::GetParameterS("cuts", opts_file);
    double numbers = ReadParam::GetParameterD("numbers", opts_file);

    cout << "david is cool = " << dcool << endl;
    cout << "cuts = " << cuts << endl;
    cout << "numbers = " << numbers << endl;

	string plots_file = string( getenv("HP_ROOT") );
    plots_file += "/parameters/plot_list.txt";

    cout << "plots_file = " << plots_file <<endl;
    std::vector<string> plot_list = ReadParam::ReadFile(plots_file);

    std::vector<ReadParam*> list;

    for(size_t i = 0; i < plot_list.size(); i++){
    	list.push_back( new ReadParam(plot_list[i], ReadParam::kPlot) );
    }

}

Harry_Plotter::~Harry_Plotter()
{

}

#endif