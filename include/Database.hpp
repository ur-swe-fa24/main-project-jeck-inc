#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <vector>
#include <string>
#include <iostream>
#include <map>
#include "Robot.hpp"
#include <unordered_map>
#include <nlohmann/json.hpp>

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
    int timeStamps;
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

    //Method to get the up time of Robot
    std::string getRobotUptime(const robot::Robot& robotInstance);

    //Method to get the total life time of robot
    std::string getRobotLifetime(const robot::Robot& robotInstance);

    //starts up the database used for analytics. Sets relevant fields to -1 so that they can be referenced later .
    void init_analytics();

    //sets the current time of the instance in the database to the one defined by the class in ut. Usually unused, as the system normally relies on system time.
    bool setCurrentTime(std::string ut);

    //sets the number of tasks completed TOTAL in the database to the number defined in tc. 
    bool setNumTaskCompleted(const int tc);
    
    //sets the number of errors TOTAL in the databse to the value defined in er.
    bool setNumOfError(const int er);
    
    //sets the number of total robots present in the system to the value defined in trb. 
    bool setTotalRobots(const int trb);

    //sets the total amount of rooms cleaned to the value defined in trc. In a v2, this will be different from tasks completed, but for now they are the same.
    bool setTotalRoomClned(const int trc);

    //updates all the valuess in database used for analytics relating to the senior manager. This uses the previous 5 classes as subroutines. 
    bool updateSM(const int ut, const std::vector<int> failedRobots, const int trb, const int trc);

    //Method to update a robots info in the database to its current data as saved in the robot class. 
    bool updateRobots(const robot::Robot& robotInstance);

    //initializes a seperate area 
    bool init_TaskCompletedAndErrorRates();

    bool updateTCER(const int tskCompleted, const int Ers);

    std::unordered_map<std::string, std::vector<int>> getTCER(const std::string time);

    //returns the relative time of the database
    int getDBTime(); 

};
} // namespace database

#endif // DATABASE_HPP
