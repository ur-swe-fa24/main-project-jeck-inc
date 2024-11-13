#include <vector>
#include <string>
#include <iostream>
#include <map>
#include "Database.hpp"
#include "Robot.hpp"

database::Database::Database(){
    isOn = true;
    startText = "{\"";
    endText = "\"}";
    interMediateText= "\":\"";
}

    //Here, we have the area for adding more robots. The commented parts will be used in the future to display functionality like storing a robots task and size.
void database::Database::add_robot(const robot::Robot& robotInstance){
    robotIds.push_back(robotInstance.getId());
    int currId = robotInstance.getId();
    std::string status = robotInstance.getStatus();
    Robot::Size size = robotInstance.getSize();
    Robot::Function currTask = robotInstance.getTask();
    std::string strSize = "";
    std::string strCurrTask = "";
    std::string room = robotInstance.getRoomAssigned();

    switch(size){
        case Robot::Size::Small: 
            strSize = "Small";
            break;
        case Robot::Size::Medium: 
            strSize = "Medium";
            break;
        case Robot::Size::Large: 
            strSize = "Large";
            break;
        default: strSize = "n/a";
            break;
    }

    switch(currTask){
        case Robot::Function::Scrub: 
            strCurrTask = "Scrubber";
            break;
        case Robot::Function::Shampoo:
            strCurrTask = "Shampooer";
            break;
        case Robot::Function::Vacuum: 
            strCurrTask = "Vacuumer";
            break;
        default: strCurrTask = "Not Assigned";
            break;
    }


    std::string strCurrID = std::to_string(currId);

    bsoncxx::builder::stream::document filter_builder{};
    filter_builder << "robotID" << strCurrID << "status" << status << "size" << strSize << "currentRoom" << room << "currentTask" << strCurrTask;

    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);

    mongocxx::database db = client["database"];
    mongocxx::collection collection = db["robots"]; 
    collection.insert_one(filter_builder.view());    
    }


//This returns all the IDs we have saved. In the future, we can search the database to return any specific robot's ID, type, and size. But, for the sake of this demo we are just going to print all the IDs we have present. 
std::string database::Database::getRobotID(const robot::Robot& robot){
    int id = robot.getId();
    std::string nf = "not_found";
    std::string strCurrID = std::to_string(id);
    std::string robotJson = startText + "robotID" + interMediateText + strCurrID + endText;
    
    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);

    mongocxx::database db = client["database"];
    mongocxx::collection collection = db["robots"]; 

    bsoncxx::builder::stream::document filter_builder{};
    filter_builder << "robotID" << strCurrID;

    auto result = collection.find_one(filter_builder.view());
    if(result){
        bsoncxx::document::view doc = result->view();

        std::string print = bsoncxx::to_json(doc);

        nlohmann::json ex1 = nlohmann::json::parse(print);
        
        // std::cout << print << std::endl;
        ex1["robotID"];

        return ex1["robotID"];
        }

    else{
        return nf;
        }
    }
//This outputs a message to the console. 
void database::Database::console_message(const std::string& message){
        std::cout << "Message: " << message << std::endl;
    }



