#include "Simulation.hpp"

namespace simulation 
{
    // Note: It is assumed the json input file will be syntactically correct with the necessary fields
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
            room.percentClean = 0; // rooms start dirty
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
        robot_dict[robotInstance.getId()] = robotInstance; 
    }


    // Get all robot IDs
    std::string simulation::Simulation::getRobotIds() const {
        std::string robotStr; // Create a string stream to build the string
        std::string sizeStr;
        std::string functionStr;

        // Iterate over the map
        for (const auto& pair : robot_dict) 
        {   
            // Get size as a string
            switch (pair.second.getSize()) 
            {
                case Robot::Size::Large:
                    sizeStr = "Large";
                    break;
                case Robot::Size::Medium:
                    sizeStr = "Medium";
                    break;
                case Robot::Size::Small:
                    sizeStr = "Small";
                    break;
                default:
                    return "Unknown robot size";
            }

            // Get functino as a string
            switch (pair.second.getTask()) 
            {
                case Robot::Function::Scrub:
                    functionStr = "Scrubber";
                    break;
                case Robot::Function::Shampoo:
                    functionStr = "Shampooer";
                    break;
                case Robot::Function::Vacuum:
                    functionStr = "Vacuumer";
                    break;
                default:
                    return "Unknown robot function";
            }
            robotStr += std::to_string(pair.first) + " - " + sizeStr + " - " + functionStr + "\n"; // Append the robot ID
        }
        return robotStr; // Return the resulting string
    }

    //function that uses setStatus of robot to fix the robot
    std::string simulation::Simulation::fix_robot(int id)  {
        
        // Use the mutex to prevent race conditions
        std::lock_guard<std::mutex> lock(simulation_mutex); 

        // Check if robot id exists in dict
        if (robot_dict.find(id) == robot_dict.end()) { // Check if ID exists in the map
            return "Robot with ID " + std::to_string(id) + " not Found";
        }
        
        // When robot is not faulty
        robot::Robot& robot = robot_dict[id]; // Get the robot reference
        if (robot.getStatus() != "Faulty") {
            return "Robot is not faulty";
        }
        
        // Actually fixing the robot
        robot.setStatus("Active"); // Set robot back to active so it can finish its task

        return "Robot is fixed";
    }

    std::string simulation::Simulation::fill_robot_fluid(int id)
    {
        // Use the mutex to prevent race conditions
        std::lock_guard<std::mutex> lock(simulation_mutex); 

        if (robot_dict.find(id) == robot_dict.end()) { // Check if ID exists in the map
            return "Robot with ID " + std::to_string(id) + " not Found";
        }
        robot_dict[id].setFluidLevel(100);
        return "Robot filled";
    }

    //function that uses getStatus of robot to view the status of robot
    std::string simulation::Simulation::robot_status(int id) {
        if (robot_dict.find(id) == robot_dict.end()) { // Check if ID exists in the map
            return "Robot with ID " + std::to_string(id) + " not Found";
        }
        return robot_dict[id].getStatus(); // Access the robot directly from the map
    }

    // Method to assign a room to one robot
    std::string Simulation::assign_task(int robotID, std::string roomID)
    {
        // Use the mutex to prevent race conditions
        std::lock_guard<std::mutex> lock(simulation_mutex); 

        // Check if the robotID exists in the robot dictionary
        if (robot_dict.find(robotID) == robot_dict.end()) {
            return "Robot ID not found: " + std::to_string(robotID);
        }

        // Check if the roomID exists in the building
        if (building.rooms.find(roomID) == building.rooms.end()) {
            return "Room ID not found: " + roomID;
        }
        
        Robot::Function robotType = robot_dict[robotID].getTask();
        std::string floor = building.rooms[roomID].floor_surface;
        
        // Make sure the robot can clean the assigned room based on floor type
        switch (robotType) {
            case Robot::Function::Scrub:
                // Scrub robot can only clean wood and tile
                if (floor != "wood" && floor != "tile") {
                    return "Scrubber cannot clean " + floor + " surfaces.";
                }
                break;
            case Robot::Function::Shampoo:
                // Shampoo robot can only clean carpet
                if (floor != "carpet") {
                    return "Shampoo robot can only clean carpet surfaces.";
                }
                break;
            case Robot::Function::Vacuum:
                // Vacuum can clean all types, no action needed
                break;
            default:
                return "Unknown robot function.";
        }
        if (robot_dict[robotID].getStatus() == "Active")
        {
            std::queue<std::string>& backLog = robot_dict[robotID].getTaskBacklog(); // Get backlog queue
            backLog.push(roomID); // Add to backlog
            return "Room assigned, added to backlog of tasks as robot is currently in another room";
        }
        
        if (robot_dict[robotID].getStatus() == "Idle")
        {
            robot_dict[robotID].setStatus("Active"); // Set status of robot to Active
            robot_dict[robotID].setRoomAssigned(roomID); // Assign the robot -> the room (robot can only have 1 room)
            building.rooms[roomID].robots_cleaning.insert(robotID); // Assign the room -> the robot (room can have many robots)
            return "Room assigned, robot getting started now";
        }
        
        return "Robot is faulty and cannot be assigned a room";
        
    }

    // Method to get info about a robot
    std::string Simulation::get_robot_info(int id)
    {
        // Check if robot id exists in dict
        if (robot_dict.find(id) == robot_dict.end()) { // Check if ID exists in the map
            return "Robot with ID " + std::to_string(id) + " not Found";
        }

        robot::Robot& robot = robot_dict[id]; // get the robot

        return "Robot " + std::to_string(id) +
        "\n\tCurrent Status: " + robot.getStatus() + 
        "\n\tCurrent Room: " + robot.getRoomAssigned() + 
        "\n\tCurrent Battery: " + std::to_string(robot.getBattery()) + 
        "\n\tCurrent Fluid Level: " + std::to_string(robot.getFluidLevel());
    }

    // Method for getting faulty robots for UI notification
    std::vector<int> Simulation::getFaultyRobots()
    {
        vector<int> returnfaultyRobots = faultyRobots;
        faultyRobots.clear();
        return returnfaultyRobots;
    }

    // Method for getting completed tasks for UI notification
    std::unordered_set<std::string> Simulation::getTasksCompleted()
    {
        unordered_set<std::string> returnTasksCompleted = tasksCompleted;
        tasksCompleted.clear();
        return returnTasksCompleted;
    }

    // method to simulate the entire operation
    void Simulation::simulate() 
    {   
        int timeCount = 0;
        auto file_logger = spdlog::basic_logger_mt("file_logger", "logs.txt");
        file_logger->info("\t----- BEGIN SIMULATION -----\n");
        while (running)
        {
            {
                // Use the mutex to prevent race conditions
                std::lock_guard<std::mutex> lock(simulation_mutex); 
                file_logger->info("\t--- TIMESTAMP {} START---", timeCount);
                
                // Iterate through every robot
                for (auto& pair : robot_dict)
                {
                    int robotID = pair.first; // Get the robot ID
                    robot::Robot& robot = pair.second; // Get the robot reference

                    std::string roomID = robot.getRoomAssigned();
                    Robot::Function robotType = robot.getTask(); // Assuming this method exists

                    // Get robot type as a string for logger
                    std::string robotTypeStr;
                    switch (robotType) {
                        case Robot::Function::Scrub:
                            robotTypeStr = "Scrub";
                            break;
                        case Robot::Function::Shampoo:
                            robotTypeStr = "Shampoo";
                            break;
                        case Robot::Function::Vacuum:
                            robotTypeStr = "Vacuum";
                            break;
                        default:
                            robotTypeStr = "Unknown";
                    }

                    
                    // Check if robot is currently Active
                    if (robot.getStatus() == "Active")
                    {
                        // Randomly "break" the robot and make it Faulty
                        int randomNum = rand() % 101;
                        if (randomNum < 5) 
                        {
                            file_logger->info("\tRobot {} has become faulty", robotID);
                            robot.setStatus("Faulty");
                            faultyRobots.push_back(robotID);
                        }
                        else
                        {
                            file_logger->info("\tRobot {} is in room {} and performing task {}", robotID, roomID, robotTypeStr);

                            std::string robotRoom = robot.getRoomAssigned();
                        
                            // Check if the room is already 100% clean
                            int roomPercentClean = building.rooms[robotRoom].percentClean; 
                            if (roomPercentClean >= 100)
                            {
                                building.rooms[robotRoom].percentClean = 100; // set the room back to 100 in case it went over

                                // Robot is\ done cleaning the room
                                building.rooms[roomID].robots_cleaning.erase(robotID); // Remove robot from the room
                                
                                std::queue<std::string>& backLog = robot_dict[robotID].getTaskBacklog(); // Get backlog queue
                                
                                tasksCompleted.insert(roomID); // Add to tasksCompleted for notifcation that room is clean;
                                
                                // Check if there is a backlog of tasks for this robot
                                if (backLog.empty())
                                {
                                    robot_dict[robotID].setRoomAssigned("-1"); // Unassign the room from the robot
                                    robot_dict[robotID].setStatus("Idle"); // Set status of robot to Idle
                                    file_logger->info("\tRobot {} has finished cleaning room {} and is now idle", robotID, roomID);
                                }
                                else
                                {
                                    // Get next room in the backlog queue and take it out of the queue
                                    std::string nextRoom = backLog.front();
                                    backLog.pop();

                                    // Assign robot the next room
                                    robot_dict[robotID].setRoomAssigned(nextRoom);
                                    building.rooms[nextRoom].robots_cleaning.insert(robotID);
                                    file_logger->info("\tRobot {} has finished cleaning room {} and is now going to clean room {}", robotID, roomID, nextRoom);
                                }
                            }    
                            else
                            {
                                // If room isn't 100% clean:
                                
                                // robot_size_power is an integer scaler variable for how much of a room the robot can clean
                                // a medium robot cleans 1.5 times as much as a small robot
                                // a large robot cleans 2 times as much as a small robot
                                int robot_size_power = 0; 
                                switch (robot_dict[robotID].getSize()) 
                                {
                                    case Robot::Size::Large:
                                        robot_size_power = 4;
                                        break;
                                    case Robot::Size::Medium:
                                        robot_size_power = 3;
                                        break;
                                    case Robot::Size::Small:
                                        robot_size_power = 2;
                                        break;
                                    default:
                                        break;
                                }

                                // The size of the room and the size of the robot together determine how long
                                // it will take for the entire room to be clean
                                // It takes the same amount of time for a small robot to clean a small room,
                                // as a medium robot to clean a medium room,
                                // and a large robot to clean a large room
                                if (building.rooms[roomID].size == "large") 
                                {
                                    building.rooms[roomID].percentClean += 3 * robot_size_power;
                                } 
                                else if (building.rooms[roomID].size == "medium") 
                                {
                                    building.rooms[roomID].percentClean += 4 * robot_size_power;
                                } 
                                else if (building.rooms[roomID].size == "small") 
                                {
                                    building.rooms[roomID].percentClean += 6 * robot_size_power;
                                }

                                // Robots has their battery go down
                                int new_battery = robot_dict[robotID].getBattery() - 2;
                                robot_dict[robotID].setBattery(new_battery); 

                                // Robot has their fluid go down
                                int new_fluid_level = robot_dict[robotID].getFluidLevel() - 1;
                                robot_dict[robotID].setFluidLevel(new_fluid_level); 

                                // Robot is out of fluid
                                if (new_fluid_level <= 0)
                                {
                                    file_logger->info("\tRobot {} is out of fluid and is leaving room {} to become idle. ", robotID, roomID);
                                    building.rooms[roomID].robots_cleaning.erase(robotID); // Remove robot from the room
                                    robot_dict[robotID].setRoomAssigned("-1"); // Unassign the room from the robot
                                    robot_dict[robotID].setStatus("Idle"); // Set status of robot to Idle
                                    
                                    // Clear out the backlog
                                    std::queue<std::string>& backLog = robot_dict[robotID].getTaskBacklog(); // Get backlog queue
                                    while (!backLog.empty())
                                    {
                                        backLog.pop();
                                    }
                                }
                                // Robot is out of battery, it uses its reserve battery to go to charging station
                                else if (new_battery <= 0)
                                {
                                    file_logger->info("\tRobot {} is out of battery and is leaving room {} to charge and become idle", robotID, roomID);
                                    building.rooms[roomID].robots_cleaning.erase(robotID); // Remove robot from the room
                                    robot_dict[robotID].setRoomAssigned("-1"); // Unassign the room from the robot
                                    robot_dict[robotID].setStatus("Idle"); // Set status of robot to Idle
                                    
                                    // Clear out the backlog
                                    std::queue<std::string>& backLog = robot_dict[robotID].getTaskBacklog(); // Get backlog queue
                                    while (!backLog.empty())
                                    {
                                        backLog.pop();
                                    }
                                }
                                else
                                {
                                    file_logger->info("\tRobot {} is now at {} battery level and at {} fluid level", robotID, new_battery, new_fluid_level);
                                }
                            }
                        }
                    }
                    else if (robot.getStatus() == "Idle")
                    {
                        if (robot_dict[robotID].getBattery() < 100)
                        {
                            // Charge robot battery
                            int new_battery = robot_dict[robotID].getBattery() + 5;
                            robot_dict[robotID].setBattery(new_battery);
                        }
                        else if (robot_dict[robotID].getBattery() > 100)
                        {
                            robot_dict[robotID].setBattery(100);
                        }
                    }
                    
                }

                // Iterate through each room and make them get dirtier:
                for (auto& pair : building.rooms)
                {
                    pair.second.percentClean -= 3; // Decrease cleanliness by 3%
                    if (pair.second.percentClean < 0) 
                    {
                        pair.second.percentClean = 0; // Ensure cleanliness doesn't go below 0%
                    }
                }
            }
            file_logger->info("\t--- TIMESTAMP {} END---\n", timeCount);
            timeCount++;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // Sleep for 1 second
        }
        file_logger->info("\t--- SIMULATION END --- ");
        cout << "Simulation shutting down" << endl;
    }

}