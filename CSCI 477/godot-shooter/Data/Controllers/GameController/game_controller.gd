extends Node2D

@export var heart_scene: PackedScene
@export var heart_spacing: int = 40
@export var heart_offset: Vector2 = Vector2(20, 20) 

var current_health: int
var max_health: int
var current_score: int = 0
var hearts: Array = []
var game_over_labels: Array = []
var game_active: bool = true

@onready var player: CharacterBody2D = get_tree().get_first_node_in_group("Player")
@onready var score_label: Label = $ScoreLabel
@onready var health_container: Node2D = $HealthContainer

signal health_changed(current_health, max_health)
signal score_changed(new_score)
signal game_over

func _ready() -> void:
	await get_tree().process_frame
	
	if player:
		if "MAX_HEALTH" in player:
			max_health = player.MAX_HEALTH
		
		if "current_health" in player:
			current_health = player.current_health
		else:
			current_health = max_health
		
		if player.has_signal("health_changed"):
			player.health_changed.connect(_on_player_health_changed)
		else:
			print("Warning: Player doesn't have health_changed signal")
			
		if player.has_signal("damage_taken"):
			player.damage_taken.connect(_on_player_damage_taken)
		else:
			print("Warning: Player doesn't have damage_taken signal")
			
	else:
		current_health = max_health
		print("Warning: No player found in group 'Player'")
		
	_connect_to_enemies()
	_create_heart_display()

func _create_heart_display() -> void:
	for heart in hearts:
		if is_instance_valid(heart):
			heart.queue_free()
	hearts.clear()
	
	for i in range(max_health):
		if heart_scene:
			var heart = heart_scene.instantiate()
			
			var viewport_size = get_viewport().get_visible_rect().size
			heart.position = Vector2(heart_offset.x + (i * heart_spacing), viewport_size.y - heart_offset.y)
			
			if health_container:
				health_container.add_child(heart)
			else:
				add_child(heart)
			
			hearts.append(heart)
			
			if heart.has_method("set_full"):
				heart.set_full(i < current_health)
			else:
				heart.visible = (i < current_health)

func _process(delta: float) -> void:
	_update_heart_positions()

func _update_heart_positions() -> void:
	var viewport_size = get_viewport().get_visible_rect().size
	
	for i in range(hearts.size()):
		if is_instance_valid(hearts[i]):
			hearts[i].position = Vector2(heart_offset.x + (i * heart_spacing), viewport_size.y - heart_offset.y)

func update_health(new_health: int) -> void:
	current_health = clamp(new_health, 0, max_health)
	
	_update_heart_display()
	
	health_changed.emit(current_health, max_health)
	
	if current_health <= 0:
		game_over.emit()
		_game_over()

func _update_heart_display() -> void:
	for i in range(hearts.size()):
		if is_instance_valid(hearts[i]):
			if hearts[i].has_method("set_full"):
				hearts[i].set_full(i < current_health)
			else:
				hearts[i].visible = (i < current_health)

func add_score(points: int) -> void:
	current_score += points
	score_changed.emit(current_score)
	
	if score_label:
		score_label.text = "Score: " + str(current_score)

func reset_game() -> void:
	current_score = 0
	if player and "current_health" in player:
		current_health = player.current_health
	else:
		current_health = max_health
	
	add_score(0)
	_update_heart_display()

func _game_over() -> void:
	if not game_active:
		return
		
	game_active = false
	print("Game Over! Final score: ", current_score)
	
	var game_over_label = Label.new()
	game_over_label.text = "GAME OVER"
	game_over_label.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	game_over_label.vertical_alignment = VERTICAL_ALIGNMENT_CENTER
	
	game_over_label.add_theme_color_override("font_color", Color.CYAN)
	
	game_over_label.add_theme_font_size_override("font_size", 64)
	
	var viewport_size = get_viewport().get_visible_rect().size
	game_over_label.position = Vector2(viewport_size.x / 2 - 150, viewport_size.y / 2 - 32)
	game_over_label.size = Vector2(300, 64) 
	
	add_child(game_over_label)
	
	get_tree().paused = true
	
	game_over_labels = [game_over_label]

func _on_player_health_changed(new_health: int) -> void:
	update_health(new_health)
	
func _on_player_damage_taken(amount: int, remaining_health: int) -> void:
	#print("Player took ", amount, " damage! Health remaining: ", remaining_health)
	if game_active:
		update_health(remaining_health)

func update_max_health(new_max: int) -> void:
	max_health = new_max
	_create_heart_display()
	
func _connect_to_enemies() -> void:
	var enemies = get_tree().get_nodes_in_group("Enemy")
	for enemy in enemies:
		if enemy.has_signal("enemy_died") and not enemy.enemy_died.is_connected(_on_enemy_died):
			enemy.enemy_died.connect(_on_enemy_died)

func _on_enemy_spawned(enemy: Node) -> void:
	if game_active and enemy.has_signal("enemy_died") and not enemy.enemy_died.is_connected(_on_enemy_died):
		enemy.enemy_died.connect(_on_enemy_died)

func _on_enemy_died(points: int) -> void:
	if game_active:
		add_score(points)
		
func is_game_active() -> bool:
	return game_active
