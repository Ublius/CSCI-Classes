/* CSCI 200: Lab 6B - SFML: Starcraft
 *
 * Author: Tristen Erpelding
 *
 * To read in a star data file, store the data in a list of objects, make modifications to the data, and then draw a star map similar to the one shown
 * 
*/

#include <SFML/Graphics.hpp>
#include "Star.h"

using namespace sf;

Star::Star(int windowWidth, int windowHeight, float maxBrightness)
{
    // Initialize member variables
    mWindowWidth = windowWidth;
    mWindowHeight = windowHeight;
    mMaxBrightness = maxBrightness;
    // Set the initial radius and position of the star
    CircleShape::setRadius(2.0);
    CircleShape::setPosition(0.0f, 0.0f);
        // Set the initial brightness of the star
    setBrightness(maxBrightness/2); 
}

void Star::setPosition(float xPos, float yPos)
{
    // Convert the star's position in the range [-1, 1] to pixel coordinates
    float xPixel;
    float yPixel;
    xPixel = ( xPos + 1.0 ) * mWindowWidth / 2.0;
    yPixel = ( yPos + 1.0 ) * mWindowHeight / 2.0;
    CircleShape::setPosition(xPixel, yPixel);
}

void Star::setBrightness(float brightness)
{
    // Calculate the shade of gray for the star's fill color based on its brightness
    float invertedBrightness = 8.0 - brightness;
    int shadeOfGray = (int)( ( 255.0 * brightness ) / invertedBrightness );
    setFillColor( Color(shadeOfGray, shadeOfGray, shadeOfGray) );
}