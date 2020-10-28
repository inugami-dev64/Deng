#ifndef DENG_SURFACE_WINDOW_H

#define MOUSE_BUTTON 0
#define KB_KEY 1
#define REFRESH_INTERVAL 10 // microseconds
#define MAX_MOUSE_PTR_DISTANCE_FROM_BORDER 100 


typedef enum WindowMode {
    DENG_WINDOW_MODE_FIXED = 0,
    DENG_WINDOW_MODE_RESIZEABLE = 1,
    DENG_WINDOW_MODE_FULL_SCREEN = 2
} WindowMode;

typedef struct VirtualMousePosition {
    int is_enabled;
    int x;
    int y;
    int orig_x;
    int orig_y;
    int movement_x;
    int movement_y;
    int is_setback;
} VirtualMousePosition;

typedef struct ActiveKeys {
    size_t key_count;
    enum DENGKey *p_keys;

    size_t btn_count;
    enum DENGMouseButton *p_btn;
} ActiveKeys;

#ifdef __linux__
    #define EVENT_MASKS KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | LeaveWindowMask | FocusChangeMask | PointerMotionMask
    
    #define DENG_CURSOR_HIDDEN "cursor/x11-cursors/invisible"
    #define DENG_CURSOR_MARK_SELECT "cursor/x11-cursors/mark_select"
    
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

    VirtualMousePosition virtual_mouse_position;

    #ifdef __linux__
        X11Handler x11_handler;
    #endif
    
} DENGWindow;

// method declarations
int is_key_active(DENGWindow *p_window, DENGKey key);
int is_mouse_btn_active(DENGWindow *p_window, DENGMouseButton btn);

void set_mouse_input_mode(DENGWindow *p_window, int mouse_mode);
void get_mouse_pos(DENGWindow *p_window, int *p_x, int *p_y);
void set_cursor(DENGWindow *p_window, const char *cursor_path);

DENGWindow *init_window(int width, int height, int border_width, const char *title, WindowMode window_mode);
void update_window(DENGWindow *p_window);
void destroy_window(DENGWindow *p_window);

#endif