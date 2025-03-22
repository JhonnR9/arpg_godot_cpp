#ifndef __STOP_COMMAND_H__
#define __STOP_COMMAND_H__

#include "core/command.h"

class Character;
class StopCommand : public Command {
    GDCLASS(StopCommand, Command)
    Character* character;

   protected:
    static void _bind_methods();

   public:
    StopCommand() = default;
    StopCommand(Character* p_character);
    virtual void run(float_t p_delta) override;
};
#endif  // __STOP_COMMAND_H__