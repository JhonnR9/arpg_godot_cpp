class_name  RunCommand extends Command

var character :Character;
var direction : Vector2;

func _init(p_character: Character, p_direction: Vector2) -> void:
	character = p_character
	self.direction = p_direction
	
func run(_delta: float) -> void:
	character.move_direction = direction
