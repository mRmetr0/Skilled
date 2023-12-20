extends Node

@export var pool_size = 25
@onready var bullet = preload("res://Scenes/Objects/bullet.tscn")

var pool :Array
var index= -1

func _ready():
	for i in pool_size:
		var b = bullet.instantiate()
		pool.append(b)

func _get_bullet(): 
	index = wrapi(index + 1, 0, pool_size)
	var pool_bullet = pool[index]
	if (pool_bullet.get_parent() == self):
		_return_bullet(pool_bullet)
	add_child(pool_bullet);
	return pool_bullet
	
func _clear_all_bullets():
	for b in get_children():
		remove_child(b)
	
func _return_bullet(p_bullet, on_collision = false):
	if (on_collision):
		call_deferred("remove_child", p_bullet)
	else:
		remove_child(p_bullet);
	
