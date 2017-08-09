#ifndef __INPUTINTERPRETER__CXX__
#define __INPUTINTERPRETER__CXX__

#include <InputInterpreter.hxx>
#include <sstream>
#include <iostream> 
#include <cctype>
#include <stdlib.h>
#include <fstream> //ifstream header
#include <Parameters.hxx>
#include <cassert>

using std::string;
using std::cout;
using std::endl;

InputInterpreter::InputInterpreter(const std::string instring, bool run_rw)
{
    m_instring = instring;

    size_t larrow = m_instring.find("<");
    size_t rarrow = m_instring.find(">");
    if(larrow != -1 && rarrow != -1){
        size_t length = rarrow - larrow - 1;
        m_instring = m_instring.substr(larrow + 1, length);
    }
    else{
        cout << __FILE__ << ":" << __LINE__ << ": ERROR : Could not find left or right arrow(s)." << endl;
        exit(0);
    }
    // m_instring = m_instring.substr(1, m_instring.size() - 2);

    m_var2rw_nbins = -999;
    m_rwvar1_nbins = -999; 
    m_rwvar2_nbins = -999;

    m_rwvar1_bins = 0x0; 
    m_var2rw_bins = 0x0;
    m_rwvar2_bins = 0x0;

    m_Is_TwoD = false;

    bool good_var2rw = false;
    bool good_rwvar1 = false;
    bool good_rwvar2 = false;

    bool good_var2rw_done = false;
    bool good_rwvar1_done = false;
    // bool good_rwvar2_done = false;

    int  counter = 0;

    // std::vector<std::string> list;
    string word;
    std::stringstream iss(m_instring);
    while( iss >> word )     
    {
        // cout << "word = " << word;// << endl;
        // if(IsNumber(word)) cout << " Is Number!";
        // cout << endl;

        if(!IsNumber(word)){
            if(!good_var2rw){
                good_var2rw = true;
                m_var2rw = word;
                // counter++;
            }
            else if(!good_rwvar1) {
                good_rwvar1 = true;
                m_rwvar1 = word;
                counter = 0;
                good_var2rw_done = true;
            }
            else if(!good_rwvar2) {
                good_rwvar2 = true;
                m_rwvar2 = word;
                counter = 0;
                good_rwvar1_done = true;
            }
        }
        else{
            if(good_var2rw && !good_var2rw_done){
                if(counter == 1) m_var2rw_nbins = atoi(word.c_str());
                else m_var2rw_vals.push_back( atof( word.c_str() ) );
            }
            else if(good_rwvar1 && !good_rwvar1_done){
                if(counter == 1) m_rwvar1_nbins = atoi(word.c_str());
                else m_rwvar1_vals.push_back( atof( word.c_str() ) );
            }
            else if(good_rwvar2){
                if(counter == 1) m_rwvar2_nbins = atoi(word.c_str());
                else m_rwvar2_vals.push_back( atof( word.c_str() ) );
            }
        }
        counter++;
        // list.push_back(word);
    }    

    if(good_var2rw){
        m_var2rw_bins = DetermineBins(m_var2rw_nbins, m_var2rw_vals);
        // for(int i = 0; i < m_var2rw_nbins + 1; i++) cout << "m_var2rw_bins[" << i << "] == "  << m_var2rw_bins[i] << endl;
    }

    if(good_rwvar1){
        m_rwvar1_bins = DetermineBins(m_rwvar1_nbins, m_rwvar1_vals);
        // for(int i = 0; i < m_rwvar1_nbins + 1; i++) cout << "m_rwvar1_bins[" << i << "] == "  << m_rwvar1_bins[i] << endl;
    }

    if(good_rwvar2){
        m_Is_TwoD = true;
        m_rwvar2_bins = DetermineBins(m_rwvar2_nbins, m_rwvar2_vals);
        // for(int i = 0; i < m_rwvar2_nbins + 1; i++) cout << "m_rwvar2_bins[" << i << "] == "  << m_rwvar2_bins[i] << endl;
    }

    if(!run_rw){
        if(good_rwvar2){
            cout << __FILE__ << ":" << __LINE__ << " : ERROR : Could not interpret params! Too many parameters..." << endl;
            exit(0);
        }

        if(good_rwvar1){
            m_rwvar2 = m_rwvar1;
            m_rwvar2_title = Parameters::GetName(Parameters::GetEnum(m_rwvar2), 1);
            m_rwvar2_nbins = m_rwvar1_nbins;

            m_rwvar2_bins = new Double_t [ m_rwvar2_nbins + 1 ];

            for(int i = 0; i < m_rwvar2_nbins + 1; i++) m_rwvar2_bins[i] = m_rwvar1_bins[i];

            good_rwvar2 = true;
            m_Is_TwoD = true;

            m_rwvar1 = "";
            m_rwvar1_nbins = -999;
            delete [] m_rwvar1_bins;
        }
        else m_Is_TwoD = false;

        if(good_var2rw){
            m_rwvar1 = m_var2rw;
            m_rwvar1_title = Parameters::GetName(Parameters::GetEnum(m_rwvar1), 1);
            m_rwvar1_nbins = m_var2rw_nbins;

            m_rwvar1_bins = new Double_t [ m_rwvar1_nbins + 1 ];

            for(int i = 0; i < m_rwvar1_nbins + 1; i++) m_rwvar1_bins[i] = m_var2rw_bins[i];

            good_rwvar1 = true;
            good_var2rw = false;

            m_var2rw = "";
            m_var2rw_nbins = -999;
            delete [] m_var2rw_bins;
        }
    }

    bool lprint = false;

    if(lprint){
        cout << "------------- SUMMARY -------------" << endl;
        if(run_rw){
            cout << "RW Var: " << m_var2rw << ": Nbins = " << m_var2rw_nbins << ":  ";// << endl;
            for(int i = 0; i < m_var2rw_nbins + 1; i++) cout << m_var2rw_bins[i] << " ";
                cout << endl;
        }

        string rw_string = "";
        if(run_rw) rw_string = "RW";

        if(good_rwvar1){
            cout << "Var " << rw_string << "1: " << m_rwvar1 << ": Nbins = " << m_rwvar1_nbins << ":  ";// << endl;
            for(int i = 0; i < m_rwvar1_nbins + 1; i++) cout << m_rwvar1_bins[i] << " ";
            cout << endl;
        }
        
        if(good_rwvar2){
            cout << "Var " << rw_string << "2: " << m_rwvar2 << ": Nbins = " << m_rwvar2_nbins << ":  ";// << endl;
            for(int i = 0; i < m_rwvar2_nbins + 1; i++) cout << m_rwvar2_bins[i] << " ";
            cout << endl;
        }    
        cout << "-----------------------------------" << endl;
    }
}

