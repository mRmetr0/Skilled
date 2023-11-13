extends Node

var player

func _ready():
	var characters = get_children()
	for child in characters:
		child.queue_free()
		remove_child(child)
	var player_scene = load(str("res://Scenes/Objects/Hero.tscn"))
	player = player_scene.instantiate()
	add_child(player)
	player.position = Vector2i(1000, 500)
	
	
func _get_closest_character(position):
	if (player == null):
		print("NO PLAYER???")
	return player;

