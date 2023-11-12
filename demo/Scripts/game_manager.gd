extends Node

var stages = []
var current_stage = ""
var stages_till_end
var levels = []
var base_path = "res://Scenes/Stages/"

var player_health = []
var player_name = []

var total_levels = -1
var new_character = null
var available_characters

func _ready():
	_start_game()
	
func _start_game():
	stages = _load_stages()
	stages_till_end = stages.size()-1
	#stages_till_end = 0
	available_characters = _get_all_characters()

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
	_load_next_level()

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
	total_levels = levels.size()

func _load_next_level():
	if levels.size() == 0:
		get_tree().change_scene_to_file("res://Scenes/Menus/stage_select.tscn")
		return

	var scene = levels[0]
	levels.erase(scene)
	total_levels -= 1

	var enough_levels = total_levels >= 0
	var rand = randi_range(0, total_levels) == 0
	if enough_levels && rand:
		var n = randi_range(0, available_characters.size()-1)
		new_character = available_characters[n]
		available_characters.erase(new_character)
		total_levels = -1
		
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
	
func _get_all_characters():
	var files = []
	var path = "res://Scenes/Objects/Characters/"
	var dir = DirAccess.open(path)
	dir.list_dir_begin()
	while true:
		var file = dir.get_next()
		if file == "":
			break
		elif not file.begins_with("."):
			files.append(file.get_basename())
	return files
	
## PLAYER DATA TRANSFER
func _save_player_data():
	var player_manager = get_node("/root/Main/PlayerManager")
	player_health.clear()
	player_name.clear()
	for index in player_manager.get_children().size():
		var player = player_manager.get_child(index)
		if player.name.begins_with("@") || player.name.begins_with("Bullet"):
			continue
		player_health.append(player.health)
		player_name.append(player.name)


func _set_player_data():
	if player_health.size() == 0:
		print("SAVING")
		_save_player_data()
		return
	
	if player_health.size() != player_name.size():
		print("DATA DOENS LIGN UP, SOMETHING WENT WRONG")
		return
	var player_manager = get_node("/root/Main/PlayerManager")
	for n in player_manager.get_children():
		player_manager.remove_child(n)
		n.queue_free()
	for n in player_name.size():
		var player_scene = load(str("res://Scenes/Objects/Characters/" + player_name[n] + ".tscn"))
		var player = player_scene.instantiate()
		player_manager.add_child(player)
		player.health = player_health[n]
		player.get_node("ProgressBar")._health_update(player.health)
		player.position = Vector2i(800 + (n*100), 500)
