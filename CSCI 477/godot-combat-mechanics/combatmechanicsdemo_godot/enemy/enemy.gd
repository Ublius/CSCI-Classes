extends StaticBody2D

@export var hp := 4
var can_be_hit := true
var is_dead    := false

@export var health_bar_offset := Vector2(-20, -45)

var health_bar: Control

@onready var anim    := $AnimatedSprite2D
@onready var hurtbox := $HurtBox
@onready var hit_sound := $HitSound
@onready var death_sound := $DeathSound

func _ready():
	hurtbox.area_entered.connect(_on_hit)
	anim.animation_finished.connect(_on_animation_finished)
	anim.play("idle")
	
	setup_health_bar()

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
	update_health_bar()
	print("Enemy hit! HP remaining: ", hp)

	if hp <= 0:
		_die()
		return

	await get_tree().create_timer(0.3).timeout
	can_be_hit = true

func _die():
	is_dead = true
	if health_bar:
		health_bar.visible = false
	anim.play("death")
	death_sound.play()

func _on_animation_finished():
	if anim.animation == "death":
		queue_free()
		
func _flash():
	for i in range(10):
		anim.modulate = Color.RED
		await get_tree().create_timer(0.05).timeout
		anim.modulate = Color.WHITE
		await get_tree().create_timer(0.05).timeout

func setup_health_bar():
	var health_bar_scene = preload("res://scenes/health_bar.tscn")
	health_bar = health_bar_scene.instantiate()
	add_child(health_bar)
	
	health_bar.position = health_bar_offset
	
	if health_bar.has_method("set_max_health"):
		health_bar.set_max_health(hp)
	elif health_bar.has_method("update_health"):
		health_bar.update_health(hp)

func update_health_bar():
	if health_bar and health_bar.has_method("update_health"):
		health_bar.update_health(hp)
