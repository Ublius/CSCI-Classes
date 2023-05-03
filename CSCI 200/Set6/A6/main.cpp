/* CSCI 200: Assessment 6 - SFML: Triangle Land
 *
 * Author: Tristen Erpelding
 *
 * Write a program to read in a list of triangle data and draw an image comprised of those triangles. 
 * Creating an abstract Triangle class
 * Create a class called ScaleneTriangle that extends Triangle
 * Create a class called IsoscelesTriangle that extends Triangle
 * Create a class called EquilateralTriangle that extends Triangle
*/

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "Triangle.hpp"
#include "ScaleneTriangle.hpp"
#include "EquilateralTriangle.hpp"
#include "IsoscelesTriangle.hpp" 

using namespace sf;
using namespace std;

int main(int argc, char* argv[]) 
{

    /////////////////////////////////////
    // BEGIN ANY FILE LOADING
    
    string filename;
    if(argc == 1)
     {
        cout << "Please enter the name for your file here: ";
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
    
    // Defines a vector of Triangle pointers to store Triangle objects
    vector<Triangle*> TriangleList;
    // Declare variables to hold input data for each triangle
    char triangleType;
    double x1, y1, x2, y2, x3, y3;
    int r, g, b;

    // Reads triangle data from an input file and creates Triangle objects of the appropriate type
    while (fileIn >> triangleType >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> r >> g >> b) 
    {
        // Create a base Triangle pointer and set it to null
        Triangle* pBaseTriangle = nullptr;
        // Set a bool to track if the triangle is valid or not
        bool validTriangle = true;

        // Switch statement to determine the type of triangle and create the appropriate object
        switch (triangleType) 
        {
            // Create a ScaleneTriangle object and set the base pointer to it
            case 'S':
                {
                    ScaleneTriangle* sTriangle = new ScaleneTriangle;
                    pBaseTriangle = sTriangle;
                    // Set the coordinates of the triangle and check if they are valid
                    validTriangle = pBaseTriangle->setCoordinates(x1, y1, x2, y2, x3, y3);
                    break;
                }
            // Create an IsoscelesTriangle object and set the base pointer to it
            case 'I':
                {
                    IsoscelesTriangle* iTriangle = new IsoscelesTriangle;
                    pBaseTriangle = iTriangle;
                    // Set the coordinates of the triangle and check if they are valid.
                    validTriangle = pBaseTriangle->setCoordinates(x1, y1, x2, y2, x3, y3);
                    break;
                }
            // Create an EquilateralTriangle object and set the base pointer to it
            case 'E':
                {
                    EquilateralTriangle* eTriangle = new EquilateralTriangle;
                    pBaseTriangle = eTriangle;
                    // Set the coordinates of the triangle and check if they are valid
                    validTriangle = pBaseTriangle->setCoordinates(x1, y1, x2, y2, x3, y3);
                    break;
                }
            // If the triangle type is invalid, set the bool to false
            default:
                validTriangle = false;
                break;
        }
        // If the triangle is valid, set its color and add it to the vector
        if (validTriangle) 
        {
            pBaseTriangle->setColor(r, g, b);
            TriangleList.push_back(pBaseTriangle);
        } 
        // If the triangle is invalid, print an error message and delete the object
        else 
        {
            cout << "triangle is invalid - " << triangleType << " " << x1 << " " <<  y1 << " " << x2 << " " <<  y2 << " " <<  x3 << " " << y3 << " " << r << " " << g << " " << b << endl;
            delete pBaseTriangle;
        }
    }
    
    //  END  ANY FILE LOADING
    /////////////////////////////////////
    
    // create a window
    RenderWindow window( VideoMode(640, 640), "SFML Test" );

    // create an event object once to store future events
    Event event;

    // while the window is open
    while( window.isOpen() ) {
        // clear any existing contents
        window.clear();

        /////////////////////////////////////
        // BEGIN DRAWING HERE

        // Drawing all triangles
        for(size_t i = 0; i < TriangleList.size(); i++)
            TriangleList.at(i)->draw(window);

        //  END  DRAWING HERE
        /////////////////////////////////////


        // display the current contents of the window
        window.display();

        /////////////////////////////////////
        // BEGIN EVENT HANDLING HERE

        // check if any events happened since the last time checked
        while( window.pollEvent(event) ) 
        {
            // if event type corresponds to pressing window X
            if(event.type == Event::Closed) 
                // tell the window to close
                window.close();
            // check addition event types to handle additional events
        }

        //  END  EVENT HANDLING HERE
        /////////////////////////////////////
    }

    return 0;
}