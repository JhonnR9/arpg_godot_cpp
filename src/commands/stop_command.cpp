#include "stop_command.h"

#include "characters/character.h"


StopCommand::StopCommand(Character* p_character) { this->character = p_character; }

void StopCommand::run(float_t p_delta) {
    if (character) {
        character->set_move_direction(Vector2(0.0f, 0.0f));
    }
}
void StopCommand::_bind_methods() {}
