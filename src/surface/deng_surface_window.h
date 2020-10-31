#ifndef DENG_SURFACE_WINDOW_H

#ifdef __cplusplus
extern "C" {
#endif

#define MOUSE_BUTTON 0
#define KB_KEY 1

#define ACTIVE_KEYS 0x0001
#define RELEASE_KEYS 0x0002

#define DENG_REFRESH_INTERVAL 10 // microseconds
#define MAX_MOUSE_PTR_DISTANCE_FROM_BORDER 100 

#define DENG_ERR_AUTOREPEAT_DISABLE 0x00000010

#define X11_WINDOW 0x01
#define WIN32_WINDOW 0x02 


typedef enum WindowMode {
    DENG_WINDOW_MODE_FIXED = 0,
    DENG_WINDOW_MODE_RESIZEABLE = 1,
    DENG_WINDOW_MODE_FULL_SCREEN = 2
} WindowMode;

typedef struct VirtualMousePosition {
    int is_enabled;
    float x;
    float y;
    float orig_x;
    float orig_y;
    float movement_x;
    float movement_y;
} VirtualMousePosition;

typedef struct ActiveKeys {
    size_t key_count;
    DENGKey *p_keys;

    size_t btn_count;
    DENGMouseButton *p_btn;
} ActiveKeys;

typedef struct ReleasedKeys {
    size_t key_count;
    DENGKey *p_keys;

    size_t btn_count;
    DENGMouseButton *p_btn;
} ReleasedKeys;

#ifdef __linux__
    #define EVENT_MASKS KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | LeaveWindowMask | FocusChangeMask | PointerMotionMask
    
    #define DENG_CURSOR_HIDDEN "cursor/x11-cursors/invisible"
    #define DENG_CURSOR_DEFAULT "default"
    
    typedef struct X11Handler {
        Display *p_display;
        Cursor default_cursor;
        int screen;
        Window window;
        XEvent event;
        GC gc;
    } X11Handler;
#endif

#ifdef WIN32
    // win32 includes
#endif

typedef struct DENGWindow {
    int width;
    int height;
    int border_width;
    const char *window_title;
    WindowMode window_mode;
    ActiveKeys active_keys;
    ReleasedKeys released_keys;
    
    int mode;
    VirtualMousePosition virtual_mouse_position;

    #ifdef __linux__ 
        X11Handler x11_handler;
    #endif
    
} DENGWindow;

// method declarations
int is_key_active(DENGWindow *p_window, DENGKey key);
int is_mouse_btn_active(DENGWindow *p_window, DENGMouseButton btn);
int is_key_released(DENGWindow *p_window, DENGKey key);
int is_mouse_btn_released(DENGWindow *p_window, DENGMouseButton btn);

void set_mouse_cursor_mode(DENGWindow *p_window, int mouse_mode);
void set_mouse_coords(DENGWindow *p_window, int x, int y);
void get_mouse_pos(DENGWindow *p_window, float *p_x, float *p_y, int init_virtual_cursor);

DENGWindow *init_window(int width, int height, int border_width, const char *title, WindowMode window_mode);
void update_window(DENGWindow *p_window);
void destroy_window(DENGWindow *p_window);

#ifdef __cplusplus
}
#endif

#endif