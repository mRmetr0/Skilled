#ifndef WEAPONSTATE_H
#define WEAPONSTATE_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/input.hpp>

namespace godot {


class WeaponState : public Object {
    GDCLASS(WeaponState, Object)
protected:
    Node* bullet_pool;
    Input* input;

    //reloading
    double reload_duration;
    double reload_time;
    bool reloading = false;

    //firing 
    int inaccuracy;
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
    
    static void _bind_methods(){}
public:
    WeaponState(){}
    ~WeaponState(){}
    virtual void start(Node& node, int p_start_clip = 0);
    virtual WeaponState* update(Node2D& node, double delta);
    virtual void shoot(Vector2 position, double angle);
    Vector2i _get_bullets();
};

class PistolState : public WeaponState {
public: 
    PistolState(){}
    ~PistolState(){}
    void start(Node& node, int p_start_clip = 0) override;
    WeaponState* update(Node2D& node,double delta) override;
    void shoot(Vector2 position, double angle) override;
    void reload();
};

class AutoState : public WeaponState {
private:
    bool empty = false;
public: 
    AutoState(){}
    ~AutoState(){}
    void start(Node& node, int p_start_clip = 0) override;
    WeaponState* update(Node2D& node,double delta) override;
    void shoot(Vector2 position, double angle) override;
};

class SpreadState : public WeaponState {
private:
    bool empty = false;
public: 
    SpreadState(){}
    ~SpreadState(){}
    void start(Node& node, int p_start_clip = 0) override;
    WeaponState* update(Node2D& node,double delta) override;
    void shoot(Vector2 position, double angle) override;
};

}
#endif