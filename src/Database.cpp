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
    std::string strCurrID = std::to_string(currId);
    std::string testing = startText + "robotID"+ interMediateText + strCurrID + endText;
    //std::cout << testing << std::endl; 

    mongocxx::uri uri("mongodb://localhost:27017");
    mongocxx::client client(uri);

    mongocxx::database db = client["database"];
    mongocxx::collection collection = db["robots"]; 
    collection.insert_one(std::move(bsoncxx::v_noabi::from_json(testing)));

    }


//This returns all the IDs we have saved. In the future, we can search the database to return any specific robot's ID, type, and size. But, for the sake of this demo we are just going to print all the IDs we have present. 
std::string database::Database::getRobotID(const robot::Robot& robot){
    int id = robot.getId();
    std::string nf = "not_found";
    std::string strCurrID = std::to_string(id);
    std::string robotJson = startText + "robotID" + interMediateText + strCurrID + endText;
    auto result = collection.find_one(bsoncxx::v_noabi::from_json(robotJson));
    if(result){
        return bsoncxx::to_json(result);
        }
    }
    else{
        return nf;
    }
//This outputs a message to the console. 
void database::Database::console_message(const std::string& message){
        std::cout << "Message: " << message << std::endl;
    }



