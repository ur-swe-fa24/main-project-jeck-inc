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
#include <wx/dcclient.h>
#include "Robot.hpp"
#include "Database.hpp"
#include "Simulation.hpp"
#include "RoundedButton.hpp"
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

// Initialize the GUI
bool MyApp::OnInit() {
    Home* home = new Home("Robot Fleet Management");
    home->Show(true);
    return true;
}

//Main Frame Constructor
Home::Home(const wxString& title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(500, 650)) {
    //Input json file for building configuration
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



    db.init_TaskCompletedAndErrorRates();
    db.init_analytics();

    //Creating Elements (Buttons and Form Field) for GUI
    wxPanel* panel = new wxPanel(this, wxID_ANY);
    panel->SetBackgroundColour(wxColour("#0d1c3f"));  //Set dark blue background color

    // Vertical box sizer for layout
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    // Cobotiq logo
    wxInitAllImageHandlers();  // Ensures PNG and other formats are supported
    wxImage::AddHandler(new wxPNGHandler());  // Add PNG handler
    wxBitmap logoBitmap(wxT("../../app/cobotiq-logo.png"), wxBITMAP_TYPE_PNG);
    wxImage logoImage = logoBitmap.ConvertToImage();  // Convert to wxImage
    logoImage.Rescale(350, 60, wxIMAGE_QUALITY_HIGH);  // Rescale the image to desired size in high quality
    wxBitmap scaledLogo(logoImage);  // Convert back to wxBitmap
    wxStaticBitmap* logo = new wxStaticBitmap(panel, wxID_ANY, scaledLogo);  // Add the scaled logo to the layout
    vbox->Add(logo, 0, wxALIGN_CENTER | wxTOP, 58);  // Add the logo with spacing

    // Warning Label
    wxStaticText* warning = new wxStaticText(panel, wxID_ANY, "*Not affiliated with Cobotiq", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    warning->SetFont(wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    warning->SetForegroundColour(wxColour("#dedede"));
    vbox->Add(warning, 0, wxALIGN_CENTER | wxTOP, 0);  // Spacing below the logo

    // Title label
    wxStaticText* label = new wxStaticText(panel, wxID_ANY, "Choose User Role:", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    label->SetFont(wxFont(17, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    label->SetForegroundColour(wxColour("#dedede"));
    vbox->Add(label, 0, wxALIGN_CENTER | wxTOP, 32);  // Spacing below the logo

    // Add instances of RoundedButton
    vbox->Add(new RoundedButton(panel, 1001, "Senior Manager"), 0, wxALIGN_CENTER | wxTOP, 15);
    vbox->Add(new RoundedButton(panel, 1002, "Building Manager"), 0, wxALIGN_CENTER | wxTOP, 15);
    vbox->Add(new RoundedButton(panel, 1003, "Building Staff"), 0, wxALIGN_CENTER | wxTOP, 15);
    vbox->Add(new RoundedButton(panel, 1004, "Field Engineer"), 0, wxALIGN_CENTER | wxTOP, 15);
    vbox->Add(new RoundedButton(panel, 1005, "Quit", wxColour("#B02121"), wxColour("#B22222")), 0, wxALIGN_CENTER | wxTOP, 15);

    // Developer label
    wxStaticText* devLabel = new wxStaticText(panel, wxID_ANY, "Developed by JECK Inc.", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    devLabel->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    devLabel->SetForegroundColour(wxColour("#dedede"));
    vbox->Add(devLabel, 0, wxALIGN_CENTER | wxTOP, 35);  // Spacing below the buttons

    // Set the layout to the panel
    panel->SetSizer(vbox);
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
        unordered_map<std::string, int> results = sim.getDBStats();
        vector<int> faultyRobots = sim.getFaultyRobots();

        // db.updateSM(0, faultyRobots, results["totalNumRobots"], results["numTasksCompleted"]);
        db.updateTCER(results["numTasksCompleted"], faultyRobots.size());
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