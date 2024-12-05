#include "Frames/bs.hpp"
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
    wxStaticText* robotRefillIdLabel = new wxStaticText(panel, wxID_ANY, "Enter a viable robot id", wxPoint(10, 130));
    robotRefillId = new wxTextCtrl(panel, wxID_ANY, "", wxPoint(10,150), wxSize(200, -1));
    
    // Create a button that will trigger the AddingRobot event
    wxButton* robotRefillButton = new wxButton(panel, 1002, "Refill Robot", wxPoint(10, 185));

    //Getting tentative time for unclean rooms
    wxStaticText* tCTLabel = new wxStaticText(panel, wxID_ANY, "Tentative Clearning Time", wxPoint(10, 250));
    tentativeCompletionRoomId = new wxTextCtrl(panel, wxID_ANY, "Room Id", wxPoint(180,245), wxSize(100, -1));
    wxButton* calculateTCTButton = new wxButton(panel, 1005, "Calculate Completion Time", wxPoint(10, 280));
    completionTime = new wxStaticText(panel, wxID_ANY, "", wxPoint(250, 285));

    //Getting list of Unclean Room
    // wxStaticText* listUncleanLabel = new wxStaticText(panel, wxID_ANY, "List of Uncleaned Rooms", wxPoint(10, 250));
    wxStaticText* listUncleanThreshLabel = new wxStaticText(panel, wxID_ANY, "Enter Threshold", wxPoint(10, 350));
    cleanThreshold = new wxTextCtrl(panel, wxID_ANY, "80", wxPoint(125,345), wxSize(50, -1));
    wxButton* cleanRoomsButton = new wxButton(panel, 1004, "List of Unclean Rooms", wxPoint(10, 380));

    //Getting list of ongoing task
    wxButton* ongoingTasksButton = new wxButton(panel, 1003, "List of Ongoing Task", wxPoint(10, 450));

    // Set the window size for the SubFrame
    this->SetSize(400, 550);
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

