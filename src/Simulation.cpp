#include "Simulation.hpp"
#include "Robot.hpp"
#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>

namespace simulation 
{
    // Method to add a robot to the simulation
    void Simulation::add_robot(const robot::Robot& robotInstance) 
    {
        robots.push_back(robotInstance.getId());
    }

    // method to simulate the entire operation
    void Simulation::simulate() 
    {   
        while (true)
        {
                // Lock the mutex to prevent race conditions
                std::lock_guard<std::mutex> lock(simulation_mutex); 
                
                // Perform simulation step here
                for (int i = 0; i < robots.size(); i++) 
                {
                    std::cout << "Simulating robot: " << robots[i] << std::endl;
                    std::cout << "\tRobot " << robots[i] << " is in room: <room_id>" << std::endl;
                    std::cout << "\tRobot " << robots[i] << " is performing task: <task_name>\n" << std::endl;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Sleep for 1 second
    }

} // namespace simulation