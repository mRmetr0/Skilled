#ifndef WEAPON_H
#define WEAPON_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

namespace godot {

class Weapon : public Node2D {
    GDCLASS(Weapon, Node2D)

private:
    Node* bullet_pool;

    //AI 
    double fire_range;
    //reloading
    double reload_duration;
    double reload_time;
    bool reloading;

    //firing 
    double rate_of_fire;
    double fire_time;
    int crit_rate;
    bool in_recoil;

    //shotgun
    double spread; 
    int shot_amount;    

    //bullet
    int bullet_damage;
    int bullet_pierce;
    int bullet_amount;
    int magazine_size;


protected:
    static void _bind_methods();

public:
    Weapon();
    ~Weapon();

    void _ready();
    void _process(double delta);
    void shoot(Vector2 position, double angle);
    void reload();

    void set_rate_of_fire(const double p_rof);
    double get_rate_of_fire() const;
    void set_fire_range(const double p_range);
    double get_fire_range() const;

    void set_reload_duration(const double p_duration);
    double get_reload_duration() const;
    void set_bullet_amount(const int p_amount);
    int get_bullet_amount() const;
    void set_magazine(const int p_magazine);
    int get_magazine() const;
    void set_crit(const int p_crit);
    int get_crit() const;

    void set_bullet_damage(const int p_damage);
    int get_bullet_damage() const;
    void set_bullet_pierce(const int p_pierce);
    int get_bullet_pierce() const;

    void set_spread(const double p_spread);
    double get_spread() const;
    void set_shot_amount(const int p_amount);
    int get_shot_amount() const;
};

}

#endif