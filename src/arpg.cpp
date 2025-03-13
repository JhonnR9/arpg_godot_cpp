#include "arpg.h"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/utility_functions.hpp"

ARPG::ARPG() {

}

void ARPG::_ready() {
    UtilityFunctions::print("ARPG node is ready!");
}

void ARPG::_bind_methods() {
    ADD_SIGNAL(MethodInfo("show_dialog", PropertyInfo(Variant::STRING, "text")));
    ADD_SIGNAL(MethodInfo("hide_dialog"));
}