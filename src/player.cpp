#include "player.h"

#include "character.h"
#include "godot_cpp/variant/vector2.hpp"
#include <godot_cpp/classes/input.hpp>


void Player::_bind_methods() {}

Player::Player() {
  set_physics_process(true);
}

void Player::_physics_process(double_t delta){
  Vector2 direction = Input::get_singleton()->get_vector("left","right", "up", "down");

  if (direction.length() > 0) move(direction); else apply_friction();

  Character::_physics_process(delta);
}

void Player::_process(double_t delta){

}