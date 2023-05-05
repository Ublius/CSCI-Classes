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

#include "Classes.h"

Classes::Classes(const std::string SUBJECT, const unsigned int COURSENUM)
{
    // Assigns the subject and course number to the member variables
    mSubject = SUBJECT;
    mCourseNum = COURSENUM;
    // Creates a new Teacher object and assigns it to the class
    mpAssignTeacher = new Teacher(this);
}

Classes::~Classes()
{
    delete mpAssignTeacher;
}

std::string Classes::getClassName() const
{
    return mSubject;
}

unsigned int Classes::getCourseNum() const
{
    return mCourseNum;
}

Teacher* Classes::getClassTeach() const
{
    return mpAssignTeacher;
}

