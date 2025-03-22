#include "character.h"

#include <cmath>

#include "godot_cpp/classes/animation_player.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/object.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/math.hpp"
#include "godot_cpp/core/object.hpp"
#include "godot_cpp/core/property_info.hpp"
#include "godot_cpp/variant/typed_array.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "godot_cpp/variant/vector2.hpp"
#include "godot_cpp/variant/vector3.hpp"

void Character::_ready() {
    life = 100.0f;
    max_life = 100.0f;
    max_move_speed = 100.0f;
    acceleration = 0.2f;
    friction = 0.2f;
    display_name = "No name";
    move_direction = Vector2(0.0f, 0.0f);

    TypedArray<Node> childs = get_children();

    for (size_t i = 0; i < childs.size(); i++) {
        if (auto anim = Object::cast_to<AnimationPlayer>(childs[i])) {
            animation_player = anim;
            break;
        }
    }
    set_physics_process(true);
}
StateMachine* Character::get_state_machine()
{
    return state_machine.ptr();
}
void Character::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_life"), &Character::get_life);
    ClassDB::bind_method(D_METHOD("set_life", "p_life"), &Character::set_life);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "life"), "set_life", "get_life");

    ClassDB::bind_method(D_METHOD("get_move_direction"), &Character::get_move_direction);
    ClassDB::bind_method(D_METHOD("set_move_direction", "p_move_direction"),
                         &Character::set_move_direction);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "move_direction"), "set_move_direction",
                 "get_move_direction");

    ClassDB::bind_method(D_METHOD("get_acceleration"), &Character::get_acceleration);
    ClassDB::bind_method(D_METHOD("set_acceleration", "p_acceleration"),
                         &Character::set_acceleration);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "acceleration", PROPERTY_HINT_RANGE, "0,1,0.01"),
                 "set_acceleration", "get_acceleration");

    ClassDB::bind_method(D_METHOD("get_friction"), &Character::get_friction);
    ClassDB::bind_method(D_METHOD("set_friction", "p_friction"), &Character::set_friction);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "friction", PROPERTY_HINT_RANGE, "0,1,0.01"),
                 "set_friction", "get_friction");

    ClassDB::bind_method(D_METHOD("get_max_move_speed"), &Character::get_max_move_speed);
    ClassDB::bind_method(D_METHOD("set_max_move_speed", "p_max_move_speed"),
                         &Character::set_max_move_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_move_speed"), "set_max_move_speed",
                 "get_max_move_speed");

    ClassDB::bind_method(D_METHOD("get_display_name"), &Character::get_display_name);
    ClassDB::bind_method(D_METHOD("set_display_name", "p_display_name"),
                         &Character::set_display_name);
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "display_name"), "set_display_name",
                 "get_display_name");
}
Character::Character() {
    set_physics_process(true);
    look_direction = LookDirection::DOWN;
    state_machine.instantiate();
}

void Character::move(Vector2 p_direction) {
    Vector2 currentVelocity = get_velocity();
    Vector2 targetVelocity = p_direction.normalized() * max_move_speed;

    Vector2 new_velocity;
    new_velocity.x = Math::lerp(currentVelocity.x, targetVelocity.x, acceleration);
    new_velocity.y = Math::lerp(currentVelocity.y, targetVelocity.y, acceleration);

    set_velocity(new_velocity);

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

void Character::apply_friction() {
    Vector2 new_velocity = get_velocity();
    new_velocity.x = Math::lerp(new_velocity.x, 0.0f, friction);
    new_velocity.y = Math::lerp(new_velocity.y, 0.0f, friction);

    if (new_velocity.length_squared() < 0.01f) {
        new_velocity = Vector2(0.0f, 0.0f);
    }

    set_velocity(new_velocity);
}

void Character::update_look_direction() {
    if (get_velocity().length_squared() == 0) {
        look_direction = DOWN;
        return;
    }

    look_direction = (Math::abs(get_velocity().x) > Math::abs(get_velocity().y))
                         ? (get_velocity().x > 0.0f ? RIGHT : LEFT)
                         : (get_velocity().y > 0.0f ? DOWN : UP);
}

String Character::get_look_direction() {
    switch (look_direction) {
        case UP:
            return "up";
        case DOWN:
            return "down";
        case LEFT:
            return "left";
        case RIGHT:
            return "right";
    }
}

void Character::_physics_process(double_t p_delta) {
    if (move_direction.length_squared() > 0) {
        move(move_direction);
    } else {
        apply_friction();
    }
}

float_t Character::get_life() { return life; }

void Character::set_life(float_t p_life) { life = p_life; }

float_t Character::get_max_move_speed() { return max_move_speed; }

void Character::set_max_move_speed(float_t p_max_move_speed) { max_move_speed = p_max_move_speed; }

float_t Character::get_acceleration() { return acceleration; }

void Character::set_acceleration(float_t p_acceleration) { acceleration = p_acceleration; }

float_t Character::get_friction() { return friction; }

void Character::set_friction(float_t p_friction) { friction = p_friction; }

Vector2 Character::get_move_direction() { return move_direction; }

void Character::set_move_direction(Vector2 p_move_direction) { move_direction = p_move_direction; }

String Character::get_display_name() { return display_name; }

void Character::set_display_name(String p_display_name) { display_name = p_display_name; }
