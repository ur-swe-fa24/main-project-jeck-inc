#include "Frames/sm.hpp"
#include "RoundedButton.hpp"
#include "Robot.hpp"
#include "Simulation.hpp"
#include "Database.hpp"

// Event table that connects the button to the event handler
wxBEGIN_EVENT_TABLE(SeniorM, wxFrame)
    EVT_BUTTON(1001, SeniorM::RobotProductivity)  // Event binding: button click (ID 1001) triggers AddingRobot
    EVT_BUTTON(1002, SeniorM::TaskCompleted)  
    EVT_BUTTON(1003, SeniorM::FaultyRobots)  
wxEND_EVENT_TABLE()

// Constructor definition
SeniorM::SeniorM(const wxString& title, Simulation& sim, Database& db)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(500, 650)), sim(sim), db(db) {
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

    wxBoxSizer* taskCompletedSizer = new wxBoxSizer(wxVERTICAL);

    // Task Completed Section
    wxStaticText* taskCompletedLabel = new wxStaticText(panel, wxID_ANY, "Number of Tasks Completed in:", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    taskCompletedLabel->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    taskCompletedLabel->SetForegroundColour(wxColour("#ffffff"));
    taskCompletedSizer->Add(taskCompletedLabel, 0, wxALIGN_LEFT | wxBOTTOM, 5);

    timeChoices.Add("30 seconds");
    timeChoices.Add("60 seconds");
    timeChoices.Add("120 seconds");
    timeChoices.Add("240 seconds");

    taskCompleteTimeComboBox = new wxComboBox(panel, wxID_ANY, "Select an option", wxDefaultPosition, wxSize(300, 30), timeChoices, wxCB_READONLY);
    taskCompletedSizer->Add(taskCompleteTimeComboBox, 0, wxALIGN_CENTER | wxBOTTOM, 10);

    taskCompletedSizer->Add(new RoundedButton(panel, 1002, "Calculate Task Completed"), 0, wxALIGN_CENTER | wxBOTTOM, 10);

    taskCompleted = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    taskCompleted->SetForegroundColour(wxColour("#ffffff"));
    taskCompletedSizer->Add(taskCompleted, 0, wxALIGN_LEFT);

    // Faulty Robots Section
    wxBoxSizer* faultyRobotsSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* faultyRobotsLabel = new wxStaticText(panel, wxID_ANY, "Number of Faulty Robots:", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
    faultyRobotsLabel->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    faultyRobotsLabel->SetForegroundColour(wxColour("#ffffff"));
    faultyRobotsSizer->Add(faultyRobotsLabel, 0, wxALIGN_LEFT | wxBOTTOM, 5);

    faultyRobotComboBox = new wxComboBox(panel, wxID_ANY, "Select an option", wxDefaultPosition, wxSize(300, 30), timeChoices, wxCB_READONLY);
    faultyRobotsSizer->Add(faultyRobotComboBox, 0, wxALIGN_CENTER | wxBOTTOM, 10);

    faultyRobotsSizer->Add(new RoundedButton(panel, 1003, "Count Faulty Robots"), 0, wxALIGN_CENTER | wxBOTTOM, 10);

    faultyRobots = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    faultyRobots->SetForegroundColour(wxColour("#ffffff"));
    faultyRobotsSizer->Add(faultyRobots, 0, wxALIGN_LEFT);

    // Add sections to the main vbox
    vbox->Add(taskCompletedSizer, 0, wxALIGN_CENTER | wxTOP, 20);
    vbox->Add(faultyRobotsSizer, 0, wxALIGN_CENTER | wxTOP, 20);

    // Set the layout to the panel
    panel->SetSizer(vbox);
}

void SeniorM::TaskCompleted(wxCommandEvent& event){
    std::string threshold = taskCompleteTimeComboBox->GetValue().ToStdString();
    int threshold_val;
    std::stringstream ss(threshold);
    ss >> threshold_val; //removing seconds to create a int value
    threshold_val /= 30;

    int total_completed_tasks = 0;
    int curr_relative_time = db.getDBTime();

    for (int i = max(0, curr_relative_time - threshold_val); i < curr_relative_time; i++){
        std::vector<int> results = db.getTCER("time"+std::to_string(i));
        total_completed_tasks += results[1];
    }

    taskCompleted->SetLabel(std::to_string(total_completed_tasks));
}


void SeniorM::FaultyRobots(wxCommandEvent& event){
    std::string threshold = faultyRobotComboBox->GetValue().ToStdString();
    int error_robots;
    std::stringstream ss(threshold);
    ss >> error_robots; //removing seconds to create a int value
    error_robots /= 30;

    int total_error_robots = 0;
    int curr_relative_time = db.getDBTime();

    for (int i = max(0, curr_relative_time - error_robots); i < curr_relative_time; i++){
        std::vector<int> results = db.getTCER("time"+std::to_string(i));
        total_error_robots += results[0];
    }

    faultyRobots->SetLabel(std::to_string(total_error_robots));
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
                robotSize = "Small";
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
                robotType = "Unknown";
        }

        //Getting the performance values
        int workTime = robot[3]; // get up
        int time = robot[2]; // get liveTime

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
        result = std::to_string(std::round(100 * totalWorkTime/totalTime)) + "%";
    }

    //Changing the result
    robotProducitivity->SetLabel(result);
}
