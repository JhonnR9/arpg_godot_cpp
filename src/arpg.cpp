#include "arpg.h"
#include "godot_cpp/variant/utility_functions.hpp"

ARPG::ARPG() {
   
}

void ARPG::_bind_methods() {}

void ARPG::_ready(){
    parse.instantiate();
    if(parse.is_valid()){
        UtilityFunctions::print(parse->load("data/test.json"));
    }
    
}