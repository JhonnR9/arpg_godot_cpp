
#ifndef  UI_H
#define  UI_H

#include "godot_cpp/classes/control.hpp"
#include "godot_cpp/classes/label.hpp"

using namespace godot;

class UI : public Control {
    GDCLASS(UI, Control);

protected:
    static void _bind_methods();
    Label* label;
public:
    UI();
    virtual void _ready() override;

    void _on_show_dialog(const String& p_text);
    void _on_hide_dialog();
 
};

#endif //  UI_H
