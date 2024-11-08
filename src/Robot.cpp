#include "Robot.hpp"
#include <iostream>
#include <string>


namespace robot {




// Initialize the static variable
int Robot::nextId = 0;

// Constructor implementation
Robot::Robot(Function task, Size size) 
    : task(task), size(size), id(nextId++), status(Robot::Status::Idle) {
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




}