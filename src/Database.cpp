#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "Database.hpp"
#include "Robot.hpp"

database::Database::Database(){
    isOn = true;
    startText = "{\"";
    endText = "\"}";
    interMediateText= "\":\"";
}

database::Database::~Database(){
    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);

    mongocxx::database db1 = client["database"];
    mongocxx::collection robs = db1["robots"]; 

    mongocxx::database db2 = client["sm"];
    mongocxx::collection stats = db2["stats"];




    
    auto result1 = robs.delete_many(make_document(kvp("testDVal", "Tr")));
    auto result2 = stats.delete_many(make_document(kvp("testDVal", "Tr")));
    //std::cout << result1->deleted_count() << std::endl;
    //std::cout << result2->deleted_count() << std::endl;
}

void database::Database::init_analytics(){
    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);

    mongocxx::database db = client["sm"];
    mongocxx::collection collection = db["stats"]; 

    auto current = std::chrono::system_clock::now();
    auto now_timet = std::chrono::system_clock::to_time_t(current);
    auto now_local = localtime(&now_timet);

    std::stringstream ss;
    ss << std::put_time(now_local, "%c");
    std::string timeString = ss.str();

    //std::cout << "Local Time " << std::put_time(now_local, "%c") << std::endl;

    bsoncxx::builder::stream::document filter_builder{};
    filter_builder << "DatabaseID" << 1 << 
    "curretTime" << timeString << 
    "numTaskCompleted" << -1 << 
    "numberOfErrors" << -1 << 
    "totalRobots" << -1 << 
    "totalRoomsCleaned" << -1; 

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
    filter_builder << "robotID" << strCurrID << "status" << status << "size" << strSize << "currentRoom" << room << "currentTask" << strCurrTask << "uptime" << 0 << "testDVal" << "Tr";

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
  
bool database::Database::setCurrentTime(const std::string ut){
    int dbID = 1;
    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);

    mongocxx::database db = client["sm"];
    mongocxx::collection collection = db["stats"]; 

    bsoncxx::builder::stream::document filter_builder{};
    filter_builder << "DatabaseID" << dbID; 

    bsoncxx::builder::stream::document update_builder;
    update_builder << "$set" << bsoncxx::builder::stream::open_document
    << "currentTime" << ut << bsoncxx::builder::stream::close_document;

    auto result = collection.update_one(filter_builder.view(), update_builder.view());

    std::cout << ut << std::endl;

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

    mongocxx::database db = client["sm"];
    mongocxx::collection collection = db["stats"]; 

    bsoncxx::builder::stream::document filter_builder{};
    filter_builder << "DatabaseID" << dbID; 

    bsoncxx::builder::stream::document update_builder;
    update_builder << "$set" << bsoncxx::builder::stream::open_document
    << "numTaskCompleted" << tc << bsoncxx::builder::stream::close_document;

    std::cout << tc << std::endl;

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

    mongocxx::database db = client["sm"];
    mongocxx::collection collection = db["stats"]; 

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

    mongocxx::database db = client["sm"];
    mongocxx::collection collection = db["stats"]; 

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

    mongocxx::database db = client["sm"];
    mongocxx::collection collection = db["stats"]; 

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

bool database::Database::updateSM(const int ut, const std::vector<int> failedRobots, const int trb, const int taskComp){
    int dbID = 1; 
    
    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);

    mongocxx::database db = client["sm"];
    mongocxx::collection collection = db["stats"]; 

    bsoncxx::builder::stream::document filter_builder{};
    filter_builder << "DatabaseID" << 1;

    auto result = collection.find_one(filter_builder.view());
    bsoncxx::document::view doc = result->view();

    std::string print = bsoncxx::to_json(doc);

    nlohmann::json ex1 = nlohmann::json::parse(print);

    int currNumFailRobots = ex1["numberOfErrors"];

    int newNumFailRobos = currNumFailRobots + failedRobots.size();
    
    int currNumRobots = ex1["totalRobots"];
    
    std::cout << "failhots" << currNumFailRobots << std::endl;
    // std::cout << "robs" << currNumRobots << std::endl; 


    auto current = std::chrono::system_clock::now();
    auto now_timet = std::chrono::system_clock::to_time_t(current);
    auto now_local = localtime(&now_timet);

    std::stringstream ss;
    ss << std::put_time(now_local, "%c");
    std::string currtime = ss.str();

    bool UTComplete = this->setCurrentTime(currtime);
    std::cout << UTComplete << std::endl;
    bool taskCompBool = setNumTaskCompleted(taskComp);
    bool totalNumOfErrBool = setNumOfError(newNumFailRobos);
    bool totalNumRobBool = setTotalRobots(trb);


    return false;
}

bool database::Database::init_TaskCompletedAndErrorRates(){
    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);

    mongocxx::database db = client["sm"];
    mongocxx::collection collection = db["TCER"]; 

    auto current = std::chrono::system_clock::now();
    auto now_timet = std::chrono::system_clock::to_time_t(current);
    auto now_local = localtime(&now_timet);

    std::stringstream ss;
    ss << std::put_time(now_local, "%c");
    std::string timeString = ss.str();

    bsoncxx::builder::stream::document filter_builder{};
    filter_builder << "DatabaseID" << 1 << 
    "curretTime" << timeString << 
    "relativeTime" << "time0" << 
    "time0TC" << 0 <<
    "Time0ER" << 0;

    timeStamps = timeStamps + 1;

    collection.insert_one(filter_builder.view());

    return true;
}

bool database::Database::updateTCER(const int tskCompleted, const int Ers){
    std::string newRelTime = "time" + std::to_string(timeStamps);
    std::string timeErrorLabel = newRelTime + "ER";
    std::string timeTaskLable = newRelTime + "TC";

    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);

    mongocxx::database db = client["sm"];
    mongocxx::collection collection = db["TCER"]; 

    auto current = std::chrono::system_clock::now();
    auto now_timet = std::chrono::system_clock::to_time_t(current);
    auto now_local = localtime(&now_timet);

    std::stringstream ss;
    ss << std::put_time(now_local, "%c");
    std::string timeString = ss.str();

    bsoncxx::builder::stream::document filter_builder{};
    filter_builder << "DatabaseID" << 1 << 
    "curretTime" << timeString << 
    "relativeTime" << newRelTime << 
    timeTaskLable << tskCompleted <<
    timeErrorLabel << Ers;

    collection.insert_one(filter_builder.view());

    timeStamps = timeStamps + 1;

    return true;
}

int database::Database::getDBTime(){
    return timeStamps;
}

std::unordered_map<std::string, std::vector<int>> database::Database::getTCER(const std::string time){
    std::unordered_map<std::string, std::vector<int>> resMap;
    std::unordered_map<std::string, std::vector<int>> nf;
    std::vector<int> emptVec = {0,0};
    std::string foundTimeERKey = time + "ER";
    std::string foundTimeTCKey = time + "TC";

    
    nf["time_not_found"] = emptVec;
    
    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);

    mongocxx::database db = client["sm"];
    mongocxx::collection collection = db["TCER"]; 

    bsoncxx::builder::stream::document filter_builder{};
    filter_builder << "relativeTime" << time;
    
    auto result = collection.find_one(filter_builder.view());
    if(result){

        bsoncxx::document::view doc = result->view();

        std::string print = bsoncxx::to_json(doc);
        nlohmann::json ex1 = nlohmann::json::parse(print);

        int errors = ex1[foundTimeERKey];
        int taskCompleted = ex1[foundTimeTCKey];

        std::vector<int> TCERatGivenTime = {errors,taskCompleted};

        return resMap;
        }

    else{
        return nf;
        }
}

