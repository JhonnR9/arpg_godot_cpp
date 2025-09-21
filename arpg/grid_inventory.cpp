#include "grid_inventory.h"

#include "godot_cpp/classes/color_rect.hpp"
#include "godot_cpp/classes/engine.hpp"
#include "godot_cpp/classes/input.hpp"
#include "godot_cpp/classes/os.hpp"
#include "godot_cpp/classes/texture_rect.hpp"
#include "godot_cpp/classes/theme.hpp"
#include "godot_cpp/templates/pair.hpp"
#include "slot_panel.h"
#include "tools/auto_resgister.h"

using namespace godot;
AUTO_REGISTER_CLASS(GridInventory)

inline Ref<StyleBox> GridInventory::get_background() const {
	return background;
}
void GridInventory::set_background(const Ref<StyleBox> &p_background) {
	_disconnect_signals(background);
	_connect_signals(p_background);
	this->background = p_background;
	queue_redraw();
}

void GridInventory::set_item_frame(const Ref<StyleBox> &p_item_frame) {
	_disconnect_signals(item_frame);
	_connect_signals(p_item_frame);
	this->item_frame = p_item_frame;
	queue_redraw();
}

void GridInventory::set_item_frame_hover(const Ref<StyleBox> &p_item_frame_hover) {
	_disconnect_signals(item_frame_hover);
	_connect_signals(p_item_frame_hover);
	this->item_frame_hover = p_item_frame_hover;
	queue_redraw();
}

void GridInventory::set_rows(int p_rows) {
	rows = p_rows;
	_generate_grid_rects();
}

void GridInventory::set_columns(int p_columns) {
	columns = p_columns;
	_generate_grid_rects();
}

void GridInventory::set_slot_size(const Size2i &p_slot_size) {
	slot_size = p_slot_size;
	_generate_grid_rects();
}

void GridInventory::set_slot_margin(const Size2i &p_slot_margin) {
	slot_margin = p_slot_margin;
	_generate_grid_rects();
}

void GridInventory::set_grid_padding(const Size2i &p_grid_padding) {
	grid_padding = p_grid_padding;
	_generate_grid_rects();
}

Size2 GridInventory::_get_minimum_size() const {
	const int total_width = grid_padding.x * 2 + columns * slot_size.x + (columns - 1) * slot_margin.x;
	const int total_height = grid_padding.y * 2 + rows * slot_size.y + (rows - 1) * slot_margin.y;

	return Size2(total_width, total_height);
}

void GridInventory::_on_style_changed() {
	queue_redraw(); // Refresh when styles update
}

void GridInventory::_connect_signals(const Ref<StyleBox> &p_style) {
	const auto callable = callable_mp(this, &GridInventory::_on_style_changed);
	if (p_style.is_valid() && !p_style->is_connected("changed", callable)) {
		p_style->connect("changed", callable);
	}
}
void GridInventory::_disconnect_signals(const Ref<StyleBox> &p_style) {
	const auto callable = callable_mp(this, &GridInventory::_on_style_changed);
	if (p_style.is_valid() && p_style->is_connected("changed", callable)) {
		p_style->disconnect("changed", callable);
	}
}

void GridInventory::_draw_background() {
	if (!background.is_valid()) {
		return;
	}
	const RID ci = get_canvas_item();
	const Rect2 rect(Point2(0, 0), get_size());
	background->draw(ci, rect);
}

int64_t GridInventory::_get_key_from_position(const Point2i pos) const {
	const Point2i adjust_point = pos - grid_padding;

	if (adjust_point.x < 0 || adjust_point.y < 0) {
		return INVALID_KEY;
	}

	const int cell_width = slot_size.x + slot_margin.x;
	const int cell_height = slot_size.y + slot_margin.y;

	if (cell_width <= 0 || cell_height <= 0) {
		return INVALID_KEY;
	}

	const int cell_x = adjust_point.x / cell_width;
	const int cell_y = adjust_point.y / cell_height;

	const int64_t key = _make_cell_key(cell_x, cell_y);

	if (const Slot *slot = cells.getptr(key)) {
		const Point2i local_pos = pos;

		if (slot->rect.has_point(local_pos)) {
			return key;
		}
		return INVALID_KEY;
	}

	return INVALID_KEY;
}

