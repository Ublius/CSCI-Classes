extends CharacterBody2D

@export var MAX_HEALTH: int = 3
@export var SPEED: float =  300.0
@export var bullet_scene: PackedScene
@export var DAMAGE: int = 1

var current_health: int
var invulnerable: bool = false
var invulnerability_time: float = 1.0

@onready var muzzle = $Muzzle
@onready var sprite: Sprite2D = $Sprite2D

signal health_changed(new_health)
signal player_died
signal damage_taken(amount, remaining_health)

func _ready() -> void:
	add_to_group("Player")
	current_health = MAX_HEALTH

func _physics_process(delta: float) -> void:
	# Get the input direction and handle the movement/deceleration.
	# As good practice, you should replace UI actions with custom gameplay actions.
	var direction = Vector2.ZERO
	direction.x = Input.get_axis("left", "right")
	direction.y = Input.get_axis("forward", "back")

	velocity = direction * SPEED
	move_and_slide()
	
	_bound_to_screen()
	
	look_at(get_global_mouse_position())
	rotate(PI/2)

func _input(event: InputEvent) -> void:
	if event.is_action_pressed("shoot"):
		_shoot()

func _shoot() -> void:
	if bullet_scene:
		var bullet = bullet_scene.instantiate()
		var shoot_direction = (get_global_mouse_position() - global_position).normalized()
		
		if bullet.has_method("set_bullet_properties"):
			bullet.set_bullet_properties(self, DAMAGE, shoot_direction)
		
		bullet.position = muzzle.global_position
		get_parent().add_child(bullet)

func _bound_to_screen() -> void:
	var viewport_rect = get_viewport().get_visible_rect()
	var screen_size = viewport_rect.size
	
	if sprite and sprite.texture:
		var sprite_size = sprite.texture.get_size() * sprite.scale
		var sprite_half_size = sprite_size / 2
		
		global_position = global_position.clamp(sprite_half_size, screen_size - sprite_half_size)
	else:
		global_position = global_position.clamp(Vector2.ZERO, screen_size)
		
func take_damage(amount: int = 1) -> void:
	if invulnerable:
		return
	
	current_health -= amount
	health_changed.emit(current_health)
	
	damage_taken.emit(amount, current_health)
	health_changed.emit(current_health)
	
	_start_invulnerability()
	_damage_flash()
	
	if current_health <= 0:
		die()

func _start_invulnerability() -> void:
	invulnerable = true
	await get_tree().create_timer(invulnerability_time).timeout
	invulnerable = false

func _damage_flash() -> void:
	modulate = Color.RED
	await get_tree().create_timer(0.1).timeout
	modulate = Color.WHITE

func die() -> void:
	player_died.emit()
	queue_free()