InputInterpreter::~InputInterpreter()
{
    delete [] m_var2rw_bins;
    delete [] m_rwvar1_bins;
    delete [] m_rwvar2_bins;
}

double * InputInterpreter::SetBinning(int nbins, double low, double high){
    // cout << "InputInterpreter::SetBinning(" << nbins << ", " << low << ", " << high <<")" << endl;

    double * bins = new double[ nbins + 1 ];
    
    double range = high - low;
    double binwidth = range/(double)nbins;
    
    for (int i=0; i < nbins + 1; i++) {
        bins[i] = low + binwidth*i;
        // if(m_verbose) cout << "Array Element: " << i << " : " << bins[i] << endl;
    }
    return bins;
}

bool InputInterpreter::IsNumber(std::string in)
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

std::vector<std::string> InputInterpreter::ReadFile(std::string infile)
{
    string tmp_line = "";
    bool complete = false;

    ifstream fin( infile.c_str() );
    assert(fin.is_open());
    string line;

    std::vector<std::string> list;
    list.clear();

    while (getline(fin, line))
    {
        // cout << "Line: " << line << endl;
        if(line.find("<") != std::string::npos){
            // cout << "Starting to read in RW" << endl;
            tmp_line = line;
            if(line.find(">") != std::string::npos) complete = true;
        }
        else if(line.find(">") != std::string::npos){
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

double * InputInterpreter::DetermineBins(int nbins, std::vector<double> binning)
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

#endif