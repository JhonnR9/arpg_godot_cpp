#include "state.h"

#include "tools/auto_resgister.h"

AUTO_REGISTER_CLASS(State)

void State::_bind_methods(){
    GDVIRTUAL_BIND(_on_state_enter, "character");
    GDVIRTUAL_BIND(_on_state_stay, "delta");
    GDVIRTUAL_BIND(_on_state_exit);
}

