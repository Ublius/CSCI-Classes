extends CharacterBody2D

@export var speed: float = 50.0
@export var health: int = 1
@export var damage_color: Color = Color.RED
@export var flash_duration: float = 0.1
@export var points_value: int = 10

var player: Node2D = null

signal enemy_died(points_value)

func _ready() -> void:
	add_to_group("Enemy")
	player = get_tree().get_first_node_in_group("Player")
	
	_ready_setup()

func _physics_process(delta: float) -> void:
	if player:
		_move_towards_player(delta)
		#
		#_rotate_towards_player(delta)
		#
		move_and_slide()
		
		_additional_behavior(delta)

func _ready_setup() -> void:
	pass

func _move_towards_player(delta: float) -> void:
	var direction = (player.global_position - global_position).normalized()
	velocity = direction * speed

func _rotate_towards_player(delta: float) -> void:
	look_at(player.global_position)

func _additional_behavior(delta: float) -> void:
	pass

func _take_damage(amount: int) -> void:
	health -= amount
	
	_flash_damage()
	
	if health <= 0:
		_die()

func _flash_damage() -> void:
	modulate = damage_color
	await get_tree().create_timer(flash_duration).timeout
	modulate = Color.WHITE

func _die() -> void:
	enemy_died.emit(points_value)
	queue_free()
