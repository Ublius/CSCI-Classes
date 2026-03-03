// Input
var keyleft = keyboard_check(vk_left)
var keyright = keyboard_check(vk_right)
var move = keyright - keyleft
var jumping = keyboard_check(vk_space)
var keyup = keyboard_check(vk_up)
var keydown = keyboard_check(vk_down)
var vmove = keydown - keyup

// Check movement
if (move != 0) {
    sprite_index = spr_player_walk
    image_xscale = move
} else {
    sprite_index = spr_player_idle
}
hsp = spd * move

// Horiz Collision
if (place_meeting(x + hsp, y, obj_block)) {
    while (!place_meeting(x + sign(hsp), y, obj_block)) {
        x += sign(hsp)
    }
    hsp = 0
}

// Check for jump
if (grounded && jumping ) {
    vsp = -jspd
    grounded = false
    sprite_index = spr_player_idle
    audio_play_sound(snd_jump, 1, false)
}

// Vert Collision
vsp += grv
if(place_meeting(x, y + vsp, obj_block)) {
	while (!place_meeting(x, y + sign(vsp), obj_block)) {
		y += sign(vsp)
	}
	vsp = 0
    grounded = true
} else {
    grounded = false
}

// Climb
if(place_meeting(x, y + 1, obj_ladder)) {
    if (vmove < 0 || (vmove == 0 && climbing) || (vmove > 0 && place_meeting(x, y + sprite_height, obj_ladder))) {
        climbing = true
    } else {
        climbing = false
    }
} else {
    climbing = false
}

if (climbing) {
    sprite_index = spr_player_climb
    vsp = vmove * spd
}

// Movement
x += hsp
y += vsp

if (y > room_height || x > room_width || x < 0) {
    global.hp -= 1
    
    if (global.hp > 0) {
        x = respawn_x
        y = respawn_y
        hsp = 0
        vsp = 0
        audio_play_sound(snd_damage, 1, false)
    } else {
        obj_game_controller.game_over = true
    }
}

if (global.hp <= 0) {
    instance_destroy(id, true)
    obj_game_controller.game_over = true
}