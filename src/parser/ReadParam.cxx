#ifndef __READPARAM__CXX__
#define __READPARAM__CXX__

#include <ReadParam.hxx>
#include <sstream>
#include <cctype>
#include <stdlib.h>
#include <fstream> //ifstream header
#include <cassert>
#include <algorithm>
#include <iterator>

using std::string;
// using std::cout;
// using std::endl;

ReadParam::ReadParam(const std::string instring, string left_arrow, string right_arrow) : m_instring(instring), m_type(Type::kStandard),
    m_truthtree(false), m_NoOptions(true), m_reset_cuts(false), m_Dim(kZero)
{
    RemoveArrows(m_instring, left_arrow, right_arrow);

    // Split all sets:
    std::vector<string> nsets;
    string tmp_instring = m_instring;
    size_t tmp_place = tmp_instring.find(":");
    if(tmp_place == string::npos){
        nsets.push_back( tmp_instring );
    }
    else{
        while( tmp_place != string::npos ){
            string set = tmp_instring.substr(0, tmp_place);
            nsets.push_back( set );
            tmp_instring = tmp_instring.substr(tmp_place + 1, tmp_instring.length());  
            tmp_place = tmp_instring.find(":");
        }
        if(!tmp_instring.empty()) nsets.push_back( tmp_instring );
    }

    std::vector<BinPar> nparams;

    string tmp_options = "";

    std::map<Type::Type, std::string> ep_list;
    // m_ep_list[ kEff ] = "Eff";
    // m_ep_list[ kPur ] = "Pur";
    // m_ep_list[ kEP ] = "EP";
    // m_ep_list[ kPID ] = "PID";
    // m_ep_list[ kTop ] = "Top";
    // m_ep_list[ kTarS ] = "TarS";
    // m_ep_list[ kTar ] = "Tar";
    ep_list[ Type::kEffVSCuts ] = "EffVSCuts";
    ep_list[ Type::kPurVSCuts ] = "PurVSCuts";
    ep_list[ Type::kEffPurVSCuts ] = "EffPurVSCuts";
    ep_list[ Type::kNCutsMinusOne ] = "NCutsMinusOne";

    for(size_t i = 0; i < nsets.size(); i++){
            // cout << "nsets[" << i << "] = " << nsets[i] << endl;

        string word;
        std::stringstream iss(nsets[i]);
        BinPar par;
        while( iss >> word )     
        {
            if(!IsNumber(word)){
                if(par.var.empty()) par.var = word;
                else if(par.var.empty()) par.var = word;
                else if(par.title.empty()) par.title = word;
                else if(par.units.empty()) par.units = word;
                else{
                    par.extra += " ";
                    par.extra += word;
                }
            }
            else{
                if(par.nbins == -999) par.nbins = atoi( word.c_str() );
                else par.bvals.push_back( atof(word.c_str()) );
            }
        }
            // Now check that we have the minimum info:
        bool pass = true;
        if(par.var.empty()) pass = false;
        if(par.nbins == -999) pass = false;
        if(par.bvals.size() < 2) pass = false;

        if(pass){
            par.good = true;
                // Check that title has been filled and set to var if not:
            if(par.title.empty()) par.title = par.var;
            par.DetermineBins();
            // cout << " nparams.push_back( par ); " << endl;
            nparams.push_back( par );
        }
        else{
            if(!par.var.empty()){        
                // Look for special plots:
                std::map<Type::Type, std::string>::iterator it = ep_list.begin();
                while(it != ep_list.end())
                {
                    if(it->second == par.var){ 
                        // cout << par.var << " == " << it->second << endl;
                        m_type = it->first;
                        par.nbins = 1;
                        par.bvals.push_back( -999. );
                        par.bvals.push_back( -998. );
                        par.DetermineBins();
                        nparams.push_back( par );
                        break;
                    }
                    it++;
                }
                    // cout << "Found Option(s)" << endl;
                tmp_options = par.var;
                if(!par.title.empty()){
                    tmp_options += " ";
                    tmp_options += par.title;
                    if(!par.units.empty()){
                        tmp_options += " ";
                        tmp_options += par.units;
                        if(!par.extra.empty()){
                            tmp_options += " ";
                            tmp_options += par.extra;
                        }
                    }
                }
            }
        }
    }

    size_t entries = nparams.size();

    if(entries == 1) m_Dim = kOne;
    else if(entries == 2) m_Dim = kTwo;
    else if(entries == 3) m_Dim = kThree;
    else{
        cout << __FILE__ << ":" << __LINE__ << " : ERROR : More than 3 dimensions (count " << entries << ")" << endl;
            exit(0); 
    }

    BinPar loop[3] = { m_varOne, m_varTwo, m_varThree };

    for(size_t i = 0; i < entries; i++){
        loop[i] = nparams[i];
        // loop[i].Print();
    }    

    if(!tmp_options.empty()) m_options[ kInput ] = tmp_options;

    ExtractOptions();

}

