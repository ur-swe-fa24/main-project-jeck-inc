#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <list>
#include <mutex>
#include "Robot.hpp"
#include <vector>
#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>
#include <string>
#include <fstream>
#include <variant>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include "spdlog/sinks/basic_file_sink.h"

using namespace std;
using namespace robot;

namespace simulation {

// Struct for each room
struct Room {
    std::string room_id;
    std::string room_name;
    std::string size;
    std::string floor_surface;
    std::string access;
    std::unordered_set<int> robots_cleaning;
    std::vector<std::string> connecting_rooms;
    int percentClean;
};

// Struct for the building as a whole, which will contain the rooms
struct Building {
    std::string name;
    std::unordered_map<std::string, Room> rooms;
};

class Simulation 
{
    private:
        std::vector<int> robots; // Attribute to hold a list of robot identifiers
        std::unordered_map<int, Robot> robot_dict; // Attribute to hold a list of robots
        std::mutex simulation_mutex; // Mutual Exclusion lock
        std::atomic<bool> running; // Control signal
        Building building; // Building layout
        vector<int> faultyRobots; // vector for faulty robots for NOTIFCATIONS
        vector<int> faultyRobotsDB; // vector for faulty robots for DB STATS
        unordered_set<std::string> tasksCompleted; // vector for robots who just completed task
        int timeCount; // integer clock ticks starting at 0 for each simulation tick
        int numTasksCompleted; // integer keeping track of how many tasks have been completed

    public:
        // Constructor
        // Simulation(Database db) : db(db), running(true) {} 
        Simulation() :  running(true) {} 
        
        // Method to add a robot to the simulation
        void add_robot(const robot::Robot& robotInstance);

        // Method to get all robot IDs
        std::string getRobotIds() const;

        // Method to simulate the entire operation
        void simulate();

        // Method to stop simulation
        void stop() { running = false; }

        // Method to load the map of a building from a json file
        void load_building(const std::string& json_file);

        // Method to assign a room to one robot
        std::string assign_task(int robotID, std::string roomID);

        // Getter for building
        Building get_building() { return building; }
  
        //Method to fix the robot when they are faulty
        std::string fix_robot(int id) ;
        
        //Method to provide robot status (Faulty, Idle or Active)
        std::string robot_status(int id);

        // Method to fill a robot with fluid
        std::string fill_robot_fluid(int id);

        // Method to get info about a robot
        std::string get_robot_info(int id);

        // Method to pass robot dict/map
        std::unordered_map<int, Robot> get_robot_dict() const { return robot_dict; };

        // Method for getting faulty robots for UI notification
        std::vector<int> getFaultyRobots();

        // vector of robot ids that have errored since this function was last called
        std::vector<int> getFaultyRobotsDB();

        // Method for getting completed tasks for UI notification
        unordered_set<std::string> getTasksCompleted();

        // Calculates efficiency of each robot and sends that result
        std::vector<std::vector<int>> getRobotPerformances();

        // Method for getting a list of room cleanliness
        // Key: string roomID
        // Value: int percentClean
        unordered_map<std::string, int> getAllRoomCleanliness();

        // Method for getting a list of ongoing tasks
        // Key: int robotID
        // Value: string roomID
        unordered_map<int, std::string> getOngoingTasks();

        // Method that takes in a room ID and returns the tentative completion time for that room
        int completionTime(std::string roomID);

        // Method that takes in a robot ID and returns the tentative completion time for that robot
        int robotCompletionTime(int robotID);

        //  Method to get a dictionary with a series of stats for the database
        //  key                -      value
        //  clockSimTime       -      int
        //  numTasksCompleted  -      int
        //  numberErrors       -      int
        //  totalNumRobots     -      int
        //  totalRoomsCleaned  -      int
        unordered_map<std::string, int> getDBStats();



};

} // namespace simulation

#endif // SIMULATION_HPP
