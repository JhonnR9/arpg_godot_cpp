#ifndef PLAYER_H
#define PLAYER_H

#include "characters/character.h"
#include "core/state_machine.h"

class Player : public Character {
  GDCLASS(Player, Character);

  Ref<StateMachine> state_machine;
protected:
  static void _bind_methods();

public:
  Player();
  virtual void _physics_process(double_t delta) override;
  virtual void _process(double_t delta) override;
  virtual	void _ready() override;
};

#endif // PLAYER_H
