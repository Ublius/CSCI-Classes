extends Node

signal time_updated(new_time: float)
signal game_won()
signal game_lost()

var start_time: float = 20.0

var current_time: float = 0.0
var game_active: bool = false
var time_penalty: float = 5.0
var player: Node2D
var victim: Node2D

func _ready() -> void:
	reset_game()

func _process(delta: float) -> void:
	if not game_active:
		return
	
	current_time -= delta
	current_time = max(0, current_time)
	time_updated.emit(current_time)
	
	if current_time <= 0:
		game_over(false)
		return
	
	check_win_condition()

func check_win_condition() -> void:
	if not game_active:
		return
	
	if not player:
		player = get_tree().get_first_node_in_group("player")
	if not victim:
		victim = get_tree().get_first_node_in_group("victim")
	
	if player and victim:
		var distance = player.global_position.distance_to(victim.global_position)
		if distance < 50.0:
			game_over(true)

func enemy_reached_player() -> void:
	if not game_active:
		return
	
	print("Enemy reached player! Time penalty: ", time_penalty)
	current_time -= time_penalty
	current_time = max(0, current_time)
	time_updated.emit(current_time)
	
	if current_time <= 0:
		game_over(false)

func game_over(is_win: bool) -> void:
	if not game_active:
		return
	
	game_active = false
	
	if is_win:
		print("Game Over - WIN!")
		game_won.emit()
	else:
		print("Game Over - LOSE!")
		game_lost.emit()

func reset_game() -> void:
	print("Resetting GameManager...")
	current_time = start_time
	game_active = true
	time_updated.emit(current_time)
	
	player = null
	victim = null
	
	print("Game reset with time: ", current_time)
