extends Resource

class_name wave_resource

@export var enemy1_type : String
@export var enemy1_amount : int
@export var enemy2_type : String
@export var enemy2_amount : int
@export var enemy3_type : String
@export var enemy3_amount : int
@export var boss_type: String

func _get_wave_info():
	var enemies : Array
	enemies + _add_enemies(enemy1_type, enemy1_amount)
	enemies + _add_enemies(enemy2_type, enemy2_amount)
	enemies + _add_enemies(enemy3_type, enemy3_amount)
	if (boss_type != null):
		enemies.append(boss_type)
		
func _add_enemies(type : String, amount : int):
	var enemies : Array
	if (type != null && amount > 0):
		for n in amount:
			enemies.append(type)
	return enemies
	
