extends Node

var characters : Array
var current_player

func _ready():
	GameManager._set_player_data()
	characters = get_children()
	var i : int = 0;
	for child in characters:
		child.id = i 
		i += 1
	if characters.size() > 0:
		current_player = characters[0]
		current_player._set_active(true)
	else:
		print("NO PLAYER!")
		
	if GameManager.new_character != null:
		# _add_character(GameManager.new_character)
		GameManager.new_character = null

func _input(event):
	if (characters.size() == 0):
		return
	if event is InputEventKey:
		current_player._set_active(false)
		var next_character_index = characters.find(current_player)
		if (Input.is_action_just_pressed("1")):
			next_character_index = 0
		elif (Input.is_action_just_pressed("2")):
			next_character_index = 1
		elif (Input.is_action_just_pressed("3")):
			next_character_index = 2
		elif (Input.is_action_just_pressed("4")):
			next_character_index = 3
		elif (Input.is_action_just_pressed("no_player")):
			pass
		if characters.size() > next_character_index:
			current_player = characters[next_character_index]
		current_player._set_active(true)
		
func _add_character(character_name):
	var player_scene = load(str("res://Scenes/Objects/Characters/" + character_name + ".tscn"))
	var player = player_scene.instantiate()
	player.id = get_children().size()
	#player._set_target(Vector2(1000, 500))
	player.position = Vector2(1000, 500)
	add_child(player)
	characters.append(player)

func _remove_character(character):
	var reset = false
	if (current_player == character):
		reset = true
	characters.erase(character)
	if (characters.size() == 0):
		get_tree().change_scene_to_file("res://Scenes/Menus/end_menu.tscn")
		return
	if reset: 
		current_player = characters[0]
		current_player._set_active(true)
	
func _get_closest_character(position):
	if (characters.size() == 0):
		return null;
	var closest = null;
	for character in characters:
		var distance = abs(character.position - position);
		if closest == null || distance < (closest.position - position):
			closest = character
	return closest

