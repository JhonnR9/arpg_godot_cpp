#include "register_types.h"
#include <godot_cpp/core/class_db.hpp>
#include "arpg.h"
#include "characters/character.h"
#include "core/command.h"
#include "core/state.h"
#include "core/state_machine.h"
#include "tools/json_parse.h"
#include "ui/ui.h"

void initialize_arpg_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

	GDREGISTER_RUNTIME_CLASS(ARPG);

	GDREGISTER_CLASS(Character);
	GDREGISTER_RUNTIME_CLASS(JsonParse);
	
	GDREGISTER_RUNTIME_CLASS(UI);
	GDREGISTER_CLASS(StateMachine);

	GDREGISTER_RUNTIME_CLASS(State);


GDREGISTER_RUNTIME_CLASS(Command);

}

void uninitialize_arpg_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
}

extern "C" {
	GDExtensionBool GDE_EXPORT arpg_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization* r_initialization) {
		godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

		init_obj.register_initializer(initialize_arpg_module);
		init_obj.register_terminator(uninitialize_arpg_module);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}
