#pragma once
#include "godot_cpp/classes/control.hpp"
#include "godot_cpp/classes/panel.hpp"
#include "godot_cpp/classes/style_box.hpp"
#include "godot_cpp/classes/texture_rect.hpp"
#include "godot_cpp/templates/hash_map.hpp"
#include "item_view.h"

namespace godot {

#define INVALID_KEY -1
#define INVALID_ID -1



class GridInventory final : public Control {
	GDCLASS(GridInventory, Control)

	friend class SlotPanel;

	Ref<StyleBox> background;
	Ref<StyleBox> item_frame;
	Ref<StyleBox> item_frame_hover;

	struct Slot {
		Rect2i rect;
		Ref<ItemView> item;
		TextureRect *icon = nullptr;
		SlotPanel* item_frame=nullptr;
	};

	int64_t hovered_slot_key = INVALID_KEY;


	int rows = 4;
	int columns = 8;
	Size2 slot_size = Vector2(16, 16);
	HashMap<int64_t, Slot> cells;
	Size2i slot_margin = Vector2(2, 2);
	Size2i grid_padding = Vector2(4, 4);

	void _draw_background();
	int64_t _get_key_from_position(Point2i pos) const;
	void _generate_grid_rects();
	void _update_item_icon(Slot & slot);
	void _on_slot_mouse_entered();
	void _on_slot_mouse_exited();
	static void _clear_slot(Slot &slot);

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

	bool add_item_at(const Ref<ItemView> &p_item, Point2i p_point);
	bool add_item(Ref<ItemView> p_item);

	Ref<StyleBox> get_background() const;
	void set_background(const Ref<StyleBox> &p_background);

	Ref<StyleBox> get_item_frame() const {
		return item_frame;
	}
	void set_item_frame(const Ref<StyleBox> &p_item_frame);

	Ref<StyleBox> get_item_frame_hover() const {
		return item_frame_hover;
	}
	void set_item_frame_hover(const Ref<StyleBox> &p_item_frame_hover);

	int get_rows() const {
		return rows;
	}
	void set_rows(int p_rows);

	int get_columns() const {
		return columns;
	}
	void set_columns(int p_columns);

	Size2i get_slot_size() const {
		return slot_size;
	}
	void set_slot_size(const Size2i &p_slot_size);

	Size2i get_slot_margin() const {
		return slot_margin;
	}
	void set_slot_margin(const Size2i &p_slot_margin);

	Size2i get_grid_padding() const {
		return grid_padding;
	}

	void set_grid_padding(const Size2i &p_grid_padding);
	Size2 _get_minimum_size() const override;
	void _on_style_changed();
	void _connect_signals(const Ref<StyleBox> &p_style);
	void _disconnect_signals(const Ref<StyleBox> &p_style);
};

} //namespace godot
