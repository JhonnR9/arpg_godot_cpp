
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
		move_direction_type(FOUR_BASED) {
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
FloatingCharacter2D::Direction FloatingCharacter2D::_get_move_direction_name() const {
	const real_t angle = fmod(get_velocity().angle(), 360.0f); // Get the angle of the character's velocity vector

	// Four-direction movement (Up, Down, Left, Right)
	if (move_direction_type == FOUR_BASED) {
		// Check if the angle is within the ranges for each direction
		if (angle >= -45.0f && angle < 45.0f) {
			return RIGHT; // Right direction (0°)
		}
		if (angle >= 45.0f && angle < 135.0f) {
			return UP; // Up direction (90°)
		}
		if (angle >= 135.0f || angle < -135.0f) {
			return LEFT; // Left direction (180°)
		}
		{
			return DOWN; // Down direction (270°)
		}
	}
	// Eight-direction movement (Up, Down, Left, Right, and diagonals)
	if (move_direction_type == EIGHT_BASED) {
		// Check the angle ranges for each direction, including diagonals
		if (angle >= -22.5f && angle < 22.5f) {
			return RIGHT; // Right direction (0°)
		}
		if (angle >= 22.5f && angle < 67.5f) {
			return RIGHT_UP; // Upper-right diagonal (45°)
		}
		if (angle >= 67.5f && angle < 112.5f) {
			return UP; // Up direction (90°)
		}
		if (angle >= 112.5f && angle < 157.5f) {
			return LEFT_UP; // Upper-left diagonal (135°)
		}
		if (angle >= 157.5f || angle < -157.5f) {
			return LEFT; // Left direction (180°)
		}
		if (angle >= -157.5f && angle < -112.5f) {
			return LEFT_DOWN; // Lower-left diagonal (225°)
		}
		if (angle >= -112.5f && angle < -67.5f) {
			return DOWN; // Down direction (270°)
		}

		return RIGHT_DOWN; // Lower-right diagonal (315°)
	}
	return DOWN;
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

void FloatingCharacter2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_friction"), &FloatingCharacter2D::get_friction);
	ClassDB::bind_method(D_METHOD("set_friction", "p_friction"), &FloatingCharacter2D::set_friction);

	ClassDB::bind_method(D_METHOD("get_motion_target_speed"), &FloatingCharacter2D::get_motion_target_speed);
	ClassDB::bind_method(D_METHOD("set_motion_target_speed", "p_motion_target_speed"), &FloatingCharacter2D::set_motion_target_speed);

	ClassDB::bind_method(D_METHOD("get_acceleration"), &FloatingCharacter2D::get_acceleration);
	ClassDB::bind_method(D_METHOD("set_acceleration", "p_acceleration"), &FloatingCharacter2D::set_acceleration);

	ClassDB::bind_method(D_METHOD("set_move_direction", "p_move_direction"), &FloatingCharacter2D::set_move_direction);

	ClassDB::bind_method(D_METHOD("get_move_direction_name"), &FloatingCharacter2D::_get_move_direction_name);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "acceleration", PROPERTY_HINT_RANGE, "0.0,1,0.01"), "set_acceleration", "get_acceleration");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "motion_target_speed"), "set_motion_target_speed", "get_motion_target_speed");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "friction", PROPERTY_HINT_RANGE, "0.01,1,0.01"), "set_friction", "get_friction");

	ADD_SIGNAL(MethodInfo("start_move"));
	ADD_SIGNAL(MethodInfo("stop_move"));

	BIND_ENUM_CONSTANT(LEFT);
	BIND_ENUM_CONSTANT(RIGHT);
	BIND_ENUM_CONSTANT(UP);
	BIND_ENUM_CONSTANT(DOWN);
	BIND_ENUM_CONSTANT(LEFT_UP);
	BIND_ENUM_CONSTANT(LEFT_DOWN);
	BIND_ENUM_CONSTANT(RIGHT_UP);
	BIND_ENUM_CONSTANT(RIGHT_DOWN);
}
