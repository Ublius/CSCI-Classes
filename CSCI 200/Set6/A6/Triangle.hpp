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

#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <vector>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Triangle : public ConvexShape 
{
  public:
    /**
     * @brief default constructor for the triangle class
    */
    Triangle();
    /**
     * @brief virtual destructor so it can clean up the class
    */
    virtual ~Triangle();
    /**
     * @brief setColor method to set the color of each triangle based on r g and b values
     * @param R red color code
     * @param G green color code
     * @param B blue color code
    */
    void setColor(const int R, const int G, const int B);
    /**
     * @brief draw method to call the draw shape function to draw in the proper window once we have the correct triangle
     * @param window window object passed by reference so we don't create more then one and it draws to the proper one
    */
    void draw(RenderTarget& window);
    /**
     * @brief virtual setCoordinates for each triangle to determine it's position
     * @param x1 x coordinate of first vertex
     * @param y1 y coordinate of first vertex
     * @param x2 x coordinate of second vertex
     * @param y2 y coordinate of second vertex
     * @param x3 x coordinate of third vertex
     * @param y3 y coordinate of third vertex
     * @return boolean indicating if the triangle is valid or not
    */
    virtual bool setCoordinates(double x1, double y1, double x2, double y2, double x3, double y3) = 0;

  protected:
    /**
     * @brief numVertices for the convexshape class
    */
    int numVertices;
    /**
     * @brief vertice list to use in the class and store the x and y values
    */
    vector<float> verticeList;
    
  private:
    /**
     * @brief private color object created to use later
    */
    Color mColor;
}; 

Triangle::Triangle()
{
    // initializing number of vertices
    numVertices = 3;
}

Triangle::~Triangle()
{
    return;
}

void Triangle::setColor(const int R, const int G, const int B)
{
    // Color objects r g and b values
    mColor.r = R;
    mColor.g = G;
    mColor.b = B;
}

void Triangle::draw(RenderTarget& window)
{
    // Created a convexshape and draws each triangle based on the vertice list
    ConvexShape firstTriangle;
    firstTriangle.setPointCount(numVertices);
    firstTriangle.setPoint(0, Vector2f(verticeList.at(0),verticeList.at(1)));
    firstTriangle.setPoint(1, Vector2f(verticeList.at(2),verticeList.at(3)));
    firstTriangle.setPoint(2, Vector2f(verticeList.at(4),verticeList.at(5)));
    firstTriangle.setFillColor(mColor);
    window.draw(firstTriangle);
}

#endif