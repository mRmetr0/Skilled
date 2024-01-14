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

    ClassDB::bind_method(D_METHOD("_take_damage"), &Player::_take_damage);
    ClassDB::bind_method(D_METHOD("_get_bullets"), &Player::_get_bullets);
    ClassDB::bind_method(D_METHOD("_get_weapon_id"), &Player::_get_weapon_id);
    ClassDB::bind_method(D_METHOD("_set_weapon", "p_id", "p_ammo"), &Player::_set_weapon);

    ADD_SIGNAL(MethodInfo("animate", PropertyInfo(Variant::INT, "type")));
}

Player::Player(){
    time_passed = 0.0;
    normal_speed = 400.0;
    slow_speed = 250.0;
    move_speed = normal_speed;

    health = 5;
    health_max = 5;
    weapon_state = nullptr;

    move_dir = Vector2(0,0);

    if (Engine::get_singleton()->is_editor_hint())
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
}

Player::~Player() {
    if (weapon_state != nullptr)
        memdelete(weapon_state);
}

void Player::_ready(){
    input = Input::get_singleton();
    hp_bar = Object::cast_to<ProgressBar>(get_node_or_null(NodePath("ProgressBar")));
    if (hp_bar != nullptr)
        hp_bar->call("_set_health", health_max, health);

    weapon_state = memnew(PistolState);
    weapon_state->start(*this);
}

void Player::_process(double delta){
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
    key_input();
    key_move(delta);
}

void Player::key_input(){
    move_dir = Vector2(0,0);
    if (input->is_action_pressed("up"))
        move_dir -= Vector2(0, 1);
    if (input->is_action_pressed("down"))
        move_dir += Vector2(0, 1);
    if (input->is_action_pressed("left"))
        move_dir -= Vector2(1, 0);
    if (input->is_action_pressed("right"))
        move_dir += Vector2(1, 0);
}
void Player::key_move(double delta){
    if (move_dir.length() == 0.0){
        emit_signal("animate", 0);
        return;
    }
    emit_signal("animate", 1);

    set_velocity(move_dir.normalized() * move_speed);
    move_and_slide();

    // set_position(get_position() + (move_dir.normalized() * move_speed * delta));
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
        break;
    }
}

#pragma region getters_setters
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

#pragma endregion getters_setters