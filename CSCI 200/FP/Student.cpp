
#include <iomanip>

#include "Student.h"

Student::Student(const std::string FIRSTNAME, const std::string LASTNAME, const int GRADYEAR, const int CURRYEAR)
{
    mFristName = FIRSTNAME;
    mLastName = LASTNAME;
    mGradYear = GRADYEAR;
    mCurrYear = CURRYEAR;
}

std::string Student::getName() const
{
    return mFristName + " " + mLastName;
}

void Student::addClass(Classes *pClassToAdd)
{
    // Check if the class is already enrolled
    for (Classes* enrolledClass : mpEnrollClasses) 
    {
        if (enrolledClass == pClassToAdd) 
        {
            std::cout << "Error: Student is already enrolled in this class." << std::endl;
            std::cout << std::endl;
            return;
        }
    }
    
    // Add the class to the student's list of enrolled classes
    mpEnrollClasses.push_back(pClassToAdd);
}

void Student::printClasses() const
{
     std::cout << std::left << std::setw(20) << "Course Name" << std::setw(10) << "Course Number" << std::endl;
    for (const auto& classes : mpEnrollClasses) 
    {
        std::cout << std::left << std::setw(20) << classes->getClassName() << std::setw(10) << classes->getCourseNum() << std::endl;
    }
}