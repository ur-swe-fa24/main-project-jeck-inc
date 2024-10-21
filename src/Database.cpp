#include <vector>
#include <string>
#include <iostream>
#include <map>
#include "Database.hpp"
#include "Robot.hpp"

    
void database::Database::add_robot(const robot::Robot& robotInstance){
        robotIds.push_back(robotInstance.getId());
        //robotTypes[robotInstance.getId()] = robotInstance.getTask();
        //robotSize[robotInstance.getId()] = robotInstance.getSize();
    }

void database::Database::getRobotIDs(const robot::Robot& robot){
    for(auto robots : robotIds){
        std::cout << "Robot ID: " << robots << std::endl;
        }
    }

void database::Database::console_message(const std::string& message){
        std::cout << "Message: " << message << std::endl;
    }



