#include "UI.h"
#include "arpg.h"
#include "godot_cpp/classes/control.hpp"
#include "godot_cpp/classes/scene_tree.hpp"

#include "godot_cpp/variant/callable.hpp"


UI::UI(){

}

void UI::_bind_methods(){
    ClassDB::bind_method(D_METHOD("_on_hide_dialog"), &UI::_on_hide_dialog);
    ClassDB::bind_method(D_METHOD("_on_show_dialog",  "p_text"), &UI::_on_show_dialog);
}

void UI::_ready() {
    set_visible(false);
    label = get_node<Label>("Panel/Label");
    
    Node* root_tree = get_tree()->get_current_scene();
    ARPG* root = Object::cast_to<ARPG>(root_tree);

    if (root) {
        root->connect("show_dialog", Callable(this, "_on_show_dialog"));
        root->connect("hide_dialog", Callable(this, "_on_hide_dialog"));
    } else {
        WARN_PRINT("Não foi possível encontrar o nó ARPG.");
    }
}


void UI::_on_show_dialog(const String &p_text){
    label->set_text(p_text);
    set_visible(true);
}

void UI::_on_hide_dialog(){
    set_visible(false);
}