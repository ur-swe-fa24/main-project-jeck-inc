# User Guide for Robot Database Simulation

## Running the System
To start up the system, follow these steps:
1. Make sure you are in the main-project-jeck-inc directory
2. Build the directory
3. Run main

**Note:** The user will need to dowloand MongoDB Compass Shell and GUI for the database calls to work correctly.

## Tests
Example tests can be found in the [tests directory](../../tests)

## Guide to Interact with GUI
The following guide explains how to interact with the Robot Database Simulation system using the GUI. The system enables users to add, manage, and check the status of robots within the simulation environment. Follow these steps to interact efficiently with the simulation and manage the robots effectively.

### 1. Adding a Robot
To add a new robot to the system, follow these steps:
1. Press the **Add Robot Button** on the GUI.
2. The system will prompt you to enter the **size** and **type** of the robot.
3. After entering the required details, the system will create a new robot with the specified characteristics.
4. The robot will be added to the system database.
5. A confirmation message will appear, indicating that the robot has been successfully added.

### 2. Fixing a Robot
If a robot is faulty and needs repair, you can initiate the fix process:
1. Press the **Fix Button** on the GUI.
2. Enter the **Robot ID** of the robot you wish to fix.
3. The system will verify the Robot ID and determine the robot's status:
   - **Invalid ID**: If the Robot ID is invalid, the system will display an error message.
   - **Not Faulty**: If the robot is not faulty, a message will be shown indicating that the robot does not require fixing.
   - **Faulty**: If the robot is faulty, it will be marked as fixed, and a confirmation message will be displayed.

### 3. Displaying All Robots
To view the IDs of all robots currently in the system:
1. Press the **Show All Robots Button** on the GUI.
2. The system will display a list of all robot IDs stored within the database.

### 4. Checking Robot Status
To check the operational status of a specific robot:
1. Press the **Show Robot Status Button** on the GUI.
2. Enter the **Robot ID** of the robot you want to check.
3. The system will validate the Robot ID:
   - **Valid ID**: If the ID is valid, the system will display the robot’s current status (e.g., Operational or Not Operational).
   - **Invalid ID**: If the ID is invalid, the system will display an error message indicating that the Robot ID is not recognized.

### 5. Quitting the Simulation
When you are finished using the system:
1. Press the **Quit Button** on the GUI.
2. The system will end the simulation thread and close the GUI, effectively shutting down the application.

### Additional Information
- Each action (such as adding, fixing, or displaying robots) runs as a separate process, allowing multiple actions to occur simultaneously within the simulation.
- Error messages will appear if invalid inputs are provided, helping users correct their actions.

