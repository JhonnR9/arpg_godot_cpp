#include "player.h"

#include "godot_cpp/variant/vector2.hpp"
#include <godot_cpp/classes/input.hpp>


void Player::_bind_methods() {
    
}

Player::Player() {
  set_physics_process(true);
}

void Player::_physics_process(double_t delta) {
  Vector2 direction = Input::get_singleton()->get_vector("left", "right", "up", "down");

  if (direction.length() > 0) {
      move(direction);
  } else {
      apply_friction();
  }

  if (move_and_slide()) {
      for (int i = 0; i < get_slide_collision_count(); i++) {
          Ref<KinematicCollision2D> collision = get_slide_collision(i);
          Vector2 normal = collision->get_normal();
          Vector2 velocity = get_velocity();
          if (velocity.dot(normal) > 0) {
              velocity = velocity.slide(normal);
              set_velocity(velocity);
          }
      }

  }
}

void Player::_process(double_t delta){

}