#include "state_machine.h"
#include "godot_cpp/classes/object.hpp"
#include "godot_cpp/variant/callable.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "characters/character.h"



void StateMachine::add_state(String name, Ref<State> p_new_state) {
    states[name] = p_new_state;
    
}


void StateMachine::set_state(String p_name) {
    if (states.has(p_name)) {
        current = p_name;
        if(auto state = Object::cast_to<State>(states[p_name])){
            state->on_state_enter(character);
        }

    }else {
        UtilityFunctions::printerr(vformat("state %s don't founded"), p_name);
    }
}


void StateMachine::remove_state(String p_name) {
    if (states.has(p_name)){
        states.erase(p_name);
    }
   
}


String StateMachine::get_current_state() {
    return current;
}

uint16_t StateMachine::get_states_count() {
    return static_cast<uint16_t>(states.size());
}

void StateMachine::update(double_t p_delta)
{
    if (states.has(current)) { 
        if (auto state = Object::cast_to<State>(states[current])) {
            state->on_state_run(p_delta);
        }
    }
}

void StateMachine::add_transition(String name, Callable& p_function)
{
    
}


StateMachine::StateMachine(){
    
}

void StateMachine::_bind_methods()
{
    
}

void StateMachine::set_character(Character* p_character)
{
    this->character = p_character;
}