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

void StateMachine::add_state(String p_name, Ref<State> p_new_state) {
    if (p_name.is_empty()) {
        UtilityFunctions::printerr("Cannot add state with empty name.");
        return;
    }
    if (p_new_state.is_null()) {
        UtilityFunctions::printerr("Cannot add null state for name: ", p_name);
        return;
    }
    if (!p_new_state->is_class("State")) {
        UtilityFunctions::printerr("Object for state ", p_name, " is not a State class.");
        return;
    }
    if (!states.has(p_name)) {
        states[p_name] = p_new_state;
        UtilityFunctions::print("Added state: ", p_name);
    } else {
        UtilityFunctions::printerr("State already exists: ", p_name);
    }
}

void StateMachine::set_state(String p_name) {
    if (p_name.is_empty()) {
        UtilityFunctions::printerr("Cannot set empty state name.");
        return;
    }
    if (!states.has(p_name)) {
        UtilityFunctions::printerr("State not found: ", p_name);
        return;
    }
    if (!next.is_empty() && next == p_name) {
        UtilityFunctions::print("State ", p_name, " already queued, ignoring.");
        return;
    }
    if (current.is_empty()) {
        Ref<State> state = states[p_name];
        if (state.is_valid() && character != nullptr) {
            UtilityFunctions::print("Directly entering state: ", p_name);
            current = p_name;
            state->_gdvirtual__on_state_enter_call(character);
        } else {
            UtilityFunctions::printerr("Cannot enter state ", p_name, ": invalid state or null character.");
        }
        return;
    }
    UtilityFunctions::print("Queueing transition to state: ", p_name);
    next = p_name; // Queue state transition for update
}

void StateMachine::remove_state(String p_name) {
    if (p_name.is_empty()) {
        UtilityFunctions::printerr("Cannot remove state with empty name.");
        return;
    }
    if (states.has(p_name)) {
        UtilityFunctions::print("Removing state: ", p_name);
        if (current == p_name) {
            current = ""; // Clear current state if removing it
        }
        if (next == p_name) {
            next = ""; // Clear next state if removing it
        }
        states.erase(p_name);
    } else {
        UtilityFunctions::printerr("Cannot remove state, not found: ", p_name);
    }
}

String StateMachine::get_current_state() const {
    return current;
}

uint16_t StateMachine::get_states_count() const {
    return static_cast<uint16_t>(states.size());
}

void StateMachine::update(float p_delta) {
    // Handle state transition
    if (!next.is_empty() && states.has(next)) {
        UtilityFunctions::print("Processing transition to state: ", next);
        // Exit current state
        if (!current.is_empty() && states.has(current)) {
            Ref<State> current_state = states[current];
            if (current_state.is_valid()) {
                UtilityFunctions::print("Exiting state: ", current);
                current_state->_gdvirtual__on_state_exit_call();
            } else {
                UtilityFunctions::printerr("Invalid current state object: ", current);
            }
        }

        // Enter next state
        Ref<State> next_state = states[next];
        if (next_state.is_valid() && character != nullptr) {
            UtilityFunctions::print("Entering state: ", next);
            current = next;
            next_state->_gdvirtual__on_state_enter_call(character);
        } else {
            UtilityFunctions::printerr("Cannot enter state ", next, ": invalid state or null character.");
        }
        next = "";
    }

    // Update current state
    if (!current.is_empty() && states.has(current)) {
        Ref<State> current_state = states[current];
        if (current_state.is_valid() && character != nullptr) {
            current_state->_gdvirtual__on_state_stay_call(p_delta);
        } else {
            UtilityFunctions::printerr("Cannot update state ", current, ": invalid state or null character.");
            current = ""; // Reset to avoid repeated errors
        }
    }
}

StateMachine::StateMachine() : character(nullptr) {
    UtilityFunctions::print("StateMachine initialized.");
}

void StateMachine::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_state_count"), &StateMachine::get_states_count);
    ClassDB::bind_method(D_METHOD("get_current_state"), &StateMachine::get_current_state);
    ClassDB::bind_method(D_METHOD("remove_state", "p_name"), &StateMachine::remove_state);
    ClassDB::bind_method(D_METHOD("set_state", "p_name"), &StateMachine::set_state);
    ClassDB::bind_method(D_METHOD("add_state", "p_name", "p_new_state"), &StateMachine::add_state);
    ClassDB::bind_method(D_METHOD("set_character", "p_character"), &StateMachine::set_character);
    ClassDB::bind_method(D_METHOD("update", "p_delta"), &StateMachine::update);
}

void StateMachine::set_character(Character* p_character) {
    this->character = p_character;
    UtilityFunctions::print("Character set for StateMachine: ", p_character ? p_character->get_class() : "null");
}