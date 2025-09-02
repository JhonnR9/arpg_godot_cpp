class_name PlayerController extends Character
@onready var machine := get_state_machine()
func _ready() -> void:
	machine.add_state("idle", PlayerIdleState.new())
	machine.add_state("run", PlayerRunState.new())
	machine.set_state("idle")
	set_process(true)
	
func _process(delta: float) -> void:
	var direction = Input.get_vector("left", "right", "up", "down")
	move_direction = direction;
	
	if move_direction.length_squared() > 0.01 :
		machine.set_state("run")	
	else:
		machine.set_state("idle")

	
