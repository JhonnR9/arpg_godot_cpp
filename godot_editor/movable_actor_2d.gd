extends MovableActor2D

func _process(_delta: float) -> void:
	var direction = Input.get_vector("left", "right", "up", "down")
	set_move_direction(direction)
	
