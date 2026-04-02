extends Control

var max_health := 4
var current_health := 4
@export var bar_width := 50
@export var bar_height := 8
@export var background_color := Color(0.2, 0.2, 0.2, 0.8)
@export var fill_color := Color(0.2, 0.8, 0.2, 1.0)

@onready var background := $Background
@onready var fill := $Fill

func _ready():
	setup_health_bar()
	update_health_display()

func setup_health_bar():
	custom_minimum_size = Vector2(bar_width, bar_height)
	
	background.color = background_color
	background.size = Vector2(bar_width, bar_height)
	
	fill.color = fill_color
	update_fill_size()

func update_health(amount: int):
	current_health = clamp(amount, 0, max_health)
	update_health_display()

func update_health_display():
	update_fill_size()
	update_fill_color()

func update_fill_size():
	var fill_percentage = float(current_health) / float(max_health)
	fill.size = Vector2(bar_width * fill_percentage, bar_height)

func update_fill_color():
	var health_percentage = float(current_health) / float(max_health)
	
	if health_percentage > 0.6:
		fill_color = Color(0.2, 0.8, 0.2, 1.0)
	elif health_percentage > 0.3:
		fill_color = Color(0.8, 0.8, 0.2, 1.0)
	else:
		fill_color = Color(0.8, 0.2, 0.2, 1.0)
	
	fill.color = fill_color

func set_max_health(value: int):
	max_health = value
	current_health = min(current_health, max_health)
	update_health_display()
