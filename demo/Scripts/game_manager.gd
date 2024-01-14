extends Node

var player_health = 5
var player_weapon_id : int = 1
var player_weapon_ammo : int = -1
var statue_health = 10

enum hazards {
	DARK,
	LARGER_HOARDES,
	SLOW_TERRAIN
}
var selected_hazards = []
var hazard_odds = 33;

func _ready():
	_start_game()
	
func _start_game():
	player_health = 0
	player_weapon_id = -1
	player_weapon_ammo = 1
	statue_health = 10
	
	selected_hazards.clear()

func  _on_wave_end():
	get_node("/root/Main/Camera2D/UILayer/battle_ui")._update_wave()
	get_node("/root/Main/Camera2D")._next_zoom()

func _on_game_end(has_won):
	BulletPool._clear_all_bullets()
	if has_won:
		get_tree().change_scene_to_file("res://Scenes/Menus/win_menu.tscn")
	else:
		get_tree().change_scene_to_file("res://Scenes/Menus/lose_menu.tscn")

		
## PLAYER DATA TRANSFER
func _save_player_data(player = null):
	if player == null:
		var player_manager = get_node("/root/Main/PlayerManager")
		player = player_manager._get_player()
		if (player == null):
			print("COULD NOT FIND PLAYER")
			return 
	player_health = player.health
	player_weapon_id = player._get_weapon_id()
	
	if (player_weapon_id == 1):
		player_weapon_ammo = 0;
	else:
		player_weapon_ammo = player._get_bullets().x;
		
func _set_player_data(player):
	if player_health <= 0:
		return
	player.health = player_health
	player.get_node("ProgressBar")._health_update(player.health)
	player._set_weapon(player_weapon_id, player_weapon_ammo);
	

#Hazard related
func _set_hazards(set_hazards):
	var hazards_to_set = []
	for hazard in set_hazards:
		if (randi() % 100 <= hazard_odds):
			hazards_to_set.append(hazard)
	selected_hazards = hazards_to_set
	return hazards_to_set
		
func _apply_hazard(hazard):
	match(hazard):
		hazards.DARK:
			var darkness_scene = load(str("res://Scenes/Objects/darkness.tscn"))
			var darkness = darkness_scene.instantiate()
			var player = get_node("/root/Main/PlayerManager/Hero")
			player.add_child(darkness)
		hazards.SLOW_TERRAIN:
			var player = get_node("/root/Main/PlayerManager/Hero")
			player.normal_speed -= 100
			player.slow_speed -= 50
		hazards.LARGER_HOARDES:
			var enemy_manager = get_node("/root/Main/EnemyManager")
			enemy_manager.horde_amount += 5
			enemy_manager.spawn_amount_max += 2
