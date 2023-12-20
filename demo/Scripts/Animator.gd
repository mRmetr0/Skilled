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

func _input(event):
	if event.is_action_pressed("1"): #IDLE
		_set_animation(0)
	if event.is_action_pressed("2"): #WALKING
		_set_animation(1)
	if event.is_action_pressed("3"): #HURT
		_set_animation(2)
	if event.is_action_pressed("4"): #DIE
		_set_animation(3)

func _flip(right = false):
	get_parent().set_flip_h(right);
