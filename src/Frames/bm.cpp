
#include "Frames/bm.hpp"
#include "Robot.hpp"
#include "Simulation.hpp"
#include "Database.hpp"


//Function Definition that will be utilized later
Robot::Function getFunctionFromInput(int choice);
Robot::Size getSizeFromInput(int choice);

// Event table that connects the button to the event handler
wxBEGIN_EVENT_TABLE(BuildingM, wxFrame)
    EVT_BUTTON(1001, BuildingM::AddingRobot)  // Event binding: button click (ID 1001) triggers AddingRobot
    EVT_BUTTON(1002, BuildingM::AssignTask)
wxEND_EVENT_TABLE()

// Constructor definition
BuildingM::BuildingM(const wxString& title, Simulation& sim, Database& db)
    : wxFrame(nullptr, wxID_ANY, title), sim(sim), db(db){

    // Create a panel for holding the GUI components
    wxPanel* panel = new wxPanel(this, wxID_ANY);

    //Getting size input
    wxStaticText* sizeLabel = new wxStaticText(panel, wxID_ANY, "Enter robot size (1: Small, 2: Medium, 3: Large):", wxPoint(10, 10));
    sizeInput = new wxTextCtrl(panel, wxID_ANY, "(1 - 2 - 3)", wxPoint(10, 30), wxSize(200, -1));

    //Getting function input
    wxStaticText* functionLabel = new wxStaticText(panel, wxID_ANY, "Enter robot function (1: Shampoo, 2: Vacuum, 3: Scrub):", wxPoint(10, 60));
    functionInput = new wxTextCtrl(panel, wxID_ANY, "(1 - 2 - 3)", wxPoint(10, 80), wxSize(200, -1));

    wxButton* addButton = new wxButton(panel, 1001, "Add Robot", wxPoint(10, 120));


    //Getting size input
    wxStaticText* robotIdLabel = new wxStaticText(panel, wxID_ANY, "Enter the robot id: (int value)", wxPoint(10, 190));
    robotId = new wxTextCtrl(panel, wxID_ANY, "(1 - 2 - 3)", wxPoint(10, 210), wxSize(200, -1));

    //Getting function input
    wxStaticText* roomIdLabel = new wxStaticText(panel, wxID_ANY, "Enter the room id: (int value)", wxPoint(10,240));
    roomId = new wxTextCtrl(panel, wxID_ANY, "(1 - 2 - 3)", wxPoint(10, 260), wxSize(200, -1));

    wxButton* assignTaskButton = new wxButton(panel, 1002, "Assign Robot", wxPoint(10, 300));

    // Set the window size for the SubFrame
    this->SetSize(400, 400);
}

// Event handler: Adding a robot to the simulation
void BuildingM::AddingRobot(wxCommandEvent& event) {
    //Getting Values from GUI
    int sizeChoice = wxAtoi(sizeInput->GetValue());
    int functionChoice = wxAtoi(functionInput->GetValue());

    // Creation of the robot
    Robot::Function func = getFunctionFromInput(functionChoice);
    Robot::Size size = getSizeFromInput(sizeChoice);
    Robot myRobot(func, size);

    //Adding Robot in both components: Simulation & Database
    sim.add_robot(myRobot);
    db.add_robot(myRobot);

    //Message of Successful Creation
    wxMessageBox("Robot added successfully with id: " + std::to_string(myRobot.getId()), "Success", wxOK | wxICON_INFORMATION);

    //Clearing the input field
    sizeInput->Clear();
    functionInput->Clear();
    
}

void BuildingM::AssignTask(wxCommandEvent& event) {
    //getting values from input field
    int robotChoice = wxAtoi(robotId->GetValue());
    std::string roomChoice = std::string(roomId->GetValue().mb_str(wxConvUTF8));

    //Message back to user
    std::string message = sim.assign_task(robotChoice, roomChoice);
    wxMessageBox(message, "Success", wxOK | wxICON_INFORMATION);

    //Clear Input Field
    robotId->Clear();
    roomId->Clear();
}

// Function to convert user input to the corresponding enum value
Robot::Function getFunctionFromInput(int choice) {
    switch (choice) {
        case 1: return Robot::Function::Shampoo;
        case 2: return Robot::Function::Vacuum;
        case 3: return Robot::Function::Scrub;
        default: throw std::invalid_argument("Invalid choice for function");
    }
}

Robot::Size getSizeFromInput(int choice) {
    switch (choice) {
        case 1: return Robot::Size::Small;
        case 2: return Robot::Size::Medium;
        case 3: return Robot::Size::Large;
        default: throw std::invalid_argument("Invalid choice for size");
    }
}

