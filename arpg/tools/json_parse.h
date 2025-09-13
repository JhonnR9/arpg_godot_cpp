
#ifndef  JSON_PARSE_H
#define  JSON_PARSE_H

#include "godot_cpp/classes/file_access.hpp"
#include "godot_cpp/classes/ref.hpp"
#include "godot_cpp/classes/ref_counted.hpp"


using namespace godot;

class JsonParse : public RefCounted {
    GDCLASS(JsonParse, RefCounted);

private:
    Ref<FileAccess> files;
protected:
    static void _bind_methods();

public:
    JsonParse();
    String load(String p_path);
    
};

#endif //  JSON_PARSE_H
