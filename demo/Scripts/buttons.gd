extends Control

## MAIN MENU
func _on_start_pressed():
	# TODO: MAKE START SCREEN FOR DIFFERENT PLAYSTYLES/DIFFICULTIES
#	get_tree().change_scene_to_file("res://Scenes/Menus/party_select.tscn")
	get_tree().change_scene_to_file("res://Scenes/Stages/main_stage.tscn")

func _on_options_pressed():
	get_tree().change_scene_to_file("res://Scenes/Stages/tutorial_stage.tscn")

func _on_quit_pressed():
	get_tree().quit()
	
## LOSE SCREEN
func _on_return_pressed():
	get_tree().change_scene_to_file("res://Scenes/Menus/start_menu.tscn")

