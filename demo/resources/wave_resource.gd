extends Resource

class_name wave_resource

@export var path = "res://Scenes/Objects/Characters/"

@export_category("SpawnData")
@export var spawn_amount_max : int
@export var spawn_rate : float

@export_category("Enemies")
@export var enemy1_type : String
@export var enemy1_amount : int
@export var enemy2_type : String
@export var enemy2_amount : int
@export var enemy3_type : String
@export var enemy3_amount : int
@export var boss_type: String

var enemies : Array[String]

func _get_wave_info():
	_add_enemies(enemy1_type, enemy1_amount)
	_add_enemies(enemy2_type, enemy2_amount)
	_add_enemies(enemy3_type, enemy3_amount)
	enemies.shuffle()
	if (boss_type != ""):
		enemies.append(boss_type)
		
func _add_enemies(type : String, amount : int):
	if (type != "" && amount > 0):
		for n in amount:
			enemies.append(path + type + ".tscn")
	
