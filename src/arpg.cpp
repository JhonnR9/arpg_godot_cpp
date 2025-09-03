#include "arpg.h"
#include "godot_cpp/variant/string.hpp"
#include "tools/auto_resgister.h"

AUTO_REGISTER_RUNTIME_CLASS(ARPG)

ARPG::ARPG() {

}


void ARPG::_bind_methods() {
    ADD_SIGNAL(MethodInfo("show_dialog", PropertyInfo(Variant::STRING, "text")));
    ADD_SIGNAL(MethodInfo("hide_dialog"));
}