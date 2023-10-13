#include "entity.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void Entity::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_speed"), &Entity::get_speed);
    ClassDB::bind_method(D_METHOD("set_speed", "p_speed"), &Entity::set_speed);
    ClassDB::add_property("Entity", PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");

    ClassDB::bind_method(D_METHOD("get_health"), &Entity::get_health);
    ClassDB::bind_method(D_METHOD("set_health", "p_health"), &Entity::set_health);
    ClassDB::add_property("Entity", PropertyInfo(Variant::FLOAT, "health"), "set_health", "get_health");
}

Entity::Entity(){
    time_passed = 0.0;
    speed = 1.0;

    if (Engine::get_singleton()->is_editor_hint())
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
}

Entity::~Entity() {}

void Entity::_take_damage(int damage) {

}

void Entity::set_speed(const double p_speed){
    speed = p_speed;
}

double Entity::get_speed() const {
    return speed;
}

void Entity::set_health(const int p_health) {
    health = p_health;
}

int Entity::get_health() const {
    return health;
}