#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "Database.hpp"
#include "Robot.hpp"

//Overview: This database class initializes and maintains a database with three distinct sections. The first section stores each robot 
//instance and the associated values with it (status, size, etc). The second section stores total amounts of things done by the cleaning 
//system (tasks, errors, etc). The final section stores only tasks compelted and errors but does so for any particular section of time.
//The later two collections are used for analytics for those in senior management, while the robot collection is used for general tab-keeping
//for the field engineer and building manager. Data is passed to the system meanger, who then passes that info to the database by using 
//the provided setter methods. If a getter method is called, all data is passed to the system manager, which then handles sending the data
//to its final destination. 

//basic initialization for the database. 
database::Database::Database(){
    isOn = true;
    startText = "{\"";
    endText = "\"}";
    interMediateText= "\":\"";
}

//Destructor for the database. This removes all the robots from the active list. Since we are treating our system as a 24/7 type of product.
//we imagine that everytime the database is reinitialized we want a fresh slate. 
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

//initializes the analytics part of the database. creates a data section for the system time, number of total task completed, number of total
//errors, number of total robots, and number of rooms cleaned. 
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

    bsoncxx::builder::stream::document filter_builder{};
    filter_builder << "DatabaseID" << 1 << 
    "curretTime" << timeString << 
    "numTaskCompleted" << -1 << 
    "numberOfErrors" << -1 << 
    "totalRobots" << -1 << 
    "totalRoomsCleaned" << -1; 

    collection.insert_one(filter_builder.view());

}

    //Here, we have the area for adding more robots. We take the robot instance, get its data using getter methods, than update the db.
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


//This returns the information about the robot that we want. It takes in a robot instance and returns a string that corresponds to the robotID.
std::string database::Database::getRobotID(const robot::Robot& robot){
    int id = robot.getId();
    std::string nf = "not_found";
    std::string strCurrID = std::to_string(id);
    
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

//Gets all the data relating to the robot instance passed to it, checks for updates relative to the old data in the db, then updates the 
//db accordingly. 
bool database::Database::updateRobots(const robot::Robot& robotInstance){
    int id = robotInstance.getId();
    int upT = 3;
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
  
//sets the currentTime based on the time inputted by the user. This is mainly used for testing, as system time is usually relied on. 
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

//sets the number of total tasks completed in the database to the input varaible tc.
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

//sets the number of total errors in the database to the input varaible er.
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

//sets the number of total amount of robots in the database to the input varaible tc.
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

//sets the number of total tasks completed in the database to the input varaible tc.
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

//updates the database using the setter methods defined above for the analytics parts of the database.
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

    auto current = std::chrono::system_clock::now();
    auto now_timet = std::chrono::system_clock::to_time_t(current);
    auto now_local = localtime(&now_timet);

    std::stringstream ss;
    ss << std::put_time(now_local, "%c");
    std::string currtime = ss.str();

    bool UTComplete = this->setCurrentTime(currtime);
    bool taskCompBool = setNumTaskCompleted(taskComp);
    bool totalNumOfErrBool = setNumOfError(newNumFailRobos);
    bool totalNumRobBool = setTotalRobots(trb);


    return false;
}

//inits the 3rd collection, which stores tasks completed and error rates for any specific time. 
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

//updates the amount of tasksCompleted and Errors for the last time stamp.
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

//returns the relative time of the database, used in the TCER database.
int database::Database::getDBTime(){
    return timeStamps;
}

//returns the amount of tasks completed and errors for the inputted time seqeuence. 
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

