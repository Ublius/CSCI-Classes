#include "color_conversion.h"

#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

/**
* @brief converts RGB values to HSV
* @param r = amount of red, g = amount of green, b = amount of blue
* @return hue, saturation, value
*/
void rgb_to_hsv( const int r, const int g, const int b, double &h, double &s, double &v )
{
    // Convert r,g,b to a value between 0-1 
    double red = r / 255.0;
    double green = g / 255.0;
    double blue = b / 255.0;

    // Get the max color nad min color
    double cMin = min(min(red, green), blue);
    double cMax = max(max(red, green), blue);
    double delta = cMax - cMin; // Difference between the max and the min

    // Value is equal to the max
    v = cMax;

    // if 0 change then h and s are 0
    if (delta == 0) 
    {
        h = 0;
        s = 0;
    } 
    else 
    {
        s = delta / cMax; // s is delta divided by the max
        double hue;

        if (red == cMax) 
            hue = (green - blue) / delta; // conversion if red is the max
        else if (green == cMax) 
            hue = 2 + (blue - red) / delta; // conversion if green is the max
        else 
            hue = 4 + (red - green) / delta; // conversion if blue is the max

        hue *= 60;

        if (hue < 0) 
            hue += 360; // adjust for the 360 degrees 

        h = hue;
    }
}


/**
* @brief converts HSV values to RGB
* @param h = amount of hue, s = amount of saturation, v = the value
* @return red, green, blue
*/
void hsv_to_rgb(const double h, const double s, const double v, int &r, int &g, int &b)
{
    double c = v * s; 
    double x = c * (1 - abs(fmod(h / 60.0, 2) - 1));
    double m = v - c;

    if (h >= 0 && h < 60) // check if hue is between 0 and 60 degrees
    { 
        r = static_cast<int>((c + m) * 255.0 + 0.5);
        g = static_cast<int>((x + m) * 255.0 + 0.5);
        b = static_cast<int>(m * 255.0 + 0.5);
    } 
    else if (h >= 60 && h < 120) // check if hue is between 60 and 120 degrees
    {
        r = static_cast<int>((x + m) * 255.0 + 0.5);
        g = static_cast<int>((c + m) * 255.0 + 0.5);
        b = static_cast<int>(m * 255.0 + 0.5);
    } 
    else if (h >= 120 && h < 180) // check if hue is between 120 and 1800 degrees
    {
        r = static_cast<int>(m * 255.0 + 0.5);
        g = static_cast<int>((c + m) * 255.0 + 0.5);
        b = static_cast<int>((x + m) * 255.0 + 0.5);
    } 
    else if (h >= 180 && h < 240) // check if hue is between 180 and 240 degrees
    {
        r = static_cast<int>(m * 255.0 + 0.5);
        g = static_cast<int>((x + m) * 255.0 + 0.5);
        b = static_cast<int>((c + m) * 255.0 + 0.5);
    } 
    else if (h >= 240 && h < 300) // check if hue is between 240 and 300 degrees
    {
        r = static_cast<int>((x + m) * 255.0 + 0.5);
        g = static_cast<int>(m * 255.0 + 0.5);
        b = static_cast<int>((c + m) * 255.0 + 0.5);
    } 
    else if (h >= 300 && h < 360) // check if hue is between 300 and 360 degrees
    {
        r = static_cast<int>((c + m) * 255.0 + 0.5);
        g = static_cast<int>(m * 255.0 + 0.5);
        b = static_cast<int>((x + m) * 255.0 + 0.5);
    }

    r = static_cast<int>(r + 0.5);
    g = static_cast<int>(g + 0.5);
    b = static_cast<int>(b + 0.5);
}