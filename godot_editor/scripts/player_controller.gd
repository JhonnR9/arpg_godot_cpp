class_name PlayerController extends Character

const INPUT_LEFT = "left"
const INPUT_RIGHT = "right"
const  INPUT_UP = "up"
const INPUT_DOWN = "down"

func _ready() -> void:
	
	var machine: StateMachine = get_state_machine()
	#machine.set_character(self)
	#machine.add_state("idle",PlayerIdleState.new())

func _process(delta: float) -> void:
	pass
	
