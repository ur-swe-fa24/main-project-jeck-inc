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

constexpr char kMongoDbUri[] = "mongodb://localhost:27017";
constexpr char kDatabaseName[] = "robotDatabase";
constexpr char kCollectionName[] = "robots";


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

    //mongocxx::instance currInst_;
    mongocxx::database db_;
    mongocxx::collection collection_;

public:
    Database();
        // uri = mongocxx::uri(kMongoDbUri),
        // client = mongocxx::client(uri),
        // db = client[kDatabaseName] {}

    // Method to add a robot to the database
    //void startDB()

    void add_robot(const robot::Robot& robotInstance);

    // Method to display a message on the console
    void console_message(const std::string& message);

    //Method to get a robots ID
    std::string getRobotID(const robot::Robot& robotInstance);
};
} // namespace database

#endif // DATABASE_HPP
