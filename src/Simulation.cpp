#include "Simulation.hpp"

namespace simulation 
{
    void Simulation::load_building(const std::string& json_file_path) {
        
        std::ifstream file(json_file_path);
    
        // open the file
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open building layout file: " + json_file_path);
        }

        nlohmann::json j;
        file >> j;
        

        building.name = j["building_name"];
       
        // Parse rooms
        for (const auto& room_json : j["rooms"]) {
            Room room;
            room.room_id = room_json["room_id"];
            room.room_name = room_json["room_name"];
            room.size = room_json["size"];
            room.floor_surface = room_json["floor_surface"];
            room.access = room_json["access"];
            room.connecting_rooms = room_json["connecting_rooms"].get<std::vector<std::string>>();
            building.rooms[room.room_id] = room;
        }
    }

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