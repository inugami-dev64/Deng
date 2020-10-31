#ifndef VULKAN_HANDLER_H
#define VULKAN_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#define DENG_VK_WSI_EXT_NAME "VK_KHR_surface"
#define DENG_VK_XLIB_SURFACE_EXT_NAME "VK_KHR_xlib_surface"
#define DENG_VK_WIN32_SURFACE_EXT_NAME "VK_KHR_win32_surface"
#define DENG_VK_DEBUG_UTILS_EXT_NAME "VK_EXT_debug_utils"

const char **get_required_surface_extensions(DENGWindow *p_window, uint32_t *p_ext_count, int add_layer_ext);
VkResult init_surface(DENGWindow *p_window, VkInstance *p_instance, VkSurfaceKHR *p_surface);

#ifdef __cplusplus
}
#endif

#endif