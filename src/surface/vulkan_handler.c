#include "deng_surface_core.h"

const char **get_required_surface_extensions(DENGWindow *p_window, uint32_t *p_count, bool_t add_layer_ext) {
    char **pp_extensions;
    if(add_layer_ext) {
        *p_count = 3;
        pp_extensions = (char**) malloc((*p_count) * sizeof(char*));
        pp_extensions[2] = (char*) malloc((strlen(DENG_VK_DEBUG_UTILS_EXT_NAME) + 1) * sizeof(char));
        strcpy(pp_extensions[2], DENG_VK_DEBUG_UTILS_EXT_NAME);
    }

    else {
        *p_count = 2;
        pp_extensions = (char**) malloc((*p_count) * sizeof(char*));
    }

    pp_extensions[0] = (char*) malloc((strlen(DENG_VK_WSI_EXT_NAME) + 1) * sizeof(char));
    strcpy(pp_extensions[0], DENG_VK_WSI_EXT_NAME);

    switch (p_window->mode)
    {
    case X11_WINDOW:
        pp_extensions[1] = (char*) malloc((strlen(DENG_VK_XLIB_SURFACE_EXT_NAME) + 1) * sizeof(char));
        strcpy(pp_extensions[1], DENG_VK_XLIB_SURFACE_EXT_NAME);
        break;

    case WIN32_WINDOW:
        pp_extensions[1] = (char*) malloc((strlen(DENG_VK_WIN32_SURFACE_EXT_NAME) + 1) * sizeof(char));
        strcpy(pp_extensions[1], DENG_VK_WIN32_SURFACE_EXT_NAME);
        break;
    
    default:
        break;
    }   

    const char **pp_const_ext = pp_extensions;

    return pp_const_ext;
}

VkResult init_surface(DENGWindow *p_window, VkInstance *p_instance, VkSurfaceKHR *p_surface) {
    #ifdef __linux__
        VkXlibSurfaceCreateInfoKHR surface_info;
        surface_info.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
        surface_info.window = p_window->x11_handler.window;
        surface_info.dpy = p_window->x11_handler.p_display;
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