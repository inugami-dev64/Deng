#include "deng_surface_core.h"

const char **get_required_surface_extensions(DENGWindow *p_window, uint32_t *p_count, int add_layer_ext) {
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
        strcpy(pp_extensions[1], DENG_VK_WIN32_SURFACE_EXT_NAME);
        break;
    
    default:
        break;
    }   

    const char **pp_const_ext = pp_extensions;

    return pp_const_ext;
}

VkResult init_surface(DENGWindow *p_window, VkInstance *p_instance, VkSurfaceKHR *p_surface) {
    switch (p_window->mode)
    {
    case X11_WINDOW: {
        VkXlibSurfaceCreateInfoKHR surface_info;
        surface_info.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
        surface_info.window = p_window->x11_handler.window;
        surface_info.dpy = p_window->x11_handler.p_display;
        surface_info.pNext = NULL;

        return vkCreateXlibSurfaceKHR(*p_instance, &surface_info, NULL, p_surface);
        break;
    }
    
    default:
        break;
    }

    return VK_ERROR_UNKNOWN;
} 