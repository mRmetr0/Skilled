#ifndef PLAYER_H
#define PLAYER_H

#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/progress_bar.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/tile_map.hpp>

#include "weapon_state.h"
#include "weapon_state.h"

namespace godot {

class Player : public Area2D {
    GDCLASS(Player, Area2D)

private:
    Node* enemy_manager;
    TileMap* tile_map;
    ProgressBar* hp_bar;
    Input* input;
    PackedVector2Array path;
    Vector2 enemy;
    Vector2 target;
    Vector2i crate;

    WeaponState* weapon_state;

    double time_passed;
    double update_frequency;
    double move_speed;
    double normal_speed;
    double slow_speed;
    double hit_stun;
    int progress;
    bool is_player;

    int health_max;

    void astar_set();
    void astar_move(double delta);
    bool is_ground();

protected:
    static void _bind_methods();

public:
    int health;
    int id;
    
    Player();
    ~Player();
    
    void _ready();
    void _process(double delta);
    void _physics_process(double delta);

    void _take_damage(int p_damage);
    void _set_active(bool p_active);
    void _set_target(Vector2 p_target);
    void _set_weapon(WeaponState weapon);
    Vector2i _get_bullets();

    //getters & setters
    void set_health(const int p_health);
    int get_health() const;
    void set_normal_speed(const double p_speed);
    double get_normal_speed() const;
    void set_slow_speed(const double p_speed);
    double get_slow_speed() const;
    void set_id(const double p_id);
    double get_id() const;
    void set_frequency(const double p_speed);
    double get_frequency() const;
};

}

#endif