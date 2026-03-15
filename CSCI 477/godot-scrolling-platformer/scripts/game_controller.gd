extends Node

var total_coins: int = 0
var player_lives: int = 3
var player_max_lives: int = 3

func coins_collected(value: int):
	total_coins += value
	EventController.emit_signal("coin_collected", total_coins)
	
func update_player_health(lives: int):
	player_lives = lives
	EventController.player_health_updated.emit(lives)

func get_player_health() -> int:
	return player_lives

func game_over():
	EventController.game_over.emit()
	
	#await get_tree().create_timer(2.0).timeout
	#get_tree().reload_current_scene()

func reset_game():
	player_lives = player_max_lives
	total_coins = 0
	update_player_health(player_lives)
