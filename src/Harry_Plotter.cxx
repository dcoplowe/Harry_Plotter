#ifndef __HARRY_PLOTTER__CXX
#define __HARRY_PLOTTER__CXX

#include <Harry_Plotter.hxx>

#include <ReadParam.hxx>
#include <iostream>
#include <DrawingTools.hxx>
#include <EffPurTools.hxx>
#include <TCanvas.h>
#include <TDatime.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>

using std::string;
using std::endl;
using std::cout;

Harry_Plotter::Harry_Plotter(std::string infile, std::string ofile) : m_filename(infile), m_recontree("default"), m_truthtree("truth"),
    m_signal(""), m_Rcuts(""), m_Tcuts(""), m_epcut(""), m_branch(-999), m_ofilename(ofile)
{
	string opts_file = string( getenv("HP_ROOT") );
    opts_file += "/parameters/run_options.txt";

    m_signal = ReadParam::GetParameterS("signal", opts_file);
    m_Rcuts = ReadParam::GetParameterS("Rcuts", opts_file);
    m_Tcuts = ReadParam::GetParameterS("Tcuts", opts_file);
    m_epcut = ReadParam::GetParameterS("effpur cut", opts_file);

    // cout << "m_branch = " << m_branch << endl;
    m_branch = ReadParam::GetParameterI("branch", opts_file);
    if(m_branch == -999) m_branch = 0;
    // cout << "m_branch = " << m_branch << endl;

    // Add default colours for particle 
    string particle = ReadParam::GetParameterS("particle", opts_file);
    string part_word;
    std::stringstream iss(particle);

    while( iss >> part_word ){
        cout << "part_word = " << part_word << endl;
        size_t equals = part_word.find("=");
        if(equals != string::npos){
            string part = part_word.substr(0, equals);
            string pdgs = part_word.substr(equals + 1, part_word.length());
            m_particles[ part ] = ReadParam::GetInt(pdgs);
        }
    }  
        // DrawingSyle::GetParticleCol(int pdg);


	string plots_file = string( getenv("HP_ROOT") );
    plots_file += "/parameters/plot_list.txt";

    cout << "plots_file = " << plots_file <<endl;
    std::vector<string> plot_list = ReadParam::ReadFile(plots_file);

    for(size_t i = 0; i < plot_list.size(); i++){
    	m_plots.push_back( new ReadParam(plot_list[i]) );
    }
    plot_list.clear();

    m_recon = new DrawingTools(m_filename, m_recontree);
    m_truth = new DrawingTools(m_filename, m_truthtree);
    m_effpur = new EffPurTools(m_filename, m_recontree, m_truthtree);


    TDatime time;
    Int_t year = time.GetYear() - 2000;
    Int_t month = time.GetMonth();
    Int_t day = time.GetDay();

    if(m_ofilename.empty()) m_ofilename = Form("Harry_Plotter_OutFile_%.2d%.2d%.2d.root", day, month, year);
    else{
        string root_t = ".root";
        size_t root_s = m_ofilename.find(root_t);
        if(root_s != string::npos) m_ofilename = m_ofilename.substr(0, root_s);

        m_ofilename = Form("%s_%.2d%.2d%.2d.root", m_ofilename.c_str(), day, month, year);        
    }

    m_outfile = new TFile(m_ofilename.c_str(), "RECREATE");
}

Harry_Plotter::~Harry_Plotter()
{
	m_plots.clear();
    delete m_outfile;
}

