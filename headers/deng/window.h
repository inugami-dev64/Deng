#ifndef WINDOWS_H
#define WINDOWS_H

#ifdef __WINDOW_CPP
    #include <stdlib.h>
    #include <vector>
    #include <string>

    #include <common/base_types.h>
    #include <common/err_def.h>
    #include <das/assets.h>
    #include <deng/deng_math.h>
    #include <vulkan/vulkan.h>
    #include <deng/surface_window.h>
#endif

namespace deng {   
    class WindowWrap
    {
    private:
        deng_SurfaceWindow *m_p_game_window;
        char *m_title;
        dengMath::vec2<deng_ui32_t> m_size;
        deng_bool_t m_is_movement;
        dengMath::vec2<deng_px_t> m_pixel_size;

    public:
        WindowWrap(int x, int y, const char *title);
        ~WindowWrap();
        deng_SurfaceWindow *getWindow();
        const char *getTitle();
        deng_bool_t isMovement();
        dengMath::vec2<deng_ui32_t> getSize();
        dengMath::vec2<deng_px_t> getPixelSize();
        void setMovement(deng_bool_t is_mov);
    };
}

#endif
