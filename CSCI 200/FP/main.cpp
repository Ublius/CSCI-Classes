/* CSCI 200: Final Project: School Management System
 *
 * Author: Tristen Erpelding
 * 
 * A comprehensive school management system. 
 * Itenables school administrators to efficiently manage various aspects of their institution, such as student enrollment, and scheduling. 
 * The system provides a user-friendly CLI (command-line) interface that enables easy navigation and interaction.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <limits>

#include "Student.h"
#include "Classes.h"
#include "Teacher.h"

using namespace std;

int main(int argc, char* argv[]) 
{
    bool openFile = true;
    bool selectStudent = true;
    
    string filename;
    if(argc == 1)
    {
        cout << "Please enter the name for your students file here: ";
        getline(cin, filename);
        cout << "Opening file '" << filename << "'" << endl;
    } 
    else if(argc == 2) 
    {
        filename = argv[1];
        cout << "Opening file '" << filename << "'" << endl;
    } 
    else 
    {
        cerr << "Usage: " << argv[0] << " [filename]" << endl;
        cerr << "  filename   - optional file to open upon start" << endl;
        return -2;
    }
    // open file for parsing
    ifstream fileIn;
    fileIn.open(filename);
    if (fileIn.fail()) 
    {
        cerr << "Error opening input file";
        return -1;
    }
    cout << "File opened successfully" << endl;


    vector<Student*> studentList;
    string firstName, lastName;
    int currYear, gradYear;

    while (fileIn >> firstName >> lastName >> currYear >> gradYear)
    {
        Student* newStudent = new Student(firstName, lastName, gradYear, currYear);
        studentList.push_back(newStudent);
    }

    int studentChoice;
    int studentCount = studentList.size();

    vector<Classes*> classList;

    unsigned int choice;
    while(true)
    {
        cout << "\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\";
        cout << "\n\n\t\tSCHOOL MANAGEMENT SYSTEM\n\n";
        cout << "\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\";
        cout << "\n\n\t\tMAIN SCREEN\n\n";
        cout << "Enter your choice: " << endl;
        cout << "1. Students information" << endl;
        cout << "2. Teacher information" << endl;
        cout << "3. Exit program" << endl;
        cout << "Choice: ";
        cin >> choice;
        cout << endl;

        if(cin.fail()) 
        {
            // clear the error flag
            cin.clear();  
            // ignore the invalid input
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid number choice." << endl;
            cout << endl;
        }

        while(choice == 1)
        {
            
            while(selectStudent)
            {
                cout << "Their are currently " << studentCount << " student's registered in the system." << endl;

                for(int i = 0; i < studentCount; i++)
                    cout << "Student " << i+1 << ": " << studentList.at(i)->getName() << endl;
                
                cout << endl;
                cout << "Which student would you like to access? ";
                cin >> studentChoice;

                if(cin.fail()) 
                {
                    // clear the error flag
                    cin.clear();  
                    // ignore the invalid input
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a valid number choice." << endl;
                    cout << endl;
                }

                while(studentChoice > studentCount || studentChoice < 1)
                {
                    cout << "That is an invalid selection. Please select again." << endl; 
                    cout << "Which student would you like to access? ";
                    cin >> studentChoice;
                }
                
                int confirmedChoice = 0;
                cout << "You have chosen student " << studentChoice << " who is "  << studentList.at(studentChoice - 1)->getName() << endl;
                cout << "Please confirm that is correct. (1 for yes or -1 for no): ";
                cin >> confirmedChoice;

                while(confirmedChoice != 1 && confirmedChoice != -1)
                {
                    cout << "That is an invalid selection. Please select again." << endl; 
                    cout << "Please confirm that is correct. (1 for yes or -1 for no): ";
                    cin >> confirmedChoice;
                }
                cout << endl;

                if(confirmedChoice == 1)
                {
                    selectStudent = false;
                    break;
                }
            }
            
            unsigned int newChoice;
            cout << "You have selected " << studentList.at(studentChoice - 1)->getName() << endl;
            cout << "What would you like to do with that student?" << endl;
            cout << "1. Add Classes" << endl;
            cout << "2. View Classes" << endl;
            cout << "3. Select new student" << endl;
            cout << "4. Return to menu" << endl;
            cout << "Choice: ";
            cin >> newChoice;
            cout << endl;

            if(cin.fail()) 
            {
                // clear the error flag
                cin.clear();  
                // ignore the invalid input
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a valid number choice." << endl;
                cout << endl;
            }

            if(newChoice == 1)
            {
                if(openFile)
                {
                    // string classesFileName = "Classes.txt";
                    string classesFileName;
                    cout << "Enter the location of the classes.txt file: ";
                    cin >> classesFileName;

                    ifstream fileIn;
                    fileIn.open(classesFileName);

                    if (fileIn.fail()) 
                    {
                        cerr << "Error opening input file";
                        return -1;
                    }
                    cout << "File opened successfully" << endl;
                    cout << endl;

                    
                    string className;
                    unsigned int coruseNum;

                    while (fileIn >> className >> coruseNum)
                    {
                        Classes* newClass = new Classes(className, coruseNum);
                        classList.push_back(newClass);
                    }
                    openFile = false;
                }

                int classChoice;
                int classCount = classList.size();
                bool selectClass = true;

                while(selectClass)
                {
                    cout << "Their are currently " << classCount << " classes's registered in the system." << endl;

                    for(int i = 0; i < classCount; i++)
                        cout << "Class " << i+1 << ": " << classList.at(i)->getClassName() << ", Course Number: " << classList.at(i)->getCourseNum() << ", Teacher: " << classList.at(i)->getClassTeach()->getFullName() << endl;
                    
                    cout << endl;
                    cout << "Which class would you like to add? (-1 to exit): ";
                    cin >> classChoice;

                    if(cin.fail()) 
                    {
                        // clear the error flag
                        cin.clear();  
                        // ignore the invalid input
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input. Please enter a valid number choice." << endl;
                        cout << endl;
                    }

                    if(classChoice == -1)
                    {
                        cout << "Returning to " << studentList.at(studentChoice - 1)->getName() << " menu" << endl;
                        cout << endl;
                        break;
                    }

                    while(classChoice > classCount || classChoice < 1)
                    {
                        cout << "That is an invalid selection. Please select again." << endl; 
                        cout << "Which class would you like to access? ";
                        cin >> classChoice;
                    }
                    
                    int confirmedChoice = 0;
                    cout << "You have chosen class " << classChoice << " which is "  << classList.at(classChoice - 1)->getClassName() << endl;
                    cout << "Please confirm that is correct. (1 for yes or -1 for no): ";
                    cin >> confirmedChoice;

                    while(confirmedChoice != 1 && confirmedChoice != -1)
                    {
                        cout << "That is an invalid selection. Please select again." << endl; 
                        cout << "Please confirm that is correct. (1 for yes or -1 for no): ";
                        cin >> confirmedChoice;
                    }
                    cout << endl;

                    if(confirmedChoice == 1)
                    {
                        studentList.at(studentChoice - 1)->addClass(classList.at(classChoice - 1));

                        int addMore = 0;
                        cout << "Would you like to add add more classes? (1 for yes or -1 for no): ";
                        cin >> addMore;

                        while(addMore != 1 && addMore != -1)
                        {
                            cout << "That is an invalid selection. Please select again." << endl; 
                            cout << "Would you like to add add more classes? (1 for yes or -1 for no): ";
                            cin >> addMore;
                        }

                        cout << endl;

                        if(addMore == -1)
                        {
                            selectClass = false;
                            break;
                        }
                    }
                }
                if(classChoice != -1)
                {
                    cout << studentList.at(studentChoice - 1)->getName() << " is now enrolled in" << endl;
                    cout << endl;
                    studentList.at(studentChoice - 1)->printClasses();
                    cout << endl;
                    
                    int continueProgram = 0;
                    cout << "Would you like to select a new student to view? (1 for yes or -1 for no): ";
                    cin >> continueProgram;

                    while(continueProgram != 1 && continueProgram != -1)
                    {
                        cout << "That is an invalid selection. Please select again." << endl; 
                        cout << "Would you like to select a new student to view? (1 for yes or -1 for no): ";
                        cin >> continueProgram;
                    }

                    cout << endl;

                    if(continueProgram == 1)
                    {
                        selectStudent = true;
                    }
                }

            }
            else if(newChoice == 2)
            {
                cout << studentList.at(studentChoice - 1)->getName() << " is enrolled in" << endl;
                cout << endl;
                studentList.at(studentChoice - 1)->printClasses();
                cout << endl;
            }
            else if(newChoice == 3)
                selectStudent = true;
            else if(newChoice == 3)
                selectStudent = true;
            else if(newChoice == 4)
            {
                selectStudent = true;
                break;
            }
            else
            {
                cout << "Invalid Selection, please select again." << endl;
                cout << endl;
            }

        }
        if(choice == 2)
        {
            if(openFile)
            {
                // string classesFileName = "Classes.txt";
                string classesFileName;
                cout << "Enter the location of the classes.txt file: ";
                cin >> classesFileName;

                ifstream fileIn;
                fileIn.open(classesFileName);

                if (fileIn.fail()) 
                {
                    cerr << "Error opening input file";
                    return -1;
                }
                cout << "File opened successfully" << endl;
                cout << endl;

                
                string className;
                unsigned int coruseNum;

                while (fileIn >> className >> coruseNum)
                {
                    Classes* newClass = new Classes(className, coruseNum);
                    classList.push_back(newClass);
                }
                openFile = false;
            }
            cout << "Curent Teachers and ther Classes:" << endl;
            for (Classes* currClass : classList)
                cout << currClass->getClassName() << " (" << currClass->getCourseNum() << "): " << currClass->getClassTeach()->getFullName() << endl;
            cout << endl;
        }
        else if(choice == 3)
        {
            cout << "Thank you for using the School Management System!" << endl;
            return 0;
        }
    }

    return 0;
}