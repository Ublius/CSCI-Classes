#include <SFML/Graphics.hpp>
using namespace sf;
#include "Star.h"

Star::Star(int windowWidth, int windowHeight, float maxBrightness){
  mWindowWidth = windowWidth;
  mWindowHeight = windowHeight;
  mMaxBrightness = maxBrightness;
  CircleShape::setRadius(2.0);
  CircleShape::setPosition(0.0f ,0.0f);
  setBrightness(maxBrightness/2); 
}

void Star::setPosition(float xPos, float yPos){
  float xPixel;
  float yPixel;
  xPixel = ( xPos + 1.0 ) * mWindowWidth / 2.0;
  yPixel = ( yPos + 1.0 ) * mWindowHeight / 2.0;
  CircleShape::setPosition(xPixel, yPixel);
}

void Star::setBrightness(float brightness){
  float invertedBrightness = 8.0 - brightness;
  int shadeOfGray = (int)( ( 255.0 * brightness ) / invertedBrightness );
  setFillColor( Color(shadeOfGray, shadeOfGray, shadeOfGray) );
}