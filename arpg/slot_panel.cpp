
#include "slot_panel.h"
#include "grid_inventory.h"
#include "tools/auto_resgister.h"
using namespace godot;

AUTO_REGISTER_INTERNAL_CLASS(SlotPanel)

void SlotPanel::_bind_methods() {
}
void SlotPanel::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_ENTER_TREE: {
			if (auto *node = get_parent()) {
				inventory = cast_to<GridInventory>(node);
			}
			break;
		}
		case NOTIFICATION_EXIT_TREE: {
			inventory = nullptr;

			if (preview) {
				memdelete(preview);
			}

			preview = nullptr;
			break;
		}
		default:;
	}
}

SlotPanel::SlotPanel() :
		inventory(nullptr), preview(nullptr) {
	preview = memnew(TextureRect);
}

Variant SlotPanel::_get_drag_data(const Vector2 &p_at_position) {
	if (!inventory) {
		preview->set_texture(Ref<Texture>{});
		preview->hide();
		ERR_PRINT("Inventory can't be null");
		return Variant();
	}
	if (!preview) {
		preview = memnew(TextureRect);
	}

	if (auto *slot = inventory->cells.getptr(key_in_grid)) {
		if (slot->item.is_valid()) {
			preview->set_texture(slot->item->get_icon());
			preview->set_expand_mode(TextureRect::EXPAND_IGNORE_SIZE);
			preview->set_stretch_mode(TextureRect::STRETCH_KEEP_ASPECT_CENTERED);
			preview->set_pivot_offset(slot->rect.get_size() / 2);
			preview->set_size(slot->rect.size);
			preview->show();
			set_drag_preview(preview);
			preview = nullptr; //now godot can delete then

			Ref<ItemView> item = slot->item;
			inventory->_clear_slot(*slot);

			return item;
		}
	}

	preview->set_texture(Ref<Texture>{});
	preview->hide();
	return Variant();
}

bool SlotPanel::_can_drop_data(const Vector2 &p_at_position, const Variant &p_data) const {
	if (!inventory) {
		return false;
	}

	if (const auto *slot = inventory->cells.getptr(inventory->hovered_slot_key)) {
		if (slot->item.is_null()) {
			return true;
		}

		const Ref<ItemView> item = p_data;
		if (item.is_null()) {
			return false;
		}

		if (slot->item->get_id() == item->get_id()) {
			return true;
		}
	}
	return false;
}

void SlotPanel::_drop_data(const Vector2 &p_at_position, const Variant &p_data) {
	if (!inventory) {
		ERR_PRINT("Inventory é nullptr em _drop_data");
		return;
	}

	if (const auto *slot = inventory->cells.getptr(inventory->hovered_slot_key)) {
		const Ref<ItemView> item = p_data;
		if (item.is_valid()) {
			const Point2i hovered_pos{
				inventory->_get_col_from_key(inventory->hovered_slot_key),
				inventory->_get_row_from_key(inventory->hovered_slot_key)
			};

			if (!inventory->add_item_at(item, hovered_pos)) {
				UtilityFunctions::print("Could not add item to destination slot.");
			}
		}
	}
	if (preview) {
		preview->set_texture(Ref<Texture>());
		preview->hide();
	}
}

