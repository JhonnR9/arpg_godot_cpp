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

#define AUTO_REGISTER_INTERNAL_CLASS(CLASSNAME)                                         \
	namespace {                                                                         \
	struct CLASSNAME##AutoRegisterInternal {                                            \
		CLASSNAME##AutoRegisterInternal() {                                             \
			ClassRegistry::get().add_register_function([]() {                           \
				GDREGISTER_INTERNAL_CLASS(CLASSNAME);                                   \
			});                                                                         \
		}                                                                               \
	};                                                                                  \
	static CLASSNAME##AutoRegisterInternal CLASSNAME##_auto_register_internal_instance; \
	}

#define AUTO_REGISTER_VIRTUAL_CLASS(CLASSNAME)                                        \
	namespace {                                                                       \
	struct CLASSNAME##AutoRegisterVirtual {                                           \
		CLASSNAME##AutoRegisterVirtual() {                                            \
			ClassRegistry::get().add_register_function([]() {                         \
				GDREGISTER_VIRTUAL_CLASS(CLASSNAME);                                  \
			});                                                                       \
		}                                                                             \
	};                                                                                \
	static CLASSNAME##AutoRegisterVirtual CLASSNAME##_auto_register_virtual_instance; \
	}

#endif // AUTO_REGISTER_H
