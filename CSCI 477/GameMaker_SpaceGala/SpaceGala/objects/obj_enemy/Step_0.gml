if (hp <= 0) {
    score += 1
    instance_destroy()
    audio_play_sound(snd_damage, 0, false)
}


