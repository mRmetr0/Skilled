#include "entity.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void Entity::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_health"), &Entity::get_health);
    ClassDB::bind_method(D_METHOD("set_health", "p_health"), &Entity::set_health);
    ClassDB::add_property("Entity", PropertyInfo(Variant::INT, "health"), "set_health", "get_health");

    ClassDB::bind_method(D_METHOD("get_speed"), &Entity::get_speed);
    ClassDB::bind_method(D_METHOD("set_speed", "p_speed"), &Entity::set_speed);
    ClassDB::add_property("Entity", PropertyInfo(Variant::FLOAT, "speed"), "set_health", "get_speed");


    ADD_SIGNAL(MethodInfo("log", PropertyInfo(Variant::OBJECT, "node"), PropertyInfo(Variant::STRING, "message")));
}

Entity::Entity(){
    speed = 100;
    health = 1;

    if (Engine::get_singleton()->is_editor_hint())
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
}

Entity::~Entity(){
}

void Entity::_ready(){
    hp_bar = Object::cast_to<ProgressBar>(get_node_or_null(NodePath("ProgressBar")));
}

void Entity::_take_damage(int p_damage) {
    health -= p_damage;
    //hit_stun = 2.0;
    if (health <= 0) {
        get_parent()->call("_remove_character", this);
        queue_free();
        return;
    }
	hp_bar->call("_health_update", health);
}

#pragma region getters_setters

void Entity::set_health(const int p_health) {
    health = p_health;
}
int Entity::get_health() const {
    return health;
}

void Entity::set_speed(const double p_speed) {
    speed = p_speed;
}

double Entity::get_speed() const {
    return speed;
}

#pragma endregion getters_setters