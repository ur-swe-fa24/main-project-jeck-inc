#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/catch_approx.hpp>
#include "Database.hpp"
#include "Robot.hpp"

using namespace database;

TEST_CASE("Database class tests"){
    mongocxx::instance currInst{};
    Database db;
    robot::Robot myRobot;
    std::cout << myRobot.getId() << std::endl;

    SECTION("tests (ask dr.martin)"){
        REQUIRE_NOTHROW(db.add_robot(myRobot));

        // REQUIRE(db.getRobotID(myRobot) == "-1");
        // REQUIRE(db.getRobotRoom(myRobot) == "-1");
        // REQUIRE(db.getRobotSize(myRobot) == "Medium");
        // REQUIRE(db.getRobotStatus(myRobot) == "Idle");
        // REQUIRE(db.getRobotTask(myRobot) == "Scrubber");

        // myRobot.setStatus("Active");
        // myRobot.setRoomAssigned("7");

        // db.updateRobots(myRobot);

        // REQUIRE(db.getRobotRoom(myRobot) == "7");
        // REQUIRE(db.getRobotStatus(myRobot) == "Active");

        REQUIRE_NOTHROW(db.init_analytics());

        REQUIRE(db.setNumOfError(1) == true);
        REQUIRE(db.setNumTaskCompleted(2) == true);
        REQUIRE(db.setTotalRobots(3) == true);
        REQUIRE(db.setTotalRoomClned(4) == true);
        REQUIRE(db.setCurrentTime(5) == true);

        std::vector<int> testing = {1,2,4};

        REQUIRE_NOTHROW(db.updateSM(2,testing,8,2));

        auto current = std::chrono::system_clock::now();
        auto now_timet = std::chrono::system_clock::to_time_t(current);
        auto now_local = localtime(&now_timet);
        std::cout << "Local Time " << std::put_time(now_local, "%c") << std::endl;

        
    }


}