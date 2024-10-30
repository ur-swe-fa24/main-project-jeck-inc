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

using namespace std;
using namespace robot;

namespace simulation {

class Simulation 
{
    private:
        std::vector<int> robots; // Attribute to hold a list of robot identifiers
        std::mutex simulation_mutex; // Mutual Exclusion lock
        std::atomic<bool> running; // Add control signal

    public:
        // Constructor
        Simulation() : running(true) {} 
        
        // Method to add a robot to the simulation
        void add_robot(const robot::Robot& robotInstance);

        // Method to simulate the entire operation
        void simulate();

        // Method to stop simulation
        void stop() { running = false; }
};

} // namespace simulation

#endif // SIMULATION_HPP
