#ifndef ENTITY_H
#define ENTITY_H

#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/progress_bar.hpp>

namespace godot {

class Entity : public Area2D {
    GDCLASS(Entity, Area2D)

private:

protected:
    ProgressBar* hp_bar;

    static void _bind_methods();

public:
    int health;
    float speed;

    Entity();
    ~Entity();

    void _ready();
    void _take_damage(int p_damage);

    //getters & setters
    void set_health(const int p_health);
    int get_health() const;
};

}

#endif