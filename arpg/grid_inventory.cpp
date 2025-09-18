#include "grid_inventory.h"

#include "godot_cpp/classes/color_rect.hpp"
#include "godot_cpp/classes/input_event.hpp"
#include "godot_cpp/classes/input_event_mouse_button.hpp"
#include "godot_cpp/classes/input_event_mouse_motion.hpp"
#include "godot_cpp/classes/texture_rect.hpp"
#include "godot_cpp/classes/theme.hpp"
#include "godot_cpp/classes/theme_db.hpp"
#include "godot_cpp/templates/pair.hpp"
#include "tools/auto_resgister.h"
#include <godot_cpp/classes/resource_loader.hpp>

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

void GridInventory::_gui_input(const Ref<InputEvent> &p_event) {
	Control::_gui_input(p_event);

	if (p_event.is_null()) {
		return;
	}

	// Handle drag preview visibility
	if (selected_slot_key != INVALID_KEY) {
		if (drag_preview) {
			drag_preview->set_position(get_local_mouse_position() - (slot_size / 2));
			drag_preview->set_visible(true);
		}
	} else {
		if (drag_preview) {
			drag_preview->set_visible(false);
		}
	}

	// Mouse motion: detect hovered slot
	if (Ref<InputEventMouseMotion> motion = p_event; motion.is_valid()) {
		const Point2i pos = motion->get_position();
		const int64_t key = _get_key_from_position(pos);

		if (Slot *slot = cells.getptr(key)) {
			if (key == hovered_slot_key) {
				return;
			}

			// Reset previous slot state
			if (Slot *prev_slot = cells.getptr(hovered_slot_key)) {
				prev_slot->state = State::NORMAL;
			}

			// Highlight new slot
			slot->state = State::HOVER;
			hovered_slot_key = key;
			queue_redraw();
		} else {
			_flush_hover_if_needed(); // Clear hover if outside grid
		}
	}

	// Mouse button: select slot or drop item
	if (Ref<InputEventMouseButton> mouse_btn = p_event; mouse_btn.is_valid()) {
		if (mouse_btn->is_pressed()) {
			const Point2i pos = mouse_btn->get_position();
			const int64_t key = _get_key_from_position(pos);

			if (Slot *slot = cells.getptr(key)) {
				if (drag_preview != nullptr) {
					if (drag_preview->is_visible()) {
						// Drop dragged item into slot
						drag_preview->set_position(slot->rect.position);
						selected_slot_key = INVALID_KEY;
					} else {
						// Pick up item from slot
						selected_slot_key = key;
					}
				}
			}
		}
	}
}

void GridInventory::_mouse_exited() {
	_flush_hover_if_needed(); // Clear hover when mouse leaves control
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

void GridInventory::_draw_all_slots() {
	const RID ci = get_canvas_item();
	for (const KeyValue<int64_t, Slot> &kv : cells) {
		const Slot &slot = kv.value;

		if (slot.state == State::HOVER && item_frame_hover.is_valid()) {
			item_frame_hover->draw(ci, slot.rect);
		} else if (slot.state == State::NORMAL && item_frame.is_valid()) {
			item_frame->draw(ci, slot.rect);
		}
	}
}

int64_t GridInventory::_get_key_from_position(const Point2i point) const {
	const Point2i adjust_point = point - grid_padding;
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
	if (const Slot *slot = cells.getptr(key); slot && slot->rect.has_point(adjust_point)) {
		return key;
	}
	return INVALID_KEY;
}

void GridInventory::_generate_grid_rects() {
	cells.clear();

	if (slot_size.x < 1 || slot_size.y < 1) {
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

			Slot value;
			value.rect = rect;

			cells.insert(key, value);
		}
	}

	// Reset hover and selection state
	hovered_slot_key = INVALID_KEY;
	selected_slot_key = INVALID_KEY;

	update_minimum_size();
	queue_redraw();
}

void GridInventory::_flush_hover_if_needed() {
	if (hovered_slot_key != INVALID_KEY) {
		if (Slot *slot = cells.getptr(hovered_slot_key)) {
			slot->state = State::NORMAL;
		}
		hovered_slot_key = INVALID_KEY;
		queue_redraw();
	}
}

void GridInventory::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_DRAW:
			_draw_background();
			_draw_all_slots();
			break;

		case NOTIFICATION_ENTER_TREE: {
			_generate_grid_rects();
			update_minimum_size();
			queue_redraw();

			_connect_signals(item_frame);
			_connect_signals(item_frame_hover);

			// Track when mouse exits control
			auto callable = callable_mp(this, &GridInventory::_mouse_exited);
			if (!is_connected("mouse_exited", callable)) {
				this->connect("mouse_exited", callable);
			}

			// Initialize drag preview (temporary item being moved)
			const auto res = ResourceLoader::get_singleton()->load("uid://cyp1n8fi003in");
			drag_preview = memnew(TextureRect);
			if (drag_preview != nullptr) {
				drag_preview->set_texture(res);
				add_child(drag_preview);
				drag_preview->set_expand_mode(TextureRect::EXPAND_IGNORE_SIZE);
				drag_preview->set_stretch_mode(TextureRect::STRETCH_KEEP_ASPECT_CENTERED);
				drag_preview->set_size(slot_size);
				drag_preview->set_pivot_offset(slot_size * 0.5);
				drag_preview->set_visible(false);
			}
			break;
		}
		case NOTIFICATION_EXIT_TREE: {
			const auto callable = callable_mp(this, &GridInventory::_mouse_exited);
			_disconnect_signals(item_frame);
			_disconnect_signals(item_frame_hover);

			if (is_connected("mouse_exited", callable)) {
				this->disconnect("mouse_exited", callable);
			}
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
void GridInventory::add_item(Ref<ItemView> item, Point2i point) {

}
void GridInventory::add_item() {

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
