/* CSCI 200: Lab 2: L2B - Color Space Converter
 *
 * Author: Tristen Erpelding
 * Resources used (Office Hours, Tutoring, Other Students, etc & in what capacity):
 *     Looked up static cast to convert the RGB values to int, I also looked up the formulas
 *     on how to convert RGB to HSV and vice versa
 * 
 * Prompt the user to input the values on the left hand side and then call the corresponding function to compute the right hand side. 
 * Display these resultant values to the user. These values will either be in RGB format or HSV format.
 */

#include <iostream>
#include "color_conversion.h"

using namespace std;

int main()
{
    int choice = 0;
    cout << "Please chose to either:\n1. Convert RGB to HSV\n2. Convert HSV to RGB" << endl;
    cout << "Choice (1-2): ";
    cin >> choice;


    if (choice == 1)
    {
        int r(0),g(0),b(0);
        double hue, saturation, value;

        cout << "Red: ";
        cin >> r;
        cout << "Green: ";
        cin >> g;
        cout << "Blue: ";
        cin >> b;

        rgb_to_hsv(r,g,b,hue,saturation,value);
        cout << "Hue: " << hue << " Saturation: " << saturation << " Value: " << value << endl;
    }

    else if (choice == 2)
    {
        double h(0),s(0),v(0);
        int r, g, b;

        cout << "Hue: ";
        cin >> h;
        cout << "Saturation: ";
        cin >> s;
        cout << "Value: ";
        cin >> v;

        hsv_to_rgb(h,s,v,r,g,b);
        cout << "Red: " << r << " Green: " << g << " Blue: " << b << endl;
    }

    return 1;
}