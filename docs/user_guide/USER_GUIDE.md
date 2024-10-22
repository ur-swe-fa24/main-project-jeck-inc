# This is your user guide's top level!

In our current sprint, we have carried out one main user story and two sub user stories. The main user story is to add the robot to the system. 

Steps to run the system: 
- Create executable using the cmake files in our system. 
- Run the executable file. 
- You will receive log in the terminal providing you options to choose if you want to add a robot or exit the system. 
    - Option to exit the system shuts the program altogether. 
    - Option to add a robot will prompt you to enter the robot's size and type. 
    - Enter your information through the terminal. Please use the integer of the corresponding choice as the input values. 

A demonstration is shown below:


![Input Display](./input1.png "")


Note: Once you have added your first robot, you will see log of simulation engine at the back updating you about the robot status. The log will be displayed through the terminal as well. As both the input and output now occur at same terminal, it will get cumbersome. We intend to remedy this issue in our next sprint with the use of wxwidget. 


Thus, we were able to implement one major story: adding a robot to the system. We were also able to provide the robot its own id which was a sub-issue. We were also able to create a system such that the simulation has information about the robots it contained. At the moment, it only utilizes the robot id but as the program grows, the implementaiton will be more advanced. 