#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/catch_approx.hpp>
#include "Robot.hpp"

using namespace robot;

TEST_CASE("Robot class tests") {

    SECTION("Check ID") {
        Robot robot(Robot::Function::Scrub, Robot::Size::Large);
        REQUIRE(robot.getId() == 0);
        Robot robot2(Robot::Function::Scrub, Robot::Size::Large);
        REQUIRE(robot2.getId() != robot.getId());
    }

    SECTION("Check Status") {
        Robot robot(Robot::Function::Shampoo, Robot::Size::Small);
        //Initial Status should be Ideal
        REQUIRE(robot.getStatus() == "Ideal");

        //Checks change of Status through set method
        robot.setStatus("Faulty");
        REQUIRE(robot.getStatus() == "Faulty");
    }

    SECTION("Checking other getter methods") {
        Robot robot(Robot::Function::Shampoo, Robot::Size::Small);
        //Check for the accurate task and size as sent
        REQUIRE(robot.getTask() == Robot::Function::Shampoo);
        REQUIRE(robot.getSize() == Robot::Size::Small);


        Robot robot2(Robot::Function::Scrub, Robot::Size::Large);
        //Check for the accurate task and size as sent
        REQUIRE(robot2.getTask() == Robot::Function::Scrub);
        REQUIRE(robot2.getSize() == Robot::Size::Large);

        
    }




}

    // SECTION("Load building and test data correctly stored") {
 
    //     REQUIRE(sim.get_building().name == "School");
    //     REQUIRE(sim.get_building().rooms["0"].room_name == "classroom");
    //     REQUIRE(sim.get_building().rooms["0"].room_id == "0");
    //     REQUIRE(sim.get_building().rooms["0"].size == "small");
    //     REQUIRE(sim.get_building().rooms["0"].floor_surface == "carpet");
    //     REQUIRE(sim.get_building().rooms["0"].access == "0");
    //     REQUIRE(sim.get_building().rooms["0"].connecting_rooms[0] == "1");
    //     REQUIRE(sim.get_building().rooms["0"].connecting_rooms[1] == "2");
    // }

    
    // SECTION("Add robot to simulation") {
    //     Robot robot(Robot::Function::Vacuum, Robot::Size::Medium);
    //     REQUIRE_NOTHROW(sim.add_robot(robot));
    // }

    // SECTION("Simulate operation") {
    //     std::thread sim_thread([&sim]() { sim.simulate(); });
        
    //     // Allow some time for the simulation to run
    //     std::this_thread::sleep_for(std::chrono::milliseconds(100)); 

    //     REQUIRE_NOTHROW(sim.stop()); // Stop the simulation
    //     REQUIRE_NOTHROW(sim_thread.join()); // Wait for the thread to finish
    // }
