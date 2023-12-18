#ifndef LABOURER_H
#define LABOURER_H


#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/progress_bar.hpp>
#include <godot_cpp/classes/tile_map.hpp>

namespace godot {
class Labourer : public Area2D{
    GDCLASS(Labourer, Area2D)

private:
    TileMap* tile_map;
    ProgressBar* hp_bar;
    PackedVector2Array path;
    Vector2 target;

    int health;
    int health_max;
    double move_speed;

    int progress;
    void astar_move(double delta);

protected:
    static void _bind_methods();

public:
    Labourer();
    ~Labourer();

    void _ready();
    void _physics_process(double delta);

    void _take_damage(int p_damage);


    //Getters + setters
    void set_health(const int p_health);
    int get_health() const;
    void set_speed(const double p_speed);
    double get_speed() const;

};

}

#endif