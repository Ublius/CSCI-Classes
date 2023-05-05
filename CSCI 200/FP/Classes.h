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

#ifndef CLASSES_H
#define CLASSES_H

#include <iostream>

#include "Teacher.h"

class Classes
{
    public:
        /**
         * @brief Constructor for the Classes object
         * @param SUBJECT The name of the subject for this course
         * @param COURSENUM The course number for this course
        */
        Classes(const std::string SUBJECT, const unsigned int COURSENUM);
        /**
         * @brief Destructor for the Classes object
        */
        ~Classes();
        /**
         * @brief Get the name of the class
         * @return The name of the class
        */
        std::string getClassName() const;
        /**
         * @brief Get the course number of the class
         * @return The course number of the class
        */
        unsigned int getCourseNum() const;
        /**
         * @brief Get the teacher for the class
         * @return A pointer to the teacher object for the class
        */
        Teacher* getClassTeach() const;
        
    private:
        std::string mSubject;
        unsigned int mCourseNum;
        Teacher* mpAssignTeacher;

};

#endif