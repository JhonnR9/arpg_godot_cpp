#pragma once
#include "godot_cpp/classes/control.hpp"
#include "godot_cpp/classes/style_box.hpp"
#include "godot_cpp/classes/texture_rect.hpp"
#include "godot_cpp/templates/hash_map.hpp"

namespace godot {

#define INVALID_KEY -1

class GridInventory final : public Control {
	GDCLASS(GridInventory, Control)

	Ref<StyleBox> background;
	Ref<StyleBox> item_frame;
	Ref<StyleBox> item_frame_hover;

	enum State {
		HOVER,
		NORMAL
	};

	struct Slot {
		State state = NORMAL;
		Rect2i rect;
		Ref<Texture2D> item_texture;
		int item_amount = 0;
	};

	int64_t hovered_slot_key = INVALID_KEY;
	int64_t selected_slot_key = INVALID_KEY;

	int rows = 4;
	int columns = 8;
	Size2 slot_size = Vector2(16, 16);
	HashMap<int64_t, Slot> cells;
	Size2i slot_margin = Vector2(2, 2);
	Size2i grid_padding = Vector2(4, 4);
	TextureRect *drag_preview = nullptr;

	void _draw_background();
	void _draw_all_slots();
	int64_t _get_key_from_position(Point2i point) const;
	void _generate_grid_rects();
	void _flush_hover_if_needed();

	struct {
		Ref<StyleBox> normal;
	} theme_cache;

	static int64_t _make_cell_key(const int col, const int row) {
		return (static_cast<int64_t>(col) << 32) | static_cast<uint32_t>(row);
	}
	static int _get_col_from_key(const int64_t key) {
		return static_cast<int>(key >> 32);
	}
	static int _get_row_from_key(const int64_t key) {
		return static_cast<int>(key & 0xFFFFFFFF);
	}

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
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
