/* CSCI 200: Final Project: School Management System
 *
 * Author: Tristen Erpelding
 * 
 * A comprehensive school management system. 
 * Itenables school administrators to efficiently manage various aspects of their institution, such as student enrollment, and scheduling. 
 * The system provides a user-friendly CLI (command-line) interface that enables easy navigation and interaction.
*/

#ifndef TEACHER_H
#define TEACHER_H

#include <iostream>
#include <vector>

class Classes;

class Teacher {
public:
    Teacher(Classes* pCLASS);

    std::string getFullName() const;

    private:
        std::string mFirstName;
        std::string mLastName;
        Classes* classTeach;
};

#endif