# This is your design document's top level!

This is where the core designs are started.
You will also store your PlantUML source files in this directory.

[ARCHITECTURE.md](Architecture/ARCHITECTURE.md)

[CLASSES.md](Classes/CLASSES.md)

## Entity Sequence Diagram 
![Entity Sequence Diagram (UI to Various Systems)](./Entities_Sequence_Diagram/image.png "")

The Entity Sequence Diagram describes the flow of data in a normal simulation. 

Building Manager will initially input a role through the GUI that needs to be validated by Simulator. Once the map is in the repertoire, the simulator process the map, part of which is creating a room instance for each room. An appropirate message is sent back depending on the process. 

GUI allow addition of robot(s) which first goes through System Manager where the instance of robots are created. The data is logged into the database and the Simulator is also informed of it. 

The next step is to assign task to the robot(s). This will occur through the Task instance that can be referred in our Classes.md. Both the Simulator and the System Manager are sent this message for proper processing. 

Simulator is also responsible for detecting issues and sending notification of completion to the user through GUI. 

Some roles have access to the robot status and other roles have access to the overall metric of the tasks. This information is pulled from the database through communication via. System Manager. The information is ultimately sent to GUI for the users. 





