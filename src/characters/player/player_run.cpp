#include "player_run.h"

#include "godot_cpp/classes/input.hpp"
#include "player.h"

void PlayerRun::_bind_methods() {}

void PlayerRun::on_state_enter(Character* p_owner) { this->owner = p_owner; }

void PlayerRun::on_state_run(double_t p_delta) {
    if (owner->get_velocity().length_squared() < 0.001f) {
        if (Player* player = Object::cast_to<Player>(owner)) {
            player->get_state_machine()->set_state("idle");
        }
       
    } else {
        
    } owner->set_move_direction(
        Input::get_singleton()->get_vector("left", "right", "up", "down"));
}

void PlayerRun::on_state_exit() {}
