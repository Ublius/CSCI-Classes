#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include <SFML/Graphics.hpp>
#include "Star.h"

using namespace sf;
using namespace std;

int main(int argc, char* argv[]) {
    // create a window
    RenderWindow window( VideoMode(640, 640), "SFML Test" );

    /////////////////////////////////////
    // BEGIN ANY FILE LOADING
    string filename;
    if(argc == 1) {
        cout << "Please enter the name for your file here: ";
        getline(cin, filename);
        cout << "Opening file '" << filename << "'" << endl;
    } else if(argc == 2) {
        filename = argv[1];
        cout << "Opening file '" << filename << "'" << endl;
    } else {
        cerr << "Usage: " << argv[0] << " [filename]" << endl;
        cerr << "  filename   - optional file to open upon start" << endl;
        return -2;
    }

    // open file for parsing
    ifstream fileIn;
    fileIn.open(filename);
    if (fileIn.fail()) {
        cerr << "Error opening input file";
        return -1;
    }
    cout << "File opened successfully" << endl;


    // Initializing variables for file reading
    vector<Star> starList;
    float xPos;
    float yPos;
    float zPos;
    float sBrightness;
    int draperValue;
    int harvardValue;

    // While true loop to help read the file and set all coordinates needed
    while(true){
        fileIn >> xPos;
        fileIn >> yPos;
        fileIn >> zPos;
        fileIn >> sBrightness;
        fileIn >> draperValue;
        fileIn >> harvardValue;
        Star tempStar(640, 640, sBrightness);
        tempStar.setPosition(xPos, yPos);
        tempStar.setBrightness(sBrightness);
        if(sBrightness>= 0.0 && sBrightness <= 8.0){starList.push_back(tempStar);}
        if( fileIn.eof() ) break;
    }

    //  END  ANY FILE LOADING
    /////////////////////////////////////

    // create an event object once to store future events
    Event event;

    // while the window is open
    while( window.isOpen() ) {
        // clear any existing contents
        window.clear();

        /////////////////////////////////////
        // BEGIN DRAWING HERE

        // Loop to draw the stars in SFML
        for(size_t i = 0; i < starList.size(); i++){
            window.draw(starList.at(i));
        }

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