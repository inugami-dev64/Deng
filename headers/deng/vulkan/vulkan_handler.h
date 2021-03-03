#ifndef VULKAN_HANDLER_H
#define VULKAN_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#define DENG_VK_WSI_EXT_NAME "VK_KHR_surface"
#define DENG_VK_XLIB_SURFACE_EXT_NAME "VK_KHR_xlib_surface"
#define DENG_VK_WIN32_SURFACE_EXT_NAME "VK_KHR_win32_surface"
#define DENG_VK_DEBUG_UTILS_EXT_NAME "VK_EXT_debug_utils"

#ifdef __VULKAN_HANDLER_C
    #include <string.h> // strlen(), strcpy()
    #include <stdlib.h> // malloc(), calloc()

    #include <common/base_types.h>
    #include <vulkan/vulkan.h>
    #include <X11/XKBlib.h> // KeySym
    #include <deng/key_definitions.h>
    #include <deng/surface_window.h>
    #ifdef __linux__
        #include <vulkan/vulkan_xlib.h>
    #endif
    #ifdef _WIN32
        #include <vulkan/vulkan_win32.h>
    #endif
#endif 

void deng_GetRequiredVKSurfaceExt (
    deng_SurfaceWindow *p_win, 
    char ***p_exts,
    deng_ui32_t *p_ext_c, 
    deng_bool_t add_layer_ext
);

VkResult deng_InitVKSurface (
    deng_SurfaceWindow *p_window, 
    VkInstance *p_instance, 
    VkSurfaceKHR *p_surface
);

#ifdef __cplusplus
}
#endif

#endif
