# User Guide for Robot Database Simulation

## Running the System
To start up the system, follow these steps:
1. Make sure you are in the main-project-jeck-inc directory
2. Build the directory
3. Run main

**Note:** The user will need to dowloand MongoDB Compass Shell and GUI for the database calls to work correctly. The user will also need ensure that a local server is running on their own machine 

## Tests
Example tests can be found in the [tests directory](../../tests)

## Guide to Interact with GUI
The following guide explains how to interact with the Robot Database Simulation system using the GUI. At the beginning you will face the home page where one has the option to choose from one of the four roles: Senior Manager, Building Manager, Building Staff and Field Engineer.
![Home](./imgs/home.png)

### Senior Manager
This role is still under work and will be completed in the next sprint. 

### Building Manager
Building Manager will have the ability to add a robot and assign task to robot. Upon pressing one of those buttons, an appropriate message will pop up. Either a success or an error (for instance, invalid robot id.).

![Building Manager](./imgs/bm-before.png)
![Assign Room](./imgs/assign-fail.png)
![Assign Room](./imgs/assign-pass.png)
![Add  Robot](./imgs/add-robot-success-msg.png)


### Building Staff
Building Staff will have the ability to check the robot status or do a maintenance update by filling it with shampoo or whatever necessary items it requires. 

![Building Staff](./imgs/bs.png)
![Filled Robot](./imgs/robot-filled.png)
![Robot Status](./imgs/robot-status.png)

### Field Engineer
Field Engineer can see all the robots in the simulation, fix a robot if it is broken and/or see the robot status of a particular robot like a Building Staff.

![Field Engineer](./imgs/fe.png)
![Robot Filled Message](./imgs/robot-filled.png)
![Robot Status](./imgs/robot-status.png)


## Building Configuration

### Map

The `building.json` file defines the structure and layout of the building for the simulation. Below is an explanation of the key components of this JSON configuration. The `building.json` configuration file is found in the app folder. 

### Key Components

- **building_name**: This field specifies the name of the building. In this example, it is set to "School". 

- **rooms**: This is an array that contains objects representing each room in the building. Each room object includes the following properties:
  - **room_id**: A unique identifier for the room (ex. "0", "1", "2").
  - **room_name**: The name of the room (ex. "classroom", "cafeteria", "lab").
  - **size**: The size of the room, which can be "small", "medium", or "large".
  - **floor_surface**: The type of floor in the room ("carpet", "wood", "tile").
  - **access**: Indicates the access level for the room
  - **connecting_rooms**: An array of room IDs that are directly connected to this room
  


