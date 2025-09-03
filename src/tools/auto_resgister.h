#ifndef AUTO_REGISTER_H
#define AUTO_REGISTER_H

#include "class_registry.h"

#define AUTO_REGISTER_CLASS(CLASSNAME)                                 \
	namespace {                                                        \
	struct CLASSNAME##AutoRegister {                                   \
		CLASSNAME##AutoRegister() {                                    \
			ClassRegistry::get().add_register_function([]() {          \
				GDREGISTER_CLASS(CLASSNAME);                           \
			});                                                        \
		}                                                              \
	};                                                                 \
	static CLASSNAME##AutoRegister CLASSNAME##_auto_register_instance; \
	}

#define AUTO_REGISTER_RUNTIME_CLASS(CLASSNAME)                                        \
	namespace {                                                                       \
	struct CLASSNAME##AutoRuntimeRegister {                                           \
		CLASSNAME##AutoRuntimeRegister() {                                            \
			ClassRegistry::get().add_register_function([]() {                         \
				GDREGISTER_RUNTIME_CLASS(CLASSNAME);                                  \
			});                                                                       \
		}                                                                             \
	};                                                                                \
	static CLASSNAME##AutoRuntimeRegister CLASSNAME##_auto_runtime_register_instance; \
	}

#define AUTO_REGISTER_ABSTRACT_CLASS(CLASSNAME)                                         \
	namespace {                                                                         \
	struct CLASSNAME##AutoRegisterAbstract {                                            \
		CLASSNAME##AutoRegisterAbstract() {                                             \
			ClassRegistry::get().add_register_function([]() {                           \
				GDREGISTER_ABSTRACT_CLASS(CLASSNAME);                                   \
			});                                                                         \
		}                                                                               \
	};                                                                                  \
	static CLASSNAME##AutoRegisterAbstract CLASSNAME##_auto_register_abstract_instance; \
	}

#endif // AUTO_REGISTER_H
