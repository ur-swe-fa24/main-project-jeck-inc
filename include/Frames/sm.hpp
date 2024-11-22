#ifndef SENIORM_H
#define SENIORM_H

#include <wx/wx.h>
#include "Simulation.hpp"  
#include "Database.hpp"

using namespace simulation;
using namespace database;

class SeniorM : public wxFrame {
public:
    // Constructor accepts the title and a reference to a Simulation object
    SeniorM(const wxString& title, simulation::Simulation& sim, database::Database& db);
    
private:
    // Event handler for adding a robot
    void RobotProductivity(wxCommandEvent& event);

    // int allRobotPerformance()
    wxComboBox* sizeComboBox;
    wxComboBox* typeComboBox;

    wxArrayString sizeChoices;
    wxArrayString typeChoices;    

    //Static Text for Response
    wxStaticText* robotProducitivity; 

    // Reference to the Simulation and Database object
    simulation::Simulation& sim;
    database::Database& db;

    // Declare the event table
    wxDECLARE_EVENT_TABLE();
};

#endif // SENIORM_H
