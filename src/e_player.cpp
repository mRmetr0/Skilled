#include "e_player.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void EPlayer::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_id"), &EPlayer::get_id);
    ClassDB::bind_method(D_METHOD("set_id", "p_id"), &EPlayer::set_id);
    ClassDB::add_property("EPlayer", PropertyInfo(Variant::INT, "id"), "set_id", "get_id");
}

EPlayer::EPlayer(){
    health = 5;

    if (Engine::get_singleton()->is_editor_hint())
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
}

EPlayer::~EPlayer(){}

void EPlayer::_ready(){
    Entity::_ready();
}

void EPlayer::_process(double delta) {
    emit_signal("log", this, "EPlayer");
}

#pragma region getters_setters

void EPlayer::set_id(const double p_id){
    id = p_id;
}
double EPlayer::get_id() const {
    return id;
}

#pragma endregion getters_setters