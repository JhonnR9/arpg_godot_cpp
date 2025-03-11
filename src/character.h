
#ifndef CHARACTER_H
#define CHARACTER_H

#include "godot_cpp/classes/character_body2d.hpp"
#include "godot_cpp/variant/vector2.hpp"
#include <cmath>

using namespace godot;

class Character : public CharacterBody2D {
	GDCLASS(Character, CharacterBody2D);
	
	float_t life {100.0f};
	float_t max_life {100.0f};
	float_t max_move_speed {50.0f};
	float_t acceleration {.2f};
	float_t friction {.2f};
	
protected:
	static void _bind_methods();

public:
	Character();

	void move(Vector2 p_direction);
	void apply_friction();

	virtual void _physics_process(double_t p_delta) override;

	float_t get_life();
	void set_life(float_t p_life);

	float_t get_move_speed();
	void set_move_speed(float_t p_move_speed);

	float_t get_max_move_speed();
	void set_max_move_speed(float_t p_move_speed);

	float_t get_acceleration();
	void set_acceleration(float_t p_accelaration);

	float_t get_friction();
	void set_friction(float_t p_friction);
	
};

#endif //  CHARACTER_H
