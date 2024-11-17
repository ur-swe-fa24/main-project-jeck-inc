
#include "Frames/bs.hpp"
#include "Robot.hpp"
#include "Simulation.hpp"
#include "Database.hpp"


// Event table that connects the button to the event handler
wxBEGIN_EVENT_TABLE(BuildingS, wxFrame)
    EVT_BUTTON(1001, BuildingS::RobotStatus)  // Event binding: button click (ID 1001) triggers AddingRobot4
    EVT_BUTTON(1002, BuildingS::RobotRefill) 
wxEND_EVENT_TABLE()

// Constructor definition
BuildingS::BuildingS(const wxString& title, Simulation& sim, Database& db)
    : wxFrame(nullptr, wxID_ANY, title), sim(sim), db(db){

    // Create a panel for holding the GUI components
    wxPanel* panel = new wxPanel(this, wxID_ANY);

    //Getting size input
    wxStaticText* robotStatusIdLabel = new wxStaticText(panel, wxID_ANY, "Enter a viable robot id", wxPoint(10, 10));
    robotStatusId = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(10, 30), wxSize(200, -1));
    
    // Create a button that will trigger the AddingRobot event
    wxButton* robotStatusButton = new wxButton(panel, 1001, "Robot Status", wxPoint(10, 65));

    //Getting size input
    wxStaticText* robotRefillIdLabel = new wxStaticText(panel, wxID_ANY, "Enter a viable robot id", wxPoint(10, 110));
    robotRefillId = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(10,130), wxSize(200, -1));
    
    // Create a button that will trigger the AddingRobot event
    wxButton* robotRefillButton = new wxButton(panel, 1002, "Refill Robot", wxPoint(10, 165));

    // Set the window size for the SubFrame
    this->SetSize(400, 350);
}

// Event handler: Adding a robot to the simulation
void BuildingS::RobotStatus(wxCommandEvent& event) {
    // Retrieve the robot ID entered by the user
    int robotId = wxAtoi(robotStatusId->GetValue());

    // Retrieve the robot's status
    std::string status = sim.robot_status(robotId);
    wxMessageBox(status, "Robot Status", wxOK | wxICON_INFORMATION);
}

void BuildingS::RobotRefill(wxCommandEvent& event) {
    int robotId = wxAtoi(robotRefillId->GetValue());

    std::string message = sim.fill_robot_fluid(robotId);
        wxMessageBox(message, "Robot Refill Status", wxOK | wxICON_INFORMATION);

}
