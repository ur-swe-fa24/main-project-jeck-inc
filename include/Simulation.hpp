#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <list>
#include <mutex>
#include "Robot.hpp"

using namespace robot;

namespace simulation {

class Simulation 
{
    private:
        std::vector<int> robots; // Attribute to hold a list of robot identifiers
        std::mutex simulation_mutex;

    public:
        // Constructor
        Simulation() = default; 
        
        // Method to add a robot to the simulation
        void add_robot(const robot::Robot& robotInstance);

        // Method to simulate the entire operation
        void simulate();
};

} // namespace simulation

#endif // SIMULATION_HPP
