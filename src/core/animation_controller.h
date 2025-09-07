
#ifndef ANIMATION_CONTROLLER_H
#define ANIMATION_CONTROLLER_H
#include "godot_cpp/classes/node.hpp"

namespace godot {
class AnimatedSprite2D;
class AnimationPlayer;
}

using namespace godot;
class AnimationController : public Node {
	GDCLASS(AnimationController, Node)

protected:
	static void _bind_methods();

public:

	AnimationController();
	void _notification(int p_what);
	PackedStringArray _get_configuration_warnings() const override;

	void set_animation_prefix(const String &p_animation_prefix);

private:
	String animation_prefix;
	String animation_suffix;
	String current_animation;
	AnimationPlayer *animation_player;
	AnimatedSprite2D *animated_sprite;

	void _on_animation_player_exit();
	void _on_animated_sprite_exit();
};

#endif //ANIMATION_CONTROLLER_H
