
#ifndef CHARACTER_H
#define CHARACTER_H


#include "godot_cpp/classes/animation_player.hpp"
#include "godot_cpp/classes/character_body2d.hpp"
#include "godot_cpp/variant/node_path.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/vector2.hpp"
#include "core/state_machine.h"
#include <cmath>

using namespace godot;

class Character : public CharacterBody2D {
	GDCLASS(Character, CharacterBody2D);
	
	String display_name;
	float_t life;
	float_t max_life;
	float_t max_move_speed;
	float_t acceleration;
	float_t friction;
	Vector2 move_direction;

	//nodes
	AnimationPlayer* animation_player;

	enum LookDirection{
		UP, DOWN, LEFT, RIGHT
	};

	LookDirection look_direction;
	
protected:
	static void _bind_methods();
	Ref<StateMachine> state_machine;

public:
	Character();

	virtual void _ready()override;

	void move(Vector2 p_direction);
	void apply_friction();
	void update_look_direction();
	String get_look_direction();
	StateMachine* get_state_machine();

	virtual void _physics_process(double_t p_delta) override;

	float_t get_life();
	void set_life(float_t p_life);

	String get_display_name();
	void set_display_name(String p_display_name);

	float_t get_move_speed();
	void set_move_speed(float_t p_move_speed);

	float_t get_max_move_speed();
	void set_max_move_speed(float_t p_move_speed);

	float_t get_acceleration();
	void set_acceleration(float_t p_accelaration);

	float_t get_friction();
	void set_friction(float_t p_friction);
	
	Vector2 get_move_direction();
	void set_move_direction(Vector2 p_move_direction);
};

#endif //  CHARACTER_H
