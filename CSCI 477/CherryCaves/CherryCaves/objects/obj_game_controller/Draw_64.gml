if room != rm_titlescreen {
    draw_set_font(fnt_default)
    draw_set_color(c_white)
    
    draw_sprite(spr_player_idle, 0, 30, 40)
    draw_text(60, 15, string(global.hp))
    
    draw_sprite(spr_cherry, 0, 100, 5)
    draw_text(175, 15, string(global.cherries))
}

if (room != rm_titlescreen) and (game_over) {
    draw_set_font(fnt_game_over)
    draw_set_colour(c_red)
    
    var view_center_x = room_width / 2
    var view_center_y = room_height / 2
    
    draw_set_halign(fa_center)
    draw_set_valign(fa_middle)
    draw_text(view_center_x, view_center_y, "GAME OVER!")

    instance_deactivate_all(true)
}