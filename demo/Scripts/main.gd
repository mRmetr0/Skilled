extends Node

func _log(node, m):
	print("NODE: "+ node.get_class() + "; LOG: " + str(m)+ ";")

func _input(event):
	if event.is_action_pressed("reload_scene"):
		get_tree().reload_current_scene()
