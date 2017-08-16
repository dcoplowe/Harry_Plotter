#ifndef _VARIABLE_
#define _VARIABLE_

#include <string>
#include <DrawingTools.hxx>

class Variable {

public: 
    Variable() : m_name(""), m_symbol(""), m_units(""), m_savename(""), m_pdg(""){;}
    Variable(std::string name) : m_name(name), m_symbol(name), m_units(""), m_savename(name), m_pdg(""){;}
    Variable(std::string name, std::string symbol, std::string units) : m_name(name), m_symbol(symbol), m_units(units), m_savename(name), m_pdg(""){;}
    Variable(std::string name, int nbins, double low, double high, std::string symbol = "", std::string units = "", std::string savename = "", std::string pdg = "") :
        m_name(name), m_symbol(symbol), m_units(units), m_savename( (savename.empty() ? name : savename) ), m_pdg(pdg), m_nbins(nbins), m_binning( DrawingTools::SetBinning(m_nbins, low, high) ) {;}
    Variable(std::string name, int nbins, double * bins, std::string symbol = "", std::string units = "", std::string savename = "", std::string pdg = "") :
        m_name(name), m_symbol(symbol), m_units(units), m_savename( (savename.empty() ? name : savename) ), m_pdg(pdg), m_nbins(nbins), m_binning( bins ) {;}

    Variable(const Variable &input){ 
        m_name = input.GetName();
        m_symbol = input.GetSymbol();
        m_units = input.GetUnits();
        m_savename = input.GetSName();
        m_pdg = input.GetPDG();
        m_nbins = input.GetNBins();
        m_binning = input.GetBinning();
    }
    
    ~Variable(){;}//delete m_binning;}

    void SetName(std::string var) { m_name = var; }
    void SetSymbol(std::string var) { m_symbol = var; }
    void SetUnits(std::string var) { m_units = var; }
    void SetPDG(std::string var) { m_pdg = var; }
    void SetSName(std::string var) { m_savename = var; }
    
    std::string GetName() const { return m_name; }
    std::string GetSymbol() const { return m_symbol; }
    std::string GetUnits() const { return m_units; }
    std::string GetSName() const { return m_savename; }
    std::string GetPDG() const { return m_pdg; }
    std::string GetAxisTitle() const { return m_symbol + (m_units.empty() ? "" : " (" + m_units + ")"); }

    void SetNBins(int var){ m_nbins = var;}
    void SetBinning(int nbins, double * binning){ m_nbins = nbins; m_binning = binning; }

    void SetBinning(int nbins, double low, double high){ m_nbins = nbins; m_binning = DrawingTools::SetBinning(m_nbins, low, high); }
    int GetNBins(){ return m_nbins; }
    double * GetBinning(){ return m_binning; }

private:
    std::string m_name;
    std::string m_symbol;
    std::string m_units;
    std::string m_savename;
    std::string m_pdg;

    int m_nbins;
    double * m_binning;
};

#endif