#include "deng_surface_core.h"


static time_t timer;
static struct X11Handler x11_handler;
static struct ActiveKeys active_keys;

static enum DENGKey recent_press_key;
static enum DENGKey recent_release_key;

static enum DENGMouseButton recent_press_btn;
static enum DENGMouseButton recent_release_btn;

void init_window(const char *p_window_title, int size_x, int size_y) {
    x11_handler.p_display = XOpenDisplay((char*) NULL);
    x11_handler.screen = DefaultScreen(x11_handler.p_display);

    XSizeHints local_size_hints;
    local_size_hints.flags |= (PMinSize | PMaxSize);
    local_size_hints.min_width = local_size_hints.max_width =  size_x;
    local_size_hints.min_height = local_size_hints.max_height = size_y;

    x11_handler.window = XCreateSimpleWindow(x11_handler.p_display, DefaultRootWindow(x11_handler.p_display), 0, 0, size_x, size_y, 5, WhitePixel(x11_handler.p_display, x11_handler.screen), BlackPixel(x11_handler.p_display, x11_handler.screen));
    
    XSetStandardProperties(x11_handler.p_display, x11_handler.window, p_window_title, p_window_title, None, NULL, 0, NULL);
    XSetWMNormalHints(x11_handler.p_display, x11_handler.window, &local_size_hints);
    int i;
    printf("%d\n", XkbGetDetectableAutoRepeat(x11_handler.p_display, &i));
    // XkbSetDetectableAutoRepeat(x11_handler.p_display, True, &i);

    XSelectInput(x11_handler.p_display, x11_handler.window, EVENT_MASKS);
    x11_handler.gc = XCreateGC(x11_handler.p_display, x11_handler.window, 0, 0);
    XSetBackground(x11_handler.p_display, x11_handler.gc, BlackPixel(x11_handler.p_display, x11_handler.screen));
    XSetForeground(x11_handler.p_display, x11_handler.gc, WhitePixel(x11_handler.p_display, x11_handler.screen));

    XClearWindow(x11_handler.p_display, x11_handler.window);
    XMapRaised(x11_handler.p_display, x11_handler.window);

    active_keys.p_keys = malloc(sizeof(enum DENGKey));
    active_keys.key_count = 0;

    active_keys.p_btn = malloc(sizeof(enum DENGMouseButton));
    active_keys.btn_count = 0;
}


// Kb key methods
static void add_key(enum DENGKey *p_key, enum DENGMouseButton *p_btn, int type) {
    switch (type)
    {
    case MOUSE_BUTTON:
        active_keys.btn_count++;
        active_keys.p_btn = realloc(active_keys.p_btn, active_keys.btn_count * sizeof(enum DENGMouseButton));
        
        active_keys.p_btn[active_keys.btn_count - 1] = *p_btn;
        break;

    case KB_KEY:
        active_keys.key_count++;
        active_keys.p_keys = realloc(active_keys.p_keys, active_keys.key_count * sizeof(enum DENGKey));

        active_keys.p_keys[active_keys.key_count - 1] = *p_key;
        break;
    
    default:
        break;
    }
    
}

size_t get_key_index(enum DENGKey key, enum DENGMouseButton btn, int type) {
    size_t index;

    switch (type)
    {
    case MOUSE_BUTTON:
        for(index = 0; index < active_keys.btn_count; index++)
            if(active_keys.p_btn[index] == btn) return index;

        index = active_keys.btn_count;
        return index;
        break;
    
    case KB_KEY: {
        for(index = 0; index < active_keys.key_count; index++) 
            if(active_keys.p_keys[index] == key) return index;

        index = active_keys.key_count;
        return index;
        break;
    }

    default:
        break;
    }
}

static void remove_key(size_t index, int type) {
    int l_index, r_index;

    switch (type)
    {
    case MOUSE_BUTTON: {
        enum DENGMouseButton tmp[active_keys.btn_count--];

        for(l_index = 0, r_index = 0; l_index < active_keys.btn_count; l_index++, r_index++) {
            if(r_index != index) tmp[l_index] = active_keys.p_btn[r_index];
            else {
                r_index++;
                tmp[l_index] = active_keys.p_btn[r_index];
            }
        }   

        free(active_keys.p_btn);
        active_keys.p_btn = malloc(active_keys.btn_count * sizeof(enum DENGMouseButton));

        for(l_index = 0; l_index < active_keys.btn_count; l_index++)
            active_keys.p_btn[l_index] = tmp[l_index];
        break;
    }

    case KB_KEY: {
        enum DENGKey tmp[active_keys.key_count--];

        for(l_index = 0, r_index = 0; l_index < active_keys.key_count; l_index++, r_index++) {
            if(r_index != index) tmp[l_index] = active_keys.p_keys[r_index];
            else { 
                r_index++;
                tmp[l_index] = active_keys.p_keys[r_index];
            }
        }

        free(active_keys.p_keys);
        active_keys.p_keys = malloc(active_keys.key_count * sizeof(enum DENGKey));

        for(l_index = 0; l_index < active_keys.key_count; l_index++)
            active_keys.p_keys[l_index] = tmp[l_index];
        break;
    }
    
    default:
        break;
    }
    

}

