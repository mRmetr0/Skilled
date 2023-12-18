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
    enum States{
        PACKING,
        MOVING
    };

    States state;

    TileMap* tile_map;
    ProgressBar* hp_bar;
    PackedVector2Array path;
    Vector2i resource;
    bool to_start;

    int health;
    int health_max;
    double move_speed;

    double time_passed;
    double packing_time;

    int progress;
    void astar_set();
    void astar_move(double delta);
    void packing_update(double delta);

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