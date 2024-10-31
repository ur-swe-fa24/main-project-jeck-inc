#include <vector>
#include <string>
#include <iostream>
#include <map>
#include "Database.hpp"
#include "Robot.hpp"



    //Here, we have the area for adding more robots. The commented parts will be used in the future to display functionality like storing a robots task and size.
void database::Database::add_robot(const robot::Robot& robotInstance){
        robotIds.push_back(robotInstance.getId());
        mongocxx::collection collection = db[kCollectionName];
        auto builder = bsoncxx::builder::stream::document{};
        bsoncxx::document::value doc_to_add =
        builder << "robotID" << robotInstance.getId() << bsoncxx::builder::stream::finalize;

        collection.insert_one(doc_to_add.view());

        //robotTypes[robotInstance.getId()] = robotInstance.getTask();
        //robotSize[robotInstance.getId()] = robotInstance.getSize();
    }
//This returns all the IDs we have saved. In the future, we can search the database to return any specific robot's ID, type, and size. But, for the sake of this demo we are just going to print all the IDs we have present. 
void database::Database::getRobotIDs(const robot::Robot& robot){
    for(auto robots : robotIds){
        std::cout << "Robot ID: " << robots << std::endl;
        }
    }
//This outputs a message to the console. 
void database::Database::console_message(const std::string& message){
        std::cout << "Message: " << message << std::endl;
    }



