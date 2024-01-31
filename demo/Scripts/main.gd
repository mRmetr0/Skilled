extends Node

class_name Main

@export var dev_mode = false
var paused = false
@onready var pause_menu = get_node("UILayer/PauseControl")

func _ready():
	pause_menu.hide()

func _input(event):
	if event.is_action_pressed("reload_scene") && dev_mode:
		get_tree().reload_current_scene()
	if event.is_action_pressed("pause"):
		_set_pause(!paused)
	if event.is_action_pressed("backspace") && paused:
		get_tree().change_scene_to_file("res://Scenes/Menus/main_menu.tscn")
		get_tree().paused = false # TODO: MAKE PAUSE MENU WORK WITH BUTTONS INSTEAD OF KEY INPUTS

func _set_pause(set_paused):
	get_tree().paused = set_paused
	if set_paused:
		pause_menu.show()
	else:
		pause_menu.hide()
	paused = set_paused
