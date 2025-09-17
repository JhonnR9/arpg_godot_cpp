#include "grid_inventory.h"

#include "godot_cpp/classes/input_event.hpp"
#include "godot_cpp/classes/input_event_mouse_button.hpp"
#include "godot_cpp/classes/input_event_mouse_motion.hpp"
#include "godot_cpp/templates/pair.hpp"
#include "tools/auto_resgister.h"
#include "godot_cpp/classes/theme_db.hpp"
#include "godot_cpp/classes/theme.hpp"

using namespace godot;
AUTO_REGISTER_CLASS(GridInventory)

inline Ref<StyleBox> GridInventory::get_background() const {
	return background;
}
void GridInventory::set_background(const Ref<StyleBox> &p_background) {
	_disconnect_signals(item_frame);
	_connect_signals(p_background);
	this->background = p_background;
	queue_redraw();
}
inline Ref<StyleBox> GridInventory::get_item_frame() const {
	return item_frame;
}
void GridInventory::set_item_frame(const Ref<StyleBox> &p_item_frame) {
	_disconnect_signals(item_frame);
	_connect_signals(p_item_frame);
	this->item_frame = p_item_frame;
	queue_redraw();
}
inline Ref<StyleBox> GridInventory::get_item_frame_hover() const {
	return item_frame_hover;
}

void GridInventory::set_item_frame_hover(const Ref<StyleBox> &p_item_frame_hover) {
	_disconnect_signals(item_frame_hover);
	_connect_signals(p_item_frame_hover);
	this->item_frame_hover = p_item_frame_hover;
	queue_redraw();
}

inline int GridInventory::get_rows() const {
	return rows;
}
void GridInventory::set_rows(int p_rows) {
	rows = p_rows;
	_generate_grid_rects();
}
inline int GridInventory::get_columns() const {
	return columns;
}
void GridInventory::set_columns(int p_columns) {
	columns = p_columns;
	_generate_grid_rects();
}

inline Size2i GridInventory::get_slot_size() const {
	return slot_size;
}
void GridInventory::set_slot_size(const Size2i &p_slot_size) {
	slot_size = p_slot_size;
	_generate_grid_rects();
}
inline Size2i GridInventory::get_slot_margin() const {
	return slot_margin;
}
void GridInventory::set_slot_margin(const Size2i &p_slot_margin) {
	slot_margin = p_slot_margin;
	_generate_grid_rects();
}
inline Size2i GridInventory::get_grid_padding() const {
	return grid_padding;
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

	const Ref<InputEventMouseMotion> motion = p_event;
	if (motion.is_valid()) {
		const Point2i pos = motion->get_position();
		const Point2i key = _get_cell_key(pos);

		if (cells.has(key)) {
			if (key == selected_slot_key) {
				return;
			}
			// Clear previous state before change
			if (cells.has(selected_slot_key)) {
				cells[selected_slot_key].state = State::DEFAULT;
			}

			cells[key].state = State::HOVER;
			selected_slot_key = key;
			queue_redraw();

		} else {
			_flush_hover_if_needed();
		}
	}

	const Ref<InputEventMouseButton> mouse_btn = p_event;
	if (mouse_btn.is_valid() && mouse_btn->is_pressed()) {
		const Point2i pos = mouse_btn->get_position();
		const Point2i key = _get_cell_key(pos);
		if (cells.has(key)) {
			// TODO drag and drop system, and slot clicked signal
		}
	}
}
void GridInventory::_mouse_exited() {
	_flush_hover_if_needed();
}
void GridInventory::_on_style_changed() {
	queue_redraw();
}
void GridInventory::_connect_signals(const Ref<StyleBox> &p_style) {
	const auto callable =  callable_mp(this, &GridInventory::_on_style_changed);

	if (p_style.is_valid() && !p_style->is_connected("changed", callable)) {
		p_style->connect("changed", callable);
	}
}
void GridInventory::_disconnect_signals(const Ref<StyleBox> &p_style) {
	const auto callable=  callable_mp(this, &GridInventory::_on_style_changed);

	if (p_style.is_valid() && !p_style->is_connected("changed", callable)) {
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
	if (item_frame.is_null() || item_frame_hover.is_null()) {
		return;
	}
	const RID ci = get_canvas_item();

	for (const KeyValue<Point2i, Slot> &kv : cells) {
		const Slot &slot = kv.value;

		if (slot.state == State::HOVER) {
			item_frame_hover->draw(ci, slot.rect);
		} else if (slot.state == State::DEFAULT) {
			item_frame->draw(ci, slot.rect);
		}
	}
}

Point2i GridInventory::_get_cell_key(const Point2i point) const {
	const Vector2i adjusted_point = point - Vector2(grid_padding.x, grid_padding.y);
	if (adjusted_point.x < 0 || adjusted_point.y < 0) {
		return Point2i(-1, -1);
	}

	const int cell_width = slot_size.x + slot_margin.x;
	const int cell_height = slot_size.y + slot_margin.y;

	if (cell_width <= 0 || cell_height <= 0) {
		return Point2i(-1, -1);
	}

	const int cell_x = adjusted_point.x / cell_width;
	const int cell_y = adjusted_point.y / cell_height;

	const Point2i key(cell_x, cell_y);
	if (cells.has(key) && cells[key].rect.has_point(point)) {
		return key;
	}

	return Point2i(-1, -1);
}

void GridInventory::_generate_grid_rects() {
	cells.clear();

	if (slot_size.x < 1 || slot_size.y < 1) {
		update_minimum_size();
		queue_redraw();
		return;
	}

	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < columns; col++) {
			const int x = grid_padding.x + col * (slot_size.x + slot_margin.x);
			const int y = grid_padding.y + row * (slot_size.y + slot_margin.y);

			const Point2i pos(x, y);
			const Rect2i rect(pos, slot_size);
			const Point2i key(col, row);

			Slot value;
			value.rect = rect;

			cells[key] = value;
		}
	}
	update_minimum_size();
	queue_redraw();
}
void GridInventory::_flush_hover_if_needed() {
	if (selected_slot_key.x >= 0 && selected_slot_key.y >= 0) {
		cells[selected_slot_key].state = State::DEFAULT;
		selected_slot_key = Point2i(-1, -1);
		queue_redraw();
	}
}

void GridInventory::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_DRAW: {
			// Godot clears the canvas on each redraw for Control nodes, so full redraw is always required.
			_draw_background();
			_draw_all_slots();
			break;
		}
		case NOTIFICATION_ENTER_TREE: {
			// Generate grid on first-time setup
			_generate_grid_rects();
			update_minimum_size();
			queue_redraw();

			_connect_signals(item_frame);
			_connect_signals(item_frame_hover);

			auto callable = callable_mp(this, &GridInventory::_mouse_exited);
			if (!is_connected("mouse_exited", callable)) {
				this->connect("mouse_exited", callable);
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
		case NOTIFICATION_APPLICATION_FOCUS_IN: {
			_connect_signals(item_frame);
			_connect_signals(item_frame_hover);
			break;
		}
		case NOTIFICATION_APPLICATION_FOCUS_OUT: {
			_disconnect_signals(item_frame);
			_disconnect_signals(item_frame_hover);
			break;
		}

		default:;
	}
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
