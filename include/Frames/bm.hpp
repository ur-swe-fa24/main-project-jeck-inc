#ifndef BUILDINGM_H
#define BUILDINGM_H

#include <wx/wx.h>
#include "Simulation.hpp"  
#include "Database.hpp"

using namespace simulation;
using namespace database;

class BuildingM : public wxFrame {
public:
    // Constructor accepts the title and a reference to a Simulation object
    BuildingM(const wxString& title, simulation::Simulation& sim, database::Database& db);
    
private:
    // Event handler for adding a robot
    void AddingRobot(wxCommandEvent& event);
    void AssignTask(wxCommandEvent& event);

    //for add robot
    wxTextCtrl* sizeInput;
    wxTextCtrl* functionInput;

    //for assign task
    wxTextCtrl* robotId;
    wxTextCtrl* roomId;

    // Reference to the Simulation and Database object
    simulation::Simulation& sim;
    database::Database& db;

    // Declare the event table
    wxDECLARE_EVENT_TABLE();
};

#endif // BUILDINGMH
