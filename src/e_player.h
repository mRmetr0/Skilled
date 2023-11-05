#ifndef E_PLAYER_H
#define E_PLAYER_H

#include <entity.h>

namespace godot {

class EPlayer : public Entity {
    GDCLASS(EPlayer, Entity)

private:

protected:
    static void _bind_methods();

public:
    int id;

    EPlayer();
    ~EPlayer();

    void _ready();
    void _process(double delta);

    //getters & setters
    void set_id(const double p_id);
    double get_id() const;
};
}

#endif