#ifndef DENG_SURFACE_WINDOW_H
#define DENG_SURFACE_WINDOW_H

#ifdef __cplusplus
extern "C" {
#endif

#define DENG_REFRESH_INTERVAL 1 // microseconds
#define MAX_MOUSE_PTR_DISTANCE_FROM_BORDER 100 

#define DENG_ERR_AUTOREPEAT_DISABLE 0x00000010

#define X11_WINDOW 0x01
#define WIN32_WINDOW 0x02 

#ifdef __DENG_SURFACE_C
    #include <stdlib.h>
    #include <signal.h>
    #include <vulkan/vulkan.h>
    #include <common/base_types.h>
#endif


#ifdef __linux__
    #define EVENT_MASKS KeyPressMask | \
    KeyReleaseMask | ButtonPressMask | \
    ButtonReleaseMask | LeaveWindowMask | \
    FocusChangeMask | PointerMotionMask
    
    #define DENG_CURSOR_HIDDEN (char*) "xcursor/invisible"
    #define DENG_CURSOR_DEFAULT (char*) "default"
    #define DENG_CURSOR_ROTATE (char*) "plus"
    #define DEFAULT_WINDOW_BORDER 5

    // X11 includes 
    #include <X11/Xutil.h>
    #include <X11/Xos.h>
    #include <X11/Xcursor/Xcursor.h>
    #include <X11/keysym.h>
    #include <X11/XKBlib.h>
    #include <vulkan/vulkan_xlib.h>

    typedef struct deng_SurfaceX11 {
        Display *p_display;
        Cursor default_cursor;
        deng_i32_t screen;
        Window window;
        XEvent event;
        GC gc;
    } deng_SurfaceX11;
#endif

#ifdef WIN32
    typedef struct deng_SurfaceWIN32 {
        WNDCLASS *p_window;
        HWND *p_hwnd;
        MSG *p_message;
        RAWINPUTDEVICE rids[2];
        RAWINPUT *p_raw_input;
        UINT raw_input_size;
        LONG mouse_x_pos;
        LONG mouse_y_pos;
    } deng_SurfaceWIN32;

    /* WIN32 message callback function */
    LRESULT CALLBACK win32_message_handler (
        HWND hwnd, 
        UINT msg, 
        WPARAM param, 
        LPARAM lparam
    );

    #ifndef __DENG_API_CORE
        #define DENG_WIN32_CLASS_NAME L"DENG_WINDOW"
    #else 
        #define DENG_WIN32_CLASS_NAME "DENG_WINDOW"
    #endif
#endif

#include <deng/key_definitions.h>
#include <deng/key_ev.h>

typedef enum deng_SurfaceWindowMode {
    DENG_WINDOW_MODE_FIXED          = 0,
    DENG_WINDOW_MODE_FULL_SCREEN    = 1,
    DENG_WINDOW_MODE_BORDERLESS     = 2,
    DENG_WINDOW_MODE_FLEXIBLE       = 3
} deng_SurfaceWindowMode;


typedef struct deng_VirtualMousePosition {
    deng_bool_t is_enabled;
    char *cursor;
    deng_bool_t is_lib_cur;
    deng_vec_t x;
    deng_vec_t y;
    deng_vec_t orig_x;
    deng_vec_t orig_y;
    deng_vec_t movement_x;
    deng_vec_t movement_y;
} deng_VirtualMousePosition;


typedef struct deng_SurfaceWindow {
    deng_i32_t width;
    deng_i32_t height;
    const char *window_title;
    deng_SurfaceWindowMode window_mode;
    
    deng_i32_t mode;
    deng_VirtualMousePosition virtual_mouse_position;

    #ifdef __linux__
        deng_SurfaceX11 x11_handler;
    #endif

    #ifdef _WIN32
        deng_SurfaceWIN32 win32_handler;
    #endif
    
} deng_SurfaceWindow;


void deng_SetMouseCursorMode (
    deng_SurfaceWindow *p_window, 
    deng_MouseMode mouse_mode
);

void deng_SetMouseCoords (
    deng_SurfaceWindow *p_window, 
    deng_i32_t x, 
    deng_i32_t y
);

void deng_GetMousePos (
    deng_SurfaceWindow *p_window, 
    deng_vec_t *p_x, 
    deng_vec_t *p_y, 
    deng_bool_t init_virtual_cursor
);

deng_SurfaceWindow *deng_InitVKSurfaceWindow (
    deng_i32_t width, 
    deng_i32_t height, 
    char *title, 
    deng_SurfaceWindowMode window_mode
);


void deng_UpdateWindow(deng_SurfaceWindow *p_window);
void deng_DestroyWindow(deng_SurfaceWindow *p_window);
deng_bool_t deng_IsRunning();

#ifdef __cplusplus
}
#endif

#endif
