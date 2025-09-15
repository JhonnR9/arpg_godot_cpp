#include "grid_inventory.h"

#include "godot_cpp/templates/pair.hpp"
#include "tools/auto_resgister.h"

AUTO_REGISTER_CLASS(GridInventory)

Ref<StyleBox> GridInventory::get_bg_style_box() const {
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
Ref<StyleBox> GridInventory::get_item_frame_style_box() const {
	return item_frame_style_box;
}
void GridInventory::set_item_frame_style_box(const Ref<StyleBox> &p_item_frame_style_box) {
	if (item_frame_style_box.is_valid()) {
		item_frame_style_box->disconnect("changed", callable_mp(this, &GridInventory::_on_changed_style_box));
	}
	this->item_frame_style_box = p_item_frame_style_box;

	if (item_frame_style_box.is_valid()) {
		item_frame_style_box->connect("changed", callable_mp(this, &GridInventory::_on_changed_style_box));
		queue_redraw();
	}
}
int GridInventory::get_rows() const {
	return rows;
}
void GridInventory::set_rows(int p_rows) {
	rows = p_rows;
	_generate_grid_rects();
}
int GridInventory::get_columns() const {
	return columns;
}
void GridInventory::set_columns(int p_columns) {
	columns = p_columns;
	_generate_grid_rects();
}
Vector2 GridInventory::get_slot_size() const {
	return slot_size;
}
void GridInventory::set_slot_size(const Vector2 &p_slot_size) {
	slot_size = p_slot_size;
	_generate_grid_rects();
}
void GridInventory::_on_changed_style_box() {
	queue_redraw();
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
Point2i GridInventory::_get_cell_key(const Point2i point) const {
	Point2i key;
	key.x = point.x / slot_size.x;
	key.y = point.y / slot_size.y;
	return key;
}
void GridInventory::_generate_grid_rects() {
	// TODO make margin system

	cells.clear();
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			Point2 position(j * slot_size.x, i * slot_size.y);
			Rect2 rect(position, slot_size);
			Point2i key = _get_cell_key(position);

			cells[key] = rect;
		}
	}

	queue_redraw();
}

void GridInventory::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_DRAW: {
			_draw_bg();
			_draw_item_frame();
			break;
		}

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

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "bg_style_box", PROPERTY_HINT_RESOURCE_TYPE, "StyleBox"), "set_bg_style_box", "get_bg_style_box");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "item_frame_style_box", PROPERTY_HINT_RESOURCE_TYPE, "StyleBox"), "set_item_frame_style_box", "get_item_frame_style_box");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "rows"), "set_rows", "get_rows");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "columns"), "set_columns", "get_columns");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "slot_size"), "set_slot_size", "get_slot_size");
}