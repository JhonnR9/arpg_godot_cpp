#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

#include <cstdint>

#include "core/command.h"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/templates/vector.hpp"
#include "godot_cpp/variant/callable.hpp"
#include "godot_cpp/variant/dictionary.hpp"
#include "godot_cpp/variant/string.hpp"


class Character;
class State;
using namespace godot;

class StateMachine : public RefCounted {
    
    GDCLASS(StateMachine, RefCounted);

   public:
    StateMachine();

   private:
    Dictionary states;
    String current;
    String next;
    Character* character;

   protected:
    static void _bind_methods();

   public:

    void set_character(Character* p_character);
    void add_state(String p_name, Ref<State> p_new_state);
    void set_state(String p_name);
    void remove_state(String p_name);
    String get_current_state();
    uint16_t get_states_count();
    void update(double_t p_delta);
};

#endif  // __STATE_MACHINE_H__
