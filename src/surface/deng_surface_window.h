#ifndef DENG_SURFACE_WINDOW_H

#define MOUSE_BUTTON 0
#define KB_KEY 1
#define REFRESH_INTERVAL 100 // microseconds

struct WindowConstructionInfo {
    int size_x;
    int size_y;
    unsigned int position_x;
    unsigned int position_y;
};

struct ActiveKeys {
    size_t key_count;
    enum DENGKey *p_keys;

    size_t btn_count;
    enum DENGMouseButton *p_btn;
};

struct WindowInfo {
    int x;
    int y;

};

#ifdef __linux__
    #define EVENT_MASKS KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | LeaveWindowMask | FocusChangeMask | PointerMotionMask
    struct X11Handler {
        Display *p_display;
        int screen;
        Window window;
        XEvent event;
        GC gc;
    };
#endif

#ifdef WIN32
    // win32 includes
#endif

// method declaration

// key vector methods
static void add_key(enum DENGKey *p_key, enum DENGMouseButton *p_btn, int type);
static void remove_key(size_t index, int type);
static size_t get_key_index(enum DENGKey key, enum DENGMouseButton, int type);
static void clean_keys(int type);

static void handle_key_events();
static void handle_mouse_events();

int is_key_active(enum DENGKey key);
int is_mouse_btn_active(enum DENGMouseButton btn);

void set_mouse_pos(int x, int y);
void get_mouse_pos(int *p_x, int *p_y);

void init_window(const char *p_window_title, int size_x, int size_y);
void update_window();
void destroy_window();

#endif