extends Area2D

@export var speed: float = 600

var direction: Vector2 = Vector2.UP

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	var timer = get_tree().create_timer(3.0)
	timer.timeout.connect(queue_free)

# Called every frame. 'delta'cds is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	position += direction * speed * delta

func _on_body_entered(body) -> void:
	if body.is_in_group("Enemies"):
		body._take_damage(1)
		queue_free()
