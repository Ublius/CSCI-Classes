// Variables
move_speed = 1;

tilemap = layer_tilemap_get_id("Tiles_Col");

facing = 0;

// Player Variables
// Stats
strength = 1;
hp = 10;

// Defaults
hp_total =  hp;
level = 1;
xp = 0;
xp_require = 100;

// Functions
function add_xp(_xp_to_add) {
    xp += _xp_to_add;
    if (xp >= xp_require) {
        level++;
        xp -= xp_require;
        xp_require *= 1.4;
        
        hp_total += 5;
        hp = hp_total;
        strength += 0.8;
    }
}
