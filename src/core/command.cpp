#include "command.h"
#include "tools/auto_resgister.h"

AUTO_REGISTER_RUNTIME_CLASS(Command)

void Command::_bind_methods(){
    GDVIRTUAL_BIND(run, "delta");
    
}