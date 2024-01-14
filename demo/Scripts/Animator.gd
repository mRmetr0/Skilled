extends AnimationPlayer

@export var follow_mouse = false

func _ready():
	play("Idle")
	set_process(follow_mouse)
		

func _process(_delta):
	get_parent().set_flip_h(get_viewport().get_mouse_position().x < get_parent().global_position.x )

func _set_animation(anim):
	match anim:
		0:
			play("Idle")
		1:
			play("Walking")
		2:
			play("Hurt")
		3:
			play("Walking")

func _flip(right = false):
	get_parent().set_flip_h(right);
