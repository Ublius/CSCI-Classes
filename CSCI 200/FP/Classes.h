/* CSCI 200: Final Project: School Management System
 *
 * Author: Tristen Erpelding
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
        Classes(const std::string SUBJECT, const unsigned int COURSENUM);
        ~Classes();
        std::string getClassName() const;
        unsigned int getCourseNum() const;
        Teacher* getClassTeach() const;
        
    private:
        std::string mSubject;
        unsigned int mCourseNum;
        Teacher* mpAssignTeacher;

};

#endif