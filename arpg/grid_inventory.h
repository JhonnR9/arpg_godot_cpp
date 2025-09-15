#pragma once
#include "godot_cpp/classes/control.hpp"
#include "godot_cpp/classes/style_box.hpp"
#include "godot_cpp/templates/hash_map.hpp"

using namespace godot;

class GridInventory final : public Control {
	GDCLASS(GridInventory, Control)

	Ref<StyleBox> bg_style_box;
	Ref<StyleBox> item_frame_style_box;

	int rows = 4;
	int columns = 8;
	Size2 slot_size = Vector2(64, 64);
	HashMap<Point2i, Rect2> cells;

	void _draw_bg();
	void _draw_item_frame();
	Point2i _get_cell_key(Point2i point) const;
	void _generate_grid_rects();


protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	GridInventory() {
		queue_redraw();
	};

	Ref<StyleBox> get_bg_style_box() const;
	void set_bg_style_box(const Ref<StyleBox> &p_bg_style_box);

	Ref<StyleBox> get_item_frame_style_box() const;
	void set_item_frame_style_box(const Ref<StyleBox> &p_item_frame_style_box);

	int get_rows() const;
	void set_rows(int p_rows);

	int get_columns() const;
	void set_columns(int p_columns);

	Size2 get_slot_size() const;
	void set_slot_size(const Size2 &p_slot_size);

	void _on_changed_style_box();

};