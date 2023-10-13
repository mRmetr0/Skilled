extends Area2D

@export var speed = 300.0
@export var damage: int = 1
@export var pierce: int = 1

func _ready():
	pass

func _physics_process(delta):
	position += transform.x * speed * delta

func _on_Bullet_body_entered(body):
	if body.is_in_group("shootables"):
		body._take_damage(damage)
		pierce -= 1
		if (pierce <= 0):
			queue_free()
		
func _set_bullet(p_damage: int, p_pierce: int, p_crit: int = 0):
	damage = p_damage;
	pierce = p_pierce;
	if randi_range(0, 100) <= p_crit:
		damage *= 2
		get_node("Sprite2D").modulate = Color(1, 0, 0)

func _on_exit_screen():
	queue_free()
