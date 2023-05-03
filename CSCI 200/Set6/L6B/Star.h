#ifndef STAR_H
#define STAR_H
#include <SFML/Graphics.hpp>
using namespace sf;

class Star : public CircleShape {
  public:
    /**
    * @brief Star overloaded constructor implementation to set everything needed
    * @param windowWidth window width passed to normalize coordinates
    * @param windowHeight window height passed to normalize coordinates
    * @param maxBrightness passed to keep track and normalize brightness
    */
    Star(int windowWidth, int windowHeight, float maxBrightness);
    /**
    * @brief setPosition function to call the circleshape position function once we normalize the x and y value coordinates
    * @param xPos x coordinate for the star
    * @param y coordinate for the star
    */
    void setPosition(float xPos, float yPos);
    /**
    * @brief setBrightness function to determine the shade of grey and brightness of the star on the map
    * @param brightness brightness value for the star to normalize
    */
    void setBrightness(float brightness);
  private:
    /**
    * @brief private window width value to keep track of
    */
    int mWindowWidth;
    /**
    * @brief private window height value to keep track of
    */
    int mWindowHeight;
    /**
    * @brief private brightness value to keep track of
    */
    float mMaxBrightness;
}; 

#endif