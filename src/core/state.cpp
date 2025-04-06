#include "state.h"

void State::_bind_methods(){
    GDVIRTUAL_BIND(_on_state_enter, "character");
    GDVIRTUAL_BIND(_on_state_stay, "delta");
    GDVIRTUAL_BIND(_on_state_exit);
}

