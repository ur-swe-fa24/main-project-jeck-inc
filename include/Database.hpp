#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <vector>
#include <string>
#include <iostream>

// Creating namespace
namespace database {

class Database {
private:
    // Attribute to hold a vector of log messages that 
    // Logss will imitate the database
    // Change its datatype as you see fit
    std::vector<std::string> loggs; 

public:
    // Method to add a robot to the database
    void add_robot(const robot::Robot& robotInstance);

    // Method to display a message on the console
    void console_message(const std::string& message);
};

} // namespace database

#endif // DATABASE_HPP
