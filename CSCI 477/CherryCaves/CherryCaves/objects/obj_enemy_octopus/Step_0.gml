if (state == "dead") {
    instance_destroy(id, true)
    exit
}

if (state == "falling") {
    vsp += grv
}

var block_collision = false

if (place_meeting(x, y + vsp, obj_block)) {
    block_collision = true
    
    while (!place_meeting(x, y + sign(vsp), obj_block)) {
        y += sign(vsp)
    }
    
    if (state == "falling" && vsp > 0) {
        vsp = 0
        state = "stuck_ground"
        alarm[0] = hang_time
    }
    else if (state == "rising" && vsp < 0) {
        vsp = 0
        state = "hanging"
        alarm[0] = hang_time
    }
    else if (state == "falling" && vsp < 0) {
        vsp = 0
        state = "stuck_ceiling"
        alarm[0] = hang_time
    }
}

y += vsp

switch (state) {
    case "stuck_ground":
        image_index = 0 
        image_yscale = 1
        break
        
    case "hanging":
        image_index = 0
        image_yscale = -1
        break
    case "stuck_ceiling":
        image_index = 0
        break
    case "rising":
    case "falling":
    default:
        image_index = 1
        break
}



var player = instance_place(x, y, obj_player)
if (player != noone && !just_hit_player) {
    if (player.bbox_bottom <= bbox_top + 4) {
        state = "dead"
        
        with (player) {
            vsp = -6
        }
        
        audio_play_sound(snd_squash, 1, false)
    } else {
        obj_player.player_hit(atk)
        just_hit_player = true
    }
}

else if (player == noone) {
    just_hit_player = false
}