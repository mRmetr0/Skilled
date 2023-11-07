extends Control

var stage_a = null
var stage_b = null

var selected_character = null

func _ready():
	if get_tree().get_current_scene().get_name() == "StageSelect":
		if GameManager.stages_till_end > 0:
			stage_a = GameManager._get_stage()
			stage_b = GameManager._get_stage(stage_a)
			get_node("Final_Stage").queue_free()
			get_node("StageA").icon = GameManager._get_border(stage_a)
			get_node("StageB").icon = GameManager._get_border(stage_b)
		elif GameManager.stages_till_end < 0:
			get_tree().change_scene_to_file("res://Scenes/Menus/start_menu.tscn")
		else:
			get_node("StageA").queue_free()
			get_node("StageB").queue_free()
	
	elif get_tree().get_current_scene().get_name() == "PartySelect":
		var containter = get_node("HBoxContainer")
		for i in containter.get_children().size():
			var button = containter.get_child(i)
			
			if i >= GameManager.available_characters.size():
				button.icon = null
				continue
				
			button.set_name(GameManager.available_characters[i])
			button.pressed.connect(_on_character_pressed.bind(button.name))
			

## MAIN MENU
func _on_start_pressed():
	get_tree().change_scene_to_file("res://Scenes/Menus/party_select.tscn")

func _on_options_pressed():
	pass

func _on_quit_pressed():
	get_tree().quit()

## PARTY SELECT
func _on_continue_pressed():
	if selected_character == null:
		print("please select a character first")
		get_node("selected_name").text = "please select a character first"
	else:
		GameManager.player_name.append(selected_character)
		GameManager.player_health.append(100)
		
		var i = GameManager.available_characters.find(selected_character)
		GameManager.available_characters.remove_at(i)
		
		get_tree().change_scene_to_file("res://Scenes/Stages/start_stage.tscn")

func _on_character_pressed(character = "lol"):
	selected_character = character
	get_node("selected_name").text = character
	
## LOSE SCREEN
func _on_return_pressed():
	get_tree().change_scene_to_file("res://Scenes/Menus/start_menu.tscn")

## INBETWEEN STAGES
func _on_stage_a_pressed():
	GameManager.stages_till_end -= 1
	GameManager._set_stage(stage_a)

func _on_stage_b_pressed():
	GameManager.stages_till_end -= 1
	GameManager._set_stage(stage_b)

func _on_final_stage_pressed():
	GameManager.stages_till_end -= 1
	GameManager._set_stage("TheEnd")
