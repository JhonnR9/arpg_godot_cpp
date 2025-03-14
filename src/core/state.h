#ifndef __STATE_H__
#define __STATE_H__

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"

using namespace godot;

template <typename K>
class State : public RefCounted{
    GDCLASS(State<K>, RefCounted)

    
public:
    virtual void _on_state_enter(Ref<State<K>> p_owner) = 0;
    virtual void _on_state_run(Ref<State<K>> p_owner, double_t delta) = 0;
    virtual void _on_state_exit(Ref<State<K>> p_owner) = 0;

};


#endif // __STATE_H__