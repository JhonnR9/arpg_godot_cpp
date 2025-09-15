#pragma once

#include "godot_cpp/classes/node2d.hpp"

namespace godot {
class AnimatedSprite2D;
class AnimationPlayer;
}

using namespace godot;
class AnimationController : public Node2D {
	GDCLASS(AnimationController, Node2D)

protected:
	static void _bind_methods();

public:

	AnimationController();
	void _notification(int p_what);
	PackedStringArray _get_configuration_warnings() const override;

	void set_animation_prefix(const String &p_animation_prefix);
	String get_animation_prefix() const;

	void set_animation_suffix(const String &p_animation_suffix);
	String get_animation_suffix() const;

	String get_current_animation_name() const;

private:
	String animation_prefix;
	String animation_suffix;
	StringName current_animation_name;
	AnimationPlayer *animation_player;
	AnimatedSprite2D *animated_sprite;

	void _on_animation_player_exit();
	void _on_animated_sprite_exit();
	void _update_animation();
};


