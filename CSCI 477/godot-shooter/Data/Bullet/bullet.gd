extends Area2D

@export var speed: float = 600

var direction: Vector2 = Vector2.UP
var damage: int = 1
var shooter: Node = null

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	var timer = get_tree().create_timer(3.0)
	timer.timeout.connect(queue_free)

func _process(delta: float) -> void:
	position += direction * speed * delta

func _on_body_entered(body) -> void:
	if body == shooter:
		return
	
	if body.is_in_group("Enemy"):
		if body.has_method("_take_damage"):
			body._take_damage(damage)
			#print("Bullet hit enemy for ", damage, " damage")
		queue_free()
	
	elif body.is_in_group("Player"):
		if body.has_method("take_damage"):
			body.take_damage(damage)
			#print("Bullet hit player for ", damage, " damage")
		queue_free()
	
	elif body.is_in_group("Walls") or body is TileMap:
		queue_free()

func set_bullet_properties(shooter_node: Node, bullet_damage: int, bullet_direction: Vector2) -> void:
	shooter = shooter_node
	damage = bullet_damage
	direction = bullet_direction
	rotation = bullet_direction.angle()
