#pragma once
#include "godot_cpp/classes/control.hpp"
#include "godot_cpp/templates/hash_map.hpp"
#include "item_view.h"

namespace godot {
class Panel;
class Label;
class TextureRect;
class LabelSettings;
class StyleBox;

class GridInventory final : public Control {
	GDCLASS(GridInventory, Control)
	// styles
	Ref<StyleBox> background;
	Ref<StyleBox> item_frame;
	Ref<StyleBox> item_frame_hover;
	Ref<LabelSettings> count_label_settings;

	struct Slot {
		Rect2i rect;
		Ref<ItemView> item;
		TextureRect *icon = nullptr;
		Panel *item_panel = nullptr;
		Label *count_label = nullptr;
	};

	static constexpr int64_t INVALID_KEY = -1;
	int64_t hovered_slot_key = INVALID_KEY;
	Ref<ItemView> item_view_backup;
	Slot* start_drag_slot;

	int rows = 4;
	int columns = 8;
	Size2i slot_size = Size2i(16, 16);
	HashMap<int64_t, Slot> cells;
	Size2i slot_margin = Size2i(2, 2);
	Size2i grid_padding = Size2i(4, 4);

	void _draw_background();
	void _generate_grid_rects();
	void _clear_grid_rects();

	[[nodiscard]] Panel* _create_item_panel(const Point2i &pos);
	[[nodiscard]] Label* _create_count_label();

	static void _clear_slot(Slot &slot);
	void _sync_item_view(Slot &slot);

	void _connect_style_signal(const Ref<StyleBox> &p_style);
	void _disconnect_style_signal(const Ref<StyleBox> &p_style);

	int64_t _get_key_from_position(Point2i pos) const;

	static int64_t _make_cell_key(const int col, const int row) {
		return (static_cast<int64_t>(col) << 32) | static_cast<uint32_t>(row);
	}
	static int _get_col_from_key(const int64_t key) {
		return static_cast<int>(key >> 32);
	}
	static int _get_row_from_key(const int64_t key) {
		return static_cast<int>(key & 0xFFFFFFFF);
	}

	// callables
	Variant _on_get_drag_data(const Vector2 &p_at_position);
	bool _on_can_drop_data(const Vector2 &p_at_position, const Variant &p_data) const;
	void _on_drop_data(const Vector2 &p_at_position, const Variant &p_data);

	void _on_style_changed();
	void _on_label_settings_style_changed();

	void _on_slot_mouse_entered();
	void _on_slot_mouse_exited();

protected:
	static void _bind_methods();
	void _notification(int p_what);

public:
	// Overrides
	Size2 _get_minimum_size() const override;

	// API
	bool add_item_at(const Ref<ItemView> &p_item, Point2i p_point);
	bool add_item(Ref<ItemView> p_item);

	// Gets and Sets
	Ref<LabelSettings> get_count_label_settings() const { return count_label_settings; }
	void set_count_label_settings(const Ref<LabelSettings> &p_count_label_settings);

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

};

} //namespace godot
