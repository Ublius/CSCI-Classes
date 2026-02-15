if (!instance_exists(obj_player)) exit;

var cam = view_camera[0];

var cam_top = camera_get_view_y(cam);
var cam_bottom = cam_top + camera_get_view_height(cam);

if (obj_player.y > y && abs(obj_player.x - x) < 768)
{
    var bullet = instance_create_layer(x, y, "Instances", obj_bullet_enemy);
    bullet.direction = point_direction(x, y, obj_player.x, obj_player.y);
    bullet.speed = bullet.spd;

    alarm[1] = shoot_speed;
}
