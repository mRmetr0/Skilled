#include "enemy.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void Enemy::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_health"), &Enemy::get_health);
    ClassDB::bind_method(D_METHOD("set_health", "p_health"), &Enemy::set_health);
    ClassDB::add_property("Enemy", PropertyInfo(Variant::FLOAT, "health"), "set_health", "get_health");

    ClassDB::bind_method(D_METHOD("get_speed"), &Enemy::get_speed);
    ClassDB::bind_method(D_METHOD("set_speed", "p_speed"), &Enemy::set_speed);
    ClassDB::add_property("Enemy", PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");

    ClassDB::bind_method(D_METHOD("get_frequency"), &Enemy::get_frequency);
    ClassDB::bind_method(D_METHOD("set_frequency", "p_frequency"), &Enemy::set_frequency);
    ClassDB::add_property("Enemy", PropertyInfo(Variant::FLOAT, "update_frequency"), "set_frequency", "get_frequency");

    ClassDB::bind_method(D_METHOD("get_attack_frequency"), &Enemy::get_attack_frequency);
    ClassDB::bind_method(D_METHOD("set_attack_frequency", "p_frequency"), &Enemy::set_attack_frequency);
    ClassDB::add_property("Enemy", PropertyInfo(Variant::FLOAT, "attack_frequency"), "set_attack_frequency", "get_attack_frequency");

    ClassDB::bind_method(D_METHOD("get_attack_range"), &Enemy::get_attack_range);
    ClassDB::bind_method(D_METHOD("set_attack_range", "p_range"), &Enemy::set_attack_range);
    ClassDB::add_property("Enemy", PropertyInfo(Variant::FLOAT, "attack_range"), "set_attack_range", "get_attack_range");


    ClassDB::bind_method(D_METHOD("_take_damage"), &Enemy::_take_damage);
    ClassDB::bind_method(D_METHOD("_handle_overlap"), &Enemy::_handle_overlap);
    

    ADD_SIGNAL(MethodInfo("log", PropertyInfo(Variant::OBJECT, "node"), PropertyInfo(Variant::STRING, "message")));
}

Enemy::Enemy(){
    time_passed = 0.0;
    speed = 1.0;
    health = 5;
    target = Vector2(0,0);
    attack_range = 150.0;
    update_frequency = 1.0;
    attack_frequency = 1.5;
    attack_timer = 0.0;
    update_timer = update_frequency;
    can_attack = false;

    if (Engine::get_singleton()->is_editor_hint())
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
}

Enemy::~Enemy() {}

void Enemy::_ready() {
    player_manager = Object::cast_to<Node>(get_node_or_null(NodePath("/root/Main/PlayerManager")));
    hp_bar = Object::cast_to<ProgressBar>(get_node_or_null(NodePath("ProgressBar")));
    tile_map = Object::cast_to<TileMap>(get_node_or_null(NodePath("/root/Main/TileMap")));

    //state = memnew (StormingState);
    //enum_state = STORMING;
    health_max = health;
}

void Enemy::_process(double delta){

    // EnemyState* _state = state->update(*this, delta);
    // if (_state != nullptr){
    //     memdelete(state);
    //     state = _state;
    // }

    /**
    switch(enum_state){
        case STORMING:
            if (!can_update(delta)) return;
            check_path = tile_map->call("_get_path_raw", get_position(), player->get_position());
            if (check_path.size() > 7) {
                emit_signal("log", this, "STORMING");
                astar_storm();
            } else {
                switch_state(HUNTING);
            }
            break;
        case ATTACKING:
            player->_take_damage(1);
            switch_state(WANDERING);
            break;
        case WANDERING: 
            if (check_path.size() < 3) {
                switch_state(HUNTING);
            } else if (check_path.size() > 10) {
                switch_state(STORMING);
            }
            //TODO: WALK TO RANDOM POSITIONS
            if (!can_update(delta)) return;
            check_path = tile_map->call("_get_path_raw", get_position(), player->get_position());
            break;
        case HUNTING:
            if (!can_update(delta)) return;
            check_path = tile_map->call("_get_path_raw", get_position(), player->get_position());
            if (get_position().distance_to(player->get_position()) <= attack_range){
                switch_state(ATTACKING);
            } else {
                astar_hunt();
            }
            break;
        default: 
            break;
    }
    /**/
    /**
    if (!can_attack){
        attack_timer += delta;
        if (attack_timer >= attack_frequency){
            can_attack = true;
            attack_timer = 0.0;
        }
    }
    update_timer += delta;
    if (update_timer < update_frequency) return;
    update_timer = 0.0;

    player = Object::cast_to<Player>(player_manager->call("_get_closest_character", get_position()));

    if (player == NULL || player == nullptr){
        emit_signal("log", this, "Player NULL");
        astar_storm();
        return;
    }

    PackedVector2Array check_path = tile_map->call("_get_path_raw", get_position(), player->get_position());

    if (check_path.size() > 7){ //Stalk
        emit_signal("log", this, "STORMING");
        astar_storm();
    }  else { //HUNT
        emit_signal("log", this, "HUNTING");
        astar_hunt();
    }
    /**/

}

void Enemy::_physics_process(double delta){
    astar_move(delta);
}

void Enemy::astar_storm(){
    crate = tile_map->call("_get_wall_raw", get_position());
    if (crate == Vector2i(-1,-1)) return;
    PackedVector2Array new_path = tile_map->call("_get_path_adjacent_raw", get_position(), tile_map->map_to_local(crate));
    if (new_path.size() > 0) 
    {
        path = new_path;
        progress = 0;
    }
}

void Enemy::astar_hunt(){
    PackedVector2Array new_path = tile_map->call("_get_path_raw", get_position(), player->get_position());
    if (new_path.size() > 0) 
    {
        path = new_path;
        progress = 0;
        crate = Vector2i(-1,-1);
    }
}

