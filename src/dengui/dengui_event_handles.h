#ifndef DENGUI_EVENT_HANDLES_H
#define DENGUI_EVENT_HANDLES_H

namespace dengUI {
    class MinimiseHandle {
    private:
        SpriteInfo m_triangle_sprite_info;
        SpriteInfo m_window_sprite_info;
        SpriteInfo m_window_border_sprite_info;
        dengUIWindowSizeMode m_window_size_mode;
        
        dengBool m_verified;
    private:
        void verifyVertices();

    public:
        MinimiseHandle(SpriteInfo &triangle_sprite_info, SpriteInfo &window_sprite_info, SpriteInfo &window_border_sprite_info);
        void onClick();
    };
}

#endif