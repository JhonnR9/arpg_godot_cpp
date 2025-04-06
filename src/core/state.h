#ifndef __STATE_H__
#define __STATE_H__

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "characters/character.h"
#include <cmath>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class State : public RefCounted {
  GDCLASS(State, RefCounted)

protected:
  static void _bind_methods();

public:
  GDVIRTUAL1(_on_state_enter, Character*)
  GDVIRTUAL1(_on_state_stay, float_t)
  GDVIRTUAL0(_on_state_exit)

};

#endif // __STATE_H__
