#include "grid_inventory.h"
#include "tools/auto_resgister.h"

AUTO_REGISTER_CLASS(GridInventory)

void GridInventory::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_DRAW: {
			ERR_FAIL_NULL(bg_style_box.ptr());

			const RID ci = get_canvas_item();
			const Vector2 control_size = get_size();
			const Rect2 rect(Point2(0, 0), Point2(control_size.x, control_size.y));

			bg_style_box->draw(ci, rect);

			break;
		}

		default:;
	}
}

Ref<StyleBox> GridInventory::get_bg_style_box() const {
	return bg_style_box;
}
void GridInventory::set_bg_style_box(const Ref<StyleBox> &p_bg_style_box) {
	if (bg_style_box.is_valid()) {
		bg_style_box->disconnect("changed", callable_mp(this, &GridInventory::_changed_bg_style_box));
	}
	this->bg_style_box = p_bg_style_box;

	if (p_bg_style_box.is_valid()) {
		p_bg_style_box->connect("changed", callable_mp(this, &GridInventory::_changed_bg_style_box));
		queue_redraw();
	}
}
Ref<StyleBox> GridInventory::get_item_frame_style_box() const {
	return item_frame_style_box;
}
void GridInventory::set_item_frame_style_box(const Ref<StyleBox> &p_item_frame_style_box) {
	this->item_frame_style_box = p_item_frame_style_box;

}
void GridInventory::_changed_bg_style_box() {
	queue_redraw();
}

void GridInventory::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_bg_style_box"), &GridInventory::get_bg_style_box);
	ClassDB::bind_method(D_METHOD("set_bg_style_box", "bg_style_box"), &GridInventory::set_bg_style_box);

	ClassDB::bind_method(D_METHOD("get_item_frame_style_box"), &GridInventory::get_item_frame_style_box);
	ClassDB::bind_method(D_METHOD("set_item_frame_style_box", "item_frame_style_box"), &GridInventory::set_item_frame_style_box);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "bg_style_box", PROPERTY_HINT_RESOURCE_TYPE, "StyleBox"), "set_bg_style_box", "get_bg_style_box");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "item_frame_style_box", PROPERTY_HINT_RESOURCE_TYPE, "StyleBox"), "set_item_frame_style_box", "get_item_frame_style_box");
}