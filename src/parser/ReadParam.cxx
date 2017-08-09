#ifndef __READPARAM__CXX__
#define __READPARAM__CXX__

#include <ReadParam.hxx>
#include <sstream>
#include <iostream> 
#include <cctype>
#include <stdlib.h>
#include <fstream> //ifstream header
#include <cassert>

using std::string;
using std::cout;
using std::endl;

ReadParam::ReadParam(const std::string instring, Type type, string left_arrow, string right_arrow)
{
    m_instring = instring;
    RemoveArrows(m_instring, left_arrow, right_arrow);

    m_Is_TwoD = false;
    if(type < kPlot){
        bool good_var2rw_done = false;
        bool good_rwvar1_done = false;
    // bool good_rwvar2_done = false;

        int  counter = 0;

    // std::vector<std::string> list;
        string word;
        std::stringstream iss(m_instring);
        while( iss >> word )     
        {
            if(!IsNumber(word)){
                if(!m_var2rw.good){
                    m_var2rw.good = true;
                    m_var2rw.var = word;
                // counter++;
                }
                else if(!m_varOne.good) {
                    m_varOne.good = true;
                    m_varOne.var = word;
                    counter = 0;
                    good_var2rw_done = true;
                }
                else if(!m_varTwo.good) {
                    m_varTwo.good = true;
                    m_varTwo.var = word;
                    counter = 0;
                    good_rwvar1_done = true;
                }
            }
            else{
                if(m_var2rw.good && !good_var2rw_done){
                    if(counter == 1) m_var2rw.nbins = atoi(word.c_str());
                    else m_var2rw.bvals.push_back( atof( word.c_str() ) );
                }
                else if(m_varOne.good && !good_rwvar1_done){
                    if(counter == 1) m_varOne.nbins = atoi(word.c_str());
                    else m_varOne.bvals.push_back( atof( word.c_str() ) );
                }
                else if(m_varTwo.good){
                    if(counter == 1) m_varTwo.nbins = atoi(word.c_str());
                    else m_varTwo.bvals.push_back( atof( word.c_str() ) );
                }
            }
            counter++;
        }    

        if(m_var2rw.good){
            m_var2rw.bins = DetermineBins(m_var2rw.nbins, m_var2rw.bvals);
        }

        if(m_varOne.good){
            m_varOne.bins = DetermineBins(m_varOne.nbins, m_varOne.bvals);
        }

        if(m_varTwo.good){
            m_Is_TwoD = true;
            m_varTwo.bins = DetermineBins(m_varTwo.nbins, m_varTwo.bvals);
        }

        if(type == kReweight){
            if(m_varTwo.good){
                cout << __FILE__ << ":" << __LINE__ << " : ERROR : Could not interpret params! Too many parameters..." << endl;
                exit(0);
            }

            if(m_varOne.good){
                m_varTwo.var = m_varOne.var;
                m_varTwo.title = m_varOne.title;
                m_varTwo.units = m_varOne.units;
                m_varTwo.nbins = m_varOne.nbins;

                m_varTwo.bins = new double [ m_varTwo.nbins + 1 ];
                for(int i = 0; i < m_varTwo.nbins + 1; i++) m_varTwo.bins[i] = m_varOne.bins[i];

                    m_varTwo.good = true;
                m_Is_TwoD = true;

                m_varOne.var = "";
                m_varOne.nbins = -999;
                delete [] m_varOne.bins;
            }
            else m_Is_TwoD = false;

            if(m_var2rw.good){
                m_varOne.var = m_var2rw.var;
                m_varOne.title = m_var2rw.title;
                m_varOne.units = m_var2rw.units;
                m_varOne.nbins = m_var2rw.nbins;

                m_varOne.bins = new double [ m_varOne.nbins + 1 ];

                for(int i = 0; i < m_varOne.nbins + 1; i++) m_varOne.bins[i] = m_var2rw.bins[i];

                    m_varOne.good = true;
                m_var2rw.good = false;

                m_var2rw.var = "";
                m_var2rw.nbins = -999;
                delete [] m_var2rw.bins;
            }
        }

        bool lprint = false;

        if(lprint){
            cout << "------------- SUMMARY -------------" << endl;
            if(type == kReweight){
                cout << "RW Var: ";
                m_var2rw.Print();
            }

            string rw_string = "";
            if(type == kReweight) rw_string = "RW";

            if(m_varOne.good){
                cout << "Var " << rw_string << "1: ";
                m_varOne.Print();
            }

            if(m_varTwo.good){
                cout << "Var " << rw_string << "2: ";
                m_varTwo.Print();
            }    
            cout << "-----------------------------------" << endl;
        }
    }
    else if(type == kPlot){
        // Required form: var symbol units nbins bins : var symbol units nbins bins (include : for 2D)
    }




}

ReadParam::~ReadParam()
{
    delete [] m_var2rw.bins;
    delete [] m_varOne.bins;
    delete [] m_varTwo.bins;
}

