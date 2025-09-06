
#include "movable_actor_2_d.h"

#include "godot_cpp/classes/engine.hpp"
#include "tools/auto_resgister.h"

AUTO_REGISTER_CLASS(MovableActor2D)

MovableActor2D::MovableActor2D() {
	// use this for top-down games
	set_motion_mode(MOTION_MODE_FLOATING);
	motion_target_speed = 100.f;
	acceleration = 0.2f;
	friction = 0.8f;
	move_direction = Vector2(0, 0);
	_has_emitting_start_move = false;
	_has_emitting_stop_move = false;

	set_physics_process(true);
}
void MovableActor2D::_notification(int p_what) {
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
real_t MovableActor2D::get_friction() const {
	return friction;
}
void MovableActor2D::set_friction(real_t p_friction) {
	friction = CLAMP(p_friction, 0.0, 1.0);
}
real_t MovableActor2D::get_acceleration() const {
	return acceleration;
}
void MovableActor2D::set_acceleration(real_t p_acceleration) {
	acceleration = CLAMP(p_acceleration, 0.0, 1.0);
}
real_t MovableActor2D::get_motion_target_speed() const {
	return motion_target_speed;
}
void MovableActor2D::set_motion_target_speed(real_t p_motion_target_speed) {
	motion_target_speed = p_motion_target_speed;
}
void MovableActor2D::set_move_direction(const Vector2 p_move_direction) {
	move_direction = p_move_direction;
}
void MovableActor2D::_calculate_acceleration() {
	_has_emitting_stop_move = false;

	if (!_has_emitting_start_move) {
		emit_signal("start_move");
		_has_emitting_start_move = true;
	}

	const Vector2 current_velocity = get_velocity();
	const Vector2 current_target_velocity = move_direction * motion_target_speed;

	Vector2 new_velocity;
	new_velocity.x = Math::lerp(current_velocity.x, current_target_velocity.x, acceleration);
	new_velocity.y = Math::lerp(current_velocity.y, current_target_velocity.y, acceleration);

	set_velocity(new_velocity);
}

void MovableActor2D::_calculate_friction() {
	_has_emitting_start_move = false;

	if (!_has_emitting_stop_move) {
		emit_signal("stop_move");
		_has_emitting_stop_move = true;
	}
	Vector2 new_velocity = get_velocity();
	// use simple linear interpolation
	new_velocity.x = Math::lerp(new_velocity.x, 0.0f, friction);
	new_velocity.y = Math::lerp(new_velocity.y, 0.0f, friction);

	constexpr real_t velocity_threshold = 0.01f;
	if (new_velocity.length_squared() < velocity_threshold) {
		new_velocity = Vector2(0.0f, 0.0f);
	}

	set_velocity(new_velocity);
}
void MovableActor2D::_apply_motion_and_slide() {
	constexpr real_t velocity_threshold = 0.01f;
	if (move_direction.length_squared() > velocity_threshold) {
		_calculate_acceleration();

	} else {
		_calculate_friction();
	}

	move_and_slide();
}

void MovableActor2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_friction"), &MovableActor2D::get_friction);
	ClassDB::bind_method(D_METHOD("set_friction", "p_friction"), &MovableActor2D::set_friction);

	ClassDB::bind_method(D_METHOD("get_motion_target_speed"), &MovableActor2D::get_motion_target_speed);
	ClassDB::bind_method(D_METHOD("set_motion_target_speed", "p_motion_target_speed"), &MovableActor2D::set_motion_target_speed);

	ClassDB::bind_method(D_METHOD("get_acceleration"), &MovableActor2D::get_acceleration);
	ClassDB::bind_method(D_METHOD("set_acceleration", "p_acceleration"), &MovableActor2D::set_acceleration);

	ClassDB::bind_method(D_METHOD("set_move_direction", "p_move_direction"), &MovableActor2D::set_move_direction);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "acceleration", PROPERTY_HINT_RANGE, "0.0,1,0.01"), "set_acceleration", "get_acceleration");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "motion_target_speed"), "set_motion_target_speed", "get_motion_target_speed");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "friction", PROPERTY_HINT_RANGE, "0.01,1,0.01"), "set_friction", "get_friction");

	ADD_SIGNAL(MethodInfo("start_move"));
	ADD_SIGNAL(MethodInfo("stop_move"));
}
