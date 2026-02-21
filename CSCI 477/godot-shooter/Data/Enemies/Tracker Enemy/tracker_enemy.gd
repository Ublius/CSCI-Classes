extends "res://Data/Enemies/Parent Enemy/base_enemy.gd"

@export var chase_range: float = 300.0
@export var attack_damage: int = 1
@export var destroy_on_hit: bool = true

var player_ref: Node2D = null

func _ready_setup() -> void:
	speed = 50.0
	health = 3
	player_ref = player

func _physics_process(delta: float) -> void:
	if player_ref:
		var direction = (player_ref.global_position - global_position).normalized()
		
		if global_position.distance_to(player_ref.global_position) <= chase_range:
			velocity = direction * speed
		else:
			velocity = Vector2.ZERO
		
		move_and_slide()
		
		for i in range(get_slide_collision_count()):
			var collision = get_slide_collision(i)
			var collider = collision.get_collider()
			
			if collider == player_ref or collider.is_in_group("Player"):
				_hit_player()
				break 
		
		if velocity.length() > 0 or true:
			look_at(player_ref.global_position)

func _hit_player() -> void:
	if player_ref and player_ref.has_method("take_damage"):
		player_ref.take_damage(attack_damage)
	
	if destroy_on_hit:
		queue_free()
