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

## MAIN MENU
func _on_start_pressed():
	get_tree().change_scene_to_file("res://Scenes/Menus/party_select.tscn")

func _on_options_pressed():
	pass

func _on_quit_pressed():
	get_tree().quit()

## PARTY SELECT
func _on_continue_pressed():
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