double * ReadParam::SetBinning(int nbins, double low, double high){
    // cout << "ReadParam::SetBinning(" << nbins << ", " << low << ", " << high <<")" << endl;

    double * bins = new double[ nbins + 1 ];
    
    double range = high - low;
    double binwidth = range/(double)nbins;
    
    for (int i=0; i < nbins + 1; i++) {
        bins[i] = low + binwidth*i;
        // if(m_verbose) cout << "Array Element: " << i << " : " << bins[i] << endl;
    }
    return bins;
}

bool ReadParam::IsNumber(std::string in)
{
    bool yes = false;

    if(isdigit( in[0] )) yes = true;
    else if(in.length() > 1 && in[0] == '.'){
        if(isdigit( in[1] ) ) yes = true;
    }
    else if(in.length() > 1 && in[0] == '-'){
        if(isdigit( in[1] ) ) yes = true;
        else if(in.length() > 2 && isdigit( in[2] ) ) yes = true;
    }
    return yes;
}

std::vector<std::string> ReadParam::ReadFile(std::string infile)
{
    string tmp_line = "";
    bool complete = false;

    std::ifstream fin( infile.c_str() );
    assert(fin.is_open());
    string line;

    std::vector<std::string> list;
    list.clear();

    while (getline(fin, line))
    {
        // cout << "Line: " << line << endl;
        if(line.find("<|") != std::string::npos){
            // cout << "Starting to read in RW" << endl;
            tmp_line = line;
            if(line.find("|>") != std::string::npos) complete = true;
        }
        else if(line.find("|>") != std::string::npos){
            tmp_line += line;
            complete = true;
        }
        else{
            tmp_line += line;
        }

        if(complete){
            list.push_back( tmp_line );
            tmp_line = "";
            complete = false;
            // cout << "push_back success" << endl;
        }
    }
    fin.close();

    return list;
}

double * ReadParam::DetermineBins(int nbins, std::vector<double> binning)
{
    double * p_binning = 0x0;
    if(binning.size() == 2) p_binning = SetBinning(nbins, binning[0], binning[1]);
    else if( (int)binning.size() == nbins + 1){
        p_binning = new double [ nbins + 1 ];
        for(int i = 0; i < nbins + 1; i++) p_binning[i] = binning[i]; 
    }
    else{
        cout << __FILE__ << ":" << __LINE__ << " : ERROR : Could not make array: nbins = " << nbins << ", array size = " << binning.size() << endl;
        exit(0);       
    }
    return p_binning;
}

void ReadParam::RemoveArrows(std::string word, std::string left_arrow, std::string right_arrow)
{
    // Add | to both arrows as we may/will have instances where the comparison symbols (</>) are used. 
    size_t larrow = word.find(left_arrow);
    size_t rarrow = word.find(right_arrow);
    if(larrow != -1 && rarrow != -1){
        size_t l_length = left_arrow.size();
        size_t r_length = right_arrow.size();
        size_t length = rarrow - larrow - r_length;
        word = word.substr(larrow + l_length, length);
    }
    else{
        cout << __FILE__ << ":" << __LINE__ << ": ERROR : Could not find left or right arrow(s)." << endl;
        exit(0);
    }
}

std::string ReadParam::GetParameterS(std::string name, std::string infile, std::string left_arrow, std::string right_arrow)
{
    std::vector<std::string> list = ReadFile(infile);

    int store = -999;
    bool called = false;
    for(size_t i = 0; i < list.size(); i++){
        string tmp = list[i];
        if(tmp.find(name) != string::npos && !called){ 
            store = i;
            called = true;
        }
        else if(called){
            cout << __FILE__ << ":" << __LINE__ << " : ERROR : Multiple instances of param name " << name << " in parameters file (" << infile << ")" << endl;
        }
    }

    string param = list[store];
    RemoveArrows(param, left_arrow, right_arrow);

    size_t place = param.find("=");
    if(place != string::npos){
        param = param.erase(0, place);
    }
    else{
        cout << __FILE__ << ":" << __LINE__ << ": ERROR : Parameter not in correct form (missing =), we have: " << param << endl;
        exit(0);
    }
    return param;
}

int ReadParam::GetParameterI(std::string name, std::string infile, std::string left_arrow, std::string right_arrow)
{
    string param = GetParameterS(name, infile, left_arrow, right_arrow);
    if(!IsNumber(param)){
        cout << __FILE__ << ":" << __LINE__ << ": ERROR : Parameter is not a number, we have: " << param << endl;
        exit(0);
    }
    return atoi(param.c_str());
}

double ReadParam::GetParameterD(std::string name, std::string infile, std::string left_arrow, std::string right_arrow)
{
    string param = GetParameterS(name, infile, left_arrow, right_arrow);
    if(!IsNumber(param)){
        cout << __FILE__ << ":" << __LINE__ << ": ERROR : Parameter is not a number, we have: " << param << endl;
        exit(0);
    }
    return atof(param.c_str());
}

#endif