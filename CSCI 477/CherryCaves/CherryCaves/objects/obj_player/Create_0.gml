spd = 4
jspd = 12
hsp = 0
vsp = 0
grv = 0.5
grounded = false
climbing = false

respawn_x = x
respawn_y = y

function player_hit(atk) {
    global.hp -= 1
}