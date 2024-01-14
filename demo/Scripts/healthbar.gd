extends ProgressBar

@onready var reload_bar = get_node("ReloadBar")
@onready var time_passed = 0.0
@onready var reload_on = false
var time
var update_amount = 0


func _ready():
	hide()

func _set_health(_max, current = -1):
	max_value = _max
	value = current
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
	update_amount += 1
	show()
	value = p_value;
	await get_tree().create_timer(1.0).timeout
	update_amount -= 1
	if (update_amount == 0):
		hide()
	
		
func _set_reload(p_time = -1.0):
	var active = (p_time != -1.0)
	if active:
		time_passed = 0.0
		time = p_time
	reload_bar.set_visible(active)
	reload_on = active
