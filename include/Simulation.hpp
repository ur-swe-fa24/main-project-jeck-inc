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
        // Database db;

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

        // Method to pass robot dict/map
        std::unordered_map<int, Robot> get_robot_dict() const { return robot_dict; };
};

} // namespace simulation

#endif // SIMULATION_HPP
