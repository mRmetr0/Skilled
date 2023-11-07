#ifndef ENEMY_H
#define ENEMY_H

#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/progress_bar.hpp>
#include <godot_cpp/classes/tile_map.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

#include "player.h"

namespace godot {

class Enemy : public Area2D {
    GDCLASS(Enemy, Area2D)

private:
    enum State {
        STALKING,
        ATTACKING,
        STORMING,
        WANDERING,
        HUNTING,
    };

    State state;

    Node* player_manager;
    TileMap* tile_map;
    ProgressBar* hp_bar;
    PackedVector2Array path;
    Player* player;
    Vector2 target;
    Vector2i crate;

    double time_passed;
    double speed;
    double update_frequency;
    double update_timer;

    double attack_frequency;
    double attack_timer;
    double attack_range;

    int progress;
    bool can_attack;

    int health_max;

    void astar_storm();
    void astar_hunt();
    void astar_move(double delta);
    void switch_state(State p_state);
    bool can_update(double delta);

protected:
    static void _bind_methods();

public:
    int health;
    
    Enemy();
    ~Enemy();
    
    void _ready();
    void _process(double delta);
    void _physics_process(double delta);
    void _take_damage(int p_damage);
    void _handle_overlap(Variant p_var);

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

}

#endif