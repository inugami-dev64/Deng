#include "../../../core/deng_core.h"

int main() {
    std::vector<std::string> file_contents;
    dengUtils::FileManager fm;
    std::vector<std::string> css_attribute_value_data;

    fm.getCSSProperyValue("body", "width", "ui_styling/window.css", &css_attribute_value_data);
    for(std::string &str : css_attribute_value_data)
        std::cout << str << "\n";
    
    css_attribute_value_data.clear();
    
    fm.getCSSProperyValue("body", "height", "ui_styling/window.css", &css_attribute_value_data);
    for(std::string &str : css_attribute_value_data)
        std::cout << str << "\n";

    return 0;
}