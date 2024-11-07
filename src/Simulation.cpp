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
        robot_list.push_back(robotInstance);
    }


    // Get all robot IDs
    std::string simulation::Simulation::getRobotIds() const {
        // std::lock_guard<std::mutex> lock(simulation_mutex); // Ensure thread safety

        std::string robotStr; // Create a string stream to build the string
        for (size_t i = 0; i < robots.size(); ++i) {
            robotStr += std::to_string( robot_list[i].getId()) + "\n"; // Append the integer and a new line
            
        };
        return robotStr; // Return the resulting string



    }

    //function that uses setStatus of robot to fix the robot
    std::string simulation::Simulation::fix_robot(int id)  {
        //Out of range robot-id 
        if (id < 0 || id >= robot_list.size()) {
            return "Robot_Id Not Found";
        }
        //When robot is not faulty
        robot::Robot robot = robot_list[id];
        if (robot.getStatus() != "Faulty"){
            return "Robot is not faulty";
        }
        //Actually Fixing the robot if needed
        robot.setStatus("Ideal");
        return "Robot is fixed";

    }

    //function that uses getStatus of robot to view the status of robot
    std::string simulation::Simulation::robot_status(int id){
        if (id < 0 || id >= robot_list.size()) {
            return "Robot_Id Not Found";
        }
        return robot_list[id].getStatus();
    }

    // method to simulate the entire operation
    void Simulation::simulate() 
    {   
        auto file_logger = spdlog::basic_logger_mt("file_logger", "logs.txt");
        while (running)
        {
            // Perform simulation step here
            for (int i = 0; i < robots.size(); i++) 
            {
                // if (robot_list[i].getStatus() == "Faulty"){
                //     continue;
                // }
                file_logger->info("Simulating robot:");
                file_logger->info("\tRobot {} is in room: <room_id>", robots[i]);
                file_logger->info("\tRobot {} is performing task: <task_name>", robots[i]);

                int randomNum = rand() % 101;
                if (randomNum < 10) {
                    file_logger->info("\tRobot {} has become faulty", robots[i]);
                    robot_list[i].setStatus("Faulty");
                }
                file_logger->info("\n");
                // std::cout << "Simulating robot: " << robots[i] << std::endl;
                // std::cout << "\tRobot " << robots[i] << " is in room: <room_id>" << std::endl;
                // std::cout << "\tRobot " << robots[i] << " is performing task: <task_name>\n" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Sleep for 1 second
        }

        std:cout << "Simulation shutting down" << std::endl;
    }

}