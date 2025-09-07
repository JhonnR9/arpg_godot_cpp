extends AnimationController

var directions: Array =["left", "right", "up", "down"]

func _ready() -> void:
	animation_prefix="idle"

func _on_floating_character_2d_start_move() -> void:
	animation_prefix = "run"

func _on_floating_character_2d_stop_move() -> void:
	animation_prefix="idle"

func _on_floating_character_2d_direction_name_changed(move_direction_name: int) -> void:
	animation_suffix=directions[move_direction_name]
	
	
