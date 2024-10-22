#include <thread>
#include <iostream>

#include "Robot.hpp"
#include "Database.hpp"
#include "Simulation.hpp"


using namespace database;
using namespace robot;
using namespace simulation;

// Function to convert user input to the corresponding enum value
Robot::Function getFunctionFromInput(int choice) {
    switch (choice) {
        case 1: return Robot::Function::Scrub;
        case 2: return Robot::Function::Vacuum;
        case 3: return Robot::Function::Shampoo;
        default: throw std::invalid_argument("Invalid choice for function");
    }
}

Robot::Size getSizeFromInput(int choice) {
    switch (choice) {
        case 1: return Robot::Size::Large;
        case 2: return Robot::Size::Medium;
        case 3: return Robot::Size::Small;
        default: throw std::invalid_argument("Invalid choice for size");
    }
}




int main(){

    Robot r(Robot::Function::Scrub, Robot::Size::Large);

    Simulation sim;
    Database db; //Create the database object.

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

            Robot::Function type_ = getFunctionFromInput(type);
            Robot::Size size_ = getSizeFromInput(size);
            Robot myRobot(type_, size_); 


            sim.add_robot(myRobot);
            db.add_robot(myRobot); //Add the robot to the database.
            db.getRobotIDs(myRobot); //Print all the IDs we have. 

            std::cout << "Choose one of the following actions using their integer id:" << std::endl;
            std::cout << "1. Add a Robot" << std::endl;
            std::cout << "2. Quit the system" << std::endl;

            std::cin >> input;

        


        };

    }





};

