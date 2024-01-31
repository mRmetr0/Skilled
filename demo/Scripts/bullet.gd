extends Area2D

@export var speed = 300.0
@export var damage: int = 1
@export var pierce: int = 1

@onready var run_speed = speed

var has_hit = false;

func _physics_process(delta):
	position += transform.x * run_speed * delta

func _on_Bullet_area_entered(body):
	if body.is_in_group("shootables") && !has_hit:
		has_hit = true
		body._take_damage(damage)
		pierce -= 1
		if (pierce <= 0):
			_die()

func _on_Bullet_body_entered(body):
	if (body.is_in_group("shootables")):
		has_hit = true
		_die();

func _set_bullet(p_damage: int, p_pierce: int, p_crit: int = 0):
	has_hit = false;
	run_speed = speed
	damage = p_damage;
	pierce = p_pierce;
	var crit = (p_crit + (10 if (GameManager.set_difficulty == GameManager.difficulty.HARD) else 0))
	if randi_range(0, 100)  <= crit:
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
