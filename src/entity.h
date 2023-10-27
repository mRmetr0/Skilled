#ifndef ENTITY_H
#define ENTITY_H

#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/progress_bar.hpp>

namespace godot {

class Entity : public Area2D {
    GDCLASS(Entity, Area2D)

private:
    ProgressBar* hp_bar;

    int health;

    void astar_set();
    void astar_move(double delta);

protected:
    static void _bind_methods();

public:
    Entity();
    ~Entity();

    void _ready();
    void _process(double delta);
    void _physics_process(double delta);
    void _take_damage(int p_damage);
};

}

#endif