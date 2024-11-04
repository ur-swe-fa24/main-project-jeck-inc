#include "Robot.hpp"
#include <iostream>
#include <string>


namespace robot {




// Initialize the static variable
int Robot::nextId = 0;

// Constructor implementation
Robot::Robot(Function task, Size size) 
    : task(task), size(size), id(nextId++), status(Robot::Status::Ideal) {
}

// Getter for the robot ID
int Robot::getId() const {
    return id;
}

std::string Robot::getStatus() const {
    switch (status) {
        case Status::Ideal: return "Ideal";
        case Status::Active: return "Active";
        case Status::Faulty: return "Faulty";
        default: return "Unknonw";
    };
}

void Robot::setStatus(std::string statusStr)  {
    if (statusStr == "Ideal") {
        status = Status::Ideal;
    } else if (statusStr == "Active") {
        status = Status::Active;
    } else if (statusStr == "Faulty") {
        status = Status::Faulty;
    };
 
}




}