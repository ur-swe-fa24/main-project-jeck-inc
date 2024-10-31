cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
message("Building Hello World prototype!")
project(hello_world LANGUAGES CXX)

include(dependencies/get_cpm.cmake)
include(dependencies/dependencies.cmake)
#include(cmake/CPM.cmake)

# Setup the adapter libraries for use!
# add_library(adapter
#     src/cout_adapter.cpp
#     src/spdlog_adapter.cpp
# )



add_library(Database
    src/Database.cpp
)

target_include_directories(Database
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

add_library(GUI
    src/GUI.cpp 
)

target_include_directories(GUI
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

add_library(Robot
    src/Robot.cpp 
)

target_include_directories(Robot
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

add_library(Simulation
    src/Simulation.cpp 
)

target_include_directories(Simulation
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

# using spdlog for loggin in spdlog_adapters
# target_link_libraries(main
#     spdlog::spdlog
# )


# Add nlohmann/json as a dependency
#CPMAddPackage("gh:nlohmann/json@3.10.5")
# CPMAddPackage(
#     NAME nlohmann_json
#     GITHUB_REPOSITORY nlohmann/json
#     VERSION 3.10.5  # specify the version you want to use
# )

# Link nlohmann/json to the Simulation library
target_link_libraries(Simulation
        nlohmann_json::nlohmann_json
)

add_subdirectory(app)