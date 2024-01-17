extends Resource

class_name wave_resource

@export var path = "res://Scenes/Objects/Characters/"
@export var give_reward_at_end: bool

@export_category("SpawnDataNormal")
@export var spawn_amount_max_normal : int
@export var spawn_rate_normal : float

@export_category("SpawnDataHard")
@export var spawn_amount_max_hard : int
@export var spawn_rate_hard : float

@export_category("Enemies")
@export var enemy1_type : String
@export var enemy1_amount : int
@export var enemy1_amount_hard : int
@export var enemy2_type : String
@export var enemy2_amount : int
@export var enemy2_amount_hard : int
@export var enemy3_type : String
@export var enemy3_amount : int
@export var enemy3_amount_hard : int
@export var boss_type: String

var enemies : Array[String]
var spawn_amount_max : int
var spawn_rate: float

func _get_wave_info():	
	if (GameManager.set_difficulty != GameManager.difficulty.HARD):
		spawn_amount_max = spawn_amount_max_normal
		spawn_rate = spawn_rate_normal
		_add_enemies(enemy1_type, enemy1_amount)
		_add_enemies(enemy2_type, enemy2_amount)
		_add_enemies(enemy3_type, enemy3_amount)
	else:
		spawn_amount_max = spawn_amount_max_hard
		spawn_rate = spawn_rate_hard
		_add_enemies(enemy1_type, enemy1_amount_hard)
		_add_enemies(enemy2_type, enemy2_amount_hard)
		_add_enemies(enemy3_type, enemy3_amount_hard)
	enemies.shuffle()
	if (boss_type != ""):
		enemies.push_front(path + boss_type + ".tscn")
		
func _add_enemies(type : String, amount : int):
	if (type != "" && amount > 0):
		for n in amount:
			enemies.append(path + type + ".tscn")
	
