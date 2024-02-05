#ifndef ENEMY_H
#define ENEMY_H

#include <godot_cpp/variant/utility_functions.hpp>

#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/progress_bar.hpp>
#include <godot_cpp/classes/tile_map.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

#include "player.h"

namespace godot {

class EnemyState;

class Enemy : public Area2D {
    GDCLASS(Enemy, Area2D)

private:
    ProgressBar* hp_bar;

    int health_max;

protected:
    static void _bind_methods();

public:
    Node* player_manager;
    TileMap* tile_map;
    Player* player;
    EnemyState* state;

    double speed;
    double update_frequency;
    double update_timer;

    double attack_frequency;
    double attack_timer;

    int health;
    int points;
    double attack_range;
    
    Enemy();
    ~Enemy();
    
    void _ready();
    void _process(double delta);
    void _physics_process(double delta);

    void _take_damage(int p_damage);
    void _animate(int p_anim, int p_flip_dir);
    void _init_health(float p_health_mod);

    void set_health(const int p_health);
    int get_health() const;
    void set_speed(const double p_speed);
    double get_speed() const;
    void set_frequency(const double p_speed);
    double get_frequency() const;
    void set_points(const int p_points);
    int get_points() const;

    void set_attack_range(const double p_range);
    double get_attack_range() const;
    void set_attack_frequency(const double p_speed);
    double get_attack_frequency() const;
};

class EnemyState : public Object {
    GDCLASS(EnemyState, Object);
protected:
    PackedVector2Array path;
    PackedVector2Array check_path;
    int progress = 0;
    double update_timer = 0;

    static void _bind_methods(){}
    bool can_update(Enemy& enemy, double delta);

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
private:
    void set_hunt(Enemy& enemy);
public:
    HuntingState(){}
    ~HuntingState(){}
    EnemyState* update (Enemy& enemy, double delta) override;
    void fixed_update(Enemy& enemy, double delta) override;
};

class DamagedState : public EnemyState {
private:
    bool animated = false;
    double hit_stun = 0.1;
public:
    DamagedState(){}
    ~DamagedState(){}
    EnemyState* update(Enemy& enemy, double delta) override;
    void fixed_update(Enemy& enemy, double delta) override;
};
}

#endif