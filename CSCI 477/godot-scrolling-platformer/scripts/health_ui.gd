extends Control

@onready var label = $Label

func _ready() -> void:
	EventController.connect("player_health_updated", on_event_health_update)
	
	if GameController.has_method("get_player_health"):
		on_event_health_update(GameController.get_player_health())

func on_event_health_update(lives: int) -> void:
	label.text = str(lives)
