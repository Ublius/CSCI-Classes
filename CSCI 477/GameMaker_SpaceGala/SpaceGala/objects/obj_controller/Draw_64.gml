draw_set_font(fnt_default)
draw_set_color(c_white)
draw_text(30, 30, "SCORE: " + string(score))

var camera_w = camera_get_view_width(camera)
var camera_h = camera_get_view_height(camera)

if (instance_exists(obj_player)) {
    var xhp = 30
    repeat(obj_player.hp) {
        draw_sprite(spr_life, 0, xhp, 750)
        xhp += 30
    }
}

if (game_over) {
    draw_set_font(fnt_game_over)
    draw_set_colour(c_red)
    draw_text(camera_get_view_height(camera) / 2, camera_get_view_height(camera) / 2, "GAME OVER!")
    instance_deactivate_all(true)
}

if (level_won) {
    draw_set_font(fnt_game_over)
    draw_set_colour(c_green)
    draw_text(camera_get_view_height(camera)/ 2, camera_get_view_height(camera) / 2, "Level Complete")
    instance_deactivate_all(true)
}


if (paused == true) {
    if (!surface_exists(paused_surf)) {
        if (paused_surf == -1) {
            instance_deactivate_all(true);
        }
    paused_surf = surface_create(room_width, room_height);
    surface_set_target(paused_surf);
    draw_surface(application_surface, 0, 0);
    surface_reset_target();
    } else {
        draw_surface(paused_surf, 0, 0);
        draw_set_alpha(0.5);
        draw_rectangle_colour(0, 0, room_width, room_height, c_black, c_black, c_black, c_black, false);
        draw_set_alpha(1);
        draw_set_halign(fa_center);
        draw_text_transformed_colour(room_width/ 2, room_height / 2, "PAUSED", 2, 2, 0, c_aqua, c_aqua, c_aqua, c_aqua, 1);
        draw_text_transformed_colour((room_width/ 2), (room_height / 2) + 50, "R to Restart", 2, 2, 0, c_aqua, c_aqua, c_aqua, c_aqua, 1);
        draw_text_transformed_colour((room_width/ 2), (room_height / 2) + 100, "Q to Quit", 2, 2, 0, c_aqua, c_aqua, c_aqua, c_aqua, 1);
        draw_set_halign(fa_left);
    }
}