class_name PlayerIdleState extends State 

var character: Character;

func _on_state_enter(p_character: Character) -> void:
	character = p_character;
	character.set_animation("idle")
	
	
func _on_state_stay(_delta: float) -> void:
	var direction = Input.get_vector("left", "right", "up", "down")
	character.move_direction = direction;
	
func _on_state_exit() -> void:
	pass
	
