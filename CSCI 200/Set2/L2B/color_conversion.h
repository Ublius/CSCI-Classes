#ifndef COLOR_CONVERSION_H
#define COLOR_CONVERSION_H

void rgb_to_hsv( const int r, const int g, const int b, 
                double &h, double &s, double &v );

void hsv_to_rgb(const double h, const double s, const double v, 
                int &r, int &g, int &b);

#endif //COLOR_CONVERSION_H