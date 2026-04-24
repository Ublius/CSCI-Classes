extends CharacterBody2D


const SPEED = 150.0
const UPDATE_INTERVAL := 0.3

var player: CharacterBody2D
var update_timer := 0.0

@onready var nav_agent := $NavigationAgent2D

func _ready() -> void:
	await get_tree().process_frame
	
	player = get_tree().get_first_node_in_group("player")
	
	nav_agent.path_desired_distance = 4.0
	nav_agent.target_desired_distance = 16.0
	nav_agent.path_max_distance = 50.0

func _physics_process(delta: float) -> void:
	if not player:
		return
	
	update_timer += delta
	if update_timer >= UPDATE_INTERVAL:
		update_timer = 0.0
		nav_agent.target_position = player.global_position
		
	_move_along_path()
	_check_player_collision()
	
func _move_along_path() -> void:
	if nav_agent.is_navigation_finished():
		velocity = Vector2.ZERO
		move_and_slide()
		return
	
	var next_point = nav_agent.get_next_path_position()
	var direction = (next_point - global_position).normalized()
	
	velocity = direction * SPEED
	move_and_slide()
	
func _check_player_collision() -> void:
	for i in get_slide_collision_count():
		var collision = get_slide_collision(i)
		if collision.get_collider() == player:
			if GameManager.has_method("enemy_reached_player"):
				GameManager.enemy_reached_player()
			queue_free()
			break
