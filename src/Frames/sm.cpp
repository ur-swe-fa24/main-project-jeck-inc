
#include "Frames/sm.hpp"
#include "Robot.hpp"
#include "Simulation.hpp"
#include "Database.hpp"


// Event table that connects the button to the event handler
wxBEGIN_EVENT_TABLE(SeniorM, wxFrame)
    EVT_BUTTON(1001, SeniorM::RobotProductivity)  // Event binding: button click (ID 1001) triggers AddingRobot
wxEND_EVENT_TABLE()

// Constructor definition
SeniorM::SeniorM(const wxString& title, Simulation& sim, Database& db)
    : wxFrame(nullptr, wxID_ANY, title), sim(sim), db(db){



    // Create a panel for holding the GUI components
    wxPanel* panel = new wxPanel(this, wxID_ANY);

    // wxStaticText* sizeLabel = new wxStaticText(panel, wxID_ANY, "Size", wxPoint(60, 20));
    wxStaticText* sizeLabel = new wxStaticText(panel, wxID_ANY, "Size", wxPoint(100, 30));  

    
    sizeChoices.Add("Large");
    sizeChoices.Add("Medium");
    sizeChoices.Add("Small");

    sizeComboBox = new wxComboBox(panel, wxID_ANY, "Select an option", 
                            wxPoint(50, 50), wxSize(150, 30), sizeChoices,
                            wxCB_READONLY);


    wxStaticText* typeLabel = new wxStaticText(panel, wxID_ANY, "Type", wxPoint(250, 30));

    
    typeChoices.Add("Scrub");
    typeChoices.Add("Vaccum");
    typeChoices.Add("Shampoo");

    typeComboBox = new wxComboBox(panel, wxID_ANY, "Select an option", 
                            wxPoint(200, 50), wxSize(150, 30), typeChoices,
                            wxCB_READONLY);


    // Create a button that will trigger the AddingRobot event
    wxButton* addButton = new wxButton(panel, 1001, "Calculate Robots Productivity", wxPoint(50, 90));

    robotProducitivity = new wxStaticText(panel, wxID_ANY, "", wxPoint(290, 100));

    // Set the window size for the SubFrame
    this->SetSize(400, 350);
}

// Event handler: Adding a robot to the simulation
void SeniorM::RobotProductivity(wxCommandEvent& event) {
    // Creating a new robot with predefined characteristics
    std::string size = sizeComboBox->GetValue().ToStdString();
    std::string type = typeComboBox->GetValue().ToStdString();

    std::vector<std::vector<int>> robotInfo = sim.getRobotPerformances();

    int totalTime = 0;
    int totalWorkTime = 0;

    
    for (auto robot: robotInfo){
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

        int workTime = robot[3];
        int time = robot[2];

        if ((size == "" || size == robotSize) && (type == "" || type == robotType)){
            totalTime += time;
            totalWorkTime += workTime;
        }




    }

    std::string result = "NA";

    if (totalTime != 0){
        result = std::to_string(std::round(totalWorkTime/totalTime)) + "%";
    }


    std::cout << "Value in the size is " + sizeComboBox->GetValue() << std::endl;
    robotProducitivity->SetLabel(result);

  

}
