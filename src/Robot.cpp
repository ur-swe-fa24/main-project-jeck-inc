#include "Robot.hpp"

namespace robot {

    // Initialize the static variable
    int Robot::nextId = 0;

    // Constructor implementation
    Robot::Robot(Function task, Size size) 
        : task(task), size(size), id(nextId++), status(Robot::Status::Idle) {
            roomAssigned = "-1";
            battery = 100;
            fluidLevel = 100;
    }

    // Getter for the robot ID
    int Robot::getId() const {
        return id;
    }

    //Getter for Status which converts enum to string for ease of use
    std::string Robot::getStatus() const {
        switch (status) {
            case Status::Idle: return "Idle";
            case Status::Active: return "Active";
            case Status::Faulty: return "Faulty";
            default: return "Unknown";
        };
    }

    //Setter for Status which converts string to enum 
    void Robot::setStatus(std::string statusStr)  {
        if (statusStr == "Idle") {
            status = Status::Idle;
        } else if (statusStr == "Active") {
            status = Status::Active;
        } else if (statusStr == "Faulty") {
            status = Status::Faulty;
        };
    
    }

    //Getter Function to get the working time of robot
    int Robot::getUpTime() const {
        return upTime;
    }

    //Getter function to get the total spawned time of the robot
    int Robot::getTotalLiveTime() const {
        return totalLiveTime;
    }

    //Getter function to set the battery of robot
    void Robot::setBattery(int new_battery){
        battery = new_battery;
    }

    //Getter function to set the room assigned for the robot (if any else -1)
    void Robot::setRoomAssigned(std::string newRoom){
        roomAssigned = newRoom;
    }  

    //Getter function to set the fluid level for the robot
    void Robot::setFluidLevel(int updatedFluidLevel)
    {
        fluidLevel = updatedFluidLevel;
    }

    //Getter function to set the total live time of the robot
    void Robot::setTotalLiveTime(int new_time){
        totalLiveTime = new_time;
    }

    //Getter functio to set the total work time of the robot
    void Robot::setUpTime(int new_up_time){
        upTime = new_up_time;
    }

}