void GridInventory::_generate_grid_rects() {
	if (cells.size() > 0) {
		for (KeyValue<int64_t, Slot> &kv : cells) {
			Slot &slot = kv.value;
			if (slot.item_frame) {
				Callable slot_mouse_entered = callable_mp(this, &GridInventory::_on_slot_mouse_entered);
				Callable slot_mouse_exited = callable_mp(this, &GridInventory::_on_slot_mouse_exited);

				slot.item_frame->disconnect("mouse_entered", slot_mouse_entered);
				slot.item_frame->disconnect("mouse_exited", slot_mouse_exited);
				slot.item_frame->queue_free();
				slot.item_frame = nullptr;
			}
		}
		cells.clear();
	}

	if (slot_size.x < 1 || slot_size.y < 1) {
		queue_redraw();
		return;
	}
	if (rows < 1 || columns < 1) {
		queue_redraw();
		return;
	}

	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < columns; col++) {
			const int x = grid_padding.x + col * (slot_size.x + slot_margin.x);
			const int y = grid_padding.y + row * (slot_size.y + slot_margin.y);

			const Point2i pos(x, y);
			const Rect2i rect(pos, slot_size);
			const int64_t key = _make_cell_key(col, row);

			Slot slot;
			slot.item_frame = memnew(SlotPanel);
			slot.rect = rect;
			slot.item_frame->set_key_in_grid(key);

			Callable slot_mouse_entered = callable_mp(this, &GridInventory::_on_slot_mouse_entered);
			Callable slot_mouse_exited = callable_mp(this, &GridInventory::_on_slot_mouse_exited);

			slot.item_frame->connect("mouse_entered", slot_mouse_entered);
			slot.item_frame->connect("mouse_exited", slot_mouse_exited);

			if (slot.item_frame) {
				slot.item_frame->set_size(slot_size);
				slot.item_frame->set_position(pos);

				if (item_frame.is_valid()) {
					slot.item_frame->add_theme_stylebox_override("panel", item_frame);
				}

				add_child(slot.item_frame);
			}

			cells.insert(key, slot);
		}
	}

	// Reset hover and selection state
	hovered_slot_key = INVALID_KEY;

	update_minimum_size();
	queue_redraw();
}

void GridInventory::_update_item_icon(Slot &slot) {
	if (slot.item.is_null()) {
		return;
	}

	if (slot.item->get_icon().is_null()) {
		if (slot.icon) {
			slot.icon->set_texture(Ref<Texture>());
		}

		UtilityFunctions::print("the item", slot.item->get_name(), "dont have icon!");
		return;
	}

	if (!slot.icon) {
		slot.icon = memnew(TextureRect); // use TextureRect for don't redraw all grid
		slot.icon->set_mouse_filter(MOUSE_FILTER_IGNORE);
		add_child(slot.icon);
	}

	slot.icon->set_texture(slot.item->get_icon());
	slot.icon->set_expand_mode(TextureRect::EXPAND_IGNORE_SIZE);
	slot.icon->set_stretch_mode(TextureRect::STRETCH_KEEP_ASPECT_CENTERED);
	slot.icon->set_size(slot.rect.size);
	slot.icon->set_pivot_offset(slot_size * 0.5);
	slot.icon->set_position(slot.rect.get_position());
}

void GridInventory::_on_slot_mouse_entered() {
	const int64_t key = _get_key_from_position(get_local_mouse_position());
	if (const Slot *slot = cells.getptr(key)) {
		hovered_slot_key = key;
		if (item_frame_hover.is_valid() && slot->item_frame) {
			slot->item_frame->add_theme_stylebox_override("panel", item_frame_hover);
			slot->item_frame->queue_redraw();
		}
	}

}
void GridInventory::_on_slot_mouse_exited() {
	if (hovered_slot_key != INVALID_KEY) {
		if (const Slot *slot = cells.getptr(hovered_slot_key)) {
			if (item_frame.is_valid() && slot->item_frame) {
				slot->item_frame->add_theme_stylebox_override("panel", item_frame);
				slot->item_frame->queue_redraw();
			}
		}
	}
	hovered_slot_key = INVALID_KEY;
}
void GridInventory::_clear_slot(Slot &slot) {
	if (!slot.item.is_null()) {
		slot.item.unref();
	}

	if (slot.icon) {
		slot.icon->hide();
		slot.icon = nullptr;
	}

}

