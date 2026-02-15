//var _hor = keyboard_check(ord("D")) - keyboard_check(ord("A"));
//var _ver = keyboard_check(ord("S")) - keyboard_check(ord("W"));

//move_and_collide(_hor * spd, _ver * spd, tilemap, undefined, undefined, undefined, spd, spd);

if (hp <= 0) {
    instance_destroy(id, true)
}