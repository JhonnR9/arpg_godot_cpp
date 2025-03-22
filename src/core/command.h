#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <cmath>
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"

using namespace godot;

class Command : public RefCounted{
    GDCLASS(Command, RefCounted)

    public:
    virtual void run(float_t delta);
};

#endif // __COMMAND_H__