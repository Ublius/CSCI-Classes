extends CharacterBody2D

@export var speed: float = 200
@export var jump_velocity: float = -350
@export var grav_multiplier: float = 1.0

var gravity = ProjectSettings.get_setting("physics/2d/default_gravity")
var facing_right: bool = true

func _physics_process(delta: float) -> void:
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
