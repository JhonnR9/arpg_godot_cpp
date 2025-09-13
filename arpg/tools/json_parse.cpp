#include "json_parse.h"

#include "auto_resgister.h"
#include "godot_cpp/classes/file_access.hpp"
#include "godot_cpp/variant/string.hpp"
#include "godot_cpp/variant/utility_functions.hpp"

AUTO_REGISTER_CLASS(JsonParse)

JsonParse::JsonParse(){
  
}
void JsonParse::_bind_methods(){}

String JsonParse::load(String p_path) {
    Ref<FileAccess> files = FileAccess::open(p_path, FileAccess::READ);
    if (!files.is_valid()) {
        UtilityFunctions::printerr("Failed to open file: " + p_path + ", please verify path");
        return "";
    }

    String json_text = files->get_as_text();
    files->close();

    return json_text;
}
