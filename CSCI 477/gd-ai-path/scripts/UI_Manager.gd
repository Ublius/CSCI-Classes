extends CanvasLayer

var time_label: Label
var win_panel: Panel
var lose_panel: Panel
var win_label: Label
var lose_label: Label
var restart_button_win: Button
var restart_button_lose: Button
var game_title: Label
var container: ColorRect

func _ready() -> void:
	process_mode = PROCESS_MODE_ALWAYS
	
	create_ui()
	connect_signals()
	hide_all_screens()
	
	get_viewport().size_changed.connect(_on_viewport_size_changed)
	
	print("UI Manager ready - panels created")

func create_ui() -> void:
	container = ColorRect.new()
	container.color = Color(0, 0, 0, 0)
	container.set_anchors_and_offsets_preset(Control.PRESET_FULL_RECT)
	add_child(container)
	
	time_label = Label.new()
	time_label.text = "Time: "
	time_label.add_theme_font_size_override("font_size", 32)
	time_label.add_theme_color_override("font_color", Color(1, 1, 1))
	time_label.horizontal_alignment = HORIZONTAL_ALIGNMENT_LEFT
	time_label.position = Vector2(20, 20)
	time_label.size = Vector2(300, 50)
	container.add_child(time_label)
	
	win_panel = create_panel(Color(0, 1, 0, 0.95))
	container.add_child(win_panel)
	win_panel.visible = false
	
	var win_container = Control.new()
	win_container.set_anchors_and_offsets_preset(Control.PRESET_FULL_RECT)
	win_panel.add_child(win_container)
	
	win_label = Label.new()
	win_label.text = "VICTORY!"
	win_label.add_theme_font_size_override("font_size", 64)
	win_label.add_theme_color_override("font_color", Color(1, 1, 0))
	win_label.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	win_label.position = Vector2(0, 50)
	win_label.size = Vector2(400, 80)
	win_container.add_child(win_label)
	
	var win_message = Label.new()
	win_message.text = "You rescued the victim!"
	win_message.add_theme_font_size_override("font_size", 24)
	win_message.add_theme_color_override("font_color", Color(1, 1, 1))
	win_message.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	win_message.position = Vector2(0, 140)
	win_message.size = Vector2(400, 40)
	win_container.add_child(win_message)
	
	restart_button_win = create_button("Play Again", Vector2(100, 220))
	win_container.add_child(restart_button_win)
	
	lose_panel = create_panel(Color(1, 0, 0, 0.95))
	container.add_child(lose_panel)
	lose_panel.visible = false
	
	var lose_container = Control.new()
	lose_container.set_anchors_and_offsets_preset(Control.PRESET_FULL_RECT)
	lose_panel.add_child(lose_container)
	
	lose_label = Label.new()
	lose_label.text = "GAME OVER!"
	lose_label.add_theme_font_size_override("font_size", 64)
	lose_label.add_theme_color_override("font_color", Color(1, 0.5, 0))
	lose_label.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	lose_label.position = Vector2(0, 50)
	lose_label.size = Vector2(400, 80)
	lose_container.add_child(lose_label)
	
	var lose_message = Label.new()
	lose_message.text = "You ran out of time!"
	lose_message.add_theme_font_size_override("font_size", 24)
	lose_message.add_theme_color_override("font_color", Color(1, 1, 1))
	lose_message.horizontal_alignment = HORIZONTAL_ALIGNMENT_CENTER
	lose_message.position = Vector2(0, 140)
	lose_message.size = Vector2(400, 40)
	lose_container.add_child(lose_message)
	
	restart_button_lose = create_button("Try Again", Vector2(100, 220))
	lose_container.add_child(restart_button_lose)

func create_panel(color: Color) -> Panel:
	var panel = Panel.new()
	panel.set_size(Vector2(400, 320))
	panel.position = Vector2(
		(get_viewport().size.x / 2) - 200,
		(get_viewport().size.y / 2) - 160
	)
	panel.add_theme_stylebox_override("panel", create_stylebox(color))
	return panel

func create_stylebox(color: Color) -> StyleBoxFlat:
	var stylebox = StyleBoxFlat.new()
	stylebox.bg_color = color
	stylebox.set_corner_radius_all(10)
	stylebox.set_border_width_all(3)
	stylebox.border_color = Color(1, 1, 1)
	return stylebox

func create_button(text: String, position: Vector2) -> Button:
	var button = Button.new()
	button.text = text
	button.position = position
	button.size = Vector2(200, 50)
	button.add_theme_font_size_override("font_size", 24)
	
	var stylebox_normal = StyleBoxFlat.new()
	stylebox_normal.bg_color = Color(0.2, 0.2, 0.8)
	stylebox_normal.set_corner_radius_all(5)
	
	var stylebox_hover = StyleBoxFlat.new()
	stylebox_hover.bg_color = Color(0.3, 0.3, 0.9)
	stylebox_hover.set_corner_radius_all(5)
	
	var stylebox_pressed = StyleBoxFlat.new()
	stylebox_pressed.bg_color = Color(0.1, 0.1, 0.6)
	stylebox_pressed.set_corner_radius_all(5)
	
	button.add_theme_stylebox_override("normal", stylebox_normal)
	button.add_theme_stylebox_override("hover", stylebox_hover)
	button.add_theme_stylebox_override("pressed", stylebox_pressed)
	
	return button

func connect_signals() -> void:
	if GameManager:
		GameManager.time_updated.connect(_on_time_updated)
		GameManager.game_won.connect(_on_game_won)
		GameManager.game_lost.connect(_on_game_lost)
		print("UI Manager connected to GameManager signals")
	else:
		print("ERROR: GameManager not found! Make sure it's added as an autoload.")
	
	restart_button_win.pressed.connect(_on_restart_pressed)
	restart_button_lose.pressed.connect(_on_restart_pressed)

func _on_time_updated(new_time: float) -> void:
	time_label.text = "Time: %.1f" % max(0, new_time)
	
	if new_time < 10:
		time_label.add_theme_color_override("font_color", Color(1, 0, 0))
	elif new_time < 30:
		time_label.add_theme_color_override("font_color", Color(1, 1, 0))
	else:
		time_label.add_theme_color_override("font_color", Color(1, 1, 1))

func _on_game_won() -> void:
	print("Game Won! Showing victory screen.")
	if win_panel:
		win_panel.visible = true
		get_tree().paused = true
	else:
		print("ERROR: win_panel is null!")

func _on_game_lost() -> void:
	print("Game Lost! Showing lose screen.")
	if lose_panel:
		lose_panel.visible = true
		get_tree().paused = true
	else:
		print("ERROR: lose_panel is null!")

func _on_restart_pressed() -> void:
	print("Restarting game...")
	get_tree().paused = false
	
	if GameManager:
		GameManager.reset_game()
	
	get_tree().reload_current_scene()

func hide_all_screens() -> void:
	if win_panel:
		win_panel.visible = false
	if lose_panel:
		lose_panel.visible = false

func _on_viewport_size_changed() -> void:
	if game_title:
		game_title.size = Vector2(get_viewport().size.x, 60)
	
	if win_panel and is_instance_valid(win_panel):
		win_panel.position = Vector2(
			(get_viewport().size.x / 2) - 200,
			(get_viewport().size.y / 2) - 160
		)
	if lose_panel and is_instance_valid(lose_panel):
		lose_panel.position = Vector2(
			(get_viewport().size.x / 2) - 200,
			(get_viewport().size.y / 2) - 160
		)
