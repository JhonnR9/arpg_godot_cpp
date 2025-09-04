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
#include "tools/auto_resgister.h"

#define DEBUG_STATE_MACHINE  0

#if DEBUG_STATE_MACHINE
    #define STATE_MACHINE_LOG(...) UtilityFunctions::print("StateMachine-> " ,__VA_ARGS__)
    #define STATE_MACHINE_ERR(...) UtilityFunctions::printerr(__VA_ARGS__)
#else
    #define STATE_MACHINE_LOG(...) ((void)0)
    #define STATE_MACHINE_ERR(...) ((void)0)
#endif

AUTO_REGISTER_CLASS(StateMachine)

void StateMachine::add_state(String p_name, Ref<State> p_new_state) {
    if (p_name.is_empty()) {
        STATE_MACHINE_ERR("Cannot add state with empty name.");
        return;
    }
    if (p_new_state.is_null()) {
        STATE_MACHINE_ERR("Cannot add null state for name: ", p_name);
        return;
    }
    if (!p_new_state->is_class("State")) {
        STATE_MACHINE_ERR("Object for state ", p_name, " is not a State class.");
        return;
    }
    if (!states.has(p_name)) {
        states[p_name] = p_new_state;
        STATE_MACHINE_LOG("Added state: ", p_name);
    } else {
        STATE_MACHINE_ERR("State already exists: ", p_name);
    }
}

void StateMachine::set_state(String p_name) {
    if (p_name.is_empty()) {
        STATE_MACHINE_ERR("Cannot set empty state name.");
        return;
    }
    if (!states.has(p_name)) {
        STATE_MACHINE_ERR("State not found: ", p_name);
        return;
    }
    if (p_name == current){
          STATE_MACHINE_LOG("State ", p_name, " already running, ignoring.");
          return;
    }
    if (!next.is_empty() && next == p_name ) {
        STATE_MACHINE_LOG("State ", p_name, " already queued, ignoring.");
        return;
    }

    if (current.is_empty()) {
        Ref<State> state = states[p_name];
        if (state.is_valid() && character != nullptr) {
            STATE_MACHINE_LOG("Directly entering state: ", p_name);
            current = p_name;
            state->_gdvirtual__on_state_enter_call(character);
        } else {
            STATE_MACHINE_ERR("Cannot enter state ", p_name, ": invalid state or null character.");
        }
        return;
    }
    STATE_MACHINE_LOG("Queueing transition to state: ", p_name);

    next = p_name; // Queue state transition for update
}


void StateMachine::remove_state(String p_name) {
    if (p_name.is_empty()) {
        STATE_MACHINE_ERR("Cannot remove state with empty name.");
        return;
    }
    if (states.has(p_name)) {
        STATE_MACHINE_LOG("Removing state: ", p_name);
        if (current == p_name) {
            current = ""; // Clear current state if removing it
        }
        if (next == p_name) {
            next = ""; // Clear next state if removing it
        }
        states.erase(p_name);
    } else {
        STATE_MACHINE_ERR("Cannot remove state, not found: ", p_name);
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
        STATE_MACHINE_LOG("Processing transition to state: ", next);
        // Exit current state
        if (!current.is_empty() && states.has(current)) {
            Ref<State> current_state = states[current];
            if (current_state.is_valid()) {
                STATE_MACHINE_LOG("Exiting state: ", current);
                current_state->_gdvirtual__on_state_exit_call();
            } else {
                STATE_MACHINE_ERR("Invalid current state object: ", current);
            }
        }

        // Enter next state
        Ref<State> next_state = states[next];
        if (next_state.is_valid() && character != nullptr) {
            STATE_MACHINE_LOG("Entering state: ", next);
            current = next;
            next_state->_gdvirtual__on_state_enter_call(character);
        } else {
            STATE_MACHINE_ERR("Cannot enter state ", next, ": invalid state or null character.");
        }
        next = "";
    }

    // Update current state
    if (!current.is_empty() && states.has(current)) {
        Ref<State> current_state = states[current];
        if (current_state.is_valid() && character != nullptr) {
            current_state->_gdvirtual__on_state_stay_call(p_delta);
        } else {
            STATE_MACHINE_ERR("Cannot update state ", current, ": invalid state or null character.");
            current = ""; // Reset to avoid repeated errors
        }
    }
}

StateMachine::StateMachine() : character(nullptr) {
    STATE_MACHINE_LOG("StateMachine initialized.");
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
    STATE_MACHINE_LOG("Character set for StateMachine: ", p_character ? p_character->get_class() : "null");
}