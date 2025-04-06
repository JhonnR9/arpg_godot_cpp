#include "state_machine.h"

#include "characters/character.h"
#include "core/command.h"
#include "godot_cpp/classes/object.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/memory.hpp"
#include "godot_cpp/variant/callable.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "state.h"
#include "characters/character.h"

void StateMachine::add_state(String p_name, Ref<State> p_new_state) { states[p_name] = p_new_state; }

void StateMachine::set_state(String p_name) {
    
    if (states.has(p_name)) {
        if(current.is_empty() ){
            current = p_name;
            auto state = Object::cast_to<State>(states[p_name]);
            state->_gdvirtual__on_state_enter_call(character);

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
            state->_gdvirtual__on_state_enter_call(this->character);
            state->_gdvirtual__on_state_exit_call();
        }
        if (auto state = Object::cast_to<State>(states[next])) {
            state->_gdvirtual__on_state_enter_call(this->character);
        }
        current = next;
        next = "";
    } 

    if (states.has(current)) {
        if (auto state = Object::cast_to<State>(states[current])) {
            state->_gdvirtual__on_state_stay_call(p_delta);
        }
    }
}

StateMachine::StateMachine() {
    current = "";
    next = "";
}

void StateMachine::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_state_count"),&StateMachine::get_states_count);
    ClassDB::bind_method(D_METHOD("get_current_state"),&StateMachine::get_current_state);
    ClassDB::bind_method(D_METHOD("remove_state", "p_name"),&StateMachine::remove_state);
    ClassDB::bind_method(D_METHOD("set_state", "p_name"), &StateMachine::set_state);
    ClassDB::bind_method(D_METHOD("add_state", "p_name", "p_character"),&StateMachine::add_state);
    ClassDB::bind_method(D_METHOD("set_character", "p_character"),&StateMachine::set_character);
}

void StateMachine::set_character(Character* p_character) { this->character = p_character; }