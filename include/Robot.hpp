#ifndef ROBOT_HPP
#define ROBOT_HPP

#include <string>


namespace robot {

// Enum class for size
enum class Size {
    Large,
    Medium,
    Small
};

// Enum class for function
enum class Function {
    Scrub,
    Vacuum,
    Shampoo
};

class Robot {
private:
    static int nextId; // Static variable to hold the next robot ID
    int id;            // Robot ID
    Function task;    // Robot task
    Size size;        // Robot size

public:
    // Constructor
    Robot(Function task, Size size);

    // Getters
    int getId() const;
    Function getTask() const;
    Size getSize() const;
};

} // namespace robot

#endif // ROBOT_HPP
