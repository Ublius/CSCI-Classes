/* CSCI 200: Final Project: School Management System
 *
 * Author: Tristen Erpelding
 * 
 * Resources:
 * Finding out how to clear cin and ignore anything that isn't a number
 * 
 * A comprehensive school management system. 
 * Itenables school administrators to efficiently manage various aspects of their institution, such as student enrollment, and scheduling. 
 * The system provides a user-friendly CLI (command-line) interface that enables easy navigation and interaction.
*/

#ifndef TEACHER_H
#define TEACHER_H

#include <iostream>
#include <vector>

// Forward declaration of classes caused errors when including classes as it was loop 
class Classes;

class Teacher {
public:
    /**
     * @brief Teacher constructor that takes a pointer to a Classes object as parameter and assigns it as the teacher's class
     * @param pCLASS A pointer to a Classes object that the teacher will be assigned to
    */
    Teacher(Classes* pCLASS);
    /**
     * @brief Returns the full name of the teacher as a string
     * @return The full name of the teacher as a string
    */
    std::string getFullName() const;

    private:
        std::string mFirstName;
        std::string mLastName;
        Classes* classTeach;
};

#endif