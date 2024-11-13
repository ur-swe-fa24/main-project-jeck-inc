
#include "Frames/sm.hpp"
#include "Robot.hpp"
#include "Simulation.hpp"
#include "Database.hpp"


// Event table that connects the button to the event handler
wxBEGIN_EVENT_TABLE(SeniorM, wxFrame)
    EVT_BUTTON(1001, SeniorM::AddingRobot)  // Event binding: button click (ID 1001) triggers AddingRobot
wxEND_EVENT_TABLE()

// Constructor definition
SeniorM::SeniorM(const wxString& title, Simulation& sim, Database& db)
    : wxFrame(nullptr, wxID_ANY, title), sim(sim), db(db){

    // Create a panel for holding the GUI components
    wxPanel* panel = new wxPanel(this, wxID_ANY);

    // Create a button that will trigger the AddingRobot event
    wxButton* addButton = new wxButton(panel, 1001, "Add Robot", wxPoint(10, 260));

    // Set the window size for the SubFrame
    this->SetSize(400, 350);
}

// Event handler: Adding a robot to the simulation
void SeniorM::AddingRobot(wxCommandEvent& event) {
    // Creating a new robot with predefined characteristics
    Robot rob(Robot::Function::Shampoo, Robot::Size::Small);

    // Add the robot to the simulation
    sim.add_robot(rob);

    // Display a message box indicating the robot was added
    wxMessageBox("Robot added successfully with id: ", "Success", wxOK | wxICON_INFORMATION);
}
