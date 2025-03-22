#ifndef __RUN_COMMAND_H__
#define __RUN_COMMAND_H__

#include "characters/character.h"
#include "core/command.h"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/vector2.hpp"

class Character;

using namespace godot;
class RunCommand : public Command {
    GDCLASS(RunCommand, Command)

    Vector2 direction;
    Character* character;

   public:
   RunCommand(Character* p_character, Vector2 p_direction);

   virtual void run(float_t p_delta) override;
};
#endif  // __RUN_COMMAND_H__