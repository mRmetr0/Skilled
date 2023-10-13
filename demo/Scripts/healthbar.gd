extends ProgressBar

@onready var reload_bar = get_node("ReloadBar")
@onready var time_passed = 0.0
@onready var reload_on = false
var time

func _ready():
	max_value = get_parent().health
	value = max_value
	reload_bar.set_visible(false)
	
func _process(delta):
	if (!reload_on):
		return
	if time_passed < time:
		time_passed += delta
		reload_bar.value = (time_passed / time) * 100
	else:
		_set_reload(-1.0)
		

func _health_update(p_value: int):
	value = p_value;
	if (value <= 0):
		queue_free();

func _set_active(active: bool):
	var new_color
	if (active):
		new_color = Color(1.0, 0.0, 1.0)
	else:
		new_color = Color(1.0, 0.0, 0.0)
	self_modulate = new_color
		
func _set_reload(p_time = -1.0):
	var active = (p_time != -1.0)
	if active:
		time_passed = 0.0
		time = p_time
	reload_bar.set_visible(active)
	reload_on = active
