class_name PlayerIdleState extends State 

func _on_state_enter(_character: Character) -> void:
	print("oiii")

func _on_state_stay(delta: float) -> void:
	print("ss")
func _on_state_exit() -> void:
	pass
	
