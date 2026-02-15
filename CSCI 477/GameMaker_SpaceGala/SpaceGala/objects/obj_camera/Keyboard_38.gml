if (instance_exists(obj_player)) {
    var camera_x = camera_get_view_x(camera)
    var camera_y = camera_get_view_y(camera)
    var camera_w = camera_get_view_width(camera)
    var camera_h = camera_get_view_height(camera)
    
    if(camera_y <= 0) spd = 0
    
    // Cap 
    if(obj_player.y + (obj_player.sprite_height / 2) >= camera_y + camera_h) obj_player.y = camera_y + camera_h - (obj_player.sprite_height / 2)
    if(obj_player.y - (obj_player.sprite_height / 2) <= camera_y) obj_player.y = camera_y + (obj_player.sprite_height / 2)
    
    speed = obj_player.speed
    //obj_player.speed = spd
}