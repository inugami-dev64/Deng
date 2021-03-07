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
    #ifdef __DENG_API_CORE
        #include <vulkan/vulkan.h>
    #endif 
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

#ifdef _WIN32
    
    #include <windows.h>
    #include <synchapi.h>    
    #include <stdio.h>
    
    typedef struct deng_SurfaceWIN32 {
        WNDCLASS win;
        HWND hwnd;
        MSG message;
        RAWINPUTDEVICE rids[2];
        RAWINPUT raw_input;
        UINT raw_input_size;
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


/*
 * Virtual cursor (VC) position in DENG means that mouse cursor is stuck to certain position
 * and is only allowed to move within one frame cycle.
 * That also means that the recorded mouse position is not corresponding to the real
 * position of mouse cursor.
 */
typedef struct deng_VCData {
    deng_bool_t is_enabled;
    #ifdef __linux__
        char *cursor;
        deng_bool_t is_lib_cur;
    #endif
    // X and Y virtual position are in 64bit floating point integer since 
    // Arithmetic operation with these types are needed in camera classes
    deng_f64_t x;
    deng_f64_t y;
    deng_px_t orig_x;
    deng_px_t orig_y;
} deng_VCData;


/*
 * Main structure for storing information about surface window
 * and its parameters. Some members are platform specific to WIN32 or Xlib
 */
typedef struct deng_SurfaceWindow {
    deng_i32_t width;
    deng_i32_t height;
    const char *window_title;
    deng_px_t mx;
    deng_px_t my;
    deng_SurfaceWindowMode window_mode;
    deng_i32_t mode;
    deng_VCData vc_data;

    #ifdef __linux__
        deng_SurfaceX11 x11_handler;
    #endif

    #ifdef _WIN32
        deng_SurfaceWIN32 win32_handler;
    #endif
    
} deng_SurfaceWindow;


/*
 * Create new platform independant deng_SurfaceWindow instance for vulkan
 * This functions uses either Xlib or WIN32 api to create window depending on the operating system
 */
deng_SurfaceWindow *deng_InitVKSurfaceWindow (
    deng_i32_t width, 
    deng_i32_t height, 
    char *title, 
    deng_SurfaceWindowMode window_mode
);


/*
 * Update window events and key arrays
 * This function is meant to be called with every loop iteration 
 */
void deng_UpdateWindow(deng_SurfaceWindow *p_win);


/*
 * Destroy window instance and free all resources that were used
 */
void deng_DestroyWindow(deng_SurfaceWindow *p_win);

/*
 * Check if window is still running and no close events have happened
 */
deng_bool_t deng_IsRunning();


/****************************************/
/****** Input device communication ******/
/****************************************/

/*
 * Switch mouse cursor behaviour within the DENG window 
 */
void deng_SetMouseCursorMode(
    deng_SurfaceWindow* p_window,
    deng_MouseMode mouse_mode
);


/*
 * Force mouse cursor to certain location on window
 */
void deng_SetMouseCoords(
    deng_SurfaceWindow* p_window,
    deng_i32_t x,
    deng_i32_t y
);


/*
 * Synchronise mouse position in deng_SurfaceWindow
 */
void deng_GetMousePos(
    deng_SurfaceWindow* p_window,
    deng_bool_t init_vc
);


/*
 * Limit the largest and smallest virtual cursor position that can be achieved using 
 * virtual mouse positioning
 */
void deng_LimitVirtualPos(
    deng_px_t max_x,
    deng_px_t min_x,
    deng_px_t max_y,
    deng_px_t min_y
);


/*
 * Set virtual mouse position overflow actions that specify what
 * should happen if virtual mouse position limit has been reached
 */
void deng_SetOverflowAction(
    deng_VCPOverflowAction x_overflow_act,
    deng_VCPOverflowAction y_overflow_act
);


/*
 * Set the cursor movement speed.
 * VC movement speed is calculated by multiplying specified sensitivity 
 * with delta movement during a frame
 */
void deng_SetVCSens(
    deng_f64_t x_sens,
    deng_f64_t y_sens
);


#ifdef __cplusplus
}
#endif

#endif
