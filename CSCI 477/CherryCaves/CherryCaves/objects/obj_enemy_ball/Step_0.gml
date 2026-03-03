vsp += grv

if (place_meeting(x, y + vsp, obj_block)) {
    vsp = bounce_power
}

y += vsp

if (place_meeting(x, y, obj_player)) {
        obj_player.player_hit(atk)
    
    instance_destroy(id, true)
    audio_play_sound(snd_squash, 1, false)
}