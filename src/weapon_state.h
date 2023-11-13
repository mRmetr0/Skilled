#ifndef WEAPONSTATE_H
#define WEAPONSTATE_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/input.hpp>

using namespace godot;

class WeaponState : public Object {
    GDCLASS(WeaponState, Object)
protected:
    Node* bullet_pool;
    Input* input;

    //reloading
    double reload_duration = 3.0;
    double reload_time = 0.0;
    bool reloading;

    //firing 
    double rate_of_fire;
    double fire_time;
    int crit_rate = 5;
    bool in_recoil = false;

    //shotgun
    double spread = 0; 
    int shot_amount = 0;    

    //bullet
    int bullet_damage = 1;
    int bullet_pierce = 0;
    int bullet_amount = 0;
    int magazine_size = 13;
    
    static void _bind_methods(){}
public:
    virtual void start(Node& node, int p_start_clip);
    virtual void update(Node2D& node, double delta);
    virtual void shoot(Vector2 position, double angle);
    virtual void reload();
};

// class PistolState : public WeaponState {
// public: 
//     void update(Node2D& node,double delta) override {WeaponState::update(node, delta);}
//     void shoot() override;
//     void reload() override;
// };

#endif