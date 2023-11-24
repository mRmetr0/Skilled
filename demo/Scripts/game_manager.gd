extends Node

var stages = []
var current_stage = ""
var stages_till_end
var levels = []
var briefing_level = ""
var base_path = "res://Scenes/Stages/"

var player_health = 0
var player_weapon_id : int = 1
var player_weapon_ammo : int = -1

enum hazards {
	DARK,
	STRONGER_ENEMIES,
	SLOW_TERRAIN
}
var selected_hazards = []
var hazard_odds = 100;

func _ready():
	_start_game()
	
func _start_game():
	stages = _load_stages()
	stages_till_end = stages.size()-1

func _get_stage(divert_from = null):
	var selection = []
	for stage in stages:
		selection.append(stage)
	if selection.size() < 2:
		print("NOT ENOUGH STAGES GO TO FINAL STAGE")
		return null
	if (divert_from != null):
		selection.erase(divert_from)
	selection.shuffle()
	return selection[0]

func _set_stage(selected_stage):
	current_stage = selected_stage
	stages.erase(selected_stage)
	_load_levels()
	get_tree().change_scene_to_file("res://Scenes/Menus/manage_menu.tscn")

func _load_stages():
	var files = []
	var dir = DirAccess.open(base_path)
	dir.list_dir_begin()
	while true:
		var file = dir.get_next()
		if file == "":
			break
		elif not file.begins_with(".") && file.get_extension() == "" && !file.begins_with("TheEnd"):
			files.append(file)
	dir.list_dir_end()
	return files;

func _load_levels():
	if current_stage == "":
		print("NO SELECTED STAGE, CANNOT LOAD LEVELS")
		return
	var files = []
	var final_level = null
	var path = str(base_path + current_stage + "/")
	var dir = DirAccess.open(path)
	dir.list_dir_begin()
	while true:
		var file = dir.get_next()
		if file == "":
			break
		elif not file.begins_with(".") && file.get_extension() == "tscn":
			if file.ends_with("final"):
				final_level = file
			else:
				files.append(path + file)
	dir.list_dir_end()
	levels = files
	levels.shuffle()
	if final_level != null:
		levels.append(final_level)

func _on_game_end():
	if levels.size() == 0:
		get_tree().change_scene_to_file("res://Scenes/Menus/stage_select.tscn")
		return
	briefing_level = levels[0]
	get_tree().change_scene_to_file("res://Scenes/Menus/manage_menu.tscn")

func _load_next_level():
	if levels.size() == 0:
		get_tree().change_scene_to_file("res://Scenes/Menus/stage_select.tscn")
		return

	var scene = levels[0]
	levels.erase(scene)
		
	get_tree().change_scene_to_file(scene)

func _get_border(stage):
	if stage == "":
		print("NO SELECTED STAGE, CANNOT GET BORDER")
		return
	var border
	var path = str(base_path + stage + "/")
	var dir = DirAccess.open(path)
	dir.list_dir_begin()
	while true:
		var file = dir.get_next()
		if file == "":
			break
		elif file.get_extension() == "png":
			border = file
	dir.list_dir_end()
	return load(str(path + border))
		
## PLAYER DATA TRANSFER
func _save_player_data(player = null):
	if player == null:
		var player_manager = get_node("/root/Main/PlayerManager")
		player = player_manager._get_player()
	player_health = player.health
	player_weapon_id = player._get_weapon_id()
	if (player_weapon_id == 1):
		player_weapon_ammo = 0;
	else:
		player_weapon_ammo = player._get_bullets().x;
		
func _set_player_data(player):
	if player_health <= 0:
		_save_player_data(player)
		return

	player.health = player_health
	player.get_node("ProgressBar")._health_update(player.health)
	player._set_weapon(player_weapon_id, player_weapon_ammo);

#Hazard related
func _set_hazards(set_hazards):
	var hazards_to_set = []
	for hazard in set_hazards:
		if (randi() % 100 <= hazard_odds):
			hazards_to_set.append(hazard)
	selected_hazards = hazards_to_set
	return hazards_to_set
		

func _apply_hazard(hazard):
	match(hazard):
		hazards.DARK:
			var darkness_scene = load(str("res://Scenes/Objects/darkness.tscn"))
			var darkness = darkness_scene.instantiate()
			var player = get_node("/root/Main/PlayerManager/Hero")
			player.add_child(darkness)
			print ("setting darkness")
		hazards.STRONGER_ENEMIES:
			return
		hazards.SLOW_TERRAIN:
			return
		
		
