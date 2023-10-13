extends AStar_Path

@onready var circle = $Circle
@export var debug = false

var tile_to_save

func _input(_event):
	if (Input.is_action_just_pressed("LClick")):
		var mouse_pos = local_to_map( get_global_mouse_position())
		if used_cells.has(mouse_pos):
			var is_ground = get_cell_source_id(0, mouse_pos) == 0
			var circle_pos = local_to_map(circle.global_position)
			if is_ground:
				_get_path(circle_pos, mouse_pos)
				tile_to_save = null;
			else:
				_get_path_adjacent(circle_pos, mouse_pos)
				tile_to_save = mouse_pos
			move()
	if (Input.is_action_just_pressed("RClick")) && debug:
		var mouse_pos = local_to_map( get_global_mouse_position())
		if used_cells.has(mouse_pos):
			_damage_tile_raw(map_to_local(mouse_pos))
			#var setter = astar.is_point_disabled(id(mouse_pos))
			#astar.set_point_disabled(id(mouse_pos), !setter)
			#var newID = 0 if setter else 1
			#set_cell(0, mouse_pos, newID, Vector2i(0,0))

func move():	
	
	for p in path:
		circle.global_position = map_to_local(p)
		await get_tree().create_timer(0.1).timeout
	if tile_to_save != null:
		_heal_tile_raw(tile_to_save)
		tile_to_save = null;
