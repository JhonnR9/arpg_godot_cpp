#ifndef PLAYER_H
#define PLAYER_H

#include "characters/character.h"


class Player : public Character {
  GDCLASS(Player, Character);
private:
void input_handle();
protected:
  static void _bind_methods();

public:
  Player();
  virtual void _process(double_t delta) override;
  virtual	void _ready() override;
};

#endif // PLAYER_H
