#include "Simulation.hpp"

namespace simulation 
{
    // Method to add a robot to the simulation
    void Simulation::add_robot(const robot::Robot& robotInstance) 
    {
        // Use the mutex to prevent race conditions
        std::lock_guard<std::mutex> lock(simulation_mutex); 
        robots.push_back(robotInstance.getId());
    }

    // method to simulate the entire operation
    void Simulation::simulate() 
    {   
        while (running)
        {
                
                
            // Perform simulation step here
            for (int i = 0; i < robots.size(); i++) 
            {
                std::cout << "Simulating robot: " << robots[i] << std::endl;
                std::cout << "\tRobot " << robots[i] << " is in room: <room_id>" << std::endl;
                std::cout << "\tRobot " << robots[i] << " is performing task: <task_name>\n" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Sleep for 1 second
        }

        std:cout << "Simulation shutting down" << std::endl;
    }

}