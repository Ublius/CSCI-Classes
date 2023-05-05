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
        cout << "Please enter the name of your students file here: ";
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

    // Declare a vector to hold pointers to Student objects
    vector<Student*> studentList;
    // Declare variables to hold information about each student
    string firstName, lastName;
    int currYear, gradYear;

    // Read in student information from a file and create new Student objects, storing them in the vector
    while (fileIn >> firstName >> lastName >> currYear >> gradYear)
    {
        Student* newStudent = new Student(firstName, lastName, gradYear, currYear);
        studentList.push_back(newStudent);
    }

    int studentChoice;
    // Get the number of students in the vector
    int studentCount = studentList.size();
    // Declare a vector to hold pointers to Classes objects
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
                // Outputting the current number of students registered in the system and their names
                cout << "Their are currently " << studentCount << " student's registered in the system." << endl;
                
                // Iterating through the student list and printing the names of each student along with their index
                for(int i = 0; i < studentCount; i++)
                    cout << "Student " << i+1 << ": " << studentList.at(i)->getName() << endl;
                
                // Prompting the user to input the index of the student they want to access.
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

                // Validate user input for student selection
                while(studentChoice > studentCount || studentChoice < 1)
                {
                    cout << "That is an invalid selection. Please select again." << endl; 
                    cout << "Which student would you like to access? ";
                    cin >> studentChoice;
                }

                // Confirm user selection
                int confirmedChoice = 0;
                cout << "You have chosen student " << studentChoice << " who is "  << studentList.at(studentChoice - 1)->getName() << endl;
                cout << "Please confirm that is correct. (1 for yes or -1 for no): ";
                cin >> confirmedChoice;

                // Validate user confirmation input
                while(confirmedChoice != 1 && confirmedChoice != -1)
                {
                    cout << "That is an invalid selection. Please select again." << endl; 
                    cout << "Please confirm that is correct. (1 for yes or -1 for no): ";
                    cin >> confirmedChoice;
                }
                cout << endl;

                // Break loop if user confirmed selection
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
                // Check if the file is already open
                if(openFile)
                {
                    // string classesFileName = "Classes.txt";
                    // Prompt user to enter the location of the classes.txt file
                    string classesFileName;
                    cout << "Enter the location of the classes.txt file: ";
                    cin >> classesFileName;

                    // Create an input file stream and open the file
                    ifstream fileIn;
                    fileIn.open(classesFileName);

                    // Check if file opening was successful
                    if (fileIn.fail()) 
                    {
                        // Print error message and exit program
                        cerr << "Error opening input file";
                        return -1;
                    }
                    // Print success message and newline character
                    cout << "File opened successfully" << endl;
                    cout << endl;

                    
                    string className;
                    unsigned int coruseNum;
                    // Read class name and course number from the file, 
                    while (fileIn >> className >> coruseNum)
                    {
                        // Create a new Class object and add it to the classList vector
                        Classes* newClass = new Classes(className, coruseNum);
                        classList.push_back(newClass);
                    }
                    // Set openFile to false to prevent the file from being opened again
                    openFile = false;
                }

                int classChoice;
                int classCount = classList.size();
                bool selectClass = true;

                while(selectClass)
                {
                    // Output the number of classes in the system and list each class with its name, course number, and teacher's name
                    cout << "Their are currently " << classCount << " classes's registered in the system." << endl;

                    for(int i = 0; i < classCount; i++)
                        cout << "Class " << i+1 << ": " << classList.at(i)->getClassName() << ", Course Number: " << classList.at(i)->getCourseNum() << ", Teacher: " << classList.at(i)->getClassTeach()->getFullName() << endl;
                    
                    cout << endl;

                    // Prompt the user to select a class to add and handle any invalid input
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

                    // Handle the case where the user chooses to exit the add class menu
                    if(classChoice == -1)
                    {
                        cout << "Returning to " << studentList.at(studentChoice - 1)->getName() << " menu" << endl;
                        cout << endl;
                        break;
                    }

                    // Ensure that the user selects a valid class to add
                    while(classChoice > classCount || classChoice < 1)
                    {
                        cout << "That is an invalid selection. Please select again." << endl; 
                        cout << "Which class would you like to access? ";
                        cin >> classChoice;
                    }
                    
                    // Prompt user to confirm their selected class choice
                    int confirmedChoice = 0;
                    cout << "You have chosen class " << classChoice << " which is "  << classList.at(classChoice - 1)->getClassName() << endl;
                    cout << "Please confirm that is correct. (1 for yes or -1 for no): ";
                    cin >> confirmedChoice;

                     // Validate user input for class confirmation
                    while(confirmedChoice != 1 && confirmedChoice != -1)
                    {
                        cout << "That is an invalid selection. Please select again." << endl; 
                        cout << "Please confirm that is correct. (1 for yes or -1 for no): ";
                        cin >> confirmedChoice;
                    }
                    cout << endl;

                    // If user confirms their class choice, add the class to the student's list
                    if(confirmedChoice == 1)
                    {
                        studentList.at(studentChoice - 1)->addClass(classList.at(classChoice - 1));

                        // Prompt user if they want to add more classes
                        int addMore = 0;
                        cout << "Would you like to add add more classes? (1 for yes or -1 for no): ";
                        cin >> addMore;

                        // Validate user input for adding more classes
                        while(addMore != 1 && addMore != -1)
                        {
                            cout << "That is an invalid selection. Please select again." << endl; 
                            cout << "Would you like to add add more classes? (1 for yes or -1 for no): ";
                            cin >> addMore;
                        }

                        cout << endl;

                        // If user chooses to not add more classes, exit the loop
                        if(addMore == -1)
                        {
                            selectClass = false;
                            break;
                        }
                    }
                }
                // Check if chose to not add more classes
                if(classChoice != -1)
                {
                    // Print the current students classes
                    cout << studentList.at(studentChoice - 1)->getName() << " is now enrolled in" << endl;
                    cout << endl;
                    studentList.at(studentChoice - 1)->printClasses();
                    cout << endl;
                    
                    int continueProgram = 0;
                    cout << "Would you like to select a new student to view? (1 for yes or -1 for no): ";
                    cin >> continueProgram;

                    // Validate user input for continuing the program
                    while(continueProgram != 1 && continueProgram != -1)
                    {
                        cout << "That is an invalid selection. Please select again." << endl; 
                        cout << "Would you like to select a new student to view? (1 for yes or -1 for no): ";
                        cin >> continueProgram;
                    }

                    cout << endl;

                    // If user chooses to continue, set selectStudent to true to allow selecting a new student
                    if(continueProgram == 1)
                    {
                        selectStudent = true;
                    }
                }

            }
            // If the user chose to view the student's classes, display the student's current classes
            else if(newChoice == 2)
            {
                cout << studentList.at(studentChoice - 1)->getName() << " is enrolled in" << endl;
                cout << endl;
                studentList.at(studentChoice - 1)->printClasses();
                cout << endl;
            }
            // If the user chose to go back to selecting a student, set selectStudent to true
            else if(newChoice == 3)
                selectStudent = true;
            // If the user chose to quit the current student, set selectStudent to true and break out of the loop
            else if(newChoice == 4)
            {
                selectStudent = true;
                break;
            }
            // If the user made an invalid selection, display an error message and prompt to select again.
            else
            {
                cout << "Invalid Selection, please select again." << endl;
                cout << endl;
            }

        }
        if(choice == 2)
        {
            // Check if the file is already open
            if(openFile)
            {
                // string classesFileName = "Classes.txt";
                // Prompt user to enter the location of the classes.txt file
                string classesFileName;
                cout << "Enter the location of the classes.txt file: ";
                cin >> classesFileName;

                // Create an input file stream and open the file
                ifstream fileIn;
                fileIn.open(classesFileName);

                // Check if file opening was successful
                if (fileIn.fail()) 
                {
                    // Print error message and exit program
                    cerr << "Error opening input file";
                    return -1;
                }
                // Print success message and newline character
                cout << "File opened successfully" << endl;
                cout << endl;

                
                string className;
                unsigned int coruseNum;
                // Read class name and course number from the file, 
                while (fileIn >> className >> coruseNum)
                {
                    // Create a new Class object and add it to the classList vector
                    Classes* newClass = new Classes(className, coruseNum);
                    classList.push_back(newClass);
                }
                // Set openFile to false to prevent the file from being opened again
                openFile = false;
            }
            // Print the curent classes and there associated teachers
            cout << "Curent Teachers and ther Classes:" << endl;
            for (Classes* currClass : classList)
                cout << currClass->getClassName() << " (" << currClass->getCourseNum() << "): " << currClass->getClassTeach()->getFullName() << endl;
            cout << endl;
        }
        // User chose to quit the program
        else if(choice == 3)
        {
            cout << "Thank you for using the School Management System!" << endl;
            return 0;
        }
    }

    return 0;
}