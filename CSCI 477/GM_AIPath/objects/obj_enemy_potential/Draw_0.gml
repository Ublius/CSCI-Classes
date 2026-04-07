draw_self()

var length = 30
var length_x = lengthdir_x(length, direction)
var length_y = lengthdir_y(length, direction)

draw_line_width_colour(x, y, x + length_x, y + length_y, 3, c_red, c_red)