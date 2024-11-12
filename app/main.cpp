#include <thread>
#include <string>
#include <wx/wx.h>
#include <wx/log.h>
#include <wx/file.h>
#include <wx/txtstrm.h>
#include <wx/textfile.h>
#include "Robot.hpp"
#include "Database.hpp"
#include "Simulation.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

using namespace database;
using namespace robot;
using namespace simulation;

//Function Definition that will be utilized later
Robot::Function getFunctionFromInput(int choice);
Robot::Size getSizeFromInput(int choice);

// Main application class
class MyApp : public wxApp {
public:
    virtual bool OnInit();
    
};

// Frame class
class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title);
    

private:
    // Event handlers of Each Button
    void OnAddRobot(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void ShowRobots(wxCommandEvent& event);
    void FixRobot(wxCommandEvent& event);
    void GetStatus(wxCommandEvent& event);

    //Variables to Store Input from GUI
    wxTextCtrl* sizeInput;
    wxTextCtrl* functionInput;
    wxTextCtrl* fixRobotInput;
    wxTextCtrl* robotStatusInput;

    
    // Simulation sim(Database db);
    Simulation sim;

    mongocxx::instance currInst{};
    Database db; //Create the database object.

    std::thread simulationThread; //thread to run simulation in the background

    //Declaring Event Table
    wxDECLARE_EVENT_TABLE();
};

//Event Table
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_BUTTON(1001, MyFrame::OnAddRobot)
    EVT_BUTTON(1002, MyFrame::OnQuit)
    EVT_BUTTON(1003, MyFrame::ShowRobots)
    EVT_BUTTON(1004, MyFrame::FixRobot)
    EVT_BUTTON(1005, MyFrame::GetStatus)
wxEND_EVENT_TABLE()

//Calling the App
wxIMPLEMENT_APP(MyApp);

//Actually running the GUI
bool MyApp::OnInit() {
    MyFrame* frame = new MyFrame("Frame Test");
    frame->Show(true);
    return true;
}


//Main Fram Constructor
MyFrame::MyFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    
    // Input json file for building configuration
    std::string file_name = "../../app/building.json";
    sim.load_building(file_name);

    // Start the simulation in a detached thread
    simulationThread = std::thread(&Simulation::simulate, std::ref(sim));
    simulationThread.detach(); // Detach the thread

    //Creating Elements (Buttons and Form Field) for GUI
    wxPanel* panel = new wxPanel(this, wxID_ANY);

    wxStaticText* sizeLabel = new wxStaticText(panel, wxID_ANY, "Enter robot size (1: Small, 2: Medium, 3: Large):", wxPoint(10, 10));
    sizeInput = new wxTextCtrl(panel, wxID_ANY, "(1 - 2 - 3)", wxPoint(10, 30), wxSize(200, -1));

    wxStaticText* functionLabel = new wxStaticText(panel, wxID_ANY, "Enter robot function (1: Shampoo, 2: Vacuum, 3: Scrub):", wxPoint(10, 60));
    functionInput = new wxTextCtrl(panel, wxID_ANY, "(1 - 2 - 3)", wxPoint(10, 80), wxSize(200, -1));

    wxButton* addButton = new wxButton(panel, 1001, "Add Robot", wxPoint(10, 110));

    wxButton* showRobots = new wxButton(panel, 1003, "Show all Robots", wxPoint(10, 160));

    fixRobotInput = new wxTextCtrl(panel, wxID_ANY, "Robot ID", wxPoint(150, 200), wxSize(100, -1));
    wxButton* fixButton = new wxButton(panel, 1004, "Fix Robot", wxPoint(10, 200));

    robotStatusInput = new wxTextCtrl(panel, wxID_ANY, "Robot ID", wxPoint(150, 230), wxSize(100, -1));
    wxButton* getStatusButton = new wxButton(panel, 1005, "Robot Status", wxPoint(10, 230));

    wxButton* quitButton = new wxButton(panel, 1002, "Quit", wxPoint(10, 260));

    //Setting the size of the GUI
    this->SetSize(400, 350);

}

// Event when Add Robot is clicked
void MyFrame::OnAddRobot(wxCommandEvent& event) {
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
    std::cout << db.getRobotID(myRobot) << std::endl;
    wxMessageBox("Robot added successfully with id: " + std::to_string(myRobot.getId()), "Success", wxOK | wxICON_INFORMATION);

    //Clearing the input field
    sizeInput->Clear();
    functionInput->Clear();
}

//Event when Quit Button is pressed
void MyFrame::OnQuit(wxCommandEvent& event) {

    sim.stop(); //Ending the thread
    Close(true);
}

//Event when FixRobot Button is pressed
void MyFrame::FixRobot(wxCommandEvent& event){
    //Getting Robot Id 
    int fixChoice = wxAtoi(fixRobotInput->GetValue());

    //Getting Status through Simulation
    std::string message = sim.fix_robot(fixChoice);
    wxMessageBox(message, "Fix Robot", wxOK | wxICON_INFORMATION);

    //Clearing the Field
    fixRobotInput->Clear();
}


//Event when Show Robot Button is Pressed
void MyFrame::ShowRobots(wxCommandEvent& event){
    //Prints out the list of robot ids in a WBox
    wxMessageBox("Robots: \n" + sim.getRobotIds(), "Robot IDs", wxOK | wxICON_INFORMATION);
}

//Event when GetStatus is Pressed
void MyFrame::GetStatus(wxCommandEvent& event){
    //Getting robot id from GUI
    int statusChoice = wxAtoi(robotStatusInput->GetValue());

    //Using sim to print out the status
    std::string message = sim.robot_status(statusChoice);
    wxMessageBox(message, "Robot Status", wxOK | wxICON_INFORMATION);
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


// Legacy Code
// int main(int argc, char **argv) {

//     wxEntry(argc, argv);  // Initialize the wxWidgets application
//     return 0;
// }