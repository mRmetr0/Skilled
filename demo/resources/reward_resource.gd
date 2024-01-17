extends Resource

class_name reward_resource

enum Type{
	HEAL_PLAYER,
	HEAL_STATUE,
	GIVE_GUN,
}
@export var title : String
@export var type : Type
@export var remove_after_use : bool

@export_category("GUN_TYPE")
@export var gun_id : int