void Harry_Plotter::Run()
{
    m_outfile->cd();

    for(size_t p = 0; p < m_plots.size(); p++){

        ReadParam * par = m_plots[p];

        TCanvas * can = 0x0;
        TH1D * hist1D = 0x0;
        TH2D * hist2D = 0x0;
        TH1D * hist1Da = 0x0;

        string option = par->GetOpt(ReadParam::kOption);

        switch ( par->GetType() ){
            case Type::kStandard:
                if(par->GetDim() == 1){
                    hist1D = Get1D(par);
                    hist1D->SetName( (par->GetVar1() + (option.empty() ? "" : "_" + option) ).c_str() );
                }
                else if(par->GetDim() == 2){
                    hist2D = Get2D(par);
                    hist2D->SetName( (par->GetVar1() + "_vs_" + par->GetVar2() + (option.empty() ? "" : "_" + option) ).c_str() );
                }
                break;
            case Type::kEff: 
                if(par->GetDim() == 1){
                    hist1D = m_effpur->EffVSVar(par->GetVar1(), par->GetVar1NBins(), par->GetVar1Bins(),
                        m_signal, CheckCuts(par, 1), par->GetVar1Title());
                    hist1D->SetName( (par->GetVar1() + "_Eff" + (option.empty() ? "" : "_" + option) ).c_str() );
                }
                else if(par->GetDim() == 2){
                    hist2D = m_effpur->EffVSVar(par->GetVar2() + ":" + par->GetVar1(), par->GetVar1NBins(), par->GetVar1Bins(), 
                        par->GetVar2NBins(), par->GetVar2Bins(), m_signal, CheckCuts(par, 1), par->GetVar1Title() + ";" + par->GetVar2Title());
                    hist2D->SetName( (par->GetVar1() + "_vs_" + par->GetVar2() + "_Eff" + (option.empty() ? "" : "_" + option) ).c_str() );
                }
                break;
            case Type::kPur: 
                if(par->GetDim() == 1){
                    hist1D = m_effpur->PurVSVar(par->GetVar1(), par->GetVar1NBins(), par->GetVar1Bins(),
                        m_signal, CheckCuts(par, 1), par->GetVar1Title());
                    hist1D->SetName( (par->GetVar1() + "_Pur" + (option.empty() ? "" : "_" + option) ).c_str() );
                }
                else if(par->GetDim() == 2){
                    hist2D = m_effpur->PurVSVar(par->GetVar2() + ":" + par->GetVar1(), par->GetVar1NBins(), par->GetVar1Bins(), 
                        par->GetVar2NBins(), par->GetVar2Bins(), m_signal, CheckCuts(par, 1), par->GetVar1Title() + ";" + par->GetVar2Title());
                    hist2D->SetName( (par->GetVar1() + "_vs_" + par->GetVar2() + "_Pur" + (option.empty() ? "" : "_" + option) ).c_str() );
                }
                break;
            case Type::kEP: 
                if(par->GetDim() == 1){
                    hist1D = m_effpur->EffVSVar(par->GetVar1(), par->GetVar1NBins(), par->GetVar1Bins(),
                        m_signal, CheckCuts(par, 1), par->GetVar1Title());
                    hist1D->SetName( (par->GetVar1() + "_EffPur" + (option.empty() ? "" : "_" + option) ).c_str() );
                    hist1Da = m_effpur->PurVSVar(par->GetVar1(), par->GetVar1NBins(), par->GetVar1Bins(),
                        m_signal, CheckCuts(par, 1), par->GetVar1Title());
                }
                else{ 
                    cout << __FILE__ << ":" << __LINE__ << " : Didn't draw " << par->GetVar1() << " vs. ";
                    cout << par->GetVar2() << " Eff. Pur. - Cannot overlay 2D histograms" << endl; 
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
                // hist1D = m_effpur->EffVSCuts(m_signal, m_branch, CheckCuts(par, 2) );
                break;
            case Type::kPurVSCuts: 
                // hist1D = m_effpur->PurVSCuts(m_signal, m_branch, CheckCuts(par, 2) );
                break;
            case Type::kEffPurVSCuts: 
                // hist1D = m_effpur->EffVSCuts(m_signal, m_branch, CheckCuts(par, 2) );
                // hist1Da = m_effpur->PurVSCuts(m_signal, m_branch, CheckCuts(par, 2) );
                break;
            case Type::kNCutsMinusOne: 
                // hist1D = m_effpur->EffVSN1Cuts(m_signal, m_branch, CheckCuts(par, 2) );
                // hist1Da = m_effpur->PurVSN1Cuts(m_signal, m_branch, CheckCuts(par, 2) );                
                break; 
            default: break;       
        }

        if(hist1D) can = new TCanvas(hist1D->GetName(), "", 600, 400);
        else if(hist2D) can = new TCanvas(hist2D->GetName(), "", 600, 400);

        if(hist1D) hist1D->SetBit(kCanDelete);
        if(hist1Da) hist1D->SetBit(kCanDelete);
        if(hist2D) hist2D->SetBit(kCanDelete);

        if(hist1D){
            can->cd();
            // Check out the opts:
            DrawingTools::SetColors(hist1D,   par->GetStyle(Style::kFC), par->GetStyle(Style::kLC), 
                par->GetStyle(Style::kFS), par->GetStyle(Style::kLS), par->GetStyle(Style::kLW));
            hist1D->Draw();
            if(hist1Da) hist1D->Draw("SAME");
        }

        if(hist2D){
            can->cd();
            hist2D->Draw("COLZ");
        }

        if(can) can->Write();

    }

    m_outfile->Close();
}

std::string Harry_Plotter::CheckCuts(ReadParam * par, int type)
{
    string cuts = "";
    if(type == 0) cuts = m_Rcuts;
    else if(type == 1) cuts = m_Tcuts;
    else if(type == 2) cuts = m_epcut;

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

int Harry_Plotter::GetParticleColor(std::string var)
{
    int colour = -999;
    std::map<std::string,int>::iterator it = m_particles.begin();
    for(; it != m_particles.end(); it++){
        if(var.find(it->first) != string::npos){
            colour = it->second;
            break;
        }
    }
    return colour;
}

#endif