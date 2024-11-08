#ifndef ROBOT_HPP
#define ROBOT_HPP

#include <string>


namespace robot {


class Robot {

public:
    
    // Enum class for size
    enum class Size {
        Large,
        Medium,
        Small
    };

    enum class Status {
        Idle, 
        Active, 
        Faulty
    };

    // Enum class for function
    enum class Function {
        Scrub,
        Vacuum,
        Shampoo
    };

    // Constructor
    Robot(Function task, Size size);

    // Getters
    int getId() const;
    Function getTask() const {return task;};
    Size getSize() const {return size;};
    std::string getStatus() const;
    void setStatus(std::string statusStr) ;
    int getBattery() const {return battery;};
    void setBattery(int updatedBattery);
    int getRoomAssigned() const {return roomAssigned;};
    void setRoomAssigned(int newRoom);


private:
    static int nextId; // Static variable to hold the next robot ID
    int id;            // Robot ID
    Function task;    // Robot task
    Size size;        // Robot size
    Status status;    // Robot status
    int battery;       //Robot Battery
    int roomAssigned;       //Room-Id Assigned

};

} // namespace robot

#endif // ROBOT_HPP
