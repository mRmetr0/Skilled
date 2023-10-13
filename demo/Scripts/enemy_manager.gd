extends Node

@export var spawn_rate = 5.0
@export var horde_amount: int = 10
@export var spawn_amount_max : int = 1

@onready var enemy = preload("res://Scenes/Objects/enemy.tscn")
@onready var rng = RandomNumberGenerator.new()
@onready var time_passed = 0.0
@onready var screen = get_window().size

var done = false

func _process(delta):
	if done:
		return
		
	if horde_amount <= 0 && get_children().size() == 0:
		done = true
		await get_tree().create_timer(3.0).timeout
		GameManager._save_player_data()
		GameManager._load_next_level()
		return
			
	time_passed += delta
	if (time_passed < spawn_rate):
		return;
	time_passed = 0.0

	var max_amount = max(1, spawn_amount_max)
	var amount = randi_range(1, max_amount)
	for i in amount:
		_spawn_enemy()


func _spawn_enemy():	
	if horde_amount == 0:
		return;
		
	var init = enemy.instantiate()
	horde_amount -= 1;
	add_child(init)
	
	#make enemy spawn anywhere around the field
	var n = rng.randi_range(1, 4)
	if (n <= 2):
		var x
		if n == 1:
			x = 0
		else: 
			x = screen.x
		var y = rng.randf_range(0, screen.y)
		init.position = Vector2(x, y)
	else:
		var x = rng.randf_range(0, screen.x)
		var y
		if n == 3:
			y = 0
		else: 
			y = screen.y
		init.position = Vector2(x, y)

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
