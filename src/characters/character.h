#ifndef CHARACTER_H
#define CHARACTER_H

#include "core/command.h"
#include "core/state_machine.h"
#include "godot_cpp/classes/animation_player.hpp"
#include "godot_cpp/classes/character_body2d.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/vector2.hpp"
#include <cstddef>
#include <godot_cpp/classes/weak_ref.hpp>

using namespace godot;

class Character : public CharacterBody2D {
	GDCLASS(Character, CharacterBody2D);

private:
	// Character attributes
	String display_name;
	float life;
	float max_life;
	float max_move_speed;
	float acceleration;
	float friction;
	Vector2 move_direction;
	Vector<Ref<Command>> commands;
	String current_animation;

	// Nodes and Objects
	AnimationPlayer *animation_player{ nullptr };
	Ref<StateMachine> state_machine{ nullptr };

	// Enum for movement directions
	enum LookDirection {
		UP,
		DOWN,
		LEFT,
		RIGHT
	};
	LookDirection look_direction;

protected:
	// Property Bindings
	static void _bind_methods();

public:
	Character();
	~Character();
	// Accessor for state machine
	Ref<StateMachine> get_state_machine();

	// Animation methods
	void set_animation(const String &p_anim_name);
	String get_look_direction() const;

private:
	void update_animation();
	void _on_child_node_tree_exit(Node* p_node);
	void _on_child_node_tree_entered(Node* p_node);
	bool has_animation_in_child;

	void connect_animation_in_child();
	void disconnect_animation_in_child();

public:
	//  Life cycle methods
	virtual void _ready() override;
	virtual void _physics_process(double p_delta) override;
	void _notification(int p_what);
	virtual PackedStringArray _get_configuration_warnings() const override;

	// Command management
	void enqueue_command(Ref<Command> p_command);
	void remove_last_command();
	void clear_all_commands();
	size_t get_commands_size() const;
	Ref<Command> get_last_command() const;

	// Movement handling
	void set_movement(Vector2 p_direction);
	void apply_friction();
	void update_look_direction();
	void apply_movement();

	// Getters and setters
	float get_life() const;
	void set_life(float p_life);

	String get_display_name() const;
	void set_display_name(String p_display_name);

	float get_move_speed() const;
	void set_move_speed(float p_move_speed);

	float get_max_move_speed() const;
	void set_max_move_speed(float p_move_speed);

	float get_acceleration() const;
	void set_acceleration(float p_acceleration);

	float get_friction() const;
	void set_friction(float p_friction);

	Vector2 get_move_direction() const;
	void set_move_direction(Vector2 p_move_direction);
};

#endif // CHARACTER_H
