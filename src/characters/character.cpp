#include "character.h"

#include <cmath>

#include "core/command.h"
#include "core/state_machine.h"
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
#include "tools/auto_resgister.h"

AUTO_REGISTER_CLASS(Character)

// -----------------------------------------
// Constructor & Initialization
// -----------------------------------------
Character::Character() {
	set_physics_process(true);
	set_process(true);

	look_direction = LookDirection::DOWN;

	life = 100.0f;
	max_life = 100.0f;
	max_move_speed = 100.0f;
	acceleration = 0.2f;
	friction = 0.2f;
	display_name = "No name";
	move_direction = Vector2(0.0f, 0.0f);
	has_animation_in_child = false;

	connect_animation_in_child();
}
Character::~Character() {
	disconnect_animation_in_child();
}
// -----------------------------------------
// Command Handling
// -----------------------------------------
void Character::enqueue_command(Ref<Command> p_command) {
	if (p_command.is_null()) {
		ERR_PRINT("Attempted to add a null command.");
		return;
	}
	commands.append(p_command);
}

void Character::remove_last_command() {
	if (commands.is_empty()) {
		ERR_PRINT("Attempted to remove a command from an empty command list.");
		return;
	}
	commands.remove_at(commands.size() - 1);
}

void Character::clear_all_commands() {
	if (!commands.is_empty()) {
		commands.clear();
	}
}

size_t Character::get_commands_size() const {
	return commands.size();
}

Ref<Command> Character::get_last_command() const {
	if (!commands.is_empty()) {
		return commands[commands.size() - 1];

	} else {
		ERR_PRINT("Attempted to get the last command from an empty command list.");
		return nullptr;
	}
}

// -----------------------------------------
// Animation Handling
// -----------------------------------------
void Character::set_animation(const String &p_anim_name) {
	auto new_animation_name = StringName(vformat("%s_%s", p_anim_name, get_look_direction()));

	if (!animation_player) {
		UtilityFunctions::printerr("Animation player could not be null ");
		return;
	}
	if (animation_player->has_animation(new_animation_name)) {
		current_animation = p_anim_name;
		UtilityFunctions::print("setting animation to: ", p_anim_name);
	} else {
		UtilityFunctions::printerr(vformat("No animation found in %s for key %s", get_name(), new_animation_name));
	}
}

void Character::update_animation() {
	if (animation_player) {
		auto new_animation_name = StringName(vformat("%s_%s", current_animation, get_look_direction()));
		if (animation_player->get_current_animation() != new_animation_name) {
			animation_player->set_current_animation(new_animation_name);
			animation_player->play();
		}
	}
}
void Character::_on_child_node_tree_exit(Node *p_node) {
	if (AnimationPlayer *node = cast_to<AnimationPlayer>(p_node)) {
		if (node == animation_player) {
			animation_player = nullptr;
		}
	}
}
void Character::_on_child_node_tree_entered(Node *p_node) {
	if (animation_player) {
		return;
	}
	if (AnimationPlayer *node = cast_to<AnimationPlayer>(p_node)) {
		animation_player = node;
	}
}
void Character::connect_animation_in_child() {
	if (!this->is_connected("child_entered_tree", callable_mp(this, &Character::_on_child_node_tree_entered)) && !this->is_connected("child_exiting_tree", callable_mp(this, &Character::_on_child_node_tree_exit))) {
		this->connect("child_entered_tree", callable_mp(this, &Character::_on_child_node_tree_entered));
		this->connect("child_exiting_tree", callable_mp(this, &Character::_on_child_node_tree_exit));
	}

}
void Character::disconnect_animation_in_child() {
	if (this->is_connected("child_entered_tree", callable_mp(this, &Character::_on_child_node_tree_entered)) && this->is_connected("child_exiting_tree", callable_mp(this, &Character::_on_child_node_tree_exit))) {
		this->disconnect("child_entered_tree", callable_mp(this, &Character::_on_child_node_tree_entered));
		this->disconnect("child_exiting_tree", callable_mp(this, &Character::_on_child_node_tree_exit));
		animation_player = nullptr;
	}
}

// -----------------------------------------
// Movement Handling
// -----------------------------------------
void Character::set_movement(Vector2 p_direction) {
	const Vector2 current_velocity = get_velocity();
	const Vector2 target_velocity = p_direction.normalized() * max_move_speed;

	Vector2 new_velocity;
	new_velocity.x = Math::lerp(current_velocity.x, target_velocity.x, acceleration);
	new_velocity.y = Math::lerp(current_velocity.y, target_velocity.y, acceleration);

	set_velocity(new_velocity);
}

