#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <vector>
#include <string>
#include <iostream>
#include <map>
#include "Robot.hpp"

using namespace robot;

// Creating namespace
namespace database{

class Database {

private:
    // Attribute to hold a vector of log messages that 
    // Logss will imitate the database
    // Change its datatype as you see fit
    std::vector<std::string> logs; 
    std::vector<int> robotIds;
    std::map<int,Robot::Function> robotTypes;
    std::map<int,Robot::Size> robotSize;
    bool isOn;

public:
    Database();
    // Method to add a robot to the database
    void add_robot(const robot::Robot& robotInstance);

    // Method to display a message on the console
    void console_message(const std::string& message);

    //Method to get a robots ID
    int getRobotIDs(const robot::Robot& robotInstance);
};

} // namespace database

#endif // DATABASE_HPP
