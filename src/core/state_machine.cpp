#include "state_machine.h"

#include "characters/character.h"
#include "core/command.h"
#include "godot_cpp/classes/object.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/core/memory.hpp"
#include "godot_cpp/variant/callable.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/variant/variant.hpp"

void StateMachine::add_state(String name, Ref<State> p_new_state) { states[name] = p_new_state; }

void StateMachine::set_state(String p_name) {
    
    if (states.has(p_name)) {
        if(current.is_empty() ){
            current = p_name;
            auto state = Object::cast_to<State>(states[p_name]);
            state->on_state_enter(character);

        }
        next = p_name;

    } else {
        UtilityFunctions::printerr(vformat("state %s don't founded"), p_name);
    }
}

void StateMachine::remove_state(String p_name) {
    if (states.has(p_name)) {
        states.erase(p_name);
    }
}

String StateMachine::get_current_state() { return current; }

uint16_t StateMachine::get_states_count() { return static_cast<uint16_t>(states.size()); }

void StateMachine::update(double_t p_delta) {

  
    if (!next.is_empty() && states.has(next) && states.has(current)) {

        if (auto state = Object::cast_to<State>(states[current])) {
            state->on_state_exit();
        }
        if (auto state = Object::cast_to<State>(states[next])) {
            state->on_state_enter(character);
        }
        current = next;
        next = "";
    } 

    if (states.has(current)) {
        if (auto state = Object::cast_to<State>(states[current])) {
            state->on_state_run(p_delta);
        }
    }
}

StateMachine::StateMachine() {
    current = "";
    next = "";
}

void StateMachine::_bind_methods() {}

void StateMachine::set_character(Character* p_character) { this->character = p_character; }