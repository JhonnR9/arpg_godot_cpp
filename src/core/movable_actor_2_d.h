
#ifndef MOVABLE_ACTOR_2_D_H
#define MOVABLE_ACTOR_2_D_H

#include "godot_cpp/classes/character_body2d.hpp"
#include "godot_cpp/classes/physics_server2d.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include <godot_cpp/classes/physics_body2d.hpp>

using namespace godot;

class MovableActor2D : public CharacterBody2D {
	GDCLASS(MovableActor2D, CharacterBody2D)

protected:
	static void _bind_methods();

public:
	MovableActor2D();

	void _notification(int p_what);

	real_t get_friction() const;
	void set_friction(real_t p_friction);

	real_t get_acceleration() const;
	void set_acceleration(real_t p_acceleration);

	real_t get_motion_target_speed() const;
	void set_motion_target_speed(real_t p_motion_target_speed);

	void set_move_direction(const Vector2 p_move_direction);
private:
	real_t motion_target_speed;
	real_t acceleration;
	real_t friction;
	Vector2 move_direction;

	bool _has_emitting_start_move;
	bool _has_emitting_stop_move;

	void _calculate_acceleration();
	void _calculate_friction();
	void _apply_motion_and_slide();

};

#endif //MOVABLE_ACTOR_2_D_H
