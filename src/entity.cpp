#include "entity.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void Entity::_bind_methods() {
    ADD_SIGNAL(MethodInfo("log", PropertyInfo(Variant::OBJECT, "node"), PropertyInfo(Variant::STRING, "message")));
}

Entity::Entity(){
    if (Engine::get_singleton()->is_editor_hint())
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
}

Entity::~Entity(){}

void Entity::_ready(){
    hp_bar = Object::cast_to<ProgressBar>(get_node_or_null(NodePath("ProgressBar")));
}

void Entity::_process(double delta){
    emit_signal("log", this, "ENTITY");
}

void Entity::_physics_process(double delta) {

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