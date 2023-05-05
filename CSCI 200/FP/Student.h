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

#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <vector>

#include "Classes.h"

class Student
{
    public:
        /**
         * @brief Constructor for the Student class.
         * @param FIRSTNAME A string representing the student's first name.
         * @param LASTNAME A string representing the student's last name.
         * @param GRADYEAR An integer representing the student's expected graduation year.
         * @param CURRYEAR An integer representing the student's current academic year.
        */
        Student(const std::string FIRSTNAME, const std::string LASTNAME, const int GRADYEAR, const int CURRYEAR);
        /**
         * @brief Get the student's expected graduation year.
         * @return An unsigned integer representing the student's expected graduation year.
        */
        unsigned int getGradYear() const;
        /**
         * @brief Get the student's current academic year.
         * @return An unsigned integer representing the student's current academic year.
        */
        unsigned int getCurrYear() const;
        /**
         * @brief Get the student's full name.
         * @return A string representing the student's full name.
        */
        std::string getName() const;
        /**
         * @brief Add a class to the student's schedule.
         * @param pClassToAdd A pointer to the class object to be added.
        */
        void addClass(Classes* pClassToAdd);
        /**
         * @brief Print the list of classes the student is enrolled in.
        */
        void printClasses() const;

    private:
        std::string mFristName;
        std::string mLastName;
        unsigned int mGradYear;
        unsigned int mCurrYear;
        std::vector<Classes*> mpEnrollClasses;
};



#endif