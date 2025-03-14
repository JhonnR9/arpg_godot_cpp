#ifndef __PLAYER_IDLE_H__
#define __PLAYER_IDLE_H__

#include "characters/player/player.h"
#include "core/state.h"
#include "godot_cpp/classes/wrapped.hpp"
#include <cmath>

using namespace godot;

class PlayerIdle : public State<Player> {
    GDCLASS(PlayerIdle, State<Player>);

protected:
    static void _bind_methods();
public:
    PlayerIdle();
    virtual void _on_state_enter(Player *p_powner) ;
    virtual void _on_state_run(Player *p_powner, double_t p_delta) ;
    virtual void _on_state_exit(Player *p_powner) ;
};

#endif // __PLAYER_IDLE_H__