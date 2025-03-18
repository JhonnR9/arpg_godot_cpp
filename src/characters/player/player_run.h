#ifndef __PLAYER_RUN_H__
#define __PLAYER_RUN_H__

#include "core/state.h"
#include "godot_cpp/classes/wrapped.hpp"
#include <cmath>

using namespace godot;

class PlayerRun : public State {
    GDCLASS(PlayerRun, State);

protected:
    static void _bind_methods();
    
public:
    virtual void on_state_enter(Character* owner) override;
    virtual void on_state_run( double_t p_delta) override;
    virtual void on_state_exit()override ;
};
#endif // __PLAYER_RUN_H__