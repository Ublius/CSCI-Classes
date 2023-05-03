/* CSCI 200: Lab 6B - SFML: Starcraft
 *
 * Author: Tristen Erpelding
 *
 * To read in a star data file, store the data in a list of objects, make modifications to the data, and then draw a star map similar to the one shown
 * 
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include <SFML/Graphics.hpp>
#include "Star.h"

using namespace sf;
using namespace std;

const int WIDTH = 640;
const int HEIGHT = 640;

int main(int argc, char* argv[]) {
    // create a window
    RenderWindow window( VideoMode(WIDTH, HEIGHT), "Star Map" );

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

    // Initializing variables for file reading
    vector<Star> starList;
    float xPos, yPos, zPos, sBrightness;
    int draperValue, harvardValue;

    // Loop through file while there are still values to read
    while(fileIn >> xPos >> yPos >> zPos >> sBrightness >> draperValue >> harvardValue)
    {
        // Create a new Star object with default window size and set its position and brightness
        Star tempStar(WIDTH, HEIGHT, sBrightness);
        tempStar.setPosition(xPos, yPos);
        tempStar.setBrightness(sBrightness);
        // Add the star to the starList if it has a valid brightness value
        if(sBrightness >= 0.0 && sBrightness <= 8.0)
            starList.push_back(tempStar);
    }

    //  END  ANY FILE LOADING
    /////////////////////////////////////

    // create an event object once to store future events
    Event event;

    // while the window is open
    while( window.isOpen() ) 
    {
        // clear any existing contents
        window.clear();

        /////////////////////////////////////
        // BEGIN DRAWING HERE

        // Loop to draw the stars in SFML
        for(size_t i = 0; i < starList.size(); i++)
            window.draw(starList.at(i));

        //  END  DRAWING HERE
        /////////////////////////////////////

        // display the current contents of the window
        window.display();

        /////////////////////////////////////
        // BEGIN EVENT HANDLING HERE
        // check if any events happened since the last time checked
        while( window.pollEvent(event) ) {
            // if event type corresponds to pressing window X
            if(event.type == Event::Closed) {
                // tell the window to close
                window.close();
            }
            // check addition event types to handle additional events
        }
        //  END  EVENT HANDLING HERE
        /////////////////////////////////////
    }

    return 0;
}