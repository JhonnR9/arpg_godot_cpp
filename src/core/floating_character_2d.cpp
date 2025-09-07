
#include "floating_character_2d.h"
#include "godot_cpp/classes/engine.hpp"
#include "tools/auto_resgister.h"

AUTO_REGISTER_CLASS(FloatingCharacter2D)

FloatingCharacter2D::FloatingCharacter2D() :
		motion_target_speed(100.f),
		acceleration(0.2f),
		friction(0.8f),
		move_direction(Vector2()),
		emitting_start_move(false),
		emitting_stop_move(false),
		move_direction_name(DOWN),
		move_direction_mode(FOUR_BASED) {
	// use this for top-down games
	set_motion_mode(MOTION_MODE_FLOATING);
	set_physics_process(true);
}
void FloatingCharacter2D::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_PHYSICS_PROCESS:
			if (Engine::get_singleton()->is_editor_hint()) {
				return;
			}
			_apply_motion_and_slide();
			_calculate_move_dir_name();
			break;

		default:
			break;
	}
}
real_t FloatingCharacter2D::get_friction() const {
	return friction;
}
void FloatingCharacter2D::set_friction(real_t p_friction) {
	friction = CLAMP(p_friction, 0.0, 1.0);
}
real_t FloatingCharacter2D::get_acceleration() const {
	return acceleration;
}
void FloatingCharacter2D::set_acceleration(real_t p_acceleration) {
	acceleration = CLAMP(p_acceleration, 0.0, 1.0);
}
real_t FloatingCharacter2D::get_motion_target_speed() const {
	return motion_target_speed;
}
void FloatingCharacter2D::set_motion_target_speed(real_t p_motion_target_speed) {
	motion_target_speed = p_motion_target_speed;
}
void FloatingCharacter2D::set_move_direction(const Vector2 p_move_direction) {
	move_direction = p_move_direction;
}
FloatingCharacter2D::Direction FloatingCharacter2D::get_move_direction_name() const {
	return move_direction_name;
}

void FloatingCharacter2D::set_move_direction_name(Direction p_move_direction_name) {
	move_direction_name = p_move_direction_name;
}
FloatingCharacter2D::DirectionMode FloatingCharacter2D::get_move_direction_mode() const {
	return move_direction_mode;
}
void FloatingCharacter2D::set_move_direction_mode(DirectionMode p_move_direction_type) {
	move_direction_mode = p_move_direction_type;
}

void FloatingCharacter2D::_calculate_acceleration() {
	emitting_stop_move = false;

	if (!emitting_start_move) {
		emit_signal("start_move");
		emitting_start_move = true;
	}

	const Vector2 current_velocity = get_velocity();
	const Vector2 current_target_velocity = move_direction * motion_target_speed;

	Vector2 new_velocity;
	new_velocity.x = Math::lerp(current_velocity.x, current_target_velocity.x, acceleration);
	new_velocity.y = Math::lerp(current_velocity.y, current_target_velocity.y, acceleration);

	set_velocity(new_velocity);
}

