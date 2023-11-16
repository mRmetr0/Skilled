extends Area2D

@export var speed = 300.0
@export var damage: int = 1
@export var pierce: int = 1

@onready var run_speed = speed

func _physics_process(delta):
	position += transform.x * run_speed * delta

func _on_Bullet_body_entered(body):
	if body.is_in_group("shootables"):
		body._take_damage(damage)
		pierce -= 1
		if (pierce <= 0):
			_die()

func _set_bullet(p_damage: int, p_pierce: int, p_crit: int = 0):
	run_speed = speed
	damage = p_damage;
	pierce = p_pierce;
	if randi_range(0, 100) <= p_crit:
		damage *= 2
		get_node("Sprite2D").modulate = Color(1, 0, 0)
	else:
		get_node("Sprite2D").modulate = Color(1, 1, 1)

func _on_exit_screen():
	_die()

func _die():
	if BulletPool.pool.has(self):
		BulletPool._return_bullet(self, true)
	else:
		print("NOT PART OF POOL, DELETING")
		queue_free()
