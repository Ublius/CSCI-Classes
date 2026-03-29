extends RigidBody2D

@export var radius: float = 20.0
var color: Color = Color(1,1,1)
var controller = null
var settled = false
var start_y = 0

@export var settle_speed_threshold: float = 20.0

func set_radius(new_radius):
	radius = new_radius
	
	if $CollisionShape2D.shape:
		$CollisionShape2D.shape = $CollisionShape2D.shape.duplicate()
		var circle_shape = $CollisionShape2D.shape as CircleShape2D
		if circle_shape:
			circle_shape.radius = radius
	
	$BallVisual.radius = radius
	$BallVisual.color = color
	$BallVisual.queue_redraw()
	
	# Mass scales with size
	mass = radius * 0.1

	if physics_material_override == null:
		var mat = PhysicsMaterial.new()
		mat.bounce = 0.6 + randf() * 0.2 
		mat.friction = 0.4
		physics_material_override = mat

func _ready():
	start_y = position.y
	set_radius(radius)
	sleeping = false

func _physics_process(delta):
	if settled:
		return
	
	# Settle only when ball is inside the platform AND slow enough
	if controller and self in controller.active_balls:
		if linear_velocity.length() < settle_speed_threshold:
			settled = true
			#controller.ball_settled(self)
