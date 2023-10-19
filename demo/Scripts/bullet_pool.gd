extends Node

@export var pool_size = 20
@onready var bullet = preload("res://Scenes/Objects/bullet.tscn")

var pool :Array
var index= -1

func _ready():
	for i in pool_size:
		var b = bullet.instantiate()
		pool.append(b)
		add_child(b)
		_return_bullet(b)

func _get_bullet():
	index = wrapi(index + 1, 0, pool_size)
	return pool[index]
	
func _return_bullet(p_bullet):
	p_bullet.position = Vector2(-200, -200)
	p_bullet.run_speed = 0.0
	p_bullet.damage = 0
	
