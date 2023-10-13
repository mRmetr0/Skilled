#ifndef ENTITY_H
#define ENTITY_H

#include <godot_cpp/classes/character_body2d.hpp>

namespace godot {

class Entity : public CharacterBody2D {
    GDCLASS(Entity, CharacterBody2D)

private:

protected:
    double time_passed;
    double speed;
    static void _bind_methods();

public:
    int health;

    Entity();
    ~Entity();
    
    void _take_damage(int damage);

    void set_speed(const double p_speed);
    double get_speed() const;
    void set_health(const int p_health);
    int get_health() const;
};

}

#endif