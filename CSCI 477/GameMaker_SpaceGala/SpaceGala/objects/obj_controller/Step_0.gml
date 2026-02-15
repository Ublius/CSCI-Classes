if (instance_exists(obj_player)) {
    if (obj_player.hp <= 0) {
        game_over = true
    }
}

if (instance_exists(obj_enemy)) {
    if (obj_enemy.y >= (room_height - sprite_width / 2)) game_over = true
}

if (!instance_exists(obj_enemy) and instance_exists(obj_player) && !level_won) {
    level_won = true
    alarm[2] = room_speed * 2;
}