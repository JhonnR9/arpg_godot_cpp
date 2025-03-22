#ifndef __STATE_H__
#define __STATE_H__

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"

using namespace godot;

class Character;  // Forward declaration


class State : public RefCounted {
  GDCLASS(State, RefCounted)

protected:
  Character *owner{nullptr};
  static void _bind_methods();

public:
  virtual void on_state_enter(Character *owner);
  virtual void on_state_run(double_t delta);
  virtual void on_state_exit();
};

#endif // __STATE_H__