void FloatingCharacter2D::_calculate_friction() {
	emitting_start_move = false;

	if (!emitting_stop_move) {
		emit_signal("stop_move");
		emitting_stop_move = true;
	}
	Vector2 new_velocity = get_velocity();
	// use simple linear interpolation
	new_velocity.x = Math::lerp(new_velocity.x, 0.0f, friction);
	new_velocity.y = Math::lerp(new_velocity.y, 0.0f, friction);

	constexpr real_t v_threshold_squared = 0.01f;
	if (new_velocity.length_squared() < v_threshold_squared) {
		new_velocity = Vector2(0.0f, 0.0f);
	}

	set_velocity(new_velocity);
}
void FloatingCharacter2D::_apply_motion_and_slide() {
	constexpr real_t v_threshold_squared = 0.01f;
	if (move_direction.length_squared() > v_threshold_squared) {
		_calculate_acceleration();

	} else {
		_calculate_friction();
	}

	move_and_slide();
}
void FloatingCharacter2D::_calculate_move_dir_name() {
	const Vector2 velocity = get_velocity();
	Direction move_cache = move_direction_name;
	// For four directions (Up, Down, Left, Right) this is more fast for performance
	if (move_direction_mode == FOUR_BASED) {
		move_direction_name =
				(Math::abs(velocity.x) > Math::abs(velocity.y)) ? (velocity.x > 0.0f ? RIGHT : LEFT) : (velocity.y > 0.0f ? DOWN : UP);
	}

	// For eight directions (Up, Down, Left, Right, and diagonals)
	const real_t angle = fmod(velocity.angle(), 360.0f); // Get the angle of the character's velocity vector

	if (move_direction_mode == EIGHT_BASED) {
		static constexpr Direction directions[] = { RIGHT, RIGHT_UP, UP, LEFT_UP, LEFT, LEFT_DOWN, DOWN, RIGHT_DOWN };
		const real_t normalized_angle = fmod(angle + 22.5f, 360.0f); // Normalize angle for easier mapping
		const int index = static_cast<int>(normalized_angle / 45.0f) % 8;

		move_direction_name = directions[index];
	}

	if (move_direction_name != move_cache) {
		emit_signal("direction_name_changed", move_direction_name);
	}
}

void FloatingCharacter2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_friction"), &FloatingCharacter2D::get_friction);
	ClassDB::bind_method(D_METHOD("set_friction", "p_friction"), &FloatingCharacter2D::set_friction);

	ClassDB::bind_method(D_METHOD("get_motion_target_speed"), &FloatingCharacter2D::get_motion_target_speed);
	ClassDB::bind_method(D_METHOD("set_motion_target_speed", "p_motion_target_speed"), &FloatingCharacter2D::set_motion_target_speed);

	ClassDB::bind_method(D_METHOD("get_acceleration"), &FloatingCharacter2D::get_acceleration);
	ClassDB::bind_method(D_METHOD("set_acceleration", "p_acceleration"), &FloatingCharacter2D::set_acceleration);

	ClassDB::bind_method(D_METHOD("set_move_direction", "p_move_direction"), &FloatingCharacter2D::set_move_direction);

	ClassDB::bind_method(D_METHOD("get_move_direction_name"), &FloatingCharacter2D::get_move_direction_name);

	ClassDB::bind_method(D_METHOD("get_move_direction_mode"), &FloatingCharacter2D::get_move_direction_mode);
	ClassDB::bind_method(D_METHOD("set_move_direction_mode", "p_move_direction_mode"), &FloatingCharacter2D::set_move_direction_mode);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "acceleration", PROPERTY_HINT_RANGE, "0.0,1,0.01"), "set_acceleration", "get_acceleration");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "motion_target_speed"), "set_motion_target_speed", "get_motion_target_speed");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "friction", PROPERTY_HINT_RANGE, "0.01,1,0.01"), "set_friction", "get_friction");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "move_direction_mode", PROPERTY_HINT_ENUM, "FourBased,EightBased"), "set_move_direction_mode", "get_move_direction_mode");

	ADD_SIGNAL(MethodInfo("start_move"));
	ADD_SIGNAL(MethodInfo("stop_move"));
	ADD_SIGNAL(MethodInfo("direction_name_changed", PropertyInfo(Variant::INT, "move_direction_name")));

	BIND_ENUM_CONSTANT(LEFT);
	BIND_ENUM_CONSTANT(RIGHT);
	BIND_ENUM_CONSTANT(UP);
	BIND_ENUM_CONSTANT(DOWN);
	BIND_ENUM_CONSTANT(LEFT_UP);
	BIND_ENUM_CONSTANT(LEFT_DOWN);
	BIND_ENUM_CONSTANT(RIGHT_UP);
	BIND_ENUM_CONSTANT(RIGHT_DOWN);

	BIND_ENUM_CONSTANT(FOUR_BASED)
	BIND_ENUM_CONSTANT(EIGHT_BASED)
}
