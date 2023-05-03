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

#ifndef SCALENETRIANGLE_HPP
#define SCALENETRIANGLE_HPP

#include <vector>
#include <cmath>

#include "Triangle.hpp"
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class ScaleneTriangle: public Triangle {
  public:
    /**
     * @brief implemented setCoordinates for each triangle to determine it's position and if it is valid or not
     * @param x1 x coordinate of first vertex
     * @param y1 y coordinate of first vertex
     * @param x2 x coordinate of second vertex
     * @param y2 y coordinate of second vertex
     * @param x3 x coordinate of third vertex
     * @param y3 y coordinate of third vertex
     * @return boolean indicating if the scalene triangle is valid or not
    */
    bool setCoordinates(double x1, double y1, double x2, double y2, double x3, double y3);
};

bool ScaleneTriangle::setCoordinates(double x1, double y1, double x2, double y2, double x3, double y3)
{
    // Calculate the distances between vertices using the Pythagorean theorem    
    float d1 = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
    float d2 = sqrt((x3-x2)*(x3-x2) + (y3-y2)*(y3-y2));
    float d3 = sqrt((x1-x3)*(x1-x3) + (y1-y3)*(y1-y3));

    // Check if the distances are positive and satisfy the triangle inequality
    if(d1>0 && d2>0 && d3>0 && (d1+d2 > d3 || d2+d3 > d1 || d1+d3 > d2))
    {
        // Add the valid vertices to the verticeList vector
        verticeList.push_back(x1);
        verticeList.push_back(y1);
        verticeList.push_back(x2);
        verticeList.push_back(y2);
        verticeList.push_back(x3);
        verticeList.push_back(y3);
        return true;
    }
    else
        return false;
}

#endif