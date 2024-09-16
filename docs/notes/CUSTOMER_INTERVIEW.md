# Customer Interview Document

## Goal & Overview

Develop a prototype cleaning robot fleet management system for the company Cobotiq. There are four primary components:

1\. a Simulation Engine to simulate “live” robots as they are sent on jobs

2\. a durable Status Database that maintains robot and job status information

3\. a System Manager that bridges the backend simulation and database systems 

4\. a Graphical User Interface with different views based upon the customer role.

## Likely Users

- Senior Management \- the C-Suite as they say\! These users care about the metrics (robot utilization, error rates) that Cobotiq talked about. They do not care about tasking robots.  
- Building Manager \- assign tasks to groups of robots (building wide missions), view status of robots.  
- Building Staff (what they called Operator) \- view robot status, assign cleaning tasks.  
- Field Engineer \- same abilities as Building Manager, but more detail about errors and robot status. Also, add in magical “fix” actions. May also add/remove robots to the system.

## Simulation Engine

The simulation is our mock of the real world. This simulation will be discrete-time - think integer time ticks, but could run off the real time. It will need to emulate how robots run based on their type and rooms in the building.

### Robots

Robots are simulated in discrete time and every time tick they have a probability of task failure. This probability will vary across platforms and should be configurable.

- Three sizes: Large, Medium, Small - determines runtime in room due to battery ca- pacity
- Three functions: Scrub, Vacuum, Shampoo - determines if a robot may clean a room type
+ Scrubbers only work on wood and tile – Shampoo only on carpet
+ Vacuum - all types
- Individual Tasks: clean room X, go home to charge, go offline

## Building Aspects
The building will have a finite number of rooms (0 < N < 11). This can be configurable at the start.
- Three room sizes: Large, Medium, Small
- Three floor types: wood, carpet, tile
- Rooms may have restricted access designated by building manager
The building will be input through a json configuration file.
- Will be organized as a dictionary like “RoomA”: [“connectingRoomB”, “connectingRoomC”]
- This file will be parsed through by the simulation engine to build the map

## Status Database
The database will be a well-organized relational database and will use MongoDB

Diagram (ERD) will be used to illustrate how the tables relate to each other. Some of the potential entities are: 
- Robots: fields for Robot ID, Size, Function, 
- Jobs: fields for Job id, robot id (which robot the job is assigned to), job description, room ID
- Rooms: fields for Room id, floor type, room size, list/vector of connecting rooms, access level, time required to clean it



## System Manager
We want to design a management system that can give multiple different users the information they need over. 
- Be able to display the priority of the robots. Which robots doing which jobs? 
- Tasks can be assigned to robots
- Keep track of Position, Task, battery, water amounts, length of queue 
- Will keep track of types of floors and assign to specific robots. 
- Will manage where the robots go 
- Robots will run until consumables are low
- Log files will be generated which will contain information about location, error messages, basic data, start, stop, cleaning setting
- Display a log that contains very technical information for those who are around to troubleshoot
- Integration with date and time. And integrate with temporary changes in dates and events


## Graphical User Interface

The GUI will have multiple pages for different needs
- Task manager/assigner page: Job Validation will be performed as not some actions like shampooing can not be performed at all rooms
- Statistics/analytics page: Logs of Productivity, Issues and Errors are shown here
- Settings page
- Simulation Map: Keep tracks of robots at a particular instance and is updated every discrete time
- Robot Attributes: Clicking on any robot on the map will lead here where it keeps track of time, percentage complete, battery used, water used, sq ft covered, of the robot, Errors if any, Last place and time when it encountered the error

