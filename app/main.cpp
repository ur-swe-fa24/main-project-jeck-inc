/*
This is our main.cpp. Also, our GUI-System Manager component. 
The wxWidget is the main controller of the system. The user interacts with the GUI (which have links to  other GUIs, each designated for a role) and provide us the relevant information. 
The input retrieved from the GUI is used by the System Mangaer aspect of the code to demand actions from Simulation or Database. 
Along with the threads created by the GUI, the System Manager also creates a thread for the simulation, a thread for updating database and a thread for notifications.
*/



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

#include "Frames/sm.hpp"
#include "Frames/bm.hpp"
#include "Frames/bs.hpp"
#include "Frames/fe.hpp"

using namespace database;
using namespace robot;
using namespace simulation;


//Definition of functions and values
void updateDatabase(Simulation& sim, Database& db);
bool live = true;
void createNotification(Simulation& sim, Database& db);



// Main application class
class MyApp : public wxApp {
public:
    virtual bool OnInit();
    
};

// Frame class
class Home : public wxFrame {
public:
    Home(const wxString& title);
    

private:
    // Event handlers of Each Button
    void OnSM(wxCommandEvent& event);
    void OnBM(wxCommandEvent& event);
    void OnBS(wxCommandEvent& event);
    void OnFE(wxCommandEvent& event);
    void Quit(wxCommandEvent& event);

    
    // Simulation sim(Database db);
    Simulation sim;

    mongocxx::instance currInst{};
    Database db; //Create the database object.

    std::thread simulationThread; //thread to run simulation in the background
    std::thread databaseThread;  //thread to update database
    std::thread notificationThread; //thread for notification

    //Declaring Event Table
    wxDECLARE_EVENT_TABLE();
};




//Event Table
wxBEGIN_EVENT_TABLE(Home, wxFrame)
    EVT_BUTTON(1001, Home::OnSM)
    EVT_BUTTON(1002, Home::OnBM)
    EVT_BUTTON(1003, Home::OnBS)
    EVT_BUTTON(1004, Home::OnFE)
    EVT_BUTTON(1005, Home::Quit)

wxEND_EVENT_TABLE()




//Calling the App
wxIMPLEMENT_APP(MyApp);

//Actually running the GUI
bool MyApp::OnInit() {
    Home* home = new Home("Frame Test");
    home->Show(true);
    return true;
}


//Main Fram Constructor
Home::Home(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    
    // Input json file for building configuration
    std::string file_name = "../../app/building.json";
    sim.load_building(file_name);

    // Start the simulation in a detached thread
    simulationThread = std::thread(&Simulation::simulate, std::ref(sim));
    simulationThread.detach(); // Detach the thread

    //Start updating the database
    databaseThread = std::thread(updateDatabase, std::ref(sim), std::ref(db));
    databaseThread.detach();

    //Start sending notifications as they come
    notificationThread = std::thread(createNotification, std::ref(sim), std::ref(db));
    notificationThread.detach();


    //Creating Elements (Buttons and Form Field) for GUI
    wxPanel* panel = new wxPanel(this, wxID_ANY);

    wxStaticText* label = new wxStaticText(panel, wxID_ANY, "Choose User Role:", wxPoint(10, 10));
    
    wxButton* seniorM = new wxButton(panel, 1001, "Senior Manager", wxPoint(10, 50));
    wxButton* buildingM = new wxButton(panel, 1002, "Building Manager", wxPoint(10, 90));
    wxButton* buildingS = new wxButton(panel, 1003, "Building Staff", wxPoint(10, 130));
    wxButton* fieldM = new wxButton(panel, 1004, "Field Engineer", wxPoint(10, 170));
    wxButton* quit = new wxButton(panel, 1005, "Quit", wxPoint(10, 210));


    //Setting the size of the GUI
    this->SetSize(400, 350);

}

//Pressing Senior Manager will take you to SM Frame
void Home::OnSM(wxCommandEvent& event){
    SeniorM* smFrame = new SeniorM("Senior Manager", std::ref(sim), std::ref(db));
    smFrame->Show(true);
}

//Pressing Building Manager will take you to BM Frame
void Home::OnBM(wxCommandEvent& event){
    BuildingM* bmFrame = new BuildingM("Building Manager", std::ref(sim), std::ref(db));
    bmFrame->Show(true);
}

//Pressing Building Staff will take you to BS Frame
void Home::OnBS(wxCommandEvent& event){
    BuildingS* bsFrame = new BuildingS("Building Staff", std::ref(sim), std::ref(db));
    bsFrame->Show(true);
}

//Pressing Field Engineer will take you to FE Frame
void Home::OnFE(wxCommandEvent& event){
    FieldE* feFrame = new FieldE("Field Engineer", std::ref(sim), std::ref(db));
    feFrame->Show(true);
}

//Event when Quit Button is pressed
void Home::Quit(wxCommandEvent& event) {
    sim.stop(); //Ending the thread
    live=false;
    Close(true);
}

void updateDatabase(Simulation& sim, Database& db){
    std::cout << "Things happening" << std::endl;
    while (live){
        
        
        std::this_thread::sleep_for(std::chrono::milliseconds(30000)); // Sleep for 30 seconds

    }

}

void createNotification(Simulation& sim, Database& db){
    std::cout << "Notification Center" << std::endl;
    while (live){
        std::vector<int> faultyRobots = sim.getFaultyRobots();

        for (auto robo : faultyRobots){
            wxMessageBox("Robot Id: " + std::to_string(robo) + " is currently faulty and requires immediate attention", "Robot Status", wxOK | wxICON_INFORMATION);
        }

        unordered_set<std::string> taskComplete = sim.getTasksCompleted();

        for (auto task : taskComplete){
            wxMessageBox("Cleaning task for room " + task + " has just been completed.", "Robot Status", wxOK | wxICON_INFORMATION);
        }


        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Sleep for 1 second
    }
}
 






