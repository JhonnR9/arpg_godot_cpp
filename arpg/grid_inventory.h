#pragma once
#include "godot_cpp/classes/control.hpp"
#include "godot_cpp/classes/style_box.hpp"
#include "godot_cpp/templates/hash_map.hpp"

namespace godot {

class GridInventory final : public Control {
	GDCLASS(GridInventory, Control)

	Ref<StyleBox> background;
	Ref<StyleBox> item_frame;
	Ref<StyleBox> item_frame_hover;

	enum State {
		HOVER,
		DEFAULT
	};

	struct Slot {
		State state = DEFAULT;
		Rect2i rect;
		Ref<Texture2D> item_texture;
		int item_amount = 0;
	};

	Point2i selected_slot_key = Point2i(-1, -1);

	int rows = 4;
	int columns = 8;
	Size2 slot_size = Vector2(16, 16);
	HashMap<Point2i, Slot> cells;
	Size2i slot_margin = Vector2(2, 2);
	Size2i grid_padding = Vector2(4, 4);

	void _draw_background();
	void _draw_all_slots();
	Point2i _get_cell_key(Point2i point) const;
	void _generate_grid_rects();
	void _flush_hover_if_needed();

	struct {
		Ref<StyleBox> normal;
	} theme_cache;

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	GridInventory()=default;

	Ref<StyleBox> get_background() const;
	void set_background(const Ref<StyleBox> &p_background);

	Ref<StyleBox> get_item_frame() const;
	void set_item_frame(const Ref<StyleBox> &p_item_frame);

	Ref<StyleBox> get_item_frame_hover() const;
	void set_item_frame_hover(const Ref<StyleBox> &p_item_frame_hover);

	int get_rows() const;
	void set_rows(int p_rows);

	int get_columns() const;
	void set_columns(int p_columns);

	Size2i get_slot_size() const;
	void set_slot_size(const Size2i &p_slot_size);

	Size2i get_slot_margin() const;
	void set_slot_margin(const Size2i &p_slot_margin);

	Size2i get_grid_padding() const;
	void set_grid_padding(const Size2i &p_grid_padding);

	Size2 _get_minimum_size() const override;

	void _gui_input(const Ref<InputEvent> &p_event) override;
	void _mouse_exited();
	void _on_style_changed();

	void _connect_signals(const Ref<StyleBox> &p_style);
	void _disconnect_signals(const Ref<StyleBox> &p_style);
};

} //namespace godot