static void clean_keys(int type) {
    switch (type)
    {
    case MOUSE_BUTTON:
        free(active_keys.p_btn);
        active_keys.p_btn = malloc(sizeof(enum DENGMouseButton));
        active_keys.key_count = 0;
        break;
    
    case KB_KEY:
        free(active_keys.p_keys);
        active_keys.p_keys = malloc(sizeof(enum DENGKey));
        active_keys.key_count = 0;   
        break;
    default:
        break;
    }
}

static void handle_key_events() {
    size_t key_index;
    switch (x11_handler.event.type)
    {
    case KeyPress: {
        recent_press_key = translateX11Key(XLookupKeysym(&x11_handler.event.xkey, 0));

        if(key_index = get_key_index(recent_press_key, DENG_MOUSE_BTN_UNKNOWN, KB_KEY) == active_keys.key_count)
            add_key(&recent_press_key, NULL, KB_KEY);

        break;
    }
        
    case KeyRelease: {
        recent_release_key = translateX11Key(XLookupKeysym(&x11_handler.event.xkey, 0));
        
        XEvent new_event;
        XPeekEvent(x11_handler.p_display, &new_event);

        if(!(new_event.type == KeyPress && new_event.xkey.time == x11_handler.event.xkey.time && new_event.xkey.keycode == x11_handler.event.xkey.keycode)) {
            for(int index = 0; index < active_keys.key_count; index++) {
                if(active_keys.p_keys[index] == recent_release_key) {
                    remove_key(index, KB_KEY);
                    break;   
                }
            }
        }
        break;
    }

    default:
        break;
    }

    
} 

static void handle_mouse_events() {
    size_t key_index;
    switch (x11_handler.event.type)
    {
    case ButtonPress: {
        recent_press_btn = translateX11Btn(x11_handler.event.xbutton.button);

        if(key_index = get_key_index(DENG_KEY_UNKNOWN, recent_press_btn, MOUSE_BUTTON) == active_keys.btn_count)
            add_key(NULL, &recent_press_btn, MOUSE_BUTTON);
        
        break;
    }
    
    case ButtonRelease:
        recent_release_btn = translateX11Btn(x11_handler.event.xbutton.button);
                
        for(int index = 0; index < active_keys.btn_count; index++) {
            if(active_keys.p_btn[index] == recent_release_btn) {
                remove_key(index, MOUSE_BUTTON);
                break;   
            }
        }
        break;

    default:
        break;
    }
}

int is_key_active(enum DENGKey key) {
    size_t index;
    for(index = 0; index < active_keys.key_count; index++) 
        if(key == active_keys.p_keys[index]) return 1;

    return 0;
}

int is_mouse_btn_active(enum DENGMouseButton btn) {
    size_t index;
    for(index = 0; index < active_keys.btn_count; index++)
        if(btn == active_keys.p_btn[index]) return 1;
    
    return 0;
}

void get_mouse_coords(int *p_x, int *p_y) {
    // dummy variables for x11
    Window return_window;
    int win_x, win_y;
    unsigned int mask;

    XQueryPointer(x11_handler.p_display, x11_handler.window, &return_window, &return_window, &win_x, &win_y, p_x, p_y, &mask);
}

void set_mouse_coords(int x, int y) {
    Cursor cursor = XcursorLibraryLoadCursor(x11_handler.p_display, "dnd-none");
    XDefineCursor(x11_handler.p_display, x11_handler.window, cursor);
    XWarpPointer(x11_handler.p_display, None, x11_handler.window, 0, 0, 0, 0, x, y);
}




void update_window() {
    if(XCheckWindowEvent(x11_handler.p_display, x11_handler.window, KeyPressMask | KeyReleaseMask, &x11_handler.event));
        handle_key_events();
    
    if(XCheckWindowEvent(x11_handler.p_display, x11_handler.window, ButtonPressMask | ButtonReleaseMask, &x11_handler.event));
        handle_mouse_events();
    
    usleep(REFRESH_INTERVAL);
}

void destroy_window() {
    XFreeGC(x11_handler.p_display, x11_handler.gc);
    XDestroyWindow(x11_handler.p_display, x11_handler.window);
    XCloseDisplay(x11_handler.p_display);
    free(active_keys.p_keys);
    exit(EXIT_SUCCESS);
}