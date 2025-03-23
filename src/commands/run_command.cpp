#include "run_command.h"

#include "characters/character.h"

void RunCommand::run(float_t p_delta) {
    if (character) {
        character->set_move_direction((character->get_move_direction() + direction).normalized());
    }
}

RunCommand::RunCommand(Character* p_character, Vector2 p_direction) {
    this->direction = p_direction;
    this->character = p_character;
}

void RunCommand::_bind_methods() {}
