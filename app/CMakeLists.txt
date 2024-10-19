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
        hello_world
)

target_compile_features(main PRIVATE cxx_std_17)