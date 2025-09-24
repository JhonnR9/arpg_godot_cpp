extends GridInventory

@export var texture : Texture2D
@export var texture2: Texture2D

func _ready() -> void:
	for i in 10:	
		var item = ItemView.new()
		item.name = "potion"
		item.icon=texture
		item.id= 25
		add_item(item)
	
	for i in 5:
		var item2 = ItemView.new()
		item2.name = "key"
		item2.icon = texture2
		item2.id = 10
		add_item(item2)
