cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/libs/mongo-cxx-driver")
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


find_package(mongocxx REQUIRED)
if (mongocxx_FOUND)
  message(STATUS "mongocxx found!")
endif()
find_package(bsoncxx REQUIRED)
if (bsoncxx_FOUND)
  message(STATUS "bsoncxx found!")
endif()
find_package(wxWidgets REQUIRED COMPONENTS net core base)
if (wxWidgets_FOUND)
    message(STATUS "wxWidgets found!")
endif()
if(wxWidgets_USE_FILE) # not defined in CONFIG mode
    include(${wxWidgets_USE_FILE})
endif()


add_library(Database
    src/Database.cpp
)

target_include_directories(Database
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





#============================================================
# FOR THE FRAMES
#===========================================================

# SENIOR MANAGER
add_library(sm
    src/Frames/sm.cpp
)

target_include_directories(sm
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_link_libraries(sm
    ${wxWidgets_LIBRARIES} 
    Simulation
    Database
    mongo::mongocxx_shared
)

#===========================================================

#BUILDING MANAGER
add_library(bm
    src/Frames/bm.cpp
)

target_include_directories(bm
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_link_libraries(bm
    ${wxWidgets_LIBRARIES} 
    Simulation
    Database
    mongo::mongocxx_shared
)

#===========================================================

#BUILDING STAFF
add_library(bs
    src/Frames/bs.cpp
)

target_include_directories(bs
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_link_libraries(bs
    ${wxWidgets_LIBRARIES} 
    Simulation
    Database
    mongo::mongocxx_shared
)

#===========================================================

#FIELD ENGINEER
add_library(fe
    src/Frames/fe.cpp
)

target_include_directories(fe
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_link_libraries(fe
    ${wxWidgets_LIBRARIES} 
    Simulation
    Database
    mongo::mongocxx_shared
)

#===========================================================



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
        spdlog::spdlog
)
add_subdirectory(app)
add_subdirectory(tests)