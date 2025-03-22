#include "player.h"

#include <godot_cpp/classes/input.hpp>

#include "characters/character.h"
#include "commands/run_command.h"
#include "commands/stop_command.h"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/core/memory.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/variant/vector2.hpp"
#include "player_idle.h"
#include "player_run.h"

void Player::_bind_methods() {}

Player::Player() { get_state_machine()->set_character(this); }

void Player::_process(double_t p_delta) {
    Input *input = Input::get_singleton();
    if (input->is_action_just_pressed("up")) {
        add_command(Ref<RunCommand>(memnew(RunCommand(this, Vector2(0.0f, -1.0f)))));
    }
    if (input->is_action_just_pressed("down")) {
        add_command(Ref<RunCommand>(memnew(RunCommand(this, Vector2(0.0f, 1.0f)))));
    }
    if (input->is_action_just_pressed("left")) {
        add_command(Ref<RunCommand>(memnew(RunCommand(this, Vector2(-1.0f, 0.0f)))));
    }
    if (input->is_action_just_pressed("right")) {
        add_command(Ref<RunCommand>(memnew(RunCommand(this, Vector2(1.0f, 0.0f)))));
    }

    if (!input->is_action_pressed("up") && !input->is_action_pressed("down") &&
        !input->is_action_pressed("left") && !input->is_action_pressed("right") && 
        get_velocity().length_squared() > 0.0f) {
        add_command(Ref<StopCommand>(memnew(StopCommand(this))));
    }
    get_state_machine()->update(p_delta);
}

void Player::_ready() {
    Character::_ready();

    get_state_machine()->add_state("idle", Ref<PlayerIdle>(memnew(PlayerIdle)));
    get_state_machine()->add_state("run", Ref<PlayerRun>(memnew(PlayerRun)));

    get_state_machine()->set_state("idle");
}