extends CharacterBody2D

@export var SPEED: float =  300.0
@export var bullet_scene: PackedScene

@onready var muzzle = $Muzzle

func _physics_process(delta: float) -> void:
	# Get the input direction and handle the movement/deceleration.
	# As good practice, you should replace UI actions with custom gameplay actions.
	var direction = Vector2.ZERO
	direction.x = Input.get_axis("left", "right")
	direction.y = Input.get_axis("forward", "back")

	velocity = direction * SPEED
	move_and_slide()
	
	look_at(get_global_mouse_position())

func _input(event: InputEvent) -> void:
	if event.is_action_pressed("shoot"):
		_shoot()

func _shoot() -> void:
	if bullet_scene:
		var bullet = bullet_scene.instantiate()
		bullet.position = muzzle.global_position
		bullet.direction = (get_global_mouse_position() - global_position).normalized()
		bullet.rotation = bullet.direction.angle()
		get_parent().add_child(bullet)
