#include "player.h"

#include <godot_cpp/classes/input.hpp>

#include "characters/character.h"
#include "commands/run_command.h"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/core/memory.hpp"

#include "godot_cpp/variant/vector2.hpp"
#include "player_idle.h"
#include "player_run.h"


namespace {
    constexpr const char* INPUT_LEFT = "left";
    constexpr const char* INPUT_RIGHT = "right";
    constexpr const char* INPUT_UP = "up";
    constexpr const char* INPUT_DOWN = "down";

    constexpr const char* STATE_IDLE = "idle";
    constexpr const char* STATE_RUN = "run";
}

void Player::_bind_methods() {}

Player::Player() { get_state_machine()->set_character(this); }

void Player::_process(double_t p_delta) {
    Character::_process(p_delta);
    Input *input = Input::get_singleton();
    auto dir = input->get_vector(INPUT_LEFT, INPUT_RIGHT, INPUT_UP, INPUT_DOWN);

    if (dir.length_squared() > 0.01f) {
        add_command(Ref<RunCommand>(memnew(RunCommand(this, dir))));
    }
    get_state_machine()->update(p_delta);
}

void Player::_ready() {
    Character::_ready();

    get_state_machine()->add_state(STATE_IDLE, Ref<PlayerIdle>(memnew(PlayerIdle)));
    get_state_machine()->add_state(STATE_RUN, Ref<PlayerRun>(memnew(PlayerRun)));

    get_state_machine()->set_state(STATE_IDLE);
}
