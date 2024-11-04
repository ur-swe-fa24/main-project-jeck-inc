add_executable(main
    main.cpp
)

target_include_directories(main
    PRIVATE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

# You will need to link your adapters to the test program!
target_link_libraries(main 
    PRIVATE
        Simulation
        GUI
        Database
        Robot
        mongo::mongocxx_shared
)

target_link_libraries(Database
    PRIVATE
        mongo::mongocxx_shared
        ${wxWidgets_LIBRARIES}  # addition

)

target_compile_features(main PRIVATE cxx_std_17)

#additions
#================================================================
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})
#==============================================================
