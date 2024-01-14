#ifndef PLAYER_H
#define PLAYER_H

#include <godot_cpp/variant/utility_functions.hpp>

#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/progress_bar.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/tile_map.hpp>

#include "weapon_state.h"

namespace godot {

class Player : public CharacterBody2D {
    GDCLASS(Player, CharacterBody2D)

private:
    ProgressBar* hp_bar;
    Input* input;
    WeaponState* weapon_state;
    Vector2 move_dir;

    double time_passed;
    double move_speed;
    double normal_speed;
    double slow_speed;
    double hit_stun;

    int health_max;

    void key_input();
    void key_move(double delta);

protected:
    static void _bind_methods();

public:
    int health;
    
    Player();
    ~Player();
    
    void _ready();
    void _process(double delta);
    void _physics_process(double delta);

    void _take_damage(int p_damage);
    
    Vector2i _get_bullets();
    int _get_weapon_id();
    void _set_weapon(int p_id, int p_ammo);

    //getters & setters
    void set_health(const int p_health);
    int get_health() const;
    void set_normal_speed(const double p_speed);
    double get_normal_speed() const;
    void set_slow_speed(const double p_speed);
    double get_slow_speed() const;
};

}

#endif