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

#ifndef COLOR_CONVERSION_H
#define COLOR_CONVERSION_H

void rgb_to_hsv( const int r, const int g, const int b, 
                double &h, double &s, double &v );

void hsv_to_rgb(const double h, const double s, const double v, 
                int &r, int &g, int &b);

#endif //COLOR_CONVERSION_H