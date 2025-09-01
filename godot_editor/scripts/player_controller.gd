class_name PlayerController extends Character

func _ready() -> void:
	var machine := get_state_machine()
	machine.add_state("idle", PlayerIdleState.new())
	machine.set_state("idle")
	

	
