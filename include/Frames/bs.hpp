#ifndef BUILDINGS_H
#define BUILDINGS_H

#include <wx/wx.h>
#include "Simulation.hpp"  
#include "Database.hpp"

using namespace simulation;
using namespace database;

class BuildingS : public wxFrame {
public:
    // Constructor accepts the title and a reference to a Simulation object
    BuildingS(const wxString& title, simulation::Simulation& sim, database::Database& db);
    
private:
    // Event handler for adding a robot
    void AddingRobot(wxCommandEvent& event);

    // Reference to the Simulation object
    simulation::Simulation& sim;
    database::Database& db;

    // Declare the event table
    wxDECLARE_EVENT_TABLE();
};

#endif // BUILDINGS_H
