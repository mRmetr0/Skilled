extends Resource

class_name reward_resource

enum Type{
	HEAL_PLAYER,
	HEAL_STATUE,
	GIVE_GUN,
	PISTOL_DMG_UP,
}
@export var title : String
@export var type : Type
@export var remove_after_use : bool

@export_category("GUN_TYPE")
@export var gun_id : int

# Add weapon switching, enemies drop ammo. Get weapon from rewards but dont dissapear after use. 
# Add upgrade to permanently make pistol better (more ammo, more damage)
