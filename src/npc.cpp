#include "npc.h"
#include "arpg.h"
#include "godot_cpp/classes/area2d.hpp"
#include "godot_cpp/classes/input.hpp"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/variant/callable.hpp"
#include "godot_cpp/variant/utility_functions.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "player.h"

NPC::NPC() {
    
}

void NPC::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_resource"), &NPC::get_resource);
    ClassDB::bind_method(D_METHOD("set_resource", "p_resource"), &NPC::set_resource);

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "resource", PROPERTY_HINT_RESOURCE_TYPE, "Resource"), 
                 "set_resource", "get_resource");

    ClassDB::bind_method(D_METHOD("_on_area_2d_body_entered", "body"), &NPC::_on_area_2d_body_entered);
    ClassDB::bind_method(D_METHOD("_on_area_2d_body_exited", "body"), &NPC::_on_area_2d_body_exited);
}

void NPC::_ready() {
    Character::_ready();
    area2d = get_node<Area2D>("Area2D");
    area2d->connect("body_entered", Callable(this, "_on_area_2d_body_entered"));
    area2d->connect("body_exited", Callable(this, "_on_area_2d_body_exited"));
}

void NPC::load_data(const Dictionary& p_data) {
    data = p_data;

    if (p_data.has("name")) {
        set_display_name((String)p_data["name"]);
    }

    if (p_data.has("age")) {
        Variant value = p_data["age"];
        if (value.get_type() == Variant::INT) {
            uint16_t temp_age = (uint16_t)value;
            age = temp_age;
        }
    }
}

void NPC::_on_area_2d_body_entered(Node2D* body) {

    if ( Player* player = Object::cast_to<Player>(body)) {
        if (ARPG* arpg = Object::cast_to<ARPG>(get_tree()->get_current_scene())){
            arpg->emit_signal("show_dialog", vformat("%s:  --ola amigo voce e um amigo!!!", get_display_name()));

        }
    }

}

void NPC::_on_area_2d_body_exited(Node2D* body) {

    if ( Player* player = Object::cast_to<Player>(body)) {
        if (ARPG* arpg = Object::cast_to<ARPG>(get_tree()->get_current_scene())){
            arpg->emit_signal("hide_dialog");
        }
    }


}

Ref<Resource> NPC::get_resource() const {
    return resource;
}

void NPC::set_resource(Ref<Resource> p_resource) {
    resource = p_resource;
}


