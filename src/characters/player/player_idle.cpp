#include "player_idle.h"


#include "commands/run_command.h"
#include "commands/stop_command.h"
#include "core/command.h"

#include "godot_cpp/classes/object.hpp"


void PlayerIdle::on_state_run(double_t p_delta) {

    if (owner->get_commands_size() > 0){

        Command* command = owner->get_last_command().ptr();

        if (Object::cast_to<RunCommand>(command)){
            command->run(p_delta);
            owner->get_state_machine()->set_state("run");
            
        }else if (Object::cast_to<StopCommand>(command)){
           command->run(p_delta);
        }
        owner->remove_last_command();
    }
}

void PlayerIdle::on_state_exit() {}

void PlayerIdle::_bind_methods() {}

void PlayerIdle::on_state_enter(Character* p_owner) { this->owner = p_owner; }
