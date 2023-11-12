#ifndef ENEMY_H
#define ENEMY_H

#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/progress_bar.hpp>
#include <godot_cpp/classes/tile_map.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
// #include <godot_cpp/classes/node3d.hpp>

#include "player.h"

namespace godot {

class EnemyState;

class Enemy : public Area2D {
    GDCLASS(Enemy, Area2D)

private:
    Node* player_manager;
    ProgressBar* hp_bar;

    int health_max;




protected:
    static void _bind_methods();

public:
    //FOR TESTING IN PUBLIC
    PackedVector2Array path;
    Vector2 target;
    Vector2i crate;

    double time_passed;
    double speed;
    double update_frequency;
    double update_timer;

    double attack_frequency;
    double attack_timer;

    int progress;
    bool can_attack;
    //FOR TESTING IN PUBLIC

    enum State {
        STALKING,
        ATTACKING,
        STORMING,
        WANDERING,
        HUNTING,
    };

    State enum_state;

    PackedVector2Array check_path;
    TileMap* tile_map;
    Player* player;
    EnemyState* state;

    int health;
    double attack_range;
    
    Enemy();
    ~Enemy();
    
    void _ready();
    void _process(double delta);
    void _physics_process(double delta);

    void _take_damage(int p_damage);
    void _handle_overlap(Variant p_var);
    bool can_update(double delta);

    //Might delete later
    void astar_move(double delta);
    void astar_storm();
    void astar_hunt();
    void switch_state(State p_state);

    void set_health(const int p_health);
    int get_health() const;
    void set_speed(const double p_speed);
    double get_speed() const;
    void set_frequency(const double p_speed);
    double get_frequency() const;

    void set_attack_range(const double p_range);
    double get_attack_range() const;
    void set_attack_frequency(const double p_speed);
    double get_attack_frequency() const;
};

class EnemyState : public Object {
    GDCLASS(EnemyState, Object);
protected:
    static void _bind_methods(){}

public:
    EnemyState(){}
    ~EnemyState(){}
    virtual EnemyState* update(Enemy& enemy, double delta);    
    virtual void fixed_update(Enemy& enemy, double delta);
};
class StormingState : public EnemyState {
private:
    Vector2i crate = Vector2i(-1,-1);
    double attack_timer = 0.0;
    bool can_attack = false;

    void set_storm(Enemy& enemy);
public:
    StormingState(){}
    ~StormingState(){}
    EnemyState* update (Enemy& enemy, double delta) override;
    void fixed_update(Enemy& enemy, double delta) override;
};

class AttackingState : public EnemyState {
public:
    AttackingState(){}
    ~AttackingState(){}
    EnemyState* update (Enemy& enemy, double delta) override;
    void fixed_update(Enemy& enemy, double delta) override;
};
class WanderingState : public EnemyState {
public:
    WanderingState(){}
    ~WanderingState(){}
    EnemyState* update (Enemy& enemy, double delta) override;
    void fixed_update(Enemy& enemy, double delta) override;
};
class HuntingState : public EnemyState {
public:
    HuntingState(){}
    ~HuntingState(){}
    EnemyState* update (Enemy& enemy, double delta) override;
    void fixed_update(Enemy& enemy, double delta) override;
};
}

#endif