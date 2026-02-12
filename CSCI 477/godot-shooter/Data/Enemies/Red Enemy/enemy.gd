extends CharacterBody2D

@export var speed: float = 100.0
@export var health: int = 3

var player: Node2D = null

func _ready() -> void:
	player = get_tree().get_first_node_in_group("Player")

func _physics_process(delta: float) -> void:
	if player:
		var direction = (player.global_position - global_position).normalized()
		velocity = direction * speed
	
		move_and_slide()
		look_at(player.global_position)
		
func _take_damage(amount: int) -> void:
	health -= amount
	modulate = Color.WHITE
	await get_tree().create_timer(0.1).timeout
	modulate = Color.RED
	
	if health <= 0:
		queue_free()
