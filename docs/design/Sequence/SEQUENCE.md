# Sequence Diagrams

## User to UI:
Each diagram describes an interaction between the User and the UI when accomplishing the following tasks. The Diagrams are broken up into those which can be done with both the technical view and simple view, and those that must be done with the technical view. 
 ### Assigning a task
 ![Entity Sequence Diagram](./User_To_UI/SV_Assign_Tasks.png)

 ### Getting Metrics
 ![Entity Sequence Diagram](./User_To_UI/SV_Get_Metrics.png)

 ### Accessing Logs for maintenence/errors
 ![Entity Sequence Diagram](./User_To_UI/TV_Access_Logs.png)

 ### Adding/Viewing Status of Robots
 ![Entity Sequence Diagram](./User_To_UI/TV_Add_View_Robot.png)

 ### View/Edit the floor map
 ![Entity Sequence Diagram](./User_To_UI/TV_View_Edit_Map.png)


## Entity:
![Entity Sequence Diagram (UI to Various Systems)](./Entities/Entities_Sequence_Diagram.png)

The Entity Sequence Diagram describes the flow of data in a normal simulation. 

Building Manager will initially input a role through the GUI that needs to be validated by Simulator. Once the map is in the repertoire, the simulator process the map, part of which is creating a room instance for each room. An appropirate message is sent back depending on the process. 

GUI allow addition of robot(s) which first goes through System Manager where the instance of robots are created. The data is logged into the database and the Simulator is also informed of it. 

The next step is to assign task to the robot(s). This will occur through the Task instance that can be referred in our Classes.md. Both the Simulator and the System Manager are sent this message for proper processing. 

Simulator is also responsible for detecting issues and sending notification of completion to the user through GUI. 

Some roles have access to the robot status and other roles have access to the overall metric of the tasks. This information is pulled from the database through communication via. System Manager. The information is ultimately sent to GUI for the users. 