extends Sprite2D

@export var full_heart_texture: Texture2D
@export var empty_heart_texture: Texture2D

func set_full(is_full: bool) -> void:
	if is_full:
		texture = full_heart_texture
	else:
		texture = empty_heart_texture
