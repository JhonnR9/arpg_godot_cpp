
#ifndef  ARPG_H
#define  ARPG_H

#include "godot_cpp/classes/node2d.hpp"

#include "json_parse.h"

using namespace godot;

class ARPG : public Node2D {
    GDCLASS(ARPG, Node2D);
    Ref<JsonParse> parse;
protected:
    static void _bind_methods();

public:
    ARPG();
    virtual void _ready() override;
 
};

#endif //  ARPG_H
