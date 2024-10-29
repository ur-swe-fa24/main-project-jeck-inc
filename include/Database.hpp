#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

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
    std::vector<int> robotIds; //vector to hold the robot's ids 
    std::map<int,Robot::Function> robotTypes; //map to hold the robot's type.
    std::map<int,Robot::Size> robotSize; //map to hold the size of each robot.
    bool isOn; //bool to check if the database is on.

public:
    Database() = default;
    // Method to add a robot to the database
    void add_robot(const robot::Robot& robotInstance);

    // Method to display a message on the console
    void console_message(const std::string& message);

    //Method to get a robots ID
    void getRobotIDs(const robot::Robot& robotInstance);
};

} // namespace database

#endif // DATABASE_HPP