void Enemy::astar_move(double delta){
    if (can_attack){
        if (crate != Vector2(-1,-1) && get_position().distance_to(tile_map->map_to_local(crate)) <= attack_range){
            tile_map->call("_damage_tile_raw", tile_map->map_to_local(crate));
            crate = Vector2i(-1,-1);
        } 
        // else if (player != nullptr && get_position().distance_to(player->get_position()) <= attack_range) {
        //     player->_take_damage(1);
        //     player == nullptr;
        // }
        can_attack = false;
    }

    if (path.size() == 0 || progress >= path.size()) return;

    target = tile_map->map_to_local(path[progress]);

    Vector2 velocity = get_position().direction_to(target)* speed;

    //If at node, changes the target. Gets stopped next loop if required
    if (get_position().distance_to(target) < 10) {
        progress++;
    }

    set_position(get_position() + velocity * delta);
}

void Enemy::_take_damage(int p_damage){
    health -= p_damage;
    if (health <= 0) {
        queue_free();
        return;
    }
	hp_bar->call("_health_update", health);
}

void Enemy::switch_state(State p_state){
    enum_state = p_state;
    update_timer = 0.0;
    can_attack = false;
    attack_timer = 0.0;

    if (path.size() > 0)
        path = tile_map->call("_get_path_raw", get_position(), get_position());
}

bool Enemy::can_update(double delta){

    if (!can_attack){
        attack_timer += delta;
        if (attack_timer >= attack_frequency){
            can_attack = true;
            attack_timer = 0.0;
        }
    }

    update_timer += delta;
    if (update_timer < update_frequency) return false;
    update_timer = 0.0;

    player = Object::cast_to<Player>(player_manager->call("_get_closest_character", get_position()));

    if (player == nullptr) {
        emit_signal("log", this, "Player NULL");
        path = tile_map->call("_get_path_raw", get_position(), get_position());
        return false;
    }

    return true;
}

void Enemy::_handle_overlap(Variant p_var){
    // Player* player = Object::cast_to<Player> (p_var);
    // if (player != nullptr) {
    //     player->_take_damage(1);
    //     player == nullptr;
    // }
}

#pragma region getters_setters

void Enemy::set_health(const int p_health) {
    health = p_health;
}

int Enemy::get_health() const {
    return health;
}

void Enemy::set_speed(const double p_speed){
    speed = p_speed;
}

double Enemy::get_speed() const {
    return speed;
}

void Enemy::set_frequency(const double p_frequency){
    update_frequency = p_frequency;
}

double Enemy::get_frequency() const {
    return update_frequency;
}

void Enemy::set_attack_range(const double p_range) {
    attack_range = p_range;
}
double Enemy::get_attack_range() const {
    return attack_range;
}

void Enemy::set_attack_frequency(const double p_speed) {
    attack_frequency = p_speed;
}
double Enemy::get_attack_frequency() const {
    return attack_frequency;
}

#pragma endregion getters_setters

#pragma region EnemyState
//BASE CLASS METHODS:
EnemyState* EnemyState::update (Enemy& enemy, double delta) {
    return nullptr;
}

void EnemyState::fixed_update(Enemy& enemy, double delta){
    return;
}
//STORMING STATE
EnemyState* StormingState::update (Enemy& enemy, double delta) {
    return nullptr;
}

void StormingState::fixed_update(Enemy& enemy, double delta){
    return;
}
//ATTACKING STATE:
EnemyState* AttackingState::update (Enemy& enemy, double delta) {
    return nullptr;
}

void AttackingState::fixed_update(Enemy& enemy, double delta){
    return;
}
//WANDERING STATE:
EnemyState* WanderingState::update(Enemy& enemy, double delta) {
//     if (enemy.check_path.size() < 3) {
//         return memnew(HuntingState);
//         //enemy.switch_state(enemy.HUNTING);
//     } else if (enemy.check_path.size() > 10) {
//         return memnew(StormingState);
//         //enemy.switch_state(enemy.STORMING);
//     }
//     //TODO: WALK TO RANDOM POSITIONS
//     if (!enemy.can_update(delta)) return nullptr;
//         enemy.check_path = enemy.tile_map->call("_get_path_raw", enemy.get_position(), enemy.player->get_position());
    return nullptr;
}
void WanderingState::fixed_update(Enemy& enemy, double delta){
    return;
}
//HUNTING STATE:
EnemyState* HuntingState::update (Enemy& enemy, double delta) {
    if (!enemy.can_update(delta)) return nullptr;
    enemy.check_path = enemy.tile_map->call("_get_path_raw", enemy.get_position(), enemy.player->get_position());
    if (enemy.get_position().distance_to(enemy.player->get_position()) <= enemy.attack_range){
        EnemyState* e = memnew(AttackingState);
        return e;
    } else {
        enemy.astar_hunt();
    }
    return nullptr;
}
void HuntingState::fixed_update(Enemy& enemy, double delta){
    return;
}
// EnemyState* HuntingState::number(EnemyState* h, double d){
//     // EnemyState* e = (EnemyState*)memalloc(sizeof(EnemyState));
//     // memnew_placement(&e, EnemyState);
//     // EnemyState* e = memnew(EnemyState);
//     // Area2D* a = memnew(Area2D);
//     return h;
// }


#pragma endregion EnemyState

Parent* Parent::update(Parent* p){
    return nullptr;
}
Parent* Child::update(Parent* p){
    //return memnew(Child);
    return nullptr;
}
Parent* Son::update(Parent* p){
    // Parent* c = memnew(Child);
    // Parent* s = memnew(Son);
    return nullptr;
}