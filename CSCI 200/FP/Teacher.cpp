/* CSCI 200: Final Project: School Management System
 *
 * Author: Tristen Erpelding
 * 
 * A comprehensive school management system. 
 * Itenables school administrators to efficiently manage various aspects of their institution, such as student enrollment, and scheduling. 
 * The system provides a user-friendly CLI (command-line) interface that enables easy navigation and interaction.
*/

#include "Teacher.h"

Teacher::Teacher(Classes *pCLASS)
{
    static const std::vector<std::string> firstNames = {"Alice", "Bob", "Charlie", "David", "Emma"};
    static const std::vector<std::string> lastNames = {"Anderson", "Brown", "Clark", "Davis", "Evans"};
    static const int numNames = firstNames.size();
    const std::string& firstName = firstNames[rand() % numNames];
    const std::string& lastName = lastNames[rand() % numNames];
    mFirstName = firstName;
    mLastName = lastName;
    classTeach = pCLASS;
}

std::string Teacher::getFullName() const
{
    return mFirstName + " " + mLastName;
}
