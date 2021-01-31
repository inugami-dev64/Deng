#ifndef WINDOWS_H
#define WINDOWS_H

namespace deng {   

    class WindowWrap
    {
    private:
        deng_SurfaceWindow *m_p_game_window;
        char *m_title;
        dengMath::vec2<deng_ui32_t> m_size;
        deng_InputMode m_input_mode;
        dengMath::vec2<double> m_pixel_size;

    public:
        WindowWrap(const int &x, const int &y, const char *title);
        ~WindowWrap();
        deng_SurfaceWindow *getWindow();
        const char *getTitle();
        deng_InputMode getInputMode();
        dengMath::vec2<deng_ui32_t> getSize();
        dengMath::vec2<double> getPixelSize();
        void setInputMode(const deng_InputMode &new_input_mode);
    };
}

#endif