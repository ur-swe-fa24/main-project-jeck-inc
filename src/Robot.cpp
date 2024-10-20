#include "Robot.hpp"
#include <iostream>
#include <string>


namespace robot {




// Initialize the static variable
int Robot::nextId = 0;

// Constructor implementation
Robot::Robot(Function task, Size size) 
    : task(task), size(size), id(nextId++) {
}

// Getter for the robot ID
int Robot::getId() const {
    return id;
}




}