# Tristen Erpelding
# CSCI 102 – F Sections
# Assessment 11
# References:
# Time: 30 minutes

import math

def print_output(thingToPrint):
    return print(f"OUTPUT {thingToPrint}")

def cylinder_vol(radius, height):
    vol = height * 3.1415 * (radius ** 2)
    vol = str(f"{vol:.2f}")
    print_output(f"{vol}")
    return vol

def lbs_to_kg(pounds):
    kilo = pounds * 0.4536
    kilo = str(f"{kilo:.4f}")
    print_output(f"{kilo}")
    return kilo

def polar_coords(x, y):
    r = ((x **2) + (y **2)) ** (1/2)
    theta = math.degrees(math.atan(y/x))
    r = str(f"{r:.2f}")
    theta = str(f"{theta:.2f}")
    print_output(f"r: {r}")
    print_output(f"theta: {theta}")

def yen_to_dollars(yen):
    dollar = yen * 0.0068
    dollar = str(f"{dollar:.2f}")
    print_output(f"{dollar}")
    return dollar

def quad_form(a, b, c):
    dis = b * b - 4 * a * c 
    sqrt_val = (abs(dis)) ** (1/2)
    val1 = ((-b + sqrt_val)/(2*a))
    val2 = ((-b - sqrt_val)/(2*a))
    if val1 > val2:
        print_output(f"{val2:.0f}")
        print_output(f"{val1:.0f}")
    else:
        print_output(f"{val1:.0f}")
        print_output(f"{val2:.0f}")

