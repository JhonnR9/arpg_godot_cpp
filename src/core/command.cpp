#include "command.h"

void Command::_bind_methods(){
    GDVIRTUAL_BIND(run, "delta");
    
}