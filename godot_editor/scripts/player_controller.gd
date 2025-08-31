class_name PlayerController extends Character

const INPUT_LEFT = "left"
const INPUT_RIGHT = "right"
const INPUT_UP = "up"
const INPUT_DOWN = "down"

func _ready() -> void:
	var machine := get_state_machine()
	set_process(true)
	var idle_state = PlayerIdleState.new()
	machine.add_state("idle", idle_state)
	machine.set_state("idle")
