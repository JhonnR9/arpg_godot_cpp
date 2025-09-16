#pragma once
#include "godot_cpp/classes/control.hpp"
#include "godot_cpp/classes/style_box.hpp"
#include "godot_cpp/templates/hash_map.hpp"

using namespace godot;

class GridInventory final : public Control {
	GDCLASS(GridInventory, Control)

	Ref<StyleBox> background;
	Ref<StyleBox> item_frame;
	Ref<StyleBox> item_frame_hover;

	enum State {
		HOVER,
		DEFAULT
	};

	Point2i selected_slot_key = Point2i(-1, -1);

	int rows = 4;
	int columns = 8;
	Size2 slot_size = Vector2(16, 16);
	HashMap<Point2i, Rect2> cells;
	Size2 slot_margin = Vector2(2, 2);
	Size2 grid_padding = Vector2(4, 4);

	void _draw_bg();
	void _draw_item_frame();
	Point2i _get_cell_key(Point2i point) const;
	void _generate_grid_rects();
	void _change_draw_state(Point2i p_key, State p_new_state);

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	GridInventory() = default;

	Ref<StyleBox> get_background() const;
	void set_background(const Ref<StyleBox> &p_bg_style_box);

	Ref<StyleBox> get_item_frame() const;
	void set_item_frame(const Ref<StyleBox> &p_item_frame_style_box);

	Ref<StyleBox> get_item_frame_hover() const;
	void set_item_frame_hover(const Ref<StyleBox> &p_item_frame_style_box_hover);

	int get_rows() const;
	void set_rows(int p_rows);

	int get_columns() const;
	void set_columns(int p_columns);

	Size2 get_slot_size() const;
	void set_slot_size(const Size2 &p_slot_size);

	Size2 get_slot_margin() const;
	void set_slot_margin(const Size2 &p_slot_margin);

	Size2 get_grid_padding() const;
	void set_grid_padding(const Size2 &p_grid_padding);

	void _on_changed_style_box();
	Size2 _get_minimum_size() const override;
};