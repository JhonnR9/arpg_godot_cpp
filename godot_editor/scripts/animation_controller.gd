extends AnimationController

var directions: Array =["left", "right", "up", "down"]

func _on_floating_character_2d_start_move() -> void:
	set_animation_prefix("run")

func _on_floating_character_2d_stop_move() -> void:
	set_animation_prefix("idle")



	
func _on_floating_character_2d_direction_name_changed(move_direction_name: int) -> void:
	set_animation_suffix(directions[move_direction_name])
	
	
