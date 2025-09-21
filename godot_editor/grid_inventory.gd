extends GridInventory

@export var texture : Texture2D
func _ready() -> void:
	var item = ItemView.new()
	item.name = "hh"
	item.icon=texture
	item.id= 25
	
	#add_item(item)
	add_item_at(item, Vector2i(0, 0));
	add_item_at(item, Vector2i(0, 1));
