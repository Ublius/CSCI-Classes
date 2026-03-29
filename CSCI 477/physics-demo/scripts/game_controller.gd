extends Node2D

@export var ball_scene: PackedScene

@onready var timer = $"../Timer"
@onready var score_label = $"../UI/ScoreLabel"
@onready var time_label = $"../UI/TimeLabel"
@onready var music = $"../AudioStreamPlayer"
@onready var platform_area = $"../Platform"

var score = 0
var time_left = 60
var active_balls = []
var ball_points = {}

func _ready():
	randomize()
	timer.start()
	music.play()

func _input(event):
	if event is InputEventMouseButton and event.pressed:
		spawn_ball(event.position)

func spawn_ball(pos):
	var ball = ball_scene.instantiate()
	ball.color = Color(randf(), randf(), randf())
	ball.controller = self
	ball.position = pos
	
	# Set radius before adding to the scene
	var radius = randf_range(10, 40)
	ball.set_radius(radius)
	
	add_child(ball)

func _on_timer_timeout():
	time_left -= 1
	time_label.text = "Time: " + str(time_left)

	if time_left <= 0:
		end_game()

# Called when a ball touches the platform
func _on_area_2d_body_entered(body: Node2D) -> void:
	if body is RigidBody2D:
		if body not in active_balls:
			active_balls.append(body)
			
			# Calculate points for this ball
			var drop_height = body.position.y - body.start_y
			var bonus = int(drop_height / 50)
			var points = 10 + bonus
			print("Bonus points from hieght: ", bonus)
			ball_points[body] = points
			
			# Add points
			score += points
			score_label.text = "Score: " + str(score)

# Called when a ball leaves the platform
func _on_area_2d_body_exited(body: Node2D) -> void:
	if body in active_balls:
		active_balls.erase(body)
		
		# Subtract previously added points
		if body in ball_points:
			score -= ball_points[body]
			score_label.text = "Score: " + str(score)
			ball_points.erase(body)

func end_game():
	timer.stop()
	get_tree().paused = true
