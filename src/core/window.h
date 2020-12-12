#ifndef WINDOWS_H
#define WINDOWS_H

namespace deng {   

    class WindowWrap
    {
    private:
        DENGWindow *m_p_game_window;
        char *m_title;
        dengMath::vec2<uint32_t> m_size;
        dengInputMode m_input_mode;
        dengMath::vec2<double> m_pixel_size;

    public:
        WindowWrap(const int &x, const int &y, const char *title);
        ~WindowWrap();
        DENGWindow *getWindow();
        const char *getTitle();
        dengInputMode getInputMode();
        dengMath::vec2<uint32_t> getSize();
        dengMath::vec2<double> getPixelSize();
        void setInputMode(const dengInputMode &new_input_mode);
    };
}

#endif