ReadParam::~ReadParam()
{
    // delete [] m_varThree.bins;
    // delete [] m_varOne.bins;
    // delete [] m_varTwo.bins;
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

void ReadParam::RemoveArrows(std::string &word, std::string left_arrow, std::string right_arrow)
{
    // Add | to both arrows as we may/will have instances where the comparison symbols (</>) are used. 
    size_t larrow = word.find(left_arrow);
    size_t rarrow = word.find(right_arrow);
    if(larrow != string::npos && rarrow != string::npos){
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
        else if(tmp.find(name) == string::npos) continue;
        else if(called){
            cout << __FILE__ << ":" << __LINE__ << " : ERROR : Multiple instances of param name " << name << " in parameters file (" << infile << ")" << endl;
        }
    }

    string param = "";
    if(store != -999){
        param = list[store];
        if(!param.empty()){
            RemoveArrows(param, left_arrow, right_arrow);

            size_t place = param.find("=");
            if(place != string::npos){
                param = param.erase(0, place + 1);
            }
            else{
                cout << __FILE__ << ":" << __LINE__ << ": ERROR : Parameter not in correct form (missing =), we have: " << param << endl;
                exit(0);
            }
        }
    }
    // This is to get rid of white space quickly:
    if(param.length() < 2) param = "";

    return param;
}

std::string ReadParam::GetParameterN(std::string name, std::string infile, std::string left_arrow, std::string right_arrow)
{
    string param = GetParameterS(name, infile, left_arrow, right_arrow);
    if(!param.empty()){
        param.erase(std::remove(param.begin(),param.end(),' '),param.end());
        if(!IsNumber(param)){
            cout << __FILE__ << ":" << __LINE__ << ": ERROR : Parameter is not a number, we have: " << param << endl;
            exit(0);
        }
    }
    else param = "-999";
    return param;
}   

int ReadParam::GetParameterI(std::string name, std::string infile, std::string left_arrow, std::string right_arrow)
{
    return atoi(GetParameterN(name, infile, left_arrow, right_arrow).c_str());
}

double ReadParam::GetParameterD(std::string name, std::string infile, std::string left_arrow, std::string right_arrow)
{
    return atof(GetParameterN(name, infile, left_arrow, right_arrow).c_str());
}

void ReadParam::ExtractOptions()
{
    if(!m_options.empty()){
        m_NoOptions = false;

        std::map<ReadParam::Opts, std::string>::iterator it = m_options.find( kInput );
        string tmp_options = "";
        if(it != m_options.end()) tmp_options = it->second;
        // kType:
        string type = FindOpt("type = ", tmp_options);
        if(!type.empty()) m_options[ kType ] = type;
        // kCut:
        string cut = FindOpt("cut = ", tmp_options);
        if(!cut.empty()) m_options[ kCuts ] = cut;
        // kStyle:
        string style = FindOpt("style = ", tmp_options);
        if(!style.empty()) m_options[ kStyle ] = style;
        // kOption:
        string option = FindOpt("option = ", tmp_options);
        if(!style.empty()) m_options[ kOption ] = option;

        // Set the style and types:

        if(OptSet(kType)){
            string tmp = GetOpt(kType);
            if(tmp.find("EP") != string::npos) m_type = Type::kEP;
            else if(tmp.find("PID") != string::npos) m_type = Type::kPID;
            else if(tmp.find("TOP") != string::npos) m_type = Type::kTop;
            else if(tmp.find("TARS") != string::npos) m_type = Type::kTarS;
            else if(tmp.find("TAR") != string::npos) m_type = Type::kTar;
            else if(tmp.find("E") != string::npos) m_type = Type::kEff;
            else if(tmp.find("P") != string::npos) m_type = Type::kPur;
        }

        if(OptSet(kStyle)){
            string tmp = GetOpt(kStyle);
            string word;
            std::stringstream iss(tmp);           
            while( iss >> word )     
            {                
                Style::Style fstyle = Style::kNotSet;
                if(word.find("ls") != string::npos ) fstyle = Style::kLS; 
                else if(word.find("lc") != string::npos ) fstyle = Style::kLC; 
                else if(word.find("lw") != string::npos ) fstyle = Style::kLW; 
                else if(word.find("fc") != string::npos ) fstyle = Style::kFC; 
                else if(word.find("fs") != string::npos ) fstyle = Style::kFS; 

                if(fstyle != Style::kNotSet){
                    word = word.substr(2, word.length());                    
                    if(IsNumber(word)) m_style[ fstyle ] = atoi(word.c_str());
                }
            }
        }

        if(OptSet(kCuts)){
            string tmp = GetOpt(kCuts);
            string reset = "reset";
            size_t pos = tmp.find(reset);
            if(pos != string::npos){
                m_reset_cuts = true;
                tmp = tmp.substr(pos + reset.length(), tmp.length());
                m_options[ kCuts ] = tmp;
            }
        }

        if(OptSet(kOption)){
            string tmp = GetOpt(kOption);
            if(tmp.find("T") != string::npos) m_truthtree = true;
            // else if(tmp.find("N") != string::npos) m_truthtree = true;
            // else if(tmp.find("SN") != string::npos) m_truthtree = true;
        }
    }

    bool print_shit = false;
    if(print_shit){

        std::map<ReadParam::Opts, string> optlist;
        optlist[ kCuts ] = "kCuts";
        optlist[ kStyle ] = "kStyle";
        optlist[ kType ] = "kType";

        std::map<ReadParam::Opts, std::string>::iterator it = m_options.begin();
        while(it != m_options.end())
        {
            std::map<ReadParam::Opts, std::string>::iterator it2 = optlist.begin();
            while(it2 != optlist.end())
            {
                if(it->first == it2->first){ 
                    cout << it2->second << " : " << it->second << endl;
                    break;
                }
                it2++;
            }
            it++;
        }
    }
}

std::string ReadParam::FindOpt(std::string name, std::string in){
    string option = "";
    size_t finder = in.find(name);
    if(finder != string::npos){
        string tmp = in.substr(finder + name.length(), in.length());
        size_t tmp_find = tmp.find(",");
        while(tmp_find != string::npos){
            tmp = tmp.substr(0, tmp_find);
            tmp_find = tmp.find(",");
        }
        // cout << name << " : " << tmp << endl;
        option = tmp;
    }
    return option;
}

std::string ReadParam::GetOpt(Opts option)
{
    string found = "";
    std::map<ReadParam::Opts, std::string>::iterator it = m_options.find( option );
    if(it != m_options.end()) found = it->second;
    return found;
}

bool ReadParam::OptSet(Opts option)
{
    bool found = false;
    std::map<ReadParam::Opts, std::string>::iterator it = m_options.find( option );
    if(it != m_options.end()) found = true;
    return found;
}

int ReadParam::GetStyle(Style::Style st)
{ 
    int found = -999;
    std::map<Style::Style, int>::iterator it = m_style.find( st );
    if(it != m_style.end()) found = it->second;
    return found; 
} 

int ReadParam::GetInt(std::string val)
{
    int num = -999;
    if(IsNumber(val)) num = atoi( val.c_str() );
    return num;
}

double ReadParam::GetDouble(std::string val)
{
    double num = -999.;
    if(IsNumber(val)) num = atof( val.c_str() );
    return num;
}

#endif

#ifndef __BINPAR__CXX__
#define __BINPAR__CXX__

BinPar::BinPar() : var(""), title(""), units(""), extra(""), nbins(-999), good(false)
{ 
    bvals.clear(); 
    bins = 0x0; 
}
    
BinPar::~BinPar(){ 
    bvals.clear(); 
}

inline void BinPar::Print()
{
    cout << "Par: " << var << " : Title: " << title << " (" << (units.empty() ? "NONE" : units) << ") ";
    cout << "NBins: " << nbins << " :";
    if(bins){
        for(int i = 0; i < nbins + 1; i++){
            cout << " " << bins[i];
        } 
        cout << endl;
    }
    else cout << "Problem Bins not set!!! " << endl;
}

inline double * BinPar::SetBinning(int tmp_nbins, double low, double high)
{
    double * tmp_bins = new double[ tmp_nbins + 1 ];
    double range = high - low;
    double binwidth = range/(double)tmp_nbins;

    for (int i=0; i < tmp_nbins + 1; i++) {
        tmp_bins[i] = low + binwidth*i;
        // if(m_verbose) cout << "Array Element: " << i << " : " << tmp_bins[i] << endl;
    }
    return tmp_bins;
}

inline void BinPar::DetermineBins(){
    if(bvals.size() == 2) bins = SetBinning(nbins, bvals[0], bvals[1]);
    else if( (int)bvals.size() == nbins + 1){
        bins = new double [ nbins + 1 ];
        for(int i = 0; i < nbins + 1; i++) bins[i] = bvals[i]; 
    }
    else{
        cout << __FILE__ << ":" << __LINE__ << " : ERROR : Could not make array: nbins = " << nbins << ", array size = " << bvals.size() << endl;
        exit(0);       
    }
}

#endif
