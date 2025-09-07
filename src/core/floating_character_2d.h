
#ifndef FLOATING_CHARACTER_2_D_H
#define FLOATING_CHARACTER_2_D_H

#include "godot_cpp/classes/character_body2d.hpp"

using namespace godot;

class FloatingCharacter2D : public CharacterBody2D {
	GDCLASS(FloatingCharacter2D, CharacterBody2D)

protected:
	static void _bind_methods();

public:
	FloatingCharacter2D();

	enum DirectionType {
		EIGHT_BASED,
		FOUR_BASED
	};

	enum Direction {
		LEFT,
		RIGHT,
		UP,
		DOWN,
		LEFT_UP,
		LEFT_DOWN,
		RIGHT_UP,
		RIGHT_DOWN
	};

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

	bool emitting_start_move;
	bool emitting_stop_move;

	Direction move_direction_name;
	DirectionType move_direction_type;

	void _calculate_acceleration();
	void _calculate_friction();
	void _apply_motion_and_slide();
	Direction _get_move_direction_name() const;
};

VARIANT_ENUM_CAST(FloatingCharacter2D::Direction);
VARIANT_ENUM_CAST(FloatingCharacter2D::DirectionType);

#endif //FLOATING_CHARACTER_2_D_H
