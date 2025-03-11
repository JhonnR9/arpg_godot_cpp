#include "character.h"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/math.hpp"
#include "godot_cpp/core/object.hpp"
#include "godot_cpp/core/property_info.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "godot_cpp/variant/vector2.hpp"
#include "godot_cpp/variant/vector3.hpp"
#include <cmath>




void Character::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_life"), &Character::get_life);
    ClassDB::bind_method(D_METHOD("set_life",  "p_life"), &Character::set_life);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "life"), "set_life", "get_life");

    ClassDB::bind_method(D_METHOD("get_acceleration"), &Character::get_acceleration);
    ClassDB::bind_method(D_METHOD("set_acceleration",  "p_acceleration"), &Character::set_acceleration);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "acceleration", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_acceleration", "get_acceleration");

    ClassDB::bind_method(D_METHOD("get_friction"), &Character::get_friction);
    ClassDB::bind_method(D_METHOD("set_friction",  "p_friction"), &Character::set_friction);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "friction", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_friction", "get_friction");


    ClassDB::bind_method(D_METHOD("get_max_move_speed"), &Character::get_max_move_speed);
    ClassDB::bind_method(D_METHOD("set_max_move_speed",  "p_max_move_speed"), &Character::set_max_move_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_move_speed"), "set_max_move_speed", "get_max_move_speed");
    
}
Character::Character() {
    set_physics_process(true);
}

void Character::move(Vector2 p_direction) {
    Vector2 currentVelocity = get_velocity();
    Vector2 targetVelocity = p_direction.normalized() * max_move_speed;

    Vector2 new_velocity;
    new_velocity.x = Math::lerp(currentVelocity.x, targetVelocity.x, acceleration);
    new_velocity.y = Math::lerp(currentVelocity.y, targetVelocity.y, acceleration);
  
    set_velocity(new_velocity);
}

void Character::apply_friction(){
    Vector2 currentVelocity = get_velocity();
    Vector2 new_velocity;

    new_velocity.x = Math::lerp(currentVelocity.x, 0.0f, friction);
    new_velocity.y = Math::lerp(currentVelocity.y, 0.0f, friction);

    set_velocity(new_velocity);

}


void Character::_physics_process(double_t p_delta){
    move_and_slide();
}

float_t Character::get_life(){
    return life;
}

void Character::set_life(float_t p_life){
    life = p_life;
}

float_t Character::get_max_move_speed(){
    return max_move_speed;
}

void Character::set_max_move_speed(float_t p_max_move_speed){
    max_move_speed = p_max_move_speed;
}

float_t Character::get_acceleration(){
    return acceleration;
}

void Character::set_acceleration(float_t p_acceleration){
    acceleration = p_acceleration;
}

float_t Character::get_friction(){
    return friction;
}

void Character::set_friction(float_t p_friction){
    friction = p_friction;
}
