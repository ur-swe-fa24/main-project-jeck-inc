find_package(Catch2 3 REQUIRED)

IF (${Catch2_FOUND})
    MESSAGE(STATUS "Found Catch2")
ELSE (${Catch2_FOUND})
    MESSAGE(STATUS "Cound not find Catch2. Check to make sure its installed on your system!")
ENDIF (${Catch2_FOUND})

add_executable(test_simulation test_simulation.cpp)

target_compile_features(test_simulation PRIVATE cxx_std_17)

# Include directories for the test executable
target_include_directories(test_simulation
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

# How we link with Catch2!
target_link_libraries(test_simulation
  PRIVATE
    Catch2::Catch2WithMain
    Simulation
    Robot
)

