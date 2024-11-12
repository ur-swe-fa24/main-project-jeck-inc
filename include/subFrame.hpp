#ifndef SUBFRAME_H
#define SUBFRAME_H

#include <wx/wx.h>
#include "Simulation.hpp"  // Assuming you have a Simulation class

using namespace simulation;

class SubFrame : public wxFrame {
public:
    // Constructor accepts the title and a reference to a Simulation object
    SubFrame(const wxString& title, simulation::Simulation& sim);
    
private:
    // Event handler for adding a robot
    void AddingRobot(wxCommandEvent& event);

    // Reference to the Simulation object
    simulation::Simulation& sim;

    // Declare the event table
    wxDECLARE_EVENT_TABLE();
};

#endif // SUBFRAME_H
