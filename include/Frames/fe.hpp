#ifndef FIELDE_H
#define FIELDE_H

#include <wx/wx.h>
#include "Simulation.hpp"  
#include "Database.hpp"

using namespace simulation;
using namespace database;

class FieldE : public wxFrame {
public:
    // Constructor accepts the title and a reference to a Simulation object
    FieldE(const wxString& title, simulation::Simulation& sim, database::Database& db);
    
private:
    // Event handler for buttons
    void ShowRobots(wxCommandEvent& event);
    void FixRobot(wxCommandEvent& event);
    void GetRobotStatus(wxCommandEvent& event);
    void GoBack(wxCommandEvent& event);

    // GUI components for robot ID input fields
    wxTextCtrl* robotIdFixInput;
    wxTextCtrl* robotIdStatusInput;

    // Reference to the Simulation object
    simulation::Simulation& sim;
    database::Database& db;

    // Declare the event table
    wxDECLARE_EVENT_TABLE();
};

#endif // FIELDE_H
