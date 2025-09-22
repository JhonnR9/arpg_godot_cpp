#pragma once
#include "godot_cpp/classes/panel.hpp"
#include "grid_inventory.h"

namespace godot {
class TextureRect;
}
namespace godot {

class GridInventory;
class SlotPanel final : public Panel {
	GDCLASS(SlotPanel, Panel)

	GridInventory *inventory;
	TextureRect *preview;
	int64_t key_in_grid = INVALID_KEY;

public:
	void set_key_in_grid(const int64_t p_key_in_grid) {
		key_in_grid = p_key_in_grid;
	}
	int64_t get_key_in_grid() const {
		return key_in_grid;
	}

protected:
	static void _bind_methods();
	void _notification(int p_what);
	public:
	SlotPanel();

	Variant _get_drag_data(const Vector2 &p_at_position) override;
	bool _can_drop_data(const Vector2 &p_at_position, const Variant &p_data) const override;
	void _drop_data(const Vector2 &p_at_position, const Variant &p_data) override;

};
} //namespace godot