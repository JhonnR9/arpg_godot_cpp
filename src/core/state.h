#ifndef __STATE_H__
#define __STATE_H__

#include "godot_cpp/classes/ref_counted.hpp"

using namespace godot;

template <typename T>
class State : public RefCounted{

public:
    virtual void _on_state_enter(Ref<State<T>> p_owner) = 0;
    virtual void _on_state_run(Ref<State<T>> p_owner, double_t delta) = 0;
    virtual void _on_state_exit(Ref<State<T>> p_owner) = 0;

};


#endif // __STATE_H__