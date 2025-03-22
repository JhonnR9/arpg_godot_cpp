#include "player_idle.h"
#include <functional>

#include "godot_cpp/classes/input.hpp"
#include "godot_cpp/classes/object.hpp"
#include "player.h"

void PlayerIdle::on_state_run(double_t p_delta) {
    Input *input = Input::get_singleton();
    if (input->is_action_just_pressed("up") || input->is_action_just_pressed("down") ||
        input->is_action_just_pressed("left") ||input->is_action_just_pressed("right")) {
        if (Player* player = Object::cast_to<Player>(owner)) {
            owner->set_move_direction(
                Input::get_singleton()->get_vector("left", "right", "up", "down"));
            player->get_state_machine()->set_state("run");
        }
    }
}

void PlayerIdle::on_state_exit() {}

void PlayerIdle::_bind_methods() {}

void PlayerIdle::on_state_enter(Character* p_owner) { this->owner = p_owner; }
