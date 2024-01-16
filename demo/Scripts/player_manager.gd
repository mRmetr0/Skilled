extends Node

var player

@export var rewards : Array[reward_resource]

func _ready():
	var player_scene = load(str("res://Scenes/Objects/Characters/Hero.tscn"))
	var battle_ui_scene = load(str("res://Scenes/Objects/battle_ui.tscn"))
	var battle_ui = battle_ui_scene.instantiate();
	player = player_scene.instantiate()
	add_child(player)
	get_node("/root/Main/Camera2D/UILayer").add_child(battle_ui)
	player.position = get_node("SpawnPoint").position
	GameManager._set_player_data(player)
	
func _get_player():
	if (player == null):
		print("NO PLAYER???")
	return player

func _game_over():
	GameManager._on_game_end(false)
