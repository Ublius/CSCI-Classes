class_name Player extends CharacterBody2D

@export var speed: float = 200
@export var jump_velocity: float = -700
@export var grav_multiplier: float = 1.0

@export var max_lives = 3
var lives = max_lives

@export var death_y_level: float = 500
var respawn_position: Vector2 = Vector2(0, 0)

var gravity = ProjectSettings.get_setting("physics/2d/default_gravity")
var facing_right: bool = true

var is_game_over: bool = false

func _ready() -> void:
	add_to_group("player")
	lives = max_lives
	respawn_position = global_position
	GameController.update_player_health(lives)

func _physics_process(delta: float) -> void:
	if is_game_over:
		return
	
	if global_position.y > death_y_level:
		die_and_respawn()
		return 
	
	# Add the gravity.
	if not is_on_floor():
		velocity.y += gravity * grav_multiplier * delta

	# Handle jump.
	if Input.is_action_just_pressed("jump") and is_on_floor():
		velocity.y = jump_velocity

	# Get horizontal input
	var direction = Input.get_axis("move_left", "move_right")
	if direction != 0:
		velocity.x = direction * speed
		if direction > 0 and not facing_right:
			flip()
		elif direction < 0 and facing_right:
			flip()
	else:
		velocity.x = move_toward(velocity.x, 0, speed * 0.2)

	move_and_slide()

func flip() -> void:
	facing_right = not facing_right
	$Sprite2D.flip_h = not facing_right
	
func die_and_respawn():
	take_damage()
	
func take_damage():
	if lives > 0:
		lives = lives - 1
		GameController.update_player_health(lives)
	if lives <= 0:
		game_over()
	else:
		respawn()
		
func respawn():
	global_position = respawn_position
	velocity = Vector2.ZERO

func game_over():
	if is_game_over:
		return
	is_game_over = true
	set_physics_process(false)
	hide()
	GameController.game_over()
	#print("Game Over!")
	#get_tree().reload_current_scene()
