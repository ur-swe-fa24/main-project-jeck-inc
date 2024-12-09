//***sm.cpp***//
#include "Frames/sm.hpp"
#include "RoundedButton.hpp"
#include "Robot.hpp"
#include "Simulation.hpp"
#include "Database.hpp"

// Event table that connects the button to the event handler
wxBEGIN_EVENT_TABLE(SeniorM, wxFrame)
    EVT_BUTTON(1001, SeniorM::RobotProductivity)  // Event binding: button click (ID 1001) triggers AddingRobot
    // EVT_BUTTON(1002, SeniorM::GoBack)
wxEND_EVENT_TABLE()

// Constructor definition
SeniorM::SeniorM(const wxString& title, Simulation& sim, Database& db)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(500, 550)), sim(sim), db(db) {
    // Create a panel for holding the GUI components
    wxPanel* panel = new wxPanel(this, wxID_ANY);
    panel->SetBackgroundColour(wxColour("#0d1c3f"));  // Dark blue background

    // Create a vertical box sizer for layout
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    // Title label
    wxStaticText* titleLabel = new wxStaticText(panel, wxID_ANY, "Senior Manager Dashboard", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    titleLabel->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    titleLabel->SetForegroundColour(wxColour("#FFFFFF"));  // White text
    vbox->Add(titleLabel, 0, wxALIGN_CENTER | wxTOP, 20);

    // Size dropdown label and combo box
    wxStaticText* sizeLabel = new wxStaticText(panel, wxID_ANY, "Size", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    sizeLabel->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    sizeLabel->SetForegroundColour(wxColour("#FFFFFF"));
    vbox->Add(sizeLabel, 0, wxALIGN_CENTER | wxTOP, 15);

    sizeChoices.Add("Large");
    sizeChoices.Add("Medium");
    sizeChoices.Add("Small");

    sizeComboBox = new wxComboBox(panel, wxID_ANY, "Select Size", wxDefaultPosition, wxSize(200, 30), sizeChoices, wxCB_READONLY);
    vbox->Add(sizeComboBox, 0, wxALIGN_CENTER | wxTOP, 5);

    // Type dropdown label and combo box
    wxStaticText* typeLabel = new wxStaticText(panel, wxID_ANY, "Type", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    typeLabel->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    typeLabel->SetForegroundColour(wxColour("#FFFFFF"));
    vbox->Add(typeLabel, 0, wxALIGN_CENTER | wxTOP, 15);

    typeChoices.Add("Scrub");
    typeChoices.Add("Vacuum");
    typeChoices.Add("Shampoo");

    typeComboBox = new wxComboBox(panel, wxID_ANY, "Select Type", wxDefaultPosition, wxSize(200, 30), typeChoices, wxCB_READONLY);
    vbox->Add(typeComboBox, 0, wxALIGN_CENTER | wxTOP, 5);

    // Calculate button using RoundedButton
    vbox->Add(new RoundedButton(panel, 1001, "Calculate Robots Productivity"), 0, wxALIGN_CENTER | wxTOP, 20);

    // Result label for productivity
    robotProducitivity = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    robotProducitivity->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    robotProducitivity->SetForegroundColour(wxColour("#FFFFFF"));
    vbox->Add(robotProducitivity, 0, wxALIGN_CENTER | wxTOP, 20);

    // Set the layout to the panel
    panel->SetSizer(vbox);
}

// Event handler: Adding a robot to the simulation
void SeniorM::RobotProductivity(wxCommandEvent& event) {
    // Creating a new robot with predefined characteristics
    std::string size = sizeComboBox->GetValue().ToStdString();
    std::string type = typeComboBox->GetValue().ToStdString();

    //Getting the info about the performance of all the robot
    std::vector<std::vector<int>> robotInfo = sim.getRobotPerformances();

    int totalTime = 0;
    int totalWorkTime = 0;

    //iterate over all the robot
    for (auto robot: robotInfo){
        //Identifying the robot's type and size for filter purposes
        //the size and type have been converted to integer which are being converted back to string

        //Getting the size of the robot
        std::string robotSize;
        switch (robot[0]){
            case 0:
                robotSize = "Large";
                break;
            case 1:
                robotSize = "Medium";
                break;
            case 2:
                robotSize = "Large";
                break;
            default:
                robotSize = "Unknown";
        }

        //Getting the type of the robot
        std::string robotType;
        switch (robot[1]){
            case 0:
                robotType = "Scrub";
                break;
            case 1:
                robotType = "Shampoo";
                break;
            case 2:
                robotType = "Vaccum";
                break;
            default:
                robotType = "Unknonw";
        }

        //Getting the performance values
        int workTime = robot[3];
        int time = robot[2];

        //Checking for the filter
        if ((size == "" || size == robotSize) && (type == "" || type == robotType)){
            totalTime += time;
            totalWorkTime += workTime;
        }
    }

    //Default Value
    std::string result = "NA";

    //Making sure there is no 0 division error
    if (totalTime != 0){
        result = std::to_string(std::round(totalWorkTime/totalTime)) + "%";
    }

    //Changing the result
    robotProducitivity->SetLabel(result);
}
