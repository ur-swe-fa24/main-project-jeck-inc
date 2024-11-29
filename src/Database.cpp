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

void database::Database::init_analytics(){
    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);

    mongocxx::database db = client["sm"];
    mongocxx::collection collection = db["stats"]; 

    bsoncxx::builder::stream::document filter_builder{};
    filter_builder << "DatabaseID" << 1 << "upTime" << -1 << "numTaskCompleted" << -1 << "numberOfErrors" << -1 << "totalRobots" << 0 << "totalRoomsCleaned" << 0; 

    collection.insert_one(filter_builder.view());

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
    filter_builder << "robotID" << strCurrID << "status" << status << "size" << strSize << "currentRoom" << room << "currentTask" << strCurrTask << "uptime" << 0;

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
    // std::string robotJson = startText + "robotID" + interMediateText + strCurrID + endText;
    
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

        return ex1["robotID"];
        }

    else{
        return nf;
        }
    }

//This returns the status of the robot referenced in the parameter. 
std::string database::Database::getRobotStatus(const robot::Robot& robot){
    int id = robot.getId();
    std::string nf = "robot_not_found_in_database";
    std::string strCurrID = std::to_string(id);
    // std::string robotJson = startText + "robotID" + interMediateText + strCurrID + endText;
    
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

        return ex1["status"];
        }

    else{
        return nf;
        }
    }

//This returns the size of the robot referenced in the parameter. 
std::string database::Database::getRobotSize(const robot::Robot& robot){
    int id = robot.getId();
    std::string nf = "robot_not_found_in_database";
    std::string strCurrID = std::to_string(id);
    // std::string robotJson = startText + "robotID" + interMediateText + strCurrID + endText;
    
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

        return ex1["size"];
        }

    else{
        return nf;
        }
    }

//This returns the room of the robot referenced in the parameter. 
std::string database::Database::getRobotRoom(const robot::Robot& robot){
    int id = robot.getId();
    std::string nf = "robot_not_found_in_database";
    std::string strCurrID = std::to_string(id);
    // std::string robotJson = startText + "robotID" + interMediateText + strCurrID + endText;
    
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

        return ex1["currentRoom"];
        }

    else{
        return nf;
        }
    }

//This returns the task of the robot referenced in the parameter. 
std::string database::Database::getRobotTask(const robot::Robot& robot){
    int id = robot.getId();
    std::string nf = "robot_not_found_in_database";
    std::string strCurrID = std::to_string(id);
    // std::string robotJson = startText + "robotID" + interMediateText + strCurrID + endText;
    
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

        return ex1["currentTask"];
        }

    else{
        return nf;
        }
    }    

//This outputs a message to the console. 
void database::Database::console_message(const std::string& message){
        std::cout << "Message: " << message << std::endl;
    }



bool database::Database::updateRobots(const robot::Robot& robotInstance){
    int id = robotInstance.getId();
    int upT = 3;//robotInstance.getLifeTime();
    std::string stID = std::to_string(id);
    std::string status = robotInstance.getStatus();
    Robot::Size size = robotInstance.getSize();
    std::string stRoom = robotInstance.getRoomAssigned();
    std::string strSize = "def";
    std::string strCurrTask = "def";
    Robot::Function task = robotInstance.getTask();

    if(status == "Active"){
        upT = upT + 1;
    }

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

    switch(task){
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

    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);

    mongocxx::database db = client["database"];
    mongocxx::collection collection = db["robots"]; 

    bsoncxx::builder::stream::document filter_builder{};
    filter_builder << "robotID" << stID;

    bsoncxx::builder::stream::document update_builder;
    update_builder << "$set" << bsoncxx::builder::stream::open_document
        << "currentRoom" << stRoom
        << "size" << strSize
        << "status" << status
        << "currentTask" << strCurrTask 
        << bsoncxx::builder::stream::close_document;

    auto result = collection.update_one(filter_builder.view(), update_builder.view());

    if(result){
        return true;
    }
    else{
        return false;
    }
}   

bool database::Database::setUpTime(const int ut){
    int dbID = 1;
    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);

    mongocxx::database db = client["database"];
    mongocxx::collection collection = db["robots"]; 

    bsoncxx::builder::stream::document filter_builder{};
    filter_builder << "DatabaseID" << dbID; 

    bsoncxx::builder::stream::document update_builder;
    update_builder << "$set" << bsoncxx::builder::stream::open_document
    << "upTime" << ut << bsoncxx::builder::stream::close_document;

    auto result = collection.update_one(filter_builder.view(), update_builder.view());

    if(result){
        return true;
    }
    else{
        return false;
    }
}

bool database::Database::setNumTaskCompleted(const int tc){
    int dbID = 1;
    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);

    mongocxx::database db = client["database"];
    mongocxx::collection collection = db["robots"]; 

    bsoncxx::builder::stream::document filter_builder{};
    filter_builder << "DatabaseID" << dbID; 

    bsoncxx::builder::stream::document update_builder;
    update_builder << "$set" << bsoncxx::builder::stream::open_document
    << "numTaskCompleted" << tc << bsoncxx::builder::stream::close_document;

    auto result = collection.update_one(filter_builder.view(), update_builder.view());

    if(result){
        return true;
    }
    else{
        return false;
    }
}

bool database::Database::setNumOfError(const int er){
    int dbID = 1;
    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);

    mongocxx::database db = client["database"];
    mongocxx::collection collection = db["robots"]; 

    bsoncxx::builder::stream::document filter_builder{};
    filter_builder << "DatabaseID" << dbID; 

    bsoncxx::builder::stream::document update_builder;
    update_builder << "$set" << bsoncxx::builder::stream::open_document
    << "numberOfErrors" << er << bsoncxx::builder::stream::close_document;

    auto result = collection.update_one(filter_builder.view(), update_builder.view());

    if(result){
        return true;
    }
    else{
        return false;
    }
}

bool database::Database::setTotalRobots(const int trb){
    int dbID = 1;
    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);

    mongocxx::database db = client["database"];
    mongocxx::collection collection = db["robots"]; 

    bsoncxx::builder::stream::document filter_builder{};
    filter_builder << "DatabaseID" << dbID; 

    bsoncxx::builder::stream::document update_builder;
    update_builder << "$set" << bsoncxx::builder::stream::open_document
    << "totalRobots" << trb << bsoncxx::builder::stream::close_document;

    auto result = collection.update_one(filter_builder.view(), update_builder.view());

    if(result){
        return true;
    }
    else{
        return false;
    }
}

bool database::Database::setTotalRoomClned(const int trc){
    int dbID = 1;
    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);

    mongocxx::database db = client["database"];
    mongocxx::collection collection = db["robots"]; 

    bsoncxx::builder::stream::document filter_builder{};
    filter_builder << "DatabaseID" << dbID; 

    bsoncxx::builder::stream::document update_builder;
    update_builder << "$set" << bsoncxx::builder::stream::open_document
    << "totalRoomsCleaned" << trc << bsoncxx::builder::stream::close_document;

    auto result = collection.update_one(filter_builder.view(), update_builder.view());

    if(result){
        return true;
    }
    else{
        return false;
    }
}