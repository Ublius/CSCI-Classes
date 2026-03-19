extends StaticBody2D

var hp         := 3
var can_be_hit := true
var is_dead    := false

@onready var anim    := $AnimatedSprite2D
@onready var hurtbox := $HurtBox
@onready var hit_sound := $HitSound

func _ready():
	hurtbox.area_entered.connect(_on_hit)
	anim.animation_finished.connect(_on_animation_finished)
	anim.play("idle")

func _on_hit(area: Area2D):
	# Check the player is actually attacking
	var player = area.get_parent()
	if not player.is_attacking:
		return

	if not can_be_hit or is_dead:
		return

	can_be_hit = false
	hp -= 1
	hit_sound.play()
	_flash()
	print("Enemy hit! HP remaining: ", hp)

	if hp <= 0:
		_die()
		return

	await get_tree().create_timer(0.3).timeout
	can_be_hit = true

func _die():
	is_dead = true
	# Disable the hurtbox so it can't be hit during death animation
	anim.play("death")

func _on_animation_finished():
	if anim.animation == "death":
		queue_free()
		
func _flash():
	for i in range(10):
		anim.modulate = Color.RED
		await get_tree().create_timer(0.05).timeout
		anim.modulate = Color.WHITE
		await get_tree().create_timer(0.05).timeout
