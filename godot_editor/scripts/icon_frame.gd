extends Button

var slot_id: int
var item_id: int

signal texture_changed(new_texture: Texture)
var _texture_data : Texture

@export_file("*.png")
var texture_path: String = "":
	set(value): 
		if !value.is_empty(): 
			var texture: Texture = load(value)
			if(texture != null):
				emit_signal("texture_changed", texture)
				_texture_data = texture
				
				



	
