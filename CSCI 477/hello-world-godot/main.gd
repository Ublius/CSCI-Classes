extends Node2D

var score = 0
var time_left = 10

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	_update_score()
	_update_timer()

	$Timer.timeout.connect(_on_timer_timeout)
	#$Timer.timeout.connect(_on_timer_timeout())

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass

func _add_score(amount: int) -> void:
	score += amount
	_update_score()

func _update_score() -> void:
	$Score.text = "Score: " + str(score)

func _update_timer() -> void:
	$TimerLabel.text = "Time Remaining: " + str(time_left)
	
func _on_timer_timeout() -> void:
	time_left -= 1
	_update_timer()
	
	if time_left <= 0:
		_game_over()
		
func _game_over() -> void:
	$Timer.stop()
	$Score.text = "GAME OVER! Final Score: " + str(score)
	
	get_tree().paused = true
	
func _player_hit() -> void:
	_game_over()
