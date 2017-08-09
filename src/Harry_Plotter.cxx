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

    int dcool = ReadParam::GetParamterI("david is cool", opts_file);
    string cuts = ReadParam::GetParamterS("cuts", opts_file);
    double numbers = ReadParam::GetParamterD("numbers", opts_file);

    cout << "david is cool = " << dcool << endl;
    cout << "cuts = " << cuts << endl;
    cout << "numbers = " << numbers << endl;

// 	<| david is cool = 10 |>
// <| cuts = dyx < 20. && dz == 1900.23 |>
// <| numbers = 1.0102203 |>

}

Harry_Plotter::~Harry_Plotter()
{

}

#endif