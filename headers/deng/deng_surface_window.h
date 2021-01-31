#ifndef DENG_SURFACE_WINDOW_H
#define DENG_SURFACE_WINDOW_H

#ifdef __cplusplus
extern "C" {
#endif

#define MOUSE_BUTTON 0
#define KB_KEY 1

#define ACTIVE_KEYS 0x0001
#define RELEASE_KEYS 0x0002

#define DENG_REFRESH_INTERVAL 1 // microseconds
#define MAX_MOUSE_PTR_DISTANCE_FROM_BORDER 100 

#define DENG_ERR_AUTOREPEAT_DISABLE 0x00000010

#define X11_WINDOW 0x01
#define WIN32_WINDOW 0x02 

#define bool_t deng_ui8_t
#define true 1
#define false 0

typedef struct deng_ActiveKeys {
    size_t key_count;
    deng_Key *p_keys;

    size_t btn_count;
    deng_MouseButton *p_btn;
} deng_ActiveKeys;

typedef struct deng_ReleasedKeys {
    size_t key_count;
    deng_Key *p_keys;

    size_t btn_count;
    deng_MouseButton *p_btn;
} deng_ReleasedKeys;

typedef enum deng_SurfaceWindowMode {
    DENG_WINDOW_MODE_FIXED          = 0,
    DENG_WINDOW_MODE_FULL_SCREEN    = 1,
    DENG_WINDOW_MODE_BORDERLESS     = 2,
    DENG_WINDOW_MODE_FLEXIBLE       = 3
} deng_SurfaceWindowMode;

typedef struct deng_VirtualMousePosition {
    bool_t is_enabled;
    float x;
    float y;
    float orig_x;
    float orig_y;
    float movement_x;
    float movement_y;
} deng_VirtualMousePosition;

#ifdef __linux__
    #define EVENT_MASKS KeyPressMask | \
    KeyReleaseMask | ButtonPressMask | \
    ButtonReleaseMask | LeaveWindowMask | \
    FocusChangeMask | PointerMotionMask
    
    #define DENG_CURSOR_HIDDEN "xcursor/invisible"
    #define DENG_CURSOR_DEFAULT "default"
    #define DEFAULT_WINDOW_BORDER 5

    typedef struct deng_SurfaceX11 {
        Display *p_display;
        Cursor default_cursor;
        int screen;
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

typedef struct deng_SurfaceWindow {
    int width;
    int height;
    const char *window_title;
    deng_SurfaceWindowMode window_mode;
    deng_ActiveKeys active_keys;
    deng_ReleasedKeys released_keys;
    
    int mode;
    deng_VirtualMousePosition virtual_mouse_position;

    #ifdef __linux__
        deng_SurfaceX11 x11_handler;
    #endif

    #ifdef _WIN32
        deng_SurfaceWIN32 win32_handler;
    #endif
    
} deng_SurfaceWindow;

/* Set mouse cursor mode */
void deng_SetMouseCursorMode (
    deng_SurfaceWindow *p_window, 
    deng_MouseMode mouse_mode
);

void deng_SetMouseCoords (
    deng_SurfaceWindow *p_window, 
    int x, 
    int y
);

void deng_GetMousePos (
    deng_SurfaceWindow *p_window, 
    float *p_x, 
    float *p_y, 
    bool_t init_virtual_cursor
);

deng_SurfaceWindow *deng_InitVKSurfaceWindow (
    int width, 
    int height, 
    char *title, 
    deng_SurfaceWindowMode window_mode
);

void deng_UpdateWindow(deng_SurfaceWindow *p_window);
void deng_DestroyWindow(deng_SurfaceWindow *p_window);
bool_t deng_IsRunning(deng_SurfaceWindow *p_window);

#ifdef __cplusplus
}
#endif

#endif
