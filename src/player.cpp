#include "player.h"
#include "enemy.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/world2d.hpp>

using namespace godot;

void Player::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_health"), &Player::get_health);
    ClassDB::bind_method(D_METHOD("set_health", "p_health"), &Player::set_health);
    ClassDB::add_property("Player", PropertyInfo(Variant::INT, "health"), "set_health", "get_health");

    ClassDB::bind_method(D_METHOD("get_normal_speed"), &Player::get_normal_speed);
    ClassDB::bind_method(D_METHOD("set_normal_speed", "p_speed"), &Player::set_normal_speed);
    ClassDB::add_property("Player", PropertyInfo(Variant::FLOAT, "normal_speed"), "set_normal_speed", "get_normal_speed");

    ClassDB::bind_method(D_METHOD("get_slow_speed"), &Player::get_slow_speed);
    ClassDB::bind_method(D_METHOD("set_slow_speed", "p_speed"), &Player::set_slow_speed);
    ClassDB::add_property("Player", PropertyInfo(Variant::FLOAT, "slow_speed"), "set_slow_speed", "get_slow_speed");

    ClassDB::bind_method(D_METHOD("get_id"), &Player::get_id);
    ClassDB::bind_method(D_METHOD("set_id", "p_id"), &Player::set_id);
    ClassDB::add_property("Player", PropertyInfo(Variant::INT, "id"), "set_id", "get_id");

    ClassDB::bind_method(D_METHOD("get_frequency"), &Player::get_frequency);
    ClassDB::bind_method(D_METHOD("set_frequency", "p_frequency"), &Player::set_frequency);
    ClassDB::add_property("Player", PropertyInfo(Variant::FLOAT, "update_frequency"), "set_frequency", "get_frequency");

    ClassDB::bind_method(D_METHOD("_take_damage"), &Player::_take_damage);
    ClassDB::bind_method(D_METHOD("_set_target", "p_target"), &Player::_set_target);
    ClassDB::bind_method(D_METHOD("_get_bullets"), &Player::_get_bullets);
    ClassDB::bind_method(D_METHOD("_get_weapon_id"), &Player::_get_weapon_id);
    ClassDB::bind_method(D_METHOD("_set_weapon", "p_id", "p_ammo"), &Player::_set_weapon);

    ADD_SIGNAL(MethodInfo("animate", PropertyInfo(Variant::INT, "type")));
}

Player::Player(){
    id = -1;
    time_passed = 0.0;
    normal_speed = 400.0;
    slow_speed = 250.0;
    move_speed = normal_speed;
    update_frequency = 5.0;
    
    enemy = Vector2(-1,-1);
    target = get_position();
    crate = Vector2i(0,0);

    health = 5;
    health_max = 5;
    is_player = false;
    weapon_state = nullptr;

    if (Engine::get_singleton()->is_editor_hint())
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
}

Player::~Player() {
    if (weapon_state != nullptr)
        memdelete(weapon_state);
}

void Player::_ready(){
    target = get_position();

    input = Input::get_singleton();
    tile_map = Object::cast_to<TileMap>(get_node_or_null(NodePath("/root/Main/TileMap")));
    hp_bar = Object::cast_to<ProgressBar>(get_node_or_null(NodePath("ProgressBar")));
    if (hp_bar != nullptr)
        hp_bar->call("_set_health", health_max, health);

    weapon_state = memnew(PistolState);
    weapon_state->start(*this);
}

void Player::_process(double delta){
    astar_set();

    WeaponState* new_weapon = weapon_state->update(*this, delta);
    if (new_weapon != nullptr){
        memdelete(weapon_state);
        new_weapon->start(*this);
        weapon_state = new_weapon;
    }

    //Player slows down when hit
    if (hit_stun > 0){  
        hit_stun -= delta;
        move_speed = slow_speed;
    } else {
        move_speed = normal_speed;
    }
}

void Player::_physics_process(double delta){
    astar_move(delta);
}
void Player::astar_set(){
    if (input->is_action_just_pressed("LClick")){
        PackedVector2Array new_path;
        if (is_ground()){
            new_path = tile_map->call("_get_path_raw", get_position(), tile_map->get_local_mouse_position());
            crate = Vector2i(0,0);
        }else{
            new_path = tile_map->call("_get_path_adjacent_raw", get_position(), tile_map->get_local_mouse_position());
            crate = tile_map->local_to_map(tile_map->get_local_mouse_position());
        }
        
        if (new_path.size() > 0) 
        {
            path = new_path;
            progress = 0;
        }
    } 
}

void Player::astar_move(double delta){
    if (crate != Vector2i(-1,-1) && progress >= path.size()){
        if (tile_map != nullptr)
            tile_map->call("_heal_tile_raw", crate);
        crate = Vector2i(-1,-1);
        return;
    }
    if (path.size() == 0 || progress >= path.size()) {
        emit_signal("animate", 0);
        return;
    }
    emit_signal("animate", 1);

    target = tile_map->map_to_local(path[progress]);

    Vector2 velocity = get_position().direction_to(target)* move_speed;

    //If at node, changes the target. Gets stopped next loop if required
    if (get_position().distance_to(target) < 10) progress++;

    set_position(get_position() + velocity * delta);
}

void Player::_take_damage(int p_damage){
    health -= p_damage;
    hit_stun = 2.0;
    if (health <= 0) {
        get_parent()->call("_game_over");
        return;
    }
	hp_bar->call("_health_update", health);
}

bool Player::is_ground(){
    if (tile_map->get_cell_source_id(0, tile_map->local_to_map(tile_map->get_global_mouse_position())) == 0)
        return true;
    if (tile_map->get_cell_atlas_coords(0, tile_map->local_to_map(tile_map->get_global_mouse_position())).x == 0)
        return true;
    return false;
}

void Player::_set_target(Vector2 p_target){
    
    PackedVector2Array new_path;

    new_path = tile_map->call("_get_path_raw", get_position(), p_target);
    crate = Vector2i(-1,-1);
    
    if (new_path.size() > 0) 
    {
        path = new_path;
        progress = 0;
    }
}

Vector2i Player::_get_bullets(){
    return weapon_state->_get_bullets();
}

int Player::_get_weapon_id(){
    return weapon_state->id;
}

void Player::_set_weapon(int p_id, int p_ammo = 0){
    WeaponState* new_state;
    switch (p_id) {
    case 1:
        memdelete(weapon_state);
        new_state = memnew(PistolState);
        new_state->start(*this, p_ammo);
        weapon_state = new_state;
        break;
    case 2:
        memdelete(weapon_state);
        new_state = memnew(AutoState);
        new_state->start(*this, p_ammo);
        weapon_state = new_state;
        break;
    case 3:
        memdelete(weapon_state);
        new_state = memnew(SpreadState);
        new_state->start(*this, p_ammo);
        weapon_state = new_state;
        break;
    default:
        return;
        break;
    }
}

#pragma region getters_setters
void Player::set_frequency(const double p_frequency){
    update_frequency = p_frequency;
}

double Player::get_frequency() const {
    return update_frequency;
}

void Player::set_health(const int p_health) {
    health = CLAMP(p_health, 0, health_max);
}
int Player::get_health() const {
    return health;
}

void Player::set_normal_speed(const double p_speed){
    normal_speed = p_speed;
}
double Player::get_normal_speed() const {
    return normal_speed;
}

void Player::set_slow_speed(const double p_speed){
    slow_speed = p_speed;
}
double Player::get_slow_speed() const {
    return slow_speed;
}

void Player::set_id(const double p_id){
    id = p_id;
}
double Player::get_id() const {
    return id;
}

#pragma endregion getters_setters