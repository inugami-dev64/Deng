#include "../../../core/deng_core.h"

int main() {
    dengUI::CharData *p_ch_data;
    dengUI::Font main_font("Source Code Pro", 18);
    char ch = 'k';
    main_font.getCharInfo(&p_ch_data, ch);

    LOG(std::string("Texture coords for char \'") + ch + "\' are:");
    LOG("{" + std::to_string(p_ch_data->ch_vector_positions.first.first) + ";" + std::to_string(p_ch_data->ch_vector_positions.first.second) + "}");
    LOG("{" + std::to_string(p_ch_data->ch_vector_positions.second.first) + ";" + std::to_string(p_ch_data->ch_vector_positions.second.second) + "}");
    LOG("{" + std::to_string(p_ch_data->ch_vector_positions.third.first) + ";" + std::to_string(p_ch_data->ch_vector_positions.third.second) + "}");
    LOG("{" + std::to_string(p_ch_data->ch_vector_positions.fourth.first) + ";" + std::to_string(p_ch_data->ch_vector_positions.fourth.second) + "}");
    return 0;
}