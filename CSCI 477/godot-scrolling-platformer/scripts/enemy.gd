extends RigidBody2D

@export var bounce_force: float = 500
@export var damage: int = 1

func _ready() -> void:
	contact_monitor = true
	max_contacts_reported = 8
	gravity_scale = 1.0
	linear_damp = 0.1
	
	body_entered.connect(_on_body_entered)
	#apply_central_impulse(Vector2(0, -bounce_force / 2))

func _on_body_entered(body: Node) -> void:
	if body is TileMapLayer:
		bounce()
	
	if body.is_in_group("player") or body is Player:
		damage_player(body)
		linear_velocity.y = abs(linear_velocity.y) * 0.5

func _integrate_forces(state: PhysicsDirectBodyState2D) -> void:
	var velocity = state.get_linear_velocity()
	velocity.x = 0
	state.set_linear_velocity(velocity)

func bounce() -> void:
	linear_velocity.y = -bounce_force

func damage_player(player: Node) -> void:
	if player.has_method("take_damage"):
		player.take_damage()
