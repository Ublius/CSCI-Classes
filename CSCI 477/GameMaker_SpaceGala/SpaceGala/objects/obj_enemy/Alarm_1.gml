var bullet = instance_create_layer(x, y, "Instances", obj_bullet_enemy)
var player_y = obj_player.y
var player_x = obj_player.x

bullet.direction = point_direction(x, y, player_x, player_y)
bullet.speed = bullet.spd

alarm[1] = shoot_speed