#pragma once
#include "godot_cpp/classes/control.hpp"
#include "godot_cpp/classes/style_box.hpp"

using namespace godot;

class GridInventory final : public Control {
	GDCLASS(GridInventory, Control)

	Ref<StyleBox> bg_style_box;
	Ref<StyleBox> item_frame_style_box;
	int rows = 5;
	int columns = 5;
	Vector2 slot_size = Vector2(64, 64);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	GridInventory() = default;

	Ref<StyleBox> get_bg_style_box() const;
	void set_bg_style_box(const Ref<StyleBox> &p_bg_style_box);

	Ref<StyleBox> get_item_frame_style_box() const;
	void set_item_frame_style_box(const Ref<StyleBox> &p_item_frame_style_box);

	void _changed_bg_style_box();
};