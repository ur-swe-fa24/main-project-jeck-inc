#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <vector>
#include <string>
#include <iostream>
#include <map>
#include "Robot.hpp"
#include <unordered_map>
#include <nlohmann/json.hpp>
//#include "Simulation.hpp"

#include <bsoncxx/v_noabi/bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/v_noabi/bsoncxx/json.hpp>
#include <bsoncxx/v_noabi/bsoncxx/oid.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include "mongocxx/instance.hpp"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

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
    std::string startText;
    std::string endText;
    std::string interMediateText;

    mongocxx::database db_;
    mongocxx::collection collection_;

public:
    Database();

    ~Database();

    void add_robot(const robot::Robot& robotInstance);

    // Method to display a message on the console
    void console_message(const std::string& message);

    //Method to get a robot's ID
    std::string getRobotID(const robot::Robot& robotInstance);

    //Method to get a robot's status
    std::string getRobotStatus(const robot::Robot& robotInstance);

    //Method to get a robot's size
    std::string getRobotSize(const robot::Robot& robotInstance);

    //Method to get a robot's current room
    std::string getRobotRoom(const robot::Robot& robotInstance);

    //Method to get a robot's current task
    std::string getRobotTask(const robot::Robot& robotInstance);

    bool update(const robot::Robot& robotInstance);
};
} // namespace database

#endif // DATABASE_HPP
