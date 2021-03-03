#define __VULKAN_HANDLER_C
#include <deng/vulkan/vulkan_handler.h>

void deng_GetRequiredVKSurfaceExt (
    deng_SurfaceWindow *p_win,
    char ***p_exts,
    deng_ui32_t *p_ext_c, 
    deng_bool_t add_layer_ext
) {
    if(add_layer_ext) {
        *p_ext_c = 3;
        *p_exts = (char**) malloc((*p_ext_c) * sizeof(char*));
        (*p_exts)[2] = calloc(32, sizeof(char));
        strcpy((*p_exts)[2], DENG_VK_DEBUG_UTILS_EXT_NAME);
    }

    else {
        *p_ext_c = 2;
        (*p_exts) = (char**) malloc((*p_ext_c) * sizeof(char*));
    }

    **p_exts = (char*) calloc(32, sizeof(char));
    strcpy(**p_exts, DENG_VK_WSI_EXT_NAME);

    switch (p_win->mode)
    {
    case X11_WINDOW:
        *(*p_exts + 1) = (char*) calloc(32, sizeof(char));
        strcpy(*(*p_exts + 1), DENG_VK_XLIB_SURFACE_EXT_NAME);
        break;

    case WIN32_WINDOW:
        *(*p_exts + 1) = (char*) calloc(32, sizeof(char));
        strcpy(*(*p_exts + 1), DENG_VK_WIN32_SURFACE_EXT_NAME);
        break;
    
    default:
        break;
    }   
}

VkResult deng_InitVKSurface (
    deng_SurfaceWindow *p_window, 
    VkInstance *p_instance, 
    VkSurfaceKHR *p_surface
) {
    #ifdef __linux__
        VkXlibSurfaceCreateInfoKHR surface_info;
        surface_info.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
        surface_info.window = p_window->x11_handler.window;
        surface_info.dpy = p_window->x11_handler.p_display;
        surface_info.flags = 0;
        surface_info.pNext = NULL;

        PFN_vkCreateXlibSurfaceKHR vkCreateXlibSurfaceKHR;
        vkCreateXlibSurfaceKHR = (PFN_vkCreateXlibSurfaceKHR) vkGetInstanceProcAddr(*p_instance, "vkCreateXlibSurfaceKHR");

        return vkCreateXlibSurfaceKHR(*p_instance, &surface_info, NULL, p_surface);
    #endif

    #ifdef _WIN32
        VkWin32SurfaceCreateInfoKHR surface_info;
        surface_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        surface_info.hinstance = p_window->win32_handler.p_window->hInstance;
        surface_info.hwnd = *p_window->win32_handler.p_hwnd;
        surface_info.pNext = NULL;
        
        PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;
        vkCreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR) vkGetInstanceProcAddr(*p_instance, "vkCreateWin32SurfaceKHR");

        return vkCreateWin32SurfaceKHR(*p_instance, &surface_info, NULL, p_surface);
    #endif

    return VK_ERROR_UNKNOWN;
} 
