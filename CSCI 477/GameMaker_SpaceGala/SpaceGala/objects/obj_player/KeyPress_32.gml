var bullet = instance_create_layer(x, y, "Instances", obj_bullet_player)

bullet.direction = point_direction(x, y, x, y-1)
bullet.speed = bullet.spd

audio_play_sound(snd_shoot, 0, false)