extends FloatingCharacter2D

func _process(_delta: float) -> void:
	set_move_direction(Input.get_vector("left", "right", "up", "down"))
	
