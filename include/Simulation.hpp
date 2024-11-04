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
    std::vector<std::string> connecting_rooms;
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
        std::vector<Robot> robot_list; // Attribute to hold a list of robots
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


        // Getter for building
        Building get_building() { return building; }
  
        //Method to fix the robot when they are faulty
        std::string fix_robot(int id) ;
        
        //Method to provide robot status (Faulty, Ideal or Active)
        std::string robot_status(int id);
};

} // namespace simulation

#endif // SIMULATION_HPP
