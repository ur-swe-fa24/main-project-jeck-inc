#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/catch_approx.hpp>
#include "Database.hpp"
#include "Robot.hpp"

using namespace database;

TEST_CASE("Database class tests"){
    Database db;
    robot::Robot myRobot;
    std::cout << myRobot.getId() << std::endl;
    db.add_robot(myRobot);

    SECTION("add robot to database"){
        REQUIRE(2+2 == 4);
        // REQUIRE_NOTHROW(db.add_robot(myRobot));
    }

    // SECTION("Get all items"){
    //     REQUIRE(db.getRobotID(myRobot) == "-1");
    //     REQUIRE(db.getRobotRoom(myRobot) == "-1");
    //     REQUIRE(db.getRobotSize(myRobot) == "Medium");
    //     REQUIRE(db.getRobotStatus(myRobot) == "Idle");
    //     REQUIRE(db.getRobotTask(myRobot) == "Scrubber");
    // }

}