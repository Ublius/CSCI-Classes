var camera = view_camera[0];

var camera_x = camera_get_view_x(camera)
var camera_y = camera_get_view_y(camera)
var camera_w = camera_get_view_width(camera)
var camera_h = camera_get_view_height(camera)

if(y + (sprite_height / 2) >= camera_y + camera_h) instance_destroy(id, false); 
if(y - (sprite_height / 2) <= camera_y) instance_destroy(id, false);

if (y < 0 + sprite_width / 2) {
    instance_destroy(id, false)
}