#include "item_view.h"

using namespace godot;
AUTO_REGISTER_CLASS(ItemView)

void ItemView::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_id"), &ItemView::get_id);
	ClassDB::bind_method(D_METHOD("set_id", "id"), &ItemView::set_id);

	ClassDB::bind_method(D_METHOD("get_icon"), &ItemView::get_icon);
	ClassDB::bind_method(D_METHOD("set_icon", "icon"), &ItemView::set_icon);

	ClassDB::bind_method(D_METHOD("get_item_amount"), &ItemView::get_item_amount);
	ClassDB::bind_method(D_METHOD("set_item_amount", "item_amount"), &ItemView::set_item_amount);

	ClassDB::bind_method(D_METHOD("get_name"), &ItemView::get_name);
	ClassDB::bind_method(D_METHOD("set_name", "name"), &ItemView::set_name);

	ClassDB::bind_method(D_METHOD("get_hint_description"), &ItemView::get_hint_description);
	ClassDB::bind_method(D_METHOD("set_hint_description", "hint_description"), &ItemView::set_hint_description);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "id"), "set_id", "get_id");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "icon", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_icon", "get_icon");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "item_amount"), "set_item_amount", "get_item_amount");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "hint_description"), "set_hint_description", "get_hint_description");
}