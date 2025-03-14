#ifndef NPC_H
#define NPC_H

#include "character.h"
#include "godot_cpp/classes/area2d.hpp"
#include "godot_cpp/classes/node2d.hpp"
#include "godot_cpp/classes/resource.hpp"
#include "godot_cpp/variant/dictionary.hpp"

class NPC : public Character {
    GDCLASS(NPC, Character);

private:
    Dictionary data;
    uint16_t age;
    Ref<Resource> resource;
    Area2D* area2d;
    bool can_talk;

protected:
    static void _bind_methods();

public:
    NPC();
    virtual void _ready() override;
    virtual void _process(double_t delta) override;
    void load_data(const Dictionary& p_data);

    Ref<Resource> get_resource() const;
    void set_resource(Ref<Resource> p_resource); 
    void _on_area_2d_body_entered(Node2D* p_body);
    void _on_area_2d_body_exited(Node2D* p_body);       
};
  
#endif // NPC_H
