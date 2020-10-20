#include "../../../core/deng_core.h"

int main() {
    std::vector<std::string> local_file_contents;
    dengUtils::FileManager local_fm;
    std::vector<std::string> local_css_attribute_value_data;

    local_fm.getCSSProperyValue("body", "width", "ui_styling/window.css", &local_css_attribute_value_data);
    for(std::string &str : local_css_attribute_value_data)
        std::cout << str << "\n";
    
    local_css_attribute_value_data.clear();
    
    local_fm.getCSSProperyValue("body", "height", "ui_styling/window.css", &local_css_attribute_value_data);
    for(std::string &str : local_css_attribute_value_data)
        std::cout << str << "\n";

    return 0;
}