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
}

Enemy::Enemy(){
    speed = 1.0;
    health = 5;
    attack_range = 150.0;
    update_frequency = 1.0;
    attack_frequency = 1.5;
    attack_timer = 0.0;
    update_timer = update_frequency;
    state = nullptr;

    if (Engine::get_singleton()->is_editor_hint())
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
}

Enemy::~Enemy() {
    if (state != nullptr)
        memdelete(state);
}

void Enemy::_ready() {
    player_manager = Object::cast_to<Node>(get_node_or_null(NodePath("/root/Main/PlayerManager")));
    hp_bar = Object::cast_to<ProgressBar>(get_node_or_null(NodePath("ProgressBar")));
    tile_map = Object::cast_to<TileMap>(get_node_or_null(NodePath("/root/Main/TileMap")));

    state = memnew (StormingState);
    health_max = health;
}

void Enemy::_process(double delta){
    EnemyState* _state = state->update(*this, delta);
    if (_state != nullptr){
        memdelete(state);
        state = _state;
    }
}

void Enemy::_physics_process(double delta){
    state->fixed_update(*this, delta);
}

void Enemy::_take_damage(int p_damage){
    health -= p_damage;
    if (health <= 0) {
        // if (state != nullptr) memdelete(state);
        queue_free();
        return;
    }
	hp_bar->call("_health_update", health);
    memdelete(state);
    state = memnew(DamagedState);
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
bool EnemyState::can_update(Enemy& enemy, double delta){
    update_timer += delta;
    if (update_timer < enemy.update_frequency) return false;
    update_timer = 0.0;

    enemy.player = Object::cast_to<Player>(enemy.player_manager->call("_get_player"));

    if (enemy.player == nullptr) {
        emit_signal("log", this, "Player NULL");
        return false;
    }
    return true;
}

EnemyState* EnemyState::update (Enemy& enemy, double delta) {
    return nullptr;
}

void EnemyState::fixed_update(Enemy& enemy, double delta){
    if (path.size() == 0 || progress >= path.size()) return;

    Vector2 target = enemy.tile_map->map_to_local(path[progress]);
    Vector2 velocity = enemy.get_position().direction_to(target)* enemy.speed;

    if (enemy.get_position().distance_to(target) < 10) progress++;
    enemy.set_position(enemy.get_position() + velocity * delta);
}
//STORMING STATE
void StormingState::set_storm(Enemy& enemy) {
    crate = enemy.tile_map->call("_get_wall_raw", enemy.get_position());
    if (crate == Vector2i(-1,-1)) return;
    PackedVector2Array new_path = enemy.tile_map->call("_get_path_adjacent_raw", enemy.get_position(), enemy.tile_map->map_to_local(crate));
    if (new_path.size() > 0) 
    {
        path = new_path;
        progress = 0;
    }
}
EnemyState* StormingState::update (Enemy& enemy, double delta) {        
    if (!can_attack){
        attack_timer += delta;
        if (attack_timer >= enemy.attack_frequency){
            can_attack = true;
            attack_timer = 0.0;
        }
    }

    if (!can_update(enemy, delta)) return nullptr;
    check_path = enemy.tile_map->call("_get_path_raw", enemy.get_position(), enemy.player->get_position());
    if (check_path.size() > 7) {
        set_storm(enemy);
    } else {
        return memnew(HuntingState);
    }
    return nullptr;
}
void StormingState::fixed_update(Enemy& enemy, double delta){
    if (crate != Vector2(-1,-1) && enemy.get_position().distance_to(enemy.tile_map->map_to_local(crate)) <= enemy.attack_range){
        enemy.tile_map->call("_damage_tile_raw", enemy.tile_map->map_to_local(crate));
        crate = Vector2i(-1,-1);
    }
    EnemyState::fixed_update(enemy, delta);
}
//ATTACKING STATE:
EnemyState* AttackingState::update (Enemy& enemy, double delta) {
    enemy.player->_take_damage(1);
    return memnew(WanderingState);
}
void AttackingState::fixed_update(Enemy& enemy, double delta){
    return;
}
//WANDERING STATE:
EnemyState* WanderingState::update(Enemy& enemy, double delta) {
    if (check_path.size() < 3) {
        return memnew(HuntingState);
    } else if (check_path.size() > 10) {
        return memnew(StormingState);
    }
//TODO: WALK TO RANDOM POSITIONS
    if (!can_update(enemy, delta)) return nullptr;
        check_path = enemy.tile_map->call("_get_path_raw", enemy.get_position(), enemy.player->get_position());
    return nullptr;
}
void WanderingState::fixed_update(Enemy& enemy, double delta){
    EnemyState::fixed_update(enemy, delta);
}
//HUNTING STATE:
void HuntingState::set_hunt(Enemy& enemy){
    PackedVector2Array new_path = enemy.tile_map->call("_get_path_raw", enemy.get_position(), enemy.player->get_position());
    if (new_path.size() > 0) 
    {
        path = new_path;
        progress = 0;
    }
}

EnemyState* HuntingState::update (Enemy& enemy, double delta) {
    if (!can_update(enemy, delta)) return nullptr;
    check_path = enemy.tile_map->call("_get_path_raw", enemy.get_position(), enemy.player->get_position());
    if (enemy.get_position().distance_to(enemy.player->get_position()) <= enemy.attack_range){
        EnemyState* e = memnew(AttackingState);
        return e;
    } else {
        set_hunt(enemy);
    }
    return nullptr;
}
void HuntingState::fixed_update(Enemy& enemy, double delta){
    EnemyState::fixed_update(enemy, delta);
}

//DAMAGED STATE:
EnemyState* DamagedState::update(Enemy& enemy, double delta){
    update_timer += delta;
    if (update_timer < hit_stun) return nullptr;
    
    enemy.player = Object::cast_to<Player>(enemy.player_manager->call("_get_player"));

    check_path = enemy.tile_map->call("_get_path_raw", enemy.get_position(), enemy.player->get_position());
    if (check_path.size() < 13)
        return memnew(HuntingState);
    else 
        return memnew(StormingState);
}
void DamagedState::fixed_update(Enemy& enemy, double delta){}

#pragma endregion EnemyState