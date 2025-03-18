#include "player_idle.h"
#include "godot_cpp/variant/utility_functions.hpp"

void PlayerIdle::on_state_run(double_t p_delta) {
  //UtilityFunctions::print("update");
}

void PlayerIdle::on_state_exit() {}

void PlayerIdle::_bind_methods() {}

void PlayerIdle::on_state_enter(Character *p_owner) {
  this->owner = p_owner;
  //UtilityFunctions::print("enter");
}
