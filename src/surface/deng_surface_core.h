#ifndef DENG_SURFACE_CORE_H
#define DENG_SURFACE_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

#define _BSD_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#ifdef _WIN32
    #define VK_USE_PLATFORM_WIN32_KHR 1
#endif

#include <vulkan/vulkan.h>
#ifdef __linux__
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #include <X11/Xos.h>
    #include <X11/Xcursor/Xcursor.h>
    #include <X11/XKBlib.h>
    #include <X11/keysym.h>
    #include <vulkan/vulkan_xlib.h>
#endif

#ifdef _WIN32
    #include <windows.h>
    #include <windowsx.h>
    #include <vulkan/vulkan_win32.h>
#endif

#include "deng_key_definitions.h"
#include "deng_surface_window.h"
#include "key_vector.h"
#include "vulkan_handler.h"


#ifdef __cplusplus
}
#endif

#endif