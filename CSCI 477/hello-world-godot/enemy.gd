extends Area2D

var speed = 50
var direction = Vector2.RIGHT

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	body_entered.connect(_on_body_entered)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	position += direction * speed * delta
	
	if position.x > 1152 or position.x < 0:
		direction.x = -direction.x
	
	if position.y > 648 or position.y < 0:
		direction.y = -direction.y

func _on_body_entered(body) -> void:
	if body.name == "Player":
		get_tree().current_scene._player_hit()
