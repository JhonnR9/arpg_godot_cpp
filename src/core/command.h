#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <cmath>
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class Command : public RefCounted{
    GDCLASS(Command, RefCounted)

    protected:
    static void _bind_methods();

    public:
    GDVIRTUAL1(run, float_t);

};

#endif // __COMMAND_H__