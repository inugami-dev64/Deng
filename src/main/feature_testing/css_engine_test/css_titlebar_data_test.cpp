#include "../../../core/deng_core.h"

// test passed
int main() {
    deng::Window *p_window;
    dengUtils::FileManager fm;
    dengCSS::CSSDataHandler css_data_handler(p_window, "ui_styling");

    dengCSS::CSSBodyData *p_objects_data;
    std::vector<std::string> files;
    
    LOG("test");
    fm.getFolderContents("ui_styling", &files, DENG_FOLDER_CONTENTS_READ_MODE_RECURSIVE, DENG_FOLDER_CONTENTS_SORT_STYLE_DONT_CARE);
    css_data_handler.readClassData("body", DENG_CSS_CLASS_READ_MODE_MAIN_CLASS, {1600, 1200});
    css_data_handler.getBodyData(&p_objects_data);

    std::cout << "background-color: {" << p_objects_data->background_color.first << ";" << p_objects_data->background_color.second << ";" << p_objects_data->background_color.third << "}\n";
    std::cout << "width: " << p_objects_data->width << "px\n"; 
    std::cout << "height: " << p_objects_data->height << "px\n";

    if(p_objects_data->p_general_border_info != nullptr) {
        std::cout << "border-color: {" << p_objects_data->p_general_border_info->border_color.first << ";" << p_objects_data->p_general_border_info->border_color.second << ";" << p_objects_data->p_general_border_info->border_color.third << "}\n";
        std::cout << "border-width: " << p_objects_data->p_general_border_info->border_width << "px\n";
    }

    if(p_objects_data->p_specified_border_info != nullptr) {
        std::cout << "border-top-color: {" << p_objects_data->p_specified_border_info->border_top_color.first << ";" << p_objects_data->p_specified_border_info->border_top_color.second << ";" << p_objects_data->p_specified_border_info->border_top_color.third << "}\n";
        std::cout << "border-top-width: " << p_objects_data->p_specified_border_info->border_top_width << "px\n";

        std::cout << "border-right-color: {" << p_objects_data->p_specified_border_info->border_right_color.first << ";" << p_objects_data->p_specified_border_info->border_right_color.second << ";" << p_objects_data->p_specified_border_info->border_right_color.third << "}\n";
        std::cout << "border-right-width: " << p_objects_data->p_specified_border_info->border_right_width << "px\n";

        std::cout << "border-bottom-color: {" << p_objects_data->p_specified_border_info->border_bottom_color.first << ";" << p_objects_data->p_specified_border_info->border_bottom_color.second << ";" << p_objects_data->p_specified_border_info->border_bottom_color.third << "}\n";
        std::cout << "border-bottom-width: " << p_objects_data->p_specified_border_info->border_bottom_width << "px\n";

        std::cout << "border-left-color: {" << p_objects_data->p_specified_border_info->border_left_color.first << ";" << p_objects_data->p_specified_border_info->border_left_color.second << ";" << p_objects_data->p_specified_border_info->border_left_color.third << "}\n";
        std::cout << "border-left-width: " << p_objects_data->p_specified_border_info->border_left_width << "px\n";
    }

    return 0;
}