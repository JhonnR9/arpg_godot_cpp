#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"

class Player : public Character {
  GDCLASS(Player, Character);

protected:
  static void _bind_methods();

public:
  Player();
  virtual void _physics_process(double_t delta) override;
  virtual void _process(double_t delta) override;
};

#endif // PLAYER_H
