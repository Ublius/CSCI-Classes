draw_set_font(fnt_default)
draw_set_color(c_white)
draw_text(30, 30, "SCORE: " + string(score))

if (instance_exists(obj_player)) {
    var xhp = 30
    repeat(obj_player.hp) {
        draw_sprite(spr_life, 0, xhp, 750)
        xhp += 30
    }
}