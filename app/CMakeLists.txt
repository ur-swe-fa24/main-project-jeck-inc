add_executable(main
    main.cpp
)

#Additions for WxWidget
#================================================================
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})
#==============================================================

# Declaring Include Directories
target_include_directories(main
    PRIVATE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

# Linking Libraries to the main program!
target_link_libraries(main 
    PRIVATE
        Simulation
        Database
        Robot
        mongo::mongocxx_shared
        bm
        bs
        fe
        sm
)

#Linking External Libraries
target_link_libraries(Database
    PRIVATE
        mongo::mongocxx_shared
        nlohmann_json::nlohmann_json
        ${wxWidgets_LIBRARIES}  # addition

)

#Compile features for main
target_compile_features(main PRIVATE cxx_std_17)