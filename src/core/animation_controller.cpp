
#include "animation_controller.h"
#include "godot_cpp/classes/animated_sprite2d.hpp"
#include "godot_cpp/classes/animation_player.hpp"
#include "tools/auto_resgister.h"

AUTO_REGISTER_CLASS(AnimationController)

AnimationController::AnimationController() :
		animation_prefix(""),
		animation_suffix(""),
		current_animation(""),
		animation_player(nullptr),
		animated_sprite(nullptr) {
}
void AnimationController::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_CHILD_ORDER_CHANGED: {
			const int index = get_child_count() - 1;

			if (index < 0) {
				return;
			}

			if (AnimatedSprite2D *node = cast_to<AnimatedSprite2D>(get_child(index, true))) {
				animated_sprite = node;
				animated_sprite->connect("tree_exiting", callable_mp(this, &AnimationController::_on_animated_sprite_exit));
			}
			if (AnimationPlayer *node = cast_to<AnimationPlayer>(get_child(index))) {
				animation_player = node;
				animation_player->connect("tree_exiting", callable_mp(this, &AnimationController::_on_animation_player_exit));
			}
			break;
		}

		default:
			break;
	}
}
PackedStringArray AnimationController::_get_configuration_warnings() const {
	PackedStringArray warnings = Node::_get_configuration_warnings();

	if (!animation_player && !animated_sprite) {
		warnings.append(
				"\tThis node requires an AnimationPlayer or (and) AnimatedSprite2D \n"
				" as a child node for animations to work! \n"
				"Please add an AnimationPlayer or AnimatedSprite2D node as a child of this Character.");
	}

	return warnings;
}
void AnimationController::set_animation_prefix(const String &p_animation_prefix) {
	if (p_animation_prefix.is_empty()) {
		WARN_PRINT("Animation Prefix could not be empty!");
		return;
	}
	animation_prefix = p_animation_prefix;
}

void AnimationController::_on_animation_player_exit() {
	animation_player->disconnect("tree_exiting", callable_mp(this, &AnimationController::_on_animation_player_exit));
	animation_player = nullptr;
}
void AnimationController::_on_animated_sprite_exit() {
	animated_sprite->disconnect("tree_exiting", callable_mp(this, &AnimationController::_on_animated_sprite_exit));
	animated_sprite = nullptr;
}

void AnimationController::_bind_methods() {
}