void GridInventory::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_DRAW:
			_draw_background();
			break;

		case NOTIFICATION_ENTER_TREE: {
			_generate_grid_rects();
			update_minimum_size();
			queue_redraw();

			_connect_signals(item_frame);
			_connect_signals(item_frame_hover);

			break;
		}
		case NOTIFICATION_EXIT_TREE: {
			_disconnect_signals(item_frame);
			_disconnect_signals(item_frame_hover);
			break;
		}
		case NOTIFICATION_APPLICATION_FOCUS_IN:
			_connect_signals(item_frame);
			_connect_signals(item_frame_hover);
			break;

		case NOTIFICATION_APPLICATION_FOCUS_OUT:
			_disconnect_signals(item_frame);
			_disconnect_signals(item_frame_hover);
			break;

		default:
			break;
	}
}
bool GridInventory::add_item_at(const Ref<ItemView> &p_item, const Point2i p_point) {
	if (p_item.is_null()) {
		ERR_PRINT("Cannot add null item to inventory.");
		return false;
	}

	const auto key = _make_cell_key(p_point.x, p_point.y);
	const bool has_key = cells.has(key);

	if (!has_key) {
		ERR_PRINT(vformat("Slot at (%d, %d) does not exist. Cannot add item: %s", p_point.x, p_point.y, p_item->get_name()));
		return false;
	}

	if (Slot *slot = cells.getptr(key)) {
		if (slot->item.is_null()) {
			slot->item = p_item;
			_update_item_icon(*slot);
			return true;
		}

		if (slot->item->get_id() == p_item->get_id()) {
			const int current_amount = slot->item->get_item_amount();
			const int incoming_amount = p_item->get_item_amount();
			slot->item->set_item_amount(current_amount + incoming_amount);
			return true;
		}
	}

	return false;
}

bool GridInventory::add_item(Ref<ItemView> p_item) {
	if (p_item.is_null()) {
		return false;
	}

	int64_t key = INVALID_KEY;
	for (const KeyValue<int64_t, Slot> &kv : cells) {
		if (kv.value.item.is_null()) {
			key = kv.key;
			break;
		}
	}

	if (key != INVALID_KEY) {
		if (Slot *slot = cells.getptr(key)) {
			slot->item = p_item;
			UtilityFunctions::print(p_item->get_name());
			_update_item_icon(*slot);
			return true;
		}
	}

	return false;
}

void GridInventory::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_background"), &GridInventory::get_background);
	ClassDB::bind_method(D_METHOD("set_background", "background"), &GridInventory::set_background);

	ClassDB::bind_method(D_METHOD("get_item_frame"), &GridInventory::get_item_frame);
	ClassDB::bind_method(D_METHOD("set_item_frame", "item_frame"), &GridInventory::set_item_frame);

	ClassDB::bind_method(D_METHOD("get_item_frame_hover"), &GridInventory::get_item_frame_hover);
	ClassDB::bind_method(D_METHOD("set_item_frame_hover", "item_frame_hover"), &GridInventory::set_item_frame_hover);

	ClassDB::bind_method(D_METHOD("get_rows"), &GridInventory::get_rows);
	ClassDB::bind_method(D_METHOD("set_rows", "rows"), &GridInventory::set_rows);

	ClassDB::bind_method(D_METHOD("get_columns"), &GridInventory::get_columns);
	ClassDB::bind_method(D_METHOD("set_columns", "columns"), &GridInventory::set_columns);

	ClassDB::bind_method(D_METHOD("get_slot_size"), &GridInventory::get_slot_size);
	ClassDB::bind_method(D_METHOD("set_slot_size", "slot_size"), &GridInventory::set_slot_size);

	ClassDB::bind_method(D_METHOD("get_slot_margin"), &GridInventory::get_slot_margin);
	ClassDB::bind_method(D_METHOD("set_slot_margin", "slot_margin"), &GridInventory::set_slot_margin);

	ClassDB::bind_method(D_METHOD("get_grid_padding"), &GridInventory::get_grid_padding);
	ClassDB::bind_method(D_METHOD("set_grid_padding", "grid_padding"), &GridInventory::set_grid_padding);

	ClassDB::bind_method(D_METHOD("add_item", "item"), &GridInventory::add_item);
	ClassDB::bind_method(D_METHOD("add_item_at", "item", "point"), &GridInventory::add_item_at);

	ADD_SUBGROUP("Styles", "");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "background", PROPERTY_HINT_RESOURCE_TYPE, "StyleBox", PROPERTY_USAGE_DEFAULT, "Background"), "set_background", "get_background");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "item_frame", PROPERTY_HINT_RESOURCE_TYPE, "StyleBox", PROPERTY_USAGE_DEFAULT, "Item Frame"), "set_item_frame", "get_item_frame");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "item_frame_hover", PROPERTY_HINT_RESOURCE_TYPE, "StyleBox", PROPERTY_USAGE_DEFAULT, "Item Frame Hover"), "set_item_frame_hover", "get_item_frame_hover");

	ADD_SUBGROUP("Layout", "");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "rows", PROPERTY_HINT_RANGE, "1,100,1"), "set_rows", "get_rows");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "columns", PROPERTY_HINT_RANGE, "1,100,1"), "set_columns", "get_columns");

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "slot_size", PROPERTY_HINT_NONE, ""), "set_slot_size", "get_slot_size");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "slot_margin", PROPERTY_HINT_NONE, ""), "set_slot_margin", "get_slot_margin");

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "grid_padding", PROPERTY_HINT_NONE, ""), "set_grid_padding", "get_grid_padding");
}
