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
    void OnAddRobot(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void ShowRobots(wxCommandEvent& event);
    void FixRobot(wxCommandEvent& event);
    void GetStatus(wxCommandEvent& event);

    wxTextCtrl* sizeInput;
    wxTextCtrl* functionInput;
    wxTextCtrl* fixRobotInput;
    wxTextCtrl* robotStatusInput;

    // Simulation sim(Database db);
    Simulation sim;

    mongocxx::instance currInst{};
    Database db; //Create the database object.

    std::thread simulationThread; 


    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_BUTTON(1001, MyFrame::OnAddRobot)
    EVT_BUTTON(1002, MyFrame::OnQuit)
    EVT_BUTTON(1003, MyFrame::ShowRobots)
    EVT_BUTTON(1004, MyFrame::FixRobot)
    EVT_BUTTON(1005, MyFrame::GetStatus)
wxEND_EVENT_TABLE()

bool MyApp::OnInit() {
    MyFrame* ui = new MyFrame("C++ GUI");
    ui->SetClientSize(800,600);
    ui->Center();
    ui->Show();

    // MyFrame* frame = new MyFrame("Frame Test");
    // frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    // Input json file for building configuration
    std::string file_name = "../../app/building.json";
    sim.load_building(file_name);

    // Start the simulation in a detached thread
    simulationThread = std::thread(&Simulation::simulate, std::ref(sim));
    simulationThread.detach(); // Detach the thread

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


    this->SetSize(400, 350);

}


void MyFrame::OnAddRobot(wxCommandEvent& event) {
    int sizeChoice = wxAtoi(sizeInput->GetValue());
    int functionChoice = wxAtoi(functionInput->GetValue());

    // try {
    Robot::Function func = getFunctionFromInput(functionChoice);
    Robot::Size size = getSizeFromInput(sizeChoice);
    Robot myRobot(func, size);
    // Robot* myRobot = new Robot(func, size);

    sim.add_robot(myRobot);
    db.add_robot(myRobot); //Add robot to the database

    wxMessageBox("Robot added successfully with id: " + std::to_string(myRobot.getId()), "Success", wxOK | wxICON_INFORMATION);
    // } catch (const std::invalid_argument& e) {
    //     wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
    // }

    sizeInput->Clear();
    functionInput->Clear();
}

void MyFrame::OnQuit(wxCommandEvent& event) {
    Close(true);
}

void MyFrame::FixRobot(wxCommandEvent& event){
    int fixChoice = wxAtoi(fixRobotInput->GetValue());
    std::string message = sim.fix_robot(fixChoice);
    wxMessageBox(message, "Fix Robot", wxOK | wxICON_INFORMATION);
    fixRobotInput->Clear();
}

void MyFrame::ShowRobots(wxCommandEvent& event){
    wxMessageBox("Robots: \n" + sim.getRobotIds(), "Robot IDs", wxOK | wxICON_INFORMATION);
}

void MyFrame::GetStatus(wxCommandEvent& event){
    int statusChoice = wxAtoi(robotStatusInput->GetValue());
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
} // Initialize the wxWidgets application

wxIMPLEMENT_APP(MyApp); 