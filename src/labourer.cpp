#include "labourer.h"

using namespace godot;

void Labourer::_bind_methods(){
    ClassDB::bind_method(D_METHOD("get_health"), &Labourer::get_health);
    ClassDB::bind_method(D_METHOD("set_health", "p_health"), &Labourer::set_health);
    ClassDB::add_property("Labourer", PropertyInfo(Variant::INT, "health"), "set_health", "get_health");

    ClassDB::bind_method(D_METHOD("get_speed"), &Labourer::get_speed);
    ClassDB::bind_method(D_METHOD("set_speed", "p_speed"), &Labourer::set_speed);
    ClassDB::add_property("Labourer", PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");
}

Labourer::Labourer(){
    state = MOVING;
    progress = 0;
    
    health = 5;
    health_max = 5;
    move_speed = 300;
    
    time_passed = 0.0;
    packing_time = 5.0;
}

Labourer::~Labourer(){}

void Labourer::_ready(){
    health_max = health;

    tile_map = Object::cast_to<TileMap>(get_node_or_null(NodePath("/root/Main/TileMap")));
    hp_bar = Object::cast_to<ProgressBar>(get_node_or_null(NodePath("ProgressBar")));
    if (hp_bar != nullptr)
        hp_bar->call("_set_health", health_max, health);
    if (tile_map != nullptr){
        resource_start = tile_map->call("_get_resource", true);
        resource_end = tile_map->call("_get_resource", false);
        // resource_start = resource_end = Vector2i(0,0);
        astar_set(false);
    }

}

void Labourer::_physics_process(double delta){
    switch (state) {
        case PACKING:
            packing_update(delta);
            break;
        case MOVING:
            astar_move(delta);
            break;
        default:
            break;
    }
}

void Labourer::packing_update(double delta){
    time_passed += delta;
    if (time_passed < packing_time) return;
    time_passed = 0.0;
    get_parent()->call("_set_resource", resource);
    // astar_set(false);
    state = MOVING;
}

void Labourer::astar_set(bool to_start){
    Vector2 next_target = to_start ? resource_start : resource_end;
    PackedVector2Array new_path;
    new_path = tile_map->call("_get_path_adjacent_raw", get_position(), tile_map->map_to_local(next_target));

    if (new_path.size() <= 0) return;
    UtilityFunctions::print(new_path);
    path = new_path;
    progress = 0;
}

void Labourer::astar_move(double delta){
    if (path.size() == 0 || progress >= path.size()) {
        emit_signal("animate", 0);
        state = PACKING;
        return;
    }
    emit_signal("animate", 1);

    Vector2 target = tile_map->map_to_local(path[progress]);

    Vector2 velocity = get_position().direction_to(target)* move_speed;

    //If at node, changes the target. Gets stopped next loop if required
    if (get_position().distance_to(target) < 10) progress++;

    set_position(get_position() + velocity * delta);
}

void Labourer::_take_damage(int p_damage){
    health -= p_damage;
    if (health <= 0) {
        get_parent()->call("_game_over");
        return;
    }
	hp_bar->call("_health_update", health);
}
#pragma region getters_setters

void Labourer::set_health(const int p_health) {
    health = CLAMP(p_health, 0, health_max);
}
int Labourer::get_health() const {
    return health;
}

void Labourer::set_speed(const double p_speed){
    move_speed = p_speed;
}
double Labourer::get_speed()const{
    return move_speed;
}

#pragma endregion getters_setters