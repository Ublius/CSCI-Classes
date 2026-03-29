extends Node2D

@export var radius: float = 20.0
@export var color: Color = Color(1,1,1)

func _draw():
	draw_circle(Vector2.ZERO, radius, color)
