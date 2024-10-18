#include "Robot.hpp"


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

// Getter for the task
Function Robot::getTask() const {
    return task;
}

// Getter for the size
Size Robot::getSize() const {
    return size;
}


}