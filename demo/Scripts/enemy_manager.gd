extends Node

#@onready var enemy = preload("res://Scenes/Objects/Characters/enemy.tscn")

@onready var weapon_drop = preload("res://Scenes/Objects/Weapons/weapon_pickup.tscn")

@onready var rng = RandomNumberGenerator.new()
@onready var time_passed = 0.0
@onready var screen = get_window().size

@export var wave_data : Array[wave_resource]
var current_wave : wave_resource
var wave_progress = 0

@export_category("SpawnLocations")
@export var Up = true
@export var Down = true
@export var Left = true
@export var Right = true
var spawn_directions = []

@export_category("Other")
enum objectives {
	SURVIVE,
	PROTECT
}
@export var objective = objectives.SURVIVE
@export var loot_moved_to_win = 5
var loot_moved = 0

func _get_closest_enemy(position):
	var enemies = get_children()
	if enemies.size() == 0:
		return null;
	var closest = null
	for entity in enemies:
		var dist = abs (entity.position - position)
		if closest == null || dist < (closest.position - position):
			closest = entity;
	return closest.position	

func _ready ():
	if Up:
		spawn_directions.append(Vector2(0, -1))
	if Down:
		spawn_directions.append(Vector2(0, 1))
	if Left:
		spawn_directions.append(Vector2(-1, 0))
	if Right:
		spawn_directions.append(Vector2(1, 0))
	
	current_wave = wave_data[0]
	current_wave._get_wave_info()
	
func _process(delta):		
#	screen = get_window().size * (1/get_node("/root/Main/Camera2D")._get_zoom())
		
	_check_win_condition()
			
	time_passed += delta
	if (time_passed < current_wave.spawn_rate):
		return;
	time_passed = 0.0

	var max_amount = max(1, current_wave.spawn_amount_max)
	var amount = randi_range(1, max_amount)
	for i in amount:
		_spawn_enemy()


func _spawn_enemy():	
	if current_wave.enemies.size() == 0:
		return;
		
	var init = load(current_wave.enemies[0]).instantiate();
	current_wave.enemies.remove_at(0)
	add_child(init)
	
	#Spawn locations 2.0
	var m = rng.randi_range(0, spawn_directions.size()-1)
	var new_pos = spawn_directions[m]
	new_pos.x *= screen.x/2
	new_pos.y *= screen.y/2
	if new_pos.x == 0:
		new_pos.x =  rng.randf_range(0, screen.x)
		new_pos.y += screen.y/2
	else:
		new_pos.x += screen.x/2
		new_pos.y =  rng.randf_range(0, screen.y)
			
	init.position = new_pos

func _remove_enemy(enemy):
#	var drop = randf_range(0, 100) <= drop_rate
#	print(drop)
#	if (drop):
#		var weapon = weapon_drop.instantiate()
#		get_parent().call_deferred("add_child", weapon)
#		weapon.position = enemy.position
	enemy.queue_free()

func _check_win_condition():
	match objective:
		objectives.SURVIVE:
			if current_wave.enemies.size() <= 0 && get_children().size() == 0:
				_end_wave()
		objectives.PROTECT:
			if (loot_moved >= loot_moved_to_win):
				_end_wave()

func _end_wave():
	await get_tree().create_timer(3.0).timeout
	
	wave_data.remove_at(0)
	
	if wave_data.size() == 0:
		GameManager._on_game_end()
		return 
	else:
		current_wave = wave_data[0]
		current_wave._get_wave_info()
		GameManager._on_wave_end()
#	screen = get_node("/root/Main/Camera2D")._get_size();
	
func _set_resource(at_end):
	if (at_end):
		loot_moved += 1
		get_node("root/Main/TileMap")._change_resources_states(at_end, int(4 * (loot_moved / loot_moved_to_win)))
	else:
		get_node("root/Main/TileMap")._change_resources_states(!at_end, 4 - int(4 * (loot_moved / loot_moved_to_win)))
