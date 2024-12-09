#include "Frames/bs.hpp"
#include "RoundedButton.hpp"
#include "Robot.hpp"
#include "Simulation.hpp"
#include "Database.hpp"

// Event table that connects the button to the event handler
wxBEGIN_EVENT_TABLE(BuildingS, wxFrame)
    EVT_BUTTON(1001, BuildingS::RobotStatus)  // Event binding: button click (ID 1001) triggers AddingRobot4
    EVT_BUTTON(1002, BuildingS::RobotRefill) 
    EVT_BUTTON(1003, BuildingS::TaskHistory) 
    EVT_BUTTON(1004, BuildingS::CleanRooms) 
    EVT_BUTTON(1005, BuildingS::TentativeCompletionTime) 
wxEND_EVENT_TABLE()

// Constructor definition
BuildingS::BuildingS(const wxString& title, Simulation& sim, Database& db) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(500, 650)), sim(sim), db(db) {
    // Create a panel for holding the GUI components
    wxPanel* panel = new wxPanel(this, wxID_ANY);
    panel->SetBackgroundColour(wxColour("#0d1c3f"));  // Dark blue background

    // Main vertical sizer for the layout
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Title label
    wxStaticText* titleLabel = new wxStaticText(panel, wxID_ANY, "Building Staff Controls", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    titleLabel->SetFont(wxFont(17, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    titleLabel->SetForegroundColour(wxColour("#ffffff"));
    mainSizer->Add(titleLabel, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 15);

    // Robot Status Section
    wxBoxSizer* robotStatusSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* robotStatusIdLabel = new wxStaticText(panel, wxID_ANY, "Enter a viable robot id:", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    robotStatusIdLabel->SetForegroundColour(wxColour("#ffffff"));
    robotStatusSizer->Add(robotStatusIdLabel, 0, wxALIGN_LEFT | wxLEFT | wxBOTTOM, 5);

    robotStatusId = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(300, 30));
    robotStatusSizer->Add(robotStatusId, 0, wxALIGN_CENTER | wxBOTTOM, 10);

    robotStatusSizer->Add(new RoundedButton(panel, 1001, "Robot Status"), 0, wxALIGN_CENTER | wxBOTTOM, 15);
    mainSizer->Add(robotStatusSizer, 0, wxALIGN_CENTER);

    // Robot Refill Section
    wxBoxSizer* robotRefillSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* robotRefillIdLabel = new wxStaticText(panel, wxID_ANY, "Enter a viable robot id:", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    robotRefillIdLabel->SetForegroundColour(wxColour("#ffffff"));
    robotRefillSizer->Add(robotRefillIdLabel, 0, wxALIGN_LEFT | wxLEFT | wxBOTTOM, 5);

    robotRefillId = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(300, 30));
    robotRefillSizer->Add(robotRefillId, 0, wxALIGN_CENTER | wxBOTTOM, 10);

    robotRefillSizer->Add(new RoundedButton(panel, 1002, "Refill Robot"), 0, wxALIGN_CENTER | wxBOTTOM, 15);
    mainSizer->Add(robotRefillSizer, 0, wxALIGN_CENTER);

    // Tentative Completion Time Section
    wxBoxSizer* completionSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* tCTLabel = new wxStaticText(panel, wxID_ANY, "Enter a Room ID:", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    tCTLabel->SetForegroundColour(wxColour("#ffffff"));
    completionSizer->Add(tCTLabel, 0, wxALIGN_LEFT | wxLEFT | wxBOTTOM, 5);

    tentativeCompletionRoomId = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(300, 30));
    completionSizer->Add(tentativeCompletionRoomId, 0, wxALIGN_CENTER | wxBOTTOM, 10);

    completionSizer->Add(new RoundedButton(panel, 1005, "Calculate Room Completion Time"), 0, wxALIGN_CENTER | wxBOTTOM, 15);
    completionTime = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    completionTime->SetForegroundColour(wxColour("#ffffff"));
    completionSizer->Add(completionTime, 0, wxALIGN_CENTER | wxTOP, 5);

    mainSizer->Add(completionSizer, 0, wxALIGN_CENTER);

    // Unclean Rooms Section
    wxBoxSizer* cleanRoomsSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* cleanThresholdLabel = new wxStaticText(panel, wxID_ANY, "Enter Threshold:", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    cleanThresholdLabel->SetForegroundColour(wxColour("#ffffff"));
    cleanRoomsSizer->Add(cleanThresholdLabel, 0, wxALIGN_LEFT | wxLEFT | wxBOTTOM, 5);

    cleanThreshold = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(300, 30));
    cleanRoomsSizer->Add(cleanThreshold, 0, wxALIGN_CENTER | wxBOTTOM, 10);

    cleanRoomsSizer->Add(new RoundedButton(panel, 1004, "List of Unclean Rooms"), 0, wxALIGN_CENTER | wxBOTTOM, 15);
    mainSizer->Add(cleanRoomsSizer, 0, wxALIGN_CENTER);

    // Ongoing Tasks Section
    mainSizer->Add(new RoundedButton(panel, 1003, "List of Ongoing Tasks"), 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 15);

    // Set the layout
    panel->SetSizer(mainSizer);
}

// Event handler: Creates a Message Box to Display to the Status of a Robot
void BuildingS::RobotStatus(wxCommandEvent& event) {
    // Retrieve the robot ID entered by the user
    int robotId = wxAtoi(robotStatusId->GetValue());

    // Retrieve the robot's status
    std::string status = sim.get_robot_info(robotId);
    wxMessageBox(status, "Robot Status", wxOK | wxICON_INFORMATION);
}

// Event handler: Refills the amount of fluid in a robot
void BuildingS::RobotRefill(wxCommandEvent& event) {
    //Retrieve value from input field
    int robotId = wxAtoi(robotRefillId->GetValue());

    //provide feedback message to user
    std::string message = sim.fill_robot_fluid(robotId);
    wxMessageBox(message, "Robot Refill Status", wxOK | wxICON_INFORMATION);

    //Clear input field
    robotRefillId->Clear();
}

// Event handler: Shows the List of Ongoing Tasks
void BuildingS::TaskHistory(wxCommandEvent& event){
    std::string result = "";

    //Getting the list of task from Simulaiton
    unordered_map<int, std::string> tasks = sim.getOngoingTasks();

    //Going through all the rooms and creating a giant string
    for (const auto& pair: tasks){
        result += std::to_string(pair.first) + "\t" + pair.second + "\n";
    }

    //Showing that information in a message box
    wxMessageBox(result, "Ongoing Tasks", wxOK | wxICON_INFORMATION);
}

// Event handler: Shows the list of Unclean Rooms
void BuildingS::CleanRooms(wxCommandEvent& event){
    //Do Nothing if the Threshold is set to null
    if (cleanThreshold->GetValue() == ""){
        return;
    } 
    
    //getting value from the GUI
    int thresh = wxAtoi(cleanThreshold->GetValue());   
    std::string result = "";

    //getting the status of rooms from simulation
    unordered_map<std::string, int> info = sim.getAllRoomCleanliness();

    //Going through each room to see if they are dirty and add them onto the list
    for (const auto& pair : info) {
        std::string room =  pair.first;
        int cleanliness = pair.second;

        //filtering with the threshold value
        if (cleanliness >= thresh){
            result += room + "\t" + std::to_string(cleanliness)+ "%\n";
        }

    }

    //Showing the result to user in the wxMessageBox
    wxMessageBox(result, "Room's Cleanliness Information", wxOK | wxICON_INFORMATION);
}

void BuildingS::TentativeCompletionTime(wxCommandEvent& event){
    int roomId = wxAtoi(tentativeCompletionRoomId->GetValue());

    int result = sim.completionTime(std::to_string(roomId));

    if (result == -1){
        wxMessageBox("No Robots are cleaning the room at the moment", "Room's Tentative Cleaning Time", wxOK | wxICON_INFORMATION);

    } else if ( result == -2){
        wxMessageBox("Room Id doesn't exist", "Room's Tentative Cleaning Time", wxOK | wxICON_INFORMATION);

    } else{
        completionTime->SetLabel(std::to_string(result) + " seconds");
    } 
}

