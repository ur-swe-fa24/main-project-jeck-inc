#include "Frames/fe.hpp"
#include "Robot.hpp"
#include "Simulation.hpp"
#include "Database.hpp"

// Event table that connects the button to the event handler
wxBEGIN_EVENT_TABLE(FieldE, wxFrame)
    EVT_BUTTON(1001, FieldE::ShowRobots)
    EVT_BUTTON(1002, FieldE::FixRobot)
    EVT_BUTTON(1003, FieldE::GetRobotStatus)
    EVT_BUTTON(1004, FieldE::GoBack)
wxEND_EVENT_TABLE()

FieldE::FieldE(const wxString& title, Simulation& sim, Database& db)
    : wxFrame(nullptr, wxID_ANY, title), sim(sim), db(db) {

    // Create a panel for holding the GUI components
    wxPanel* panel = new wxPanel(this, wxID_ANY);

    // Create buttons and input fields
    wxButton* showRobotsButton = new wxButton(panel, 1001, "Show All Robots", wxPoint(10, 10));

    wxStaticText* robotIdFixLabel = new wxStaticText(panel, wxID_ANY, "Fix Robot ID:", wxPoint(150, 70));
    robotIdFixInput = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(150, 90), wxSize(100, -1));
    wxButton* fixRobotButton = new wxButton(panel, 1002, "Fix Robot", wxPoint(10, 90));

    wxStaticText* robotIdStatusLabel = new wxStaticText(panel, wxID_ANY, "Status Robot ID:", wxPoint(150, 140));
    robotIdStatusInput = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(150, 160), wxSize(100, -1));
    wxButton* getStatusButton = new wxButton(panel, 1003, "Robot Status", wxPoint(10, 160));

    // wxButton* backButton = new wxButton(panel, 1004, "Back", wxPoint(10, 200));

    // Set the window size
    this->SetSize(400, 400);
}


// Event handler: Showing all robots
void FieldE::ShowRobots(wxCommandEvent& event) {
    // Retrieve and display all robot IDs from the simulation
    wxMessageBox("Robots: \n" + sim.getRobotIds(), "Robot IDs", wxOK | wxICON_INFORMATION);
}

// Event handler: Fixing a specific robot
void FieldE::FixRobot(wxCommandEvent& event) {
    // Retrieve the robot ID entered by the user
    int robotId = wxAtoi(robotIdFixInput->GetValue());

    // Fix the robot in the simulation
    std::string result = sim.fix_robot(robotId);
    wxMessageBox(result, "Fix Robot", wxOK | wxICON_INFORMATION);

    //Clear input field
    robotIdFixInput->Clear();
}

// Event handler: Getting the status of a specific robot
void FieldE::GetRobotStatus(wxCommandEvent& event) {
    // Retrieve the robot ID entered by the user
    int robotId = wxAtoi(robotIdStatusInput->GetValue());

    // Retrieve the robot's status
    std::string status = sim.get_robot_info(robotId);
    wxMessageBox(status, "Robot Status", wxOK | wxICON_INFORMATION);

    //Clear input field
    robotIdStatusInput->Clear();
}

void FieldE::GoBack(wxCommandEvent& event) {
    // Stop the simulation if necessary and return to the main frame
    this->Close(true);
}


