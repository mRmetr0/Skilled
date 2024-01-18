extends TileMap
class_name AStar_Path

@onready var astar = AStar2D.new()
@onready var used_cells = get_used_cells(0)
@onready var rng = RandomNumberGenerator.new()

var wall_cells : Array
var statue_cells
var statue_health
@onready var statue_health_bar = get_node("ProgressBar")
@onready var statue_sprite = get_node("Sprite2D")
var player

var path : PackedVector2Array


func _ready():
	_add_points()
	_connect_points()
	statue_health = GameManager.statue_health
	statue_health_bar._set_health(statue_health, statue_health)
	await get_tree().create_timer(0.5).timeout
	player = get_node("/root/Main/PlayerManager")._get_player()
	

func _process(delta):
	if (player == null):
		return
	var index_z = 0 if player.global_position.y > 800 else 10;
	statue_sprite.set_z_index(index_z)
	
func _add_points():
	for cell in used_cells:
		astar.add_point(id(cell), cell, 1.0)
		if get_cell_source_id(0, cell) != 0:
			astar.set_point_disabled(id(cell), true)
			if (get_cell_source_id(0, cell) == 11): #register statue cells
				statue_cells = cell
				wall_cells.append(cell)
		else:	##randomize ground tile
			var cell_id = get_cell_source_id(0, cell)
			var coords = get_cell_atlas_coords(0, cell)
			var x_coord = rng.randi_range(0, 11)
			set_cell(0, cell, cell_id, Vector2i(x_coord, coords.y))
			
	
func _connect_points():
	for cell in used_cells:
		## Only adjacent movement
		var neighbors = [Vector2i(1,0), Vector2i(-1,0), Vector2i(0,1), Vector2i(0, -1)]
		for neighbor in neighbors:
			var next_cell = cell + neighbor
			if used_cells.has(next_cell):
				astar.connect_points(id(cell), id (next_cell), false)


func _get_path_raw(start, end):
	var n_start_id = astar.get_closest_point(local_to_map(start))
	var n_end = local_to_map(end)
	var n_path = astar.get_point_path(n_start_id, id(n_end))
	if (n_path.size() > 1):
		n_path.remove_at(0)
	return n_path	
	
func _get_path_adjacent_raw(start, end):
	var n_start_id = astar.get_closest_point( local_to_map(start))
	var n_end = local_to_map(end)
	if (n_start_id == id(n_end)): 
		return
		
	var neighbors = [Vector2i(-1,0), Vector2i(0,1), Vector2i(0, -1), Vector2i(1,0)]
	var best_path = astar.get_point_path(n_start_id, id(n_end))
#	var best_path = astar.get_point_path(id(n_start), id(n_end + Vector2i(1,0)))
	for neighbor in neighbors:
		if !used_cells.has( n_end + neighbor) || !astar.has_point(id(n_end + neighbor)):
			continue
		var new_path = astar.get_point_path(n_start_id, id(n_end + neighbor))
		if (new_path.size() != 0 && new_path.size() < best_path.size()) || (best_path.size() == 0 && new_path.size() > 0) : 
			best_path = new_path;
	best_path.remove_at(0)
	return best_path

func _get_wall_raw(start):
	if (wall_cells.size() == 0):
		return Vector2i(-1,-1);
		
	var closest : Vector2i
	for cell in wall_cells:
		var dist = start.distance_to(map_to_local(cell))
		if closest == Vector2i.ZERO || dist < start.distance_to(map_to_local(closest)):
			closest = cell
	return closest
	
func id (point):
	var a = point.x
	var b = point.y
	return (a+b) * (a+b+1)/2+b
	
func _damage_tile_raw(tile):
	var n_tile = local_to_map(tile)
	var value  = get_cell_atlas_coords(0, n_tile)
	var tile_id = get_cell_source_id(0, n_tile)
	if tile_id == 0:
		return		
	elif tile_id == 11:
		statue_health -= 1
		statue_health_bar._health_update(statue_health)
		if (statue_health <= 0):
			GameManager._on_game_end(false)
		return 

	if (value.x == 1):
		set_cell(0, n_tile, tile_id, Vector2i(0, value.y))
		astar.set_point_disabled(id(n_tile), false)
		wall_cells.erase(n_tile)
	elif (value.x > 1):
		set_cell(0, n_tile, tile_id, Vector2i(value.x-1, value.y))

func _heal_tile_raw(tile):
	var n_tile = tile
	var value = get_cell_atlas_coords(0, n_tile)
	var tile_id = get_cell_source_id(0, n_tile)
	if (get_cell_source_id(0, n_tile) == 10 || get_cell_source_id(0, n_tile) >= 20):
		return
	if value.x > 0 && value.x < tile_id:
		set_cell(0, n_tile, tile_id, Vector2i(value.x + 1, value.y))


## STUFF FOR GDSCRIP TESTING ##

func _get_path_adjacent(start, end):
	var neighbors = [Vector2i(-1,0), Vector2i(0,1), Vector2i(0, -1)]
	var best_path = astar.get_point_path(id(start), id(end + Vector2i(1,0)))
	for neighbor in neighbors:
		var new_path = astar.get_point_path(id(start), id(end + neighbor))
		if (new_path.size() != 0 && new_path.size() < best_path.size()) || (best_path.size() == 0 && new_path.size() > 0) : 
			best_path = new_path;
	path.clear()
	path = best_path

func _get_path(start, end):
	path.clear()
	path = astar.get_point_path(id(start),id(end))
	path.remove_at(0)
