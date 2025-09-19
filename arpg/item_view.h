#pragma once

#include "godot_cpp/classes/ref_counted.hpp"
#include "tools/auto_resgister.h"
#include <godot_cpp/classes/texture2d.hpp>

namespace godot {
class ItemView final : public RefCounted {
	GDCLASS(ItemView, RefCounted);

protected:
	static void _bind_methods();

public:
	ItemView() = default;

private:
	int id;
	Ref<Texture2D> icon;
	int item_amount = 1;
	String name;
	String hint_description;

public:
	int get_id() const { return id; }
	void set_id(const int id) { this->id = id; }

	Ref<Texture2D> get_icon() const { return icon; }
	void set_icon(const Ref<Texture2D> &icon) { this->icon = icon; }

	int get_item_amount() const { return item_amount; }
	void set_item_amount(const int item_amount) { this->item_amount = item_amount; }

	String get_name() const { return name; }
	void set_name(const String &name) { this->name = name; }

	String get_hint_description() const { return hint_description; }
	void set_hint_description(const String &hint_description) { this->hint_description = hint_description; }
};

} //namespace godot
