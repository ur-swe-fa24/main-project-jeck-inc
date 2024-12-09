#include "Frames/fe.hpp"
#include "RoundedButton.hpp"
#include "Robot.hpp"
#include "Simulation.hpp"
#include "Database.hpp"

// Event table that connects the button to the event handler
wxBEGIN_EVENT_TABLE(FieldE, wxFrame)
    EVT_BUTTON(1001, FieldE::ShowRobots)
    EVT_BUTTON(1002, FieldE::FixRobot)
    EVT_BUTTON(1003, FieldE::GetRobotStatus)
wxEND_EVENT_TABLE()

FieldE::FieldE(const wxString& title, Simulation& sim, Database& db) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(500, 650)), sim(sim), db(db) {
    // Create a panel for holding the GUI components
    wxPanel* panel = new wxPanel(this, wxID_ANY);
    panel->SetBackgroundColour(wxColour("#0d1c3f"));  // Dark blue background

    // Main vertical sizer for layout
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Title Label
    wxStaticText* titleLabel = new wxStaticText(panel, wxID_ANY, "Field Engineer Controls", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    titleLabel->SetFont(wxFont(17, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    titleLabel->SetForegroundColour(wxColour("#ffffff"));
    mainSizer->Add(titleLabel, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 25);  // Adjusted vertical spacing

    // Show All Robots Button
    mainSizer->Add(new RoundedButton(panel, 1001, "Show All Robots"), 0, wxALIGN_CENTER | wxBOTTOM, 30);  // Increased bottom margin

    // Fix Robot Section
    wxBoxSizer* fixRobotSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* robotIdFixLabel = new wxStaticText(panel, wxID_ANY, "Fix Robot ID:", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    robotIdFixLabel->SetForegroundColour(wxColour("#ffffff"));
    fixRobotSizer->Add(robotIdFixLabel, 0, wxALIGN_LEFT | wxBOTTOM, 5);

    robotIdFixInput = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(300, 30));
    fixRobotSizer->Add(robotIdFixInput, 0, wxALIGN_CENTER | wxBOTTOM, 10);

    fixRobotSizer->Add(new RoundedButton(panel, 1002, "Fix Robot"), 0, wxALIGN_CENTER | wxBOTTOM, 30);  // Added more space below
    mainSizer->Add(fixRobotSizer, 0, wxALIGN_CENTER);

    // Robot Status Section
    wxBoxSizer* statusRobotSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* robotIdStatusLabel = new wxStaticText(panel, wxID_ANY, "Status Robot ID:", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    robotIdStatusLabel->SetForegroundColour(wxColour("#ffffff"));
    statusRobotSizer->Add(robotIdStatusLabel, 0, wxALIGN_LEFT | wxBOTTOM, 5);

    robotIdStatusInput = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(300, 30));
    statusRobotSizer->Add(robotIdStatusInput, 0, wxALIGN_CENTER | wxBOTTOM, 10);

    statusRobotSizer->Add(new RoundedButton(panel, 1003, "Robot Status"), 0, wxALIGN_CENTER | wxBOTTOM, 30);  // Added more space below
    mainSizer->Add(statusRobotSizer, 0, wxALIGN_CENTER);

    // Set the layout to the panel
    panel->SetSizer(mainSizer);
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