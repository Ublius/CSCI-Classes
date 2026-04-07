if (path != noone) {
    path_delete(path)
}

path = path_add()
mp_grid_path(obj_grid.grid, path, x, y, obj_player.x, obj_player.y, false)
path_start(path, 2, path_action_stop, true)