#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/catch_approx.hpp>
#include "Simulation.hpp"
#include "Robot.hpp"

using namespace simulation;


//Test Case for Simulation Class
TEST_CASE("Simulation class tests") {
    Simulation sim;

    //Section to check the load building 
    SECTION("Load building from JSON") {
        std::string json_file = "../../tests/test_building.json"; 
        REQUIRE_NOTHROW(sim.load_building(json_file));
    }

    //Section to check correct representation of actual values
    SECTION("Load building and test data correctly stored") {
        std::string json_file = "../../tests/test_building.json"; 
        sim.load_building(json_file);
        REQUIRE(sim.get_building().name == "School");
        REQUIRE(sim.get_building().rooms["0"].room_name == "classroom");
        REQUIRE(sim.get_building().rooms["0"].room_id == "0");
        REQUIRE(sim.get_building().rooms["0"].size == "small");
        REQUIRE(sim.get_building().rooms["0"].floor_surface == "carpet");
        REQUIRE(sim.get_building().rooms["0"].access == "0");
        REQUIRE(sim.get_building().rooms["0"].connecting_rooms[0] == "1");
        REQUIRE(sim.get_building().rooms["0"].connecting_rooms[1] == "2");
    }

    //Adding robot to Simulation
    SECTION("Add robot to simulation") {
        Robot robot(Robot::Function::Vacuum, Robot::Size::Medium);
        REQUIRE_NOTHROW(sim.add_robot(robot));
    }

    //Operations in the smulate method that runs in background
    SECTION("Simulate operation") {
        std::thread sim_thread([&sim]() { sim.simulate(); });
        
        // Allow some time for the simulation to run
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); 

        REQUIRE_NOTHROW(sim.stop()); // Stop the simulation
        REQUIRE_NOTHROW(sim_thread.join()); // Wait for the thread to finish
    }

    // Section to check assigning tasks to robots
    SECTION("Assign tasks to robots") {
        Robot robot(Robot::Function::Vacuum, Robot::Size::Medium);
        sim.add_robot(robot);
        std::string json_file = "../../tests/test_building.json"; 
        sim.load_building(json_file);
        std::string roomID = "0"; 
      
        // Assign task to robot
        REQUIRE_NOTHROW(sim.assign_task(robot.getId(), roomID));
        REQUIRE(sim.robot_status(robot.getId()) == "Active");
    }

   
}