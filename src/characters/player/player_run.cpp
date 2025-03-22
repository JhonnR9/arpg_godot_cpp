#include "player_run.h"

#include "commands/run_command.h"
#include "commands/stop_command.h"
#include "godot_cpp/classes/input.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "player.h"

void PlayerRun::_bind_methods() {}

void PlayerRun::on_state_enter(Character* p_owner) { this->owner = p_owner; }

void PlayerRun::on_state_run(double_t p_delta) {

    if (owner->get_commands_size() > 0){
        Command* command = owner->get_last_command().ptr();

        if (Object::cast_to<RunCommand>(command)){
            command->run(p_delta);
            
        }else if (Object::cast_to<StopCommand>(command)){
            command->run(p_delta);
            owner->get_state_machine()->set_state("idle");
        }

        owner->remove_last_command();
    }
}

void PlayerRun::on_state_exit() {}
