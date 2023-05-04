/* CSCI 200: Final Project: School Management System
 *
 * Author: Tristen Erpelding
 * 
 * A comprehensive school management system. 
 * Itenables school administrators to efficiently manage various aspects of their institution, such as student enrollment, and scheduling. 
 * The system provides a user-friendly CLI (command-line) interface that enables easy navigation and interaction.
*/

#include "Classes.h"

Classes::Classes(const std::string SUBJECT, const unsigned int COURSENUM)
{
    mSubject = SUBJECT;
    mCourseNum = COURSENUM;
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

