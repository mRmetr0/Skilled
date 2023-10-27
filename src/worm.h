#ifndef WORM_H
#define WORM_H

#include <entity.h>

namespace godot {

class Worm : public Entity {
    GDCLASS(Worm, Entity)

private:

protected:
    static void _bind_methods();

public:
    Worm();
    ~Worm();

    void _process(double delta);
};

}

#endif