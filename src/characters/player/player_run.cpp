#include "player_run.h"

#include "commands/run_command.h"
#include "godot_cpp/classes/input.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/variant/vector2.hpp"
#include "player.h"

void PlayerRun::_bind_methods() {}

void PlayerRun::on_state_enter(Character* p_owner) {
    this->owner = p_owner;
    p_owner->set_animation("idle");
}

void PlayerRun::on_state_run(double_t p_delta) {

    if (Command* command = owner->get_last_command().ptr()) {

        if (Object::cast_to<RunCommand>(command)) {
            command->run(p_delta);
        } 
        owner->remove_last_command();
    }else {
        owner->set_move_direction(Vector2(0.0f, 0.0f));
    }

    if (owner->get_velocity().length_squared() < 0.01f) {
        owner->get_state_machine()->set_state("idle");
    }
}

void PlayerRun::on_state_exit() {
    if (owner)
        owner->set_move_direction(Vector2(0.0f, 0.0f));
}
