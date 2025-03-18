#include "arpg.h"
#include "godot_cpp/variant/string.hpp"

ARPG::ARPG() {

}


void ARPG::_bind_methods() {
    ADD_SIGNAL(MethodInfo("show_dialog", PropertyInfo(Variant::STRING, "text")));
    ADD_SIGNAL(MethodInfo("hide_dialog"));
}