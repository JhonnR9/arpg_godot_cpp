#include "character.h"

// Standard Library
#include <cmath>

// Project includes
#include "core/command.h"

// Godot includes
#include "godot_cpp/classes/animation_player.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/node.hpp"
#include "godot_cpp/classes/object.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/core/class_db.hpp"
#include "godot_cpp/core/math.hpp"
#include "godot_cpp/core/object.hpp"
#include "godot_cpp/core/property_info.hpp"
#include "godot_cpp/variant/string_name.hpp"
#include "godot_cpp/variant/typed_array.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "godot_cpp/variant/vector2.hpp"
#include "godot_cpp/variant/vector3.hpp"

// -----------------------------------------
// Constants
// -----------------------------------------
namespace {
const char* DEFAULT_DISPLAY_NAME = "No name";
const char* LOOK_UP = "up";
const char* LOOK_DOWN = "down";
const char* LOOK_LEFT = "left";
const char* LOOK_RIGHT = "right";
}  // namespace

// -----------------------------------------
// Constructor & Initialization
// -----------------------------------------
Character::Character() {
    set_physics_process(true);
    set_process(true);
    look_direction = LookDirection::DOWN;
    state_machine.instantiate();

    life = 100.0f;
    max_life = 100.0f;
    max_move_speed = 100.0f;
    acceleration = 0.2f;
    friction = 0.2f;
    display_name = DEFAULT_DISPLAY_NAME;
    move_direction = Vector2(0.0f, 0.0f);
}

void Character::_ready() {
    TypedArray<Node> childs = get_children();

    for (size_t i = 0; i < childs.size(); i++) {
        if (auto anim = Object::cast_to<AnimationPlayer>(childs[i])) {
            animation_player = anim;
            break;
        }
    }
    set_physics_process(true);
}

// -----------------------------------------
// Command Handling
// -----------------------------------------
void Character::add_command(Ref<Command> p_command) { commands.append(p_command); }

void Character::remove_last_command() { commands.remove_at(commands.size() - 1); }

void Character::clear_all_commands() { commands.clear(); }

size_t Character::get_commands_size() const { return commands.size(); }

Ref<Command> Character::get_last_command() const {
    if (!commands.is_empty()) {
        return commands[commands.size() - 1];
    }
    return nullptr;
}

// -----------------------------------------
// Animation Handling
// -----------------------------------------
void Character::set_animation(String p_anim_name) {
    auto new_animation_name = StringName(vformat("%s_%s", p_anim_name, get_look_direction()));
    if (animation_player->has_animation(new_animation_name)) {
        current_animation = p_anim_name;
    }
    else {
        UtilityFunctions::printerr(
            vformat("No animation found in %s for key %s", get_name(), new_animation_name));
    }
}

void Character::update_animation()
{
    if (animation_player) {
        auto new_animation_name = StringName(vformat("%s_%s", current_animation, get_look_direction()));
        animation_player->set_current_animation(new_animation_name);
    }
}

// -----------------------------------------
// Movement Handling
// -----------------------------------------
void Character::set_movement(Vector2 p_direction) {
    Vector2 current_velocity = get_velocity();
    Vector2 target_velocity = p_direction.normalized() * max_move_speed;

    Vector2 new_velocity;
    new_velocity.x = Math::lerp(current_velocity.x, target_velocity.x, acceleration);
    new_velocity.y = Math::lerp(current_velocity.y, target_velocity.y, acceleration);

    set_velocity(new_velocity);
}

void Character::apply_movement() {
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
    look_direction = (Math::abs(get_velocity().x) > Math::abs(get_velocity().y))? 
    (get_velocity().x > 0.0f ? RIGHT : LEFT)
    : (get_velocity().y > 0.0f ? DOWN : UP);
}

String Character::get_look_direction() const {
    switch (look_direction) {
        case UP:
            return LOOK_UP;
        case DOWN:
            return LOOK_DOWN;
        case LEFT:
            return LOOK_LEFT;
        case RIGHT:
            return LOOK_RIGHT;
    }
    return LOOK_DOWN;
}

// -----------------------------------------
// Physics Processing
// -----------------------------------------
void Character::_physics_process(double_t p_delta) {
    if (move_direction.length_squared() > 0.01f) {
        set_movement(move_direction);
        apply_movement();
    } else if (get_velocity().length_squared() > 0.0f) {
        apply_friction();
        apply_movement();
    }
}
// -----------------------------------------
// Processing
// -----------------------------------------
void Character::_process(double p_delta) {
    update_look_direction();
    update_animation();

}

// -----------------------------------------
// Property Bindings
// -----------------------------------------
void Character::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_life"), &Character::get_life);
    ClassDB::bind_method(D_METHOD("set_life", "p_life"), &Character::set_life);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "life"), "set_life", "get_life");

    ClassDB::bind_method(D_METHOD("get_acceleration"), &Character::get_acceleration);
    ClassDB::bind_method(D_METHOD("set_acceleration", "p_acceleration"),
                         &Character::set_acceleration);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "acceleration", PROPERTY_HINT_RANGE, "0.01,1,0.01"),
                 "set_acceleration", "get_acceleration");

    ClassDB::bind_method(D_METHOD("get_friction"), &Character::get_friction);
    ClassDB::bind_method(D_METHOD("set_friction", "p_friction"), &Character::set_friction);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "friction", PROPERTY_HINT_RANGE, "0.01,1,0.01"),
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

// -----------------------------------------
// Getters & Setters
// -----------------------------------------
float_t Character::get_life() const { return life; }
void Character::set_life(float_t p_life) { life = p_life; }

float_t Character::get_max_move_speed() const { return max_move_speed; }
void Character::set_max_move_speed(float_t p_max_move_speed) { max_move_speed = p_max_move_speed; }

float_t Character::get_acceleration() const { return acceleration; }
void Character::set_acceleration(float_t p_acceleration) { acceleration = p_acceleration; }

float_t Character::get_friction() const { return friction; }
void Character::set_friction(float_t p_friction) { friction = p_friction; }

Vector2 Character::get_move_direction() const { return move_direction; }
void Character::set_move_direction(Vector2 p_move_direction) { move_direction = p_move_direction; }

String Character::get_display_name() const { return display_name; }
void Character::set_display_name(String p_display_name) { display_name = p_display_name; }

StateMachine* Character::get_state_machine() { return state_machine.ptr(); }
