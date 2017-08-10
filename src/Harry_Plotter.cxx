#ifndef __HARRY_PLOTTER__CXX
#define __HARRY_PLOTTER__CXX

#include <Harry_Plotter.hxx>

#include <ReadParam.hxx>
#include <iostream>
#include <DrawingTools.hxx>

using std::string;
using std::endl;
using std::cout;

Harry_Plotter::Harry_Plotter(std::string infile) : m_filename(infile), m_recontree("default"), m_truthtree("truth"), m_Rcuts(""), m_Tcuts("")
{
	// string opts_file = string( getenv("HP_ROOT") );
 //    opts_file += "/parameters/run_options.txt";

 //    cout << "opts_file = " << opts_file <<endl;
 //    int dcool = ReadParam::GetParameterI("david is cool", opts_file);
 //    string cuts = ReadParam::GetParameterS("cuts", opts_file);
 //    double numbers = ReadParam::GetParameterD("numbers", opts_file);

 //    cout << "david is cool = " << dcool << endl;
 //    cout << "cuts = " << cuts << endl;
 //    cout << "numbers = " << numbers << endl;

	string plots_file = string( getenv("HP_ROOT") );
    plots_file += "/parameters/plot_list.txt";

    cout << "plots_file = " << plots_file <<endl;
    std::vector<string> plot_list = ReadParam::ReadFile(plots_file);

    for(size_t i = 0; i < plot_list.size(); i++){
    	m_plots.push_back( new ReadParam(plot_list[i]) );
    }
    plot_list.clear();

    DrawingTools * m_recon = new DrawingTools(m_filename, m_recontree);
    DrawingTools * m_truth = new DrawingTools(m_filename, m_truthtree);
}

Harry_Plotter::~Harry_Plotter()
{
	m_plots.clear();
}

void Harry_Plotter::Run()
{
    // Initialise relevent classes:
    // EffPurTools * builder = new EffPurTools(m_filename, m_reco, m_truth);

    for(size_t p = 0; p < m_plots.size(); p++){

        ReadParam * par = m_plots[p];

        TH1D * hist1D;
        TH2D * hist2D;

        switch ( par->GetType() ){
            case Type::kStandard:
                if(par->GetDim() == 1){
                    hist1D = Get1D(par);
                }
                else if(par->GetDim() == 2){
                    hist2D = Get2D(par);
                }

                break;
            case Type::kEff: 
                if(par->GetDim() == 1){

                }
                else if(par->GetDim() == 2){
                    
                }
                break;
            case Type::kPur: 
                if(par->GetDim() == 1){

                }
                else if(par->GetDim() == 2){
                    
                }
                break;
            case Type::kEP: 
                if(par->GetDim() == 1){

                }
                else if(par->GetDim() == 2){
                    
                }
                break;
            case Type::kPID: 
                if(par->GetDim() == 1){

                }
                else if(par->GetDim() == 2){
                    
                }
                break;
            case Type::kTop: 
                if(par->GetDim() == 1){

                }
                else if(par->GetDim() == 2){
                    
                }
                break;
            case Type::kTarS: 
                if(par->GetDim() == 1){

                }
                else if(par->GetDim() == 2){
                    
                }
                break;
            case Type::kTar: 
                if(par->GetDim() == 1){

                }
                else if(par->GetDim() == 2){
                    
                }
                break;
            case Type::kEffVSCuts: 
            if(par->GetDim() == 1){

                }
                else if(par->GetDim() == 2){
                    
                }
                break;
            case Type::kPurVSCuts: 
                if(par->GetDim() == 1){

                }
                else if(par->GetDim() == 2){
                    
                }
                break;
            case Type::kEffPurVSCuts: 
                if(par->GetDim() == 1){

                }
                else if(par->GetDim() == 2){
                    
                }
                break;
            case Type::kNCutsMinusOne: 
                if(par->GetDim() == 1){

                }
                else if(par->GetDim() == 2){
                    
                }
                break; 
            default: break;       
        }

    }

}

std::string Harry_Plotter::CheckCuts(ReadParam * par, bool is_recon)
{
    string cuts = (is_recon ? m_Rcuts : m_Tcuts);
    if(!par->GetOpt(ReadParam::kCuts).empty()){
         if(par->ResetCuts()) cuts = par->GetOpt(ReadParam::kCuts);
         else{
            if(!cuts.empty()){
                cuts += " && ";
                cuts += par->GetOpt(ReadParam::kCuts);
            }
            else cuts = par->GetOpt(ReadParam::kCuts);
         }
    }
    return cuts;
}

TH1D * Harry_Plotter::Get1D(ReadParam * par)
{
    TH1D * hist; 
    if(par->Truth()) hist = m_truth->GetHisto(par->GetVar1(), par->GetVar1NBins(), par->GetVar1Bins(), par->GetVar1Title(), CheckCuts(par, false));
    else hist = m_recon->GetHisto(par->GetVar1(), par->GetVar1NBins(), par->GetVar1Bins(), par->GetVar1Title(), CheckCuts(par));

    // Other things like normalisation, STYLE:
    return hist;
}

TH2D * Harry_Plotter::Get2D(ReadParam * par)
{
    TH2D * hist; 
    if(par->Truth()) hist = m_truth->GetHisto(par->GetVar2() + ":" + par->GetVar1(), par->GetVar1NBins(), par->GetVar1Bins(),
        par->GetVar2NBins(), par->GetVar2Bins(), par->GetVar1Title() + ";" + par->GetVar2Title(), CheckCuts(par, false));
    else hist = m_recon->GetHisto(par->GetVar2() + ":" + par->GetVar1(), par->GetVar1NBins(), par->GetVar1Bins(),
        par->GetVar2NBins(), par->GetVar2Bins(), par->GetVar1Title() + ";" + par->GetVar2Title(), CheckCuts(par));
    // Other things like normalisation, STYLE:
    return hist;
}

#endif