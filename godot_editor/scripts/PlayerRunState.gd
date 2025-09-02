class_name PlayerRunState extends State

func _on_state_enter(character: Character) -> void:
	character.set_animation("run")
	
func  _on_state_stay(delta: float) -> void:
	pass
func _on_state_exit() -> void:
	pass
