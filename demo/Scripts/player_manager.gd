extends Node

var player

func _ready():
#	var characters = get_children()
#	for child in characters:
#		child.queue_free()
#		remove_child(child)
	var player_scene = load(str("res://Scenes/Objects/Characters/Hero.tscn"))
	var battle_ui_scene = load(str("res://Scenes/Objects/battle_ui.tscn"))
	var battle_ui = battle_ui_scene.instantiate();
	player = player_scene.instantiate()
	add_child(player)
	get_node("/root/Main/Camera2D/UILayer").add_child(battle_ui)
	player.position = get_node("SpawnPoint").position
	GameManager._set_player_data(player)
	
#	if get_tree().get_current_scene().get_name() != "start_stage.tscn":
#		for hazard in GameManager.selected_hazards:
#			GameManager._apply_hazard(hazard)
#		GameManager.selected_hazards.clear()
	
func _get_player():
	if (player == null):
		print("NO PLAYER???")
	return player

func _game_over():
	GameManager._on_game_end(false)
