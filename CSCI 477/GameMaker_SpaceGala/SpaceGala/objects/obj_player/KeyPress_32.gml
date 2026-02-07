var bullet = instance_create_layer(x, y, "Instances", obj_bullet_player)

bullet.direction = point_direction(x, y, x, y-1)
bullet.speed = bullet.spd