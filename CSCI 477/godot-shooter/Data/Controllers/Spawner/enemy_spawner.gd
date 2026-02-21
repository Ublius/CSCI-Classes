extends Node2D

@export var enemy_scenes: Array[PackedScene]
@export var spawn_interval: float = 2.0
@export var max_enemies: int = 10

@onready var game_controller = get_tree().get_first_node_in_group("GameController")
var spawning_enabled: bool = true
var current_enemies: int = 0

func _ready() -> void:
	start_spawning()

func start_spawning() -> void:
	while spawning_enabled:
		await get_tree().create_timer(spawn_interval).timeout
		
		if game_controller and game_controller.has_method("is_game_active"):
			if not game_controller.game_active:
				continue
				
		if current_enemies >= max_enemies:
			continue
		
		spawn_enemy()

func spawn_enemy() -> void:
	if enemy_scenes.is_empty() or not spawning_enabled:
		return
		
	if game_controller and game_controller.has_method("is_game_active"):
		if not game_controller.game_active:
			return
	
	var enemy_scene = enemy_scenes[randi() % enemy_scenes.size()]
	var enemy = enemy_scene.instantiate()
	
	var viewport_size = get_viewport().get_visible_rect().size
	var spawn_pos = Vector2(randf_range(50, viewport_size.x - 50), randf_range(50, viewport_size.y - 50))
	enemy.global_position = spawn_pos
	
	if enemy.has_signal("enemy_died"):
		enemy.enemy_died.connect(_on_enemy_died)
	
	add_child(enemy)
	current_enemies += 1
	
	if game_controller and game_controller.has_method("_on_enemy_spawned"):
		game_controller._on_enemy_spawned(enemy)
		
func _on_enemy_died(points: int) -> void:
	current_enemies -= 1

func stop_spawning() -> void:
	spawning_enabled = false

func start_spawning_again() -> void:
	spawning_enabled = true
	start_spawning()
