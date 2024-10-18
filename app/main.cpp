#include <thread>
#include <iostream>

#include "database.hpp"
#include "robot.hpp"
#include "simulation.hpp"


using namespace database;
using namespace robot;
using namespace simulation;

// Function to convert user input to the corresponding enum value
robot::Function getFunctionFromInput(int choice) {
    switch (choice) {
        case 1: return robot::Function::Scrub;
        case 2: return robot::Function::Vacuum;
        case 3: return robot::Function::Shampoo;
        default: throw std::invalid_argument("Invalid choice for function");
    }
}

robot::Size getSizeFromInput(int choice) {
    switch (choice) {
        case 1: return robot::Size::Large;
        case 2: return robot::Size::Medium;
        case 3: return robot::Size::Small;
        default: throw std::invalid_argument("Invalid choice for size");
    }
}




int main(){

    Simulation sim;
    Database db;

    std::thread t(&Simulation::simulate, std::ref(sim));
    t.detach();

    std::cout << "Choose one of the following actions using their integer id:" << std::endl;
    std::cout << "1. Add a Robot" << std::endl;
    std::cout << "2. Quit the system" << std::endl;

    int input;
    std::cin >> input;

    while (input != 2){
        if (input == 1){
            std::cout << "Enter the size of the robot from the following options using the integer id" << std::endl;
            std::cout << "1. Small" << std::endl;
            std::cout << "2. Medium" << std::endl;
            std::cout << "3. Large" << std::endl;

            int size;
            std::cin >> size ;

            std::cout << "Enter the function of the robot from the following options using the integer id" << std::endl;
            std::cout  << "1. Shampoo" << std::endl;
            std::cout  << "2. Vacuum" << std::endl;
            std::cout  << "3. Scrub" << std::endl;

            int type;
            std::cin >> type;

            robot::Function type_ = getFunctionFromInput(type);
            robot::Size size_ = getSizeFromInput(size);
            robot::Robot myRobot(type_, size_); 

            sim.add_robot(myRobot);
            db.add_robot(myRobot);


        };

    }





};

