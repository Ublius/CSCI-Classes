extends CharacterBody2D

const SPEED   := 200.0
const GRAVITY := 980.0

var is_attacking := false
var facing       := 1   # 1 = right, -1 = left

@onready var anim    := $AnimatedSprite2D
@onready var hitbox	 := $AttackHitbox
@onready var hitbox_col  := $AttackHitbox/CollisionShape2D

func _ready():
	# Hitbox starts disabled — only active during attack
	hitbox_col.disabled = true
	
	# When the attack animation finishes, end the attack
	anim.animation_finished.connect(_on_animation_finished)

func _physics_process(delta):
	# Gravity
	if not is_on_floor():
		velocity.y += GRAVITY * delta

	# Don't allow movement or new attacks while attacking
	if not is_attacking:
		_handle_movement()
		_handle_attack_input()
	else:
		_update_hitbox_per_frame()
	move_and_slide()
	_update_animation()

func _handle_movement():
	var dir = Input.get_axis("ui_left", "ui_right")

	if dir != 0:
		velocity.x   = dir * SPEED
		facing        = int(sign(dir))
		anim.flip_h   = facing == -1
	else:
		velocity.x = move_toward(velocity.x, 0, SPEED)
	
	hitbox.position.x = facing * abs(hitbox.position.x)

func _handle_attack_input():
	if Input.is_action_just_pressed("ui_accept"):
		_start_attack()

func _start_attack(): 
	hitbox_col.disabled = false
	is_attacking      = true
	velocity.x = 0
	anim.play("attack")

func _on_animation_finished():
	if anim.animation == "attack":
		is_attacking = false
		hitbox_col.disabled = true

func _update_animation():
	if not is_attacking:
		anim.play("walk")

func _update_hitbox_per_frame():
	match anim.frame:
		2:
			hitbox_col.disabled = false
			hitbox_col.shape.size = Vector2(20, 40)
			hitbox_col.position = Vector2(facing * 30, 0)
		8:
			hitbox_col.disabled = false
			hitbox_col.shape.size = Vector2(40, 20)
			hitbox_col.position = Vector2(facing * 30, 0)
		9:
			hitbox_col.disabled = false
			hitbox_col.shape.size = Vector2(40, 20)
			hitbox_col.position = Vector2(facing * 30, 0)
		_:
			hitbox_col.disabled = true
			hitbox_col.position = Vector2(0,0)
