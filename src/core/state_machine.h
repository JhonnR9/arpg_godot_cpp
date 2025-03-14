#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/templates/vector.hpp"
#include "state.h"
#include <cstdint>



using namespace godot;
template<typename K>
class StateMacine: RefCounted {
    GDCLASS(StateMacine, RefCounted);
    
public:
    StateMacine();

private:
    Vector<Ref<State<K>>> states;
    Ref<K> current;

public:
    void add_state(State<K> p_new_state);
    void set_state(State<K> p_new_state);
    void remove_state(State<K> p_new_state);
    State<K> get_current_state();
    uint16_t get_states_count();
};


#endif // __STATE_MACHINE_H__

