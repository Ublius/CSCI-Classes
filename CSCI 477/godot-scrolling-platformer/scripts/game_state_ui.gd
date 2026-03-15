extends Control

@onready var game_over_label = $CenterContainer/Label
@onready var center_container = $CenterContainer

func _ready():
	EventController.game_over.connect(_on_game_over)
	hide()
	
	anchor_left = 0
	anchor_top = 0
	anchor_right = 1
	anchor_bottom = 1
	offset_left = 0
	offset_top = 0
	offset_right = 0
	offset_bottom = 0
	
	center_container.anchor_left = 0
	center_container.anchor_top = 0
	center_container.anchor_right = 1
	center_container.anchor_bottom = 1
	center_container.offset_left = 0
	center_container.offset_top = 0
	center_container.offset_right = 0
	center_container.offset_bottom = 0
	
func _on_game_over():
	show()
