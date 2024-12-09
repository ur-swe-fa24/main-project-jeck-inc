//***main.cpp***//
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
Home::Home(const wxString& title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(450, 550)) {
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

    //Creating Elements (Buttons and Form Field) for GUI
    wxPanel* panel = new wxPanel(this, wxID_ANY);
    panel->SetBackgroundColour(wxColour("#0d1c3f"));  //Set dark blue background color

    //Title label
    wxStaticText* label = new wxStaticText(panel, wxID_ANY, "Choose User Role:", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);  // CHANGED: Centered title label
    label->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));  //Styled label font


     // Vertical box sizer for layout
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
    vbox->Add(label, 0, wxALIGN_CENTER | wxTOP, 50);  // Adjusted spacing above the label

    // // Create custom-styled buttons
    // auto createStyledButton = [&](wxWindow* parent, int id, const wxString& labelText) -> wxButton* {
    //     wxButton* button = new wxButton(parent, id, labelText, wxDefaultPosition, wxSize(300, 50), wxBORDER_NONE);  // CHANGED: Removed default button border
    //     button->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));  // CHANGED: Bold font for text
    //     button->SetBackgroundColour(wxColour("#595959"));  // Dark Gray Background
    //     button->SetForegroundColour(wxColour("#ffffff"));  // Whiteish text
    //     button->SetWindowStyleFlag(wxBORDER_SIMPLE);  // CHANGED: Added simple border style
    //     // button->SetWindowBorderRadius(10);  // CHANGED: Added rounded corners (adjust radius as needed)
    //     return button;
    // };

    // Helper class for custom rounded buttons
    class RoundedButton : public wxPanel {
    public:
        RoundedButton(wxWindow* parent, int id, const wxString& label, const wxSize& size = wxSize(300, 50))
            : wxPanel(parent, id, wxDefaultPosition, size, wxBORDER_NONE), label(label) {
            SetBackgroundColour(wxColour("#0d1c3f"));  // Match parent background
            Bind(wxEVT_PAINT, &RoundedButton::OnPaint, this);
            Bind(wxEVT_LEFT_DOWN, &RoundedButton::OnClick, this);
        }

    private:
        wxString label;

        void OnPaint(wxPaintEvent& event) {
            wxPaintDC dc(this);

            // Set rounded rectangle background and border
            wxBrush brush(wxColour("#595959"));  // Dark gray background
            wxPen pen(wxColour("#A9A9A9"), 2);   // Gray border
            dc.SetBrush(brush);
            dc.SetPen(pen);
            dc.DrawRoundedRectangle(0, 0, GetSize().GetWidth(), GetSize().GetHeight(), 20);  // Rounded corners radius: 20

            // Draw button text
            dc.SetTextForeground(wxColour("#ffffff"));  // Whiteish text
            dc.SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
            wxSize textSize = dc.GetTextExtent(label);
            dc.DrawText(label, (GetSize().GetWidth() - textSize.GetWidth()) / 2,
                        (GetSize().GetHeight() - textSize.GetHeight()) / 2);
        }

        void OnClick(wxMouseEvent& event) {
            wxCommandEvent clickEvent(wxEVT_BUTTON, GetId());
            clickEvent.SetEventObject(this);
            GetParent()->ProcessWindowEvent(clickEvent);  // Forward the click event to the parent
        }
    };

    // Add instances of RoundedButton
    vbox->Add(new RoundedButton(panel, 1001, "Senior Manager"), 0, wxALIGN_CENTER | wxTOP, 20);
    vbox->Add(new RoundedButton(panel, 1002, "Building Manager"), 0, wxALIGN_CENTER | wxTOP, 20);
    vbox->Add(new RoundedButton(panel, 1003, "Building Staff"), 0, wxALIGN_CENTER | wxTOP, 20);
    vbox->Add(new RoundedButton(panel, 1004, "Field Engineer"), 0, wxALIGN_CENTER | wxTOP, 20);
    vbox->Add(new RoundedButton(panel, 1005, "Quit"), 0, wxALIGN_CENTER | wxTOP, 20);


    // // Add buttons with custom styling
    // vbox->Add(createStyledButton(panel, 1001, "Senior Manager"), 0, wxALIGN_CENTER | wxALL, 10);
    // vbox->Add(createStyledButton(panel, 1002, "Building Manager"), 0, wxALIGN_CENTER | wxALL, 10);
    // vbox->Add(createStyledButton(panel, 1003, "Building Staff"), 0, wxALIGN_CENTER | wxALL, 10);
    // vbox->Add(createStyledButton(panel, 1004, "Field Engineer"), 0, wxALIGN_CENTER | wxALL, 10);
    // vbox->Add(createStyledButton(panel, 1005, "Quit"), 0, wxALIGN_CENTER | wxALL, 10);

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