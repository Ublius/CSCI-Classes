extends "res://Data/Enemies/Parent Enemy/base_enemy.gd"

@export var bullet_scene: PackedScene
@export var shoot_cooldown: float = 2.0
@export var shoot_range: float = 400.0
@export var rotation_speed: float = 5.0 
@export var attack_damage: int = 2

@onready var muzzle = $Muzzle
@onready var shoot_timer: Timer = $ShootTimer

var can_shoot: bool = true
var player_ref: Node2D = null

func _ready_setup() -> void:
	speed = 0.0 
	health = 2 
	
	player_ref = player
	
	if not shoot_timer:
		shoot_timer = Timer.new()
		shoot_timer.one_shot = true
		shoot_timer.timeout.connect(_on_shoot_timer_timeout)
		add_child(shoot_timer)
	
	shoot_timer.wait_time = shoot_cooldown

func _physics_process(delta: float) -> void:
	if not player_ref:
		return
	
	var direction_to_player = (player_ref.global_position - global_position).normalized()
	
	var target_angle = direction_to_player.angle()
	var current_angle = rotation
	var new_angle = lerp_angle(current_angle, target_angle, rotation_speed * delta)
	rotation = new_angle
	
	var distance_to_player = global_position.distance_to(player_ref.global_position)
	if distance_to_player <= shoot_range and can_shoot:
		_shoot()
	
	velocity = Vector2.ZERO

func _shoot() -> void:
	if bullet_scene and muzzle and can_shoot:
		can_shoot = false
		
		var bullet = bullet_scene.instantiate()
		
		if bullet.has_method("set_bullet_properties"):
			var shoot_direction = Vector2.RIGHT.rotated(rotation)
			bullet.set_bullet_properties(self, attack_damage, shoot_direction)
		
		bullet.global_position = muzzle.global_position
		
		get_parent().add_child(bullet)
		
		shoot_timer.start()

func _on_shoot_timer_timeout() -> void:
	can_shoot = true
