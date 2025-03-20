#include "player_idle.h"

#include "godot_cpp/classes/input.hpp"
#include "godot_cpp/classes/object.hpp"
#include "player.h"

void PlayerIdle::on_state_run(double_t p_delta) {
    if (Input::get_singleton()->is_action_just_pressed("up") ||
        Input::get_singleton()->is_action_just_pressed("down") ||
        Input::get_singleton()->is_action_just_pressed("left") ||
        Input::get_singleton()->is_action_just_pressed("right")) {
          if (Player* player  = Object::cast_to<Player>(owner)){
            owner->set_move_direction(
              Input::get_singleton()->get_vector("left", "right", "up", "down"));
            player->get_state_machine()->set_state("run");
          }
    }
}

void PlayerIdle::on_state_exit() {}

void PlayerIdle::_bind_methods() {}

void PlayerIdle::on_state_enter(Character *p_owner) { this->owner = p_owner; }
