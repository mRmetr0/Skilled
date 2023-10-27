#include "worm.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void Worm::_bind_methods() {
}

Worm::Worm(){
    
    if (Engine::get_singleton()->is_editor_hint())
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
}

Worm::~Worm(){}

void Worm::_process(double delta) {
    emit_signal("log", this, "WORM");
}