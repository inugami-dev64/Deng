#ifndef DENG_SURFACE_WINDOW_H

struct WindowConstructionInfo {
    int size_x;
    int size_y;
    unsigned int position_x;
    unsigned int position_y;
};

struct DENGKeyStatus {
    enum DENGKeyMode key_mode;
    enum DENGKey key;
};

struct ActiveKeys {
    size_t key_count;
    struct DENGKeyStatus *p_keys;
};

#ifdef __linux__
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

// method declarations
void init_window(const char *p_window_title, int size_x, int size_y);
static void add_key(struct DENGKeyStatus *p_key);
static void remove_key(int index);
static void clean_keys();
static void handle_key_events();
int is_key_active(enum DENGKey key);
void update_window();
void destroy_window();

#endif