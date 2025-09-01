class_name PlayerIdleState extends State 

const INPUT_LEFT = "left"
const INPUT_RIGHT = "right"
const INPUT_UP = "up"
const INPUT_DOWN = "down"

var character: Character;

func _on_state_enter(_character: Character) -> void:
	print("enter")
	character = _character;

func _on_state_stay(_delta: float) -> void:
	var direction = Input.get_vector(INPUT_LEFT, INPUT_RIGHT, INPUT_UP, INPUT_DOWN)
	character.move_direction = direction;

func _on_state_exit() -> void:
	print("exit")
	
