#ifndef CLASSREGISTRY_H
#define CLASSREGISTRY_H

#include <vector>
#include <functional>

class ClassRegistry {
public:
	static ClassRegistry& get() {
		static ClassRegistry instance;
		return instance;
	}

	using RegisterFunction = std::function<void()>;

	void add_register_function(RegisterFunction func) {
		register_functions.push_back(std::move(func));
	}

	void execute_all() const {
		for (const auto& func : register_functions) {
			func();
		}
	}

private:
	ClassRegistry() = default;
	~ClassRegistry() = default;

	ClassRegistry(const ClassRegistry&) = delete;
	ClassRegistry& operator=(const ClassRegistry&) = delete;
	ClassRegistry(ClassRegistry&&) = delete;
	ClassRegistry& operator=(ClassRegistry&&) = delete;

	std::vector<RegisterFunction> register_functions;
};


#endif // CLASSREGISTRY_H
