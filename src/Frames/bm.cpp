#include "Frames/bm.hpp"
#include "RoundedButton.hpp"  // Include the custom button class
#include "Robot.hpp"
#include "Simulation.hpp"
#include "Database.hpp"

// Function Definition that will be utilized later
Robot::Function getFunctionFromInput(int choice);
Robot::Size getSizeFromInput(int choice);

// Event table that connects the button to the event handler
wxBEGIN_EVENT_TABLE(BuildingM, wxFrame)
    EVT_BUTTON(1001, BuildingM::AddingRobot)  // Event binding: button click (ID 1001) triggers AddingRobot
    EVT_BUTTON(1002, BuildingM::AssignTask)
    EVT_BUTTON(1003, BuildingM::BacklogCompletionTime)
wxEND_EVENT_TABLE()

BuildingM::BuildingM(const wxString& title, Simulation& sim, Database& db)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(500, 600)), sim(sim), db(db) {

    // Create a panel for holding the GUI components
    wxPanel* panel = new wxPanel(this, wxID_ANY);
    panel->SetBackgroundColour(wxColour("#0d1c3f"));  // Match the theme

    // Main vertical box sizer
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Title label
    wxStaticText* titleLabel = new wxStaticText(panel, wxID_ANY, "Building Manager Controls", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    titleLabel->SetFont(wxFont(17, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    titleLabel->SetForegroundColour(wxColour("#dedede"));
    mainSizer->Add(titleLabel, 0, wxALIGN_CENTER | wxTOP, 20);

    // Dropdowns section
    wxBoxSizer* dropdownSizer = new wxBoxSizer(wxVERTICAL);

    // Robot size dropdown
    wxStaticText* sizeLabel = new wxStaticText(panel, wxID_ANY, "Select robot size:", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    sizeLabel->SetForegroundColour(wxColour("#dedede"));
    dropdownSizer->Add(sizeLabel, 0, wxALIGN_LEFT | wxLEFT | wxBOTTOM, 5);

    sizeDropdown = new wxComboBox(panel, wxID_ANY, "", wxDefaultPosition, wxSize(300, 30), 0, nullptr, wxCB_READONLY);
    sizeDropdown->Append("Small");
    sizeDropdown->Append("Medium");
    sizeDropdown->Append("Large");
    dropdownSizer->Add(sizeDropdown, 0, wxALIGN_CENTER | wxBOTTOM, 15);

    // Robot function dropdown
    wxStaticText* functionLabel = new wxStaticText(panel, wxID_ANY, "Select robot function:", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    functionLabel->SetForegroundColour(wxColour("#dedede"));
    dropdownSizer->Add(functionLabel, 0, wxALIGN_LEFT | wxLEFT | wxBOTTOM, 5);

    functionDropdown = new wxComboBox(panel, wxID_ANY, "", wxDefaultPosition, wxSize(300, 30), 0, nullptr, wxCB_READONLY);
    functionDropdown->Append("Shampoo");
    functionDropdown->Append("Vacuum");
    functionDropdown->Append("Scrub");
    dropdownSizer->Add(functionDropdown, 0, wxALIGN_CENTER | wxBOTTOM, 20);

    mainSizer->Add(dropdownSizer, 0, wxALIGN_CENTER | wxTOP, 20);

    // Add Robot button
    mainSizer->Add(new RoundedButton(panel, 1001, "Add Robot"), 0, wxALIGN_CENTER | wxBOTTOM, 20);

    // Text fields section
    wxBoxSizer* textFieldSizer = new wxBoxSizer(wxVERTICAL);

    // Robot ID text field
    wxStaticText* robotIdLabel = new wxStaticText(panel, wxID_ANY, "Enter robot ID (integer):", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    robotIdLabel->SetForegroundColour(wxColour("#dedede"));
    textFieldSizer->Add(robotIdLabel, 0, wxALIGN_LEFT | wxLEFT | wxBOTTOM, 5);

    robotId = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(300, 30));
    textFieldSizer->Add(robotId, 0, wxALIGN_CENTER | wxBOTTOM, 15);

    // Room ID text field
    wxStaticText* roomIdLabel = new wxStaticText(panel, wxID_ANY, "Enter room ID (integer):", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    roomIdLabel->SetForegroundColour(wxColour("#dedede"));
    textFieldSizer->Add(roomIdLabel, 0, wxALIGN_LEFT | wxLEFT | wxBOTTOM, 5);

    roomId = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(300, 30));
    textFieldSizer->Add(roomId, 0, wxALIGN_CENTER | wxBOTTOM, 20);

    mainSizer->Add(textFieldSizer, 0, wxALIGN_CENTER | wxTOP, 20);

    // Assign Robot button
    mainSizer->Add(new RoundedButton(panel, 1002, "Assign Robot"), 0, wxALIGN_CENTER | wxBOTTOM, 20);

    // Set the layout
    panel->SetSizer(mainSizer);

    //
    wxStaticText* completionTimeLabel = new wxStaticText(panel, wxID_ANY, "Robot Backlog Completion Time", wxPoint(10,370));
    wxStaticText* robotIdCompletionLabel = new wxStaticText(panel, wxID_ANY, "Robot Id", wxPoint(10,400));

    robotIdCompletionTime = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(90, 395));
    wxButton* getCompletionTimeButton = new wxButton(panel, 1003, "Calculate Completion Time", wxPoint(10, 430));    
    completionTime = new wxStaticText(panel, wxID_ANY, "", wxPoint(290, 435));

    
    

    // Set the window size for the SubFrame
    this->SetSize(400, 520);
}


void BuildingM::BacklogCompletionTime(wxCommandEvent& event){
    int robotId = wxAtoi(robotIdCompletionTime->GetValue());

    int result = sim.robotCompletionTime(robotId);

    if (result == -1){
        wxMessageBox("Robot is not active at the moment", "Robot's Tentative Cleaning Time", wxOK | wxICON_INFORMATION);

    }else if ( result == -2){
        wxMessageBox("Robot Id doesn't exist", "Robot's Tentative Cleaning Time", wxOK | wxICON_INFORMATION);

    }else{
        completionTime->SetLabel(std::to_string(result) + " seconds");
    }
}

// Event handler: Adding a robot to the simulation
void BuildingM::AddingRobot(wxCommandEvent& event) {
    wxString sizeSelection = sizeDropdown->GetValue();
    wxString functionSelection = functionDropdown->GetValue();

    Robot::Size size;
    Robot::Function func;

    // Map size and function selections to enums
    if (sizeSelection == "Small")
        size = Robot::Size::Small;
    else if (sizeSelection == "Medium")
        size = Robot::Size::Medium;
    else if (sizeSelection == "Large")
        size = Robot::Size::Large;

    if (functionSelection == "Shampoo")
        func = Robot::Function::Shampoo;
    else if (functionSelection == "Vacuum")
        func = Robot::Function::Vacuum;
    else if (functionSelection == "Scrub")
        func = Robot::Function::Scrub;

    // Create and add robot
    //Getting Values from GUI
    std::vector<int> f = {1,2,3};
    int sizeChoice = wxAtoi(sizeInput->GetValue());
    int functionChoice = wxAtoi(functionInput->GetValue());

    // Creation of the robot
    Robot::Function func = getFunctionFromInput(functionChoice);
    Robot::Size size = getSizeFromInput(sizeChoice);
    Robot myRobot(func, size);
    sim.add_robot(myRobot);
    db.add_robot(myRobot);

    wxMessageBox("Robot added successfully with ID: " + std::to_string(myRobot.getId()), "Success", wxOK | wxICON_INFORMATION);

    // Clear selections
    sizeDropdown->SetSelection(wxNOT_FOUND);
    functionDropdown->SetSelection(wxNOT_FOUND);
}

void BuildingM::AssignTask(wxCommandEvent& event) {
    int robotChoice = wxAtoi(robotId->GetValue());
    wxString roomChoice = roomId->GetValue();

    std::string message = sim.assign_task(robotChoice, std::string(roomChoice.mb_str(wxConvUTF8)));
    wxMessageBox(message, "Success", wxOK | wxICON_INFORMATION);

    // Clear input fields
    robotId->Clear();
    roomId->Clear();
}
