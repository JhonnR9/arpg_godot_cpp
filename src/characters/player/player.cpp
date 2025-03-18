#include "player.h"

#include <godot_cpp/classes/input.hpp>

#include "characters/character.h"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/core/memory.hpp"
#include "godot_cpp/variant/callable.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/variant/vector2.hpp"
#include "player_idle.h"
#include "player_run.h"

void Player::_bind_methods() {}

Player::Player() {
    set_physics_process(true);
    state_machine.instantiate();
    state_machine->set_character(this);
}

void Player::_physics_process(double_t delta) {
    Vector2 direction = Input::get_singleton()->get_vector("left", "right", "up", "down");

    if (direction.length() > 0) {
        move(direction);
    } else {
        apply_friction();
    }

   
}

void Player::_process(double_t p_delta) { state_machine->update(p_delta); }

void Player::_ready() {
    Character::_ready();

    state_machine->add_state("idle", Ref<PlayerIdle>(memnew(PlayerIdle)));
    state_machine->add_state("run", Ref<PlayerRun>(memnew(PlayerRun)));
    state_machine->set_state("idle");
}