void Character::apply_movement() {
	if (move_and_slide()) {
		for (int i = 0; i < get_slide_collision_count(); i++) {
			const Ref<KinematicCollision2D> collision = get_slide_collision(i);
			Vector2 normal = collision->get_normal();

			if (Vector2 velocity = get_velocity(); velocity.dot(normal) > 0) {
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
	if (get_velocity().length_squared() > 0.01f) {
		const Vector2 velocity = get_velocity();
		look_direction =
				(Math::abs(velocity.x) > Math::abs(velocity.y)) ? (velocity.x > 0.0f ? RIGHT : LEFT) : (velocity.y > 0.0f ? DOWN : UP);
	}
}

String Character::get_look_direction() const {
	switch (look_direction) {
		case UP:
			return "up";
		case DOWN:
			return "down";
		case LEFT:
			return "left";
		case RIGHT:
			return "right";
		default:
			return "down";
	}
}

// -----------------------------------------
// Life cycle methods
// -----------------------------------------
void Character::_ready() {
	set_process(true);
}
void Character::_physics_process(double_t p_delta) {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	if (move_direction.length_squared() > 0.01f) {
		set_movement(move_direction);
		apply_movement();
	} else if (get_velocity().length_squared() > 0.02f) {
		apply_friction();
		apply_movement();
	}
}
void Character::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			state_machine = Ref(memnew(StateMachine));
			state_machine->set_character(this);

			connect_animation_in_child();
			break;
		}

		case NOTIFICATION_EXIT_TREE:
			disconnect_animation_in_child();
			break;

		case NOTIFICATION_PROCESS: {
			if (Engine::get_singleton()->is_editor_hint()) {
				return;
			}

			const double delta = get_process_delta_time();
			if (!state_machine.is_null() && state_machine.is_valid()) {
				state_machine->update(static_cast<float>(delta));
			}

			update_look_direction();
			update_animation();

			if (commands.is_empty()) {
				return;
			}

			for (const Ref<Command> &command : commands) {
				command->_gdvirtual_run_call(static_cast<float>(delta));
			}

			commands.clear();

			break;
		}

		default:
			break;
	}
}
PackedStringArray Character::_get_configuration_warnings() const {
	PackedStringArray warnings = CharacterBody2D::_get_configuration_warnings();

	if (!animation_player) {
		warnings.append(
				"This node requires an AnimationPlayer as a child node for animations to work "
				"correctly.\n"
				"Without it, animation playback will not function, and related features such as "
				"state-driven animations will be disabled.\n"
				"Please add an AnimationPlayer node as a child of this Character.");
	}

	return warnings;
}

// -----------------------------------------
// Property Bindings
// -----------------------------------------
void Character::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_life"), &Character::get_life);
	ClassDB::bind_method(D_METHOD("set_life", "p_life"), &Character::set_life);

	ClassDB::bind_method(D_METHOD("get_acceleration"), &Character::get_acceleration);
	ClassDB::bind_method(D_METHOD("set_acceleration", "p_acceleration"), &Character::set_acceleration);

	ClassDB::bind_method(D_METHOD("get_friction"), &Character::get_friction);
	ClassDB::bind_method(D_METHOD("set_friction", "p_friction"), &Character::set_friction);

	ClassDB::bind_method(D_METHOD("get_max_move_speed"), &Character::get_max_move_speed);
	ClassDB::bind_method(D_METHOD("set_max_move_speed", "p_max_move_speed"), &Character::set_max_move_speed);

	ClassDB::bind_method(D_METHOD("get_move_direction"), &Character::get_move_direction);
	ClassDB::bind_method(D_METHOD("set_move_direction", "p_max_move_speed"), &Character::set_move_direction);

	ClassDB::bind_method(D_METHOD("get_display_name"), &Character::get_display_name);
	ClassDB::bind_method(D_METHOD("set_display_name", "p_display_name"), &Character::set_display_name);

	ClassDB::bind_method(D_METHOD("get_state_machine"), &Character::get_state_machine);
	ClassDB::bind_method(D_METHOD("set_animation", "p_animation_name"), &Character::set_animation);

	ClassDB::bind_method(D_METHOD("add_enqueue", "p_command"), &Character::enqueue_command);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "life"), "set_life", "get_life");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "acceleration", PROPERTY_HINT_RANGE, "0.01,1,0.01"), "set_acceleration", "get_acceleration");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "friction", PROPERTY_HINT_RANGE, "0.01,1,0.01"), "set_friction", "get_friction");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_move_speed"), "set_max_move_speed", "get_max_move_speed");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "move_direction"), "set_move_direction", "get_move_direction");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "display_name"), "set_display_name", "get_display_name");
}

// -----------------------------------------
// Getters & Setters
// -----------------------------------------
float_t Character::get_life() const {
	return life;
}
void Character::set_life(float_t p_life) {
	life = p_life;
}

float_t Character::get_max_move_speed() const {
	return max_move_speed;
}
void Character::set_max_move_speed(float_t p_max_move_speed) {
	max_move_speed = p_max_move_speed;
}

float_t Character::get_acceleration() const {
	return acceleration;
}
void Character::set_acceleration(float_t p_acceleration) {
	acceleration = p_acceleration;
}

float_t Character::get_friction() const {
	return friction;
}
void Character::set_friction(float_t p_friction) {
	friction = p_friction;
}

Vector2 Character::get_move_direction() const {
	return move_direction;
}
void Character::set_move_direction(Vector2 p_move_direction) {
	move_direction = p_move_direction;
}

String Character::get_display_name() const {
	return display_name;
}
void Character::set_display_name(String p_display_name) {
	display_name = p_display_name;
}

Ref<StateMachine> Character::get_state_machine() {
	if (state_machine.is_null() || !state_machine.is_valid()) {
		state_machine = Ref(memnew(StateMachine));
		state_machine->set_character(this);
	}
	return state_machine;
}
