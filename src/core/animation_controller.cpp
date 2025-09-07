
#include "animation_controller.h"
#include "godot_cpp/classes/animated_sprite2d.hpp"
#include "godot_cpp/classes/animation_player.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/sprite_frames.hpp"
#include "tools/auto_resgister.h"

AUTO_REGISTER_CLASS(AnimationController)

AnimationController::AnimationController() :
		animation_prefix(""),
		animation_suffix(""),
		current_animation_name(""),
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
				Callable callable = callable_mp(this, &AnimationController::_on_animated_sprite_exit);
				if (!animated_sprite->is_connected("tree_exiting", callable)) {
					animated_sprite->connect("tree_exiting", callable);
				}

			}
			if (AnimationPlayer *node = cast_to<AnimationPlayer>(get_child(index))) {
				animation_player = node;

				Callable callable = callable_mp(this, &AnimationController::_on_animation_player_exit);
				if (!animation_player->is_connected("tree_exiting", callable)) {
					animation_player->connect("tree_exiting", callable);
				}

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
	if (p_animation_prefix != animation_prefix) {
		animation_prefix = p_animation_prefix;
		_update_animation();
	}
}
String AnimationController::get_animation_prefix() const {
	return animation_prefix;
}
void AnimationController::set_animation_suffix(const String &p_animation_suffix) {
	if (p_animation_suffix.is_empty()) {
		WARN_PRINT("Animation Suffix could not be empty!");
		return;
	}
	if (p_animation_suffix != animation_suffix) {
		animation_suffix = p_animation_suffix;
		_update_animation();
	}
}
String AnimationController::get_animation_suffix() const {
	return animation_suffix;
}
String AnimationController::get_current_animation_name() const {
	return current_animation_name;
}

void AnimationController::_on_animation_player_exit() {
	animation_player->disconnect("tree_exiting", callable_mp(this, &AnimationController::_on_animation_player_exit));
	animation_player = nullptr;
}
void AnimationController::_on_animated_sprite_exit() {
	animated_sprite->disconnect("tree_exiting", callable_mp(this, &AnimationController::_on_animated_sprite_exit));
	animated_sprite = nullptr;
}

void AnimationController::_update_animation() {
	current_animation_name = animation_prefix + "_" + animation_suffix;

	if (!animation_player && !animated_sprite) {
		WARN_PRINT("You need to set up at least one animation node for animation work!");
		return;
	}

	bool has_animation = false;

	if (animation_player) {
		if (animation_player->has_animation(current_animation_name)) {
			animation_player->play(current_animation_name);
			has_animation = true;
		}
	}

	if (animated_sprite) {
		const Ref<SpriteFrames> sf = animated_sprite->get_sprite_frames();
		ERR_FAIL_COND_MSG(!sf.ptr(), "No sprite frame found in animated sprite 2D");
		if (sf->has_animation(current_animation_name)) {
			animated_sprite->play(current_animation_name);
			has_animation = true;
		}
	}

	if (!has_animation) {
		WARN_PRINT("Could not find animation with name: " + current_animation_name);
	}
}

void AnimationController::_bind_methods() {

	ClassDB::bind_method(D_METHOD("get_animation_prefix"), &AnimationController::get_animation_prefix);
	ClassDB::bind_method(D_METHOD("set_animation_prefix", "p_animation_prefix"), &AnimationController::set_animation_prefix);

	ClassDB::bind_method(D_METHOD("get_animation_suffix"), &AnimationController::get_animation_suffix);
	ClassDB::bind_method(D_METHOD("set_animation_suffix", "p_animation_suffix"), &AnimationController::set_animation_suffix);

	ClassDB::bind_method(D_METHOD("get_current_animation_name"), &AnimationController::get_current_animation_name);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "animation_prefix"), "set_animation_prefix", "get_animation_prefix");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "animation_suffix"), "set_animation_suffix", "get_animation_suffix");
}