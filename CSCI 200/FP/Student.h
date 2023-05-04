/* CSCI 200: Final Project: School Management System
 *
 * Author: Tristen Erpelding
 * 
 * A comprehensive school management system. 
 * Itenables school administrators to efficiently manage various aspects of their institution, such as student enrollment, and scheduling. 
 * The system provides a user-friendly CLI (command-line) interface that enables easy navigation and interaction.
*/

#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <vector>

#include "Classes.h"

class Student
{
    public:
        Student(const std::string FIRSTNAME, const std::string LASTNAME, const int GRADYEAR, const int CURRYEAR);
        unsigned int getGradYear() const;
        unsigned int getCurrYear() const;
        std::string getName() const;
        void addClass(Classes* pClassToAdd);
        void printClasses() const;

    private:
        std::string mFristName;
        std::string mLastName;
        unsigned int mGradYear;
        unsigned int mCurrYear;
        std::vector<Classes*> mpEnrollClasses;
};



#endif