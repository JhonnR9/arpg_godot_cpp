#ifndef ARPG_H
#define ARPG_H

#include "godot_cpp/classes/node2d.hpp"

using namespace godot;

class ARPG : public Node2D {
    GDCLASS(ARPG, Node2D);

protected:
    static void _bind_methods();

public:
    ARPG();

};

#endif // ARPG_H