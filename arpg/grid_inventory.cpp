#include "grid_inventory.h"

#include "godot_cpp/templates/pair.hpp"
#include "tools/auto_resgister.h"

AUTO_REGISTER_CLASS(GridInventory)

inline Ref<StyleBox> GridInventory::get_bg_style_box() const {
	return bg_style_box;
}
void GridInventory::set_bg_style_box(const Ref<StyleBox> &p_bg_style_box) {
	if (bg_style_box.is_valid()) {
		bg_style_box->disconnect("changed", callable_mp(this, &GridInventory::_on_changed_style_box));
	}
	this->bg_style_box = p_bg_style_box;

	if (p_bg_style_box.is_valid()) {
		p_bg_style_box->connect("changed", callable_mp(this, &GridInventory::_on_changed_style_box));
		queue_redraw();
	}
}
inline Ref<StyleBox> GridInventory::get_item_frame_style_box() const {
	return item_frame_style_box;
}
void GridInventory::set_item_frame_style_box(const Ref<StyleBox> &p_item_frame_style_box) {
	if (item_frame_style_box.is_valid()) {
		item_frame_style_box->disconnect("changed", callable_mp(this, &GridInventory::_on_changed_style_box));
	}
	this->item_frame_style_box = p_item_frame_style_box;

	if (p_item_frame_style_box.is_valid()) {
		p_item_frame_style_box->connect("changed", callable_mp(this, &GridInventory::_on_changed_style_box));
		queue_redraw();
	}
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

inline Vector2 GridInventory::get_slot_size() const {
	return slot_size;
}
void GridInventory::set_slot_size(const Vector2 &p_slot_size) {
	slot_size = p_slot_size;
	_generate_grid_rects();
}
inline Size2 GridInventory::get_slot_margin() const {
	return slot_margin;
}
void GridInventory::set_slot_margin(const Size2 &p_slot_margin) {
	slot_margin = p_slot_margin;
	_generate_grid_rects();
}
inline Size2 GridInventory::get_grid_padding() const {
	return grid_padding;
}
void GridInventory::set_grid_padding(const Size2 &p_grid_padding) {
	grid_padding = p_grid_padding;
	_generate_grid_rects();
}

void GridInventory::_on_changed_style_box() {
	queue_redraw();
}
Size2 GridInventory::_get_minimum_size() const {
	const real_t total_width = grid_padding.x * 2 + columns * slot_size.x + (columns - 1) * slot_margin.x;
	const real_t total_height = grid_padding.y * 2 + rows * slot_size.y + (rows - 1) * slot_margin.y;

	return Size2(total_width, total_height);
}

void GridInventory::_draw_bg() {
	ERR_FAIL_NULL(bg_style_box.ptr());

	const RID ci = get_canvas_item();
	const Rect2 rect(Point2(0, 0), get_size());

	bg_style_box->draw(ci, rect);
}
void GridInventory::_draw_item_frame() {
	ERR_FAIL_NULL(item_frame_style_box.ptr());
	const RID ci = get_canvas_item();

	for (const KeyValue<Point2i, Rect2> &kv : cells) {
		const Rect2 &rect = kv.value;
		item_frame_style_box->draw(ci, rect);
	}
}
inline Point2i GridInventory::_get_cell_key(const Point2i point) const {
	return Point2i(point.x / slot_size.x, point.y / slot_size.y);
}

void GridInventory::_generate_grid_rects() {
	cells.clear();

	if (slot_size.x < 1 || slot_size.y < 1) {
		update_minimum_size();
		queue_redraw();
		return;
	}

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			Point2 position(
					grid_padding.x + j * (slot_size.x + slot_margin.x),
					grid_padding.y + i * (slot_size.y + slot_margin.y));

			const Rect2 rect(position, slot_size);
			Point2i key = _get_cell_key(position);

			cells[key] = rect;
		}
	}
	update_minimum_size();
	queue_redraw();
}

void GridInventory::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_DRAW: {
			_draw_bg();
			_draw_item_frame();
			break;
		}
		case NOTIFICATION_ENTER_TREE:
			_generate_grid_rects();
			break;
		default:;
	}
}

void GridInventory::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_bg_style_box"), &GridInventory::get_bg_style_box);
	ClassDB::bind_method(D_METHOD("set_bg_style_box", "bg_style_box"), &GridInventory::set_bg_style_box);

	ClassDB::bind_method(D_METHOD("get_item_frame_style_box"), &GridInventory::get_item_frame_style_box);
	ClassDB::bind_method(D_METHOD("set_item_frame_style_box", "item_frame_style_box"), &GridInventory::set_item_frame_style_box);

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

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "bg_style_box", PROPERTY_HINT_RESOURCE_TYPE, "StyleBox", PROPERTY_USAGE_DEFAULT, "Background Style Box"), "set_bg_style_box", "get_bg_style_box");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "item_frame_style_box", PROPERTY_HINT_RESOURCE_TYPE, "StyleBox", PROPERTY_USAGE_DEFAULT, "Item Frame Style Box"), "set_item_frame_style_box", "get_item_frame_style_box");

	ADD_PROPERTY(PropertyInfo(Variant::INT, "rows", PROPERTY_HINT_RANGE, "1,100,1"), "set_rows", "get_rows");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "columns", PROPERTY_HINT_RANGE, "1,100,1"), "set_columns", "get_columns");

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "slot_size", PROPERTY_HINT_NONE, ""), "set_slot_size", "get_slot_size");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "slot_margin", PROPERTY_HINT_NONE, ""), "set_slot_margin", "get_slot_margin");

	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "grid_padding", PROPERTY_HINT_NONE, ""), "set_grid_padding", "get_grid_padding");
}
