cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
message("Building Hello World!")
project(hello_world LANGUAGES CXX)

include(dependencies/get_cpm.cmake)
include(dependencies/dependencies.cmake)

# Setup the adapter libraries for use!
# add_library(adapter
#     src/cout_adapter.cpp
#     src/spdlog_adapter.cpp
# )

add_library(hello_world
    src/Database.cpp
    src/GUI.cpp 
    src/Robot.cpp 
    src/Simulation.cpp 
)

target_include_directories(hello_world
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

#using spdlog for loggin in spdlog_adapters
target_link_libraries(hello_world
    spdlog::spdlog
)

add_subdirectory(app)