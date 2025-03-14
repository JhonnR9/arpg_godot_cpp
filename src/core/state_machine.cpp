#include "state_machine.h"

template<typename K>
void StateMacine<K>::add_state(State<K> p_new_state)
{
    states.append(p_new_state);
}


template<typename K>
void StateMacine<K>::set_state(State<K> p_new_state)
{
    current=p_new_state;
}


template<typename K>
void StateMacine<K>::remove_state(State<K> p_new_state)
{
    if (states.has(p_new_state)) {
        states.erase(p_new_state);
    }
}


template<typename K>
State<K> StateMacine<K>::get_current_state()
{
    return current;
}


template<typename K>
uint16_t StateMacine<K>::get_states_count()
{
    return states.size();
}


