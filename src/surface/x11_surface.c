#include "deng_surface_core.h"


// static method declarations
static void add_key(DENGWindow *p_window, DENGKey *p_key, DENGMouseButton *p_btn, int type);
static void remove_key(DENGWindow *p_window, size_t index, int type);
static size_t get_key_index(DENGWindow *p_window, DENGKey key, DENGMouseButton, int type);
static void clean_keys(DENGWindow *p_window, int type);

static int setback_mouse_pointer(DENGWindow *p_window, int x, int y);
static void handle_key_events();
static void handle_mouse_events();

static DENGKey recent_press_key;
static DENGKey recent_release_key;

static DENGMouseButton recent_press_btn;
static DENGMouseButton recent_release_btn;

DENGWindow *init_window(int width, int height, int border_width, const char *title, WindowMode window_mode) {
    DENGWindow *p_window = malloc(sizeof(DENGWindow));

    p_window->width = width;
    p_window->height = height;
    p_window->border_width = border_width;
    p_window->virtual_mouse_position.is_enabled = 0;
    p_window->virtual_mouse_position.movement_x = 0;
    p_window->virtual_mouse_position.movement_y = 0;

    p_window->x11_handler.p_display = XOpenDisplay((char*) NULL);
    p_window->x11_handler.screen = DefaultScreen(p_window->x11_handler.p_display);

    XSizeHints local_size_hints;
    local_size_hints.flags |= (PMinSize | PMaxSize);
    local_size_hints.min_width = local_size_hints.max_width =  p_window->width;
    local_size_hints.min_height = local_size_hints.max_height = p_window->height;

    p_window->x11_handler.window = XCreateSimpleWindow(p_window->x11_handler.p_display, DefaultRootWindow(p_window->x11_handler.p_display), 0, 0, width, height, border_width, WhitePixel(p_window->x11_handler.p_display, p_window->x11_handler.screen), BlackPixel(p_window->x11_handler.p_display, p_window->x11_handler.screen));
    
    XSetStandardProperties(p_window->x11_handler.p_display, p_window->x11_handler.window, title, title, None, NULL, 0, NULL);
    XSetWMNormalHints(p_window->x11_handler.p_display, p_window->x11_handler.window, &local_size_hints);
    int i;
    printf("%d\n", XkbGetDetectableAutoRepeat(p_window->x11_handler.p_display, &i));
    // XkbSetDetectableAutoRepeat(x11_handler.p_display, True, &i);

    XSelectInput(p_window->x11_handler.p_display, p_window->x11_handler.window, EVENT_MASKS);
    p_window->x11_handler.gc = XCreateGC(p_window->x11_handler.p_display, p_window->x11_handler.window, 0, 0);
    XSetBackground(p_window->x11_handler.p_display, p_window->x11_handler.gc, BlackPixel(p_window->x11_handler.p_display, p_window->x11_handler.screen));
    XSetForeground(p_window->x11_handler.p_display, p_window->x11_handler.gc, WhitePixel(p_window->x11_handler.p_display, p_window->x11_handler.screen));

    XClearWindow(p_window->x11_handler.p_display, p_window->x11_handler.window);
    XMapRaised(p_window->x11_handler.p_display, p_window->x11_handler.window);

    p_window->active_keys.p_keys = malloc(sizeof(enum DENGKey));
    p_window->active_keys.key_count = 0;

    p_window->active_keys.p_btn = malloc(sizeof(enum DENGMouseButton));
    p_window->active_keys.btn_count = 0;

    return p_window;
}


// Kb key methods
static void add_key(DENGWindow *p_window, DENGKey *p_key, DENGMouseButton *p_btn, int type) {
    switch (type)
    {
    case MOUSE_BUTTON:
        p_window->active_keys.btn_count++;
        p_window->active_keys.p_btn = realloc(p_window->active_keys.p_btn, p_window->active_keys.btn_count * sizeof(DENGMouseButton));
        
        p_window->active_keys.p_btn[p_window->active_keys.btn_count - 1] = *p_btn;
        break;

    case KB_KEY:
        p_window->active_keys.key_count++;
        p_window->active_keys.p_keys = realloc(p_window->active_keys.p_keys, p_window->active_keys.key_count * sizeof(DENGKey));

        p_window->active_keys.p_keys[p_window->active_keys.key_count - 1] = *p_key;
        break;
    
    default:
        break;
    }
    
}

size_t get_key_index(DENGWindow *p_window, DENGKey key, DENGMouseButton btn, int type) {
    size_t index;

    switch (type)
    {
    case MOUSE_BUTTON:
        for(index = 0; index < p_window->active_keys.btn_count; index++)
            if(p_window->active_keys.p_btn[index] == btn) return index;

        index = p_window->active_keys.btn_count;
        return index;
        break;
    
    case KB_KEY: {
        for(index = 0; index < p_window->active_keys.key_count; index++) 
            if(p_window->active_keys.p_keys[index] == key) return index;

        index = p_window->active_keys.key_count;
        return index;
        break;
    }

    default:
        break;
    }
}

static void remove_key(DENGWindow *p_window, size_t index, int type) {
    int l_index, r_index;

    switch (type)
    {
    case MOUSE_BUTTON: {
        DENGMouseButton tmp[p_window->active_keys.btn_count--];

        for(l_index = 0, r_index = 0; l_index < p_window->active_keys.btn_count; l_index++, r_index++) {
            if(r_index != index) tmp[l_index] = p_window->active_keys.p_btn[r_index];
            else {
                r_index++;
                tmp[l_index] = p_window->active_keys.p_btn[r_index];
            }
        }   

        free(p_window->active_keys.p_btn);
        p_window->active_keys.p_btn = malloc(p_window->active_keys.btn_count * sizeof(DENGMouseButton));

        for(l_index = 0; l_index < p_window->active_keys.btn_count; l_index++)
            p_window->active_keys.p_btn[l_index] = tmp[l_index];
        break;
    }

    case KB_KEY: {
        DENGKey tmp[p_window->active_keys.key_count--];

        for(l_index = 0, r_index = 0; l_index < p_window->active_keys.key_count; l_index++, r_index++) {
            if(r_index != index) tmp[l_index] = p_window->active_keys.p_keys[r_index];
            else { 
                r_index++;
                tmp[l_index] = p_window->active_keys.p_keys[r_index];
            }
        }

        free(p_window->active_keys.p_keys);
        p_window->active_keys.p_keys = malloc(p_window->active_keys.key_count * sizeof(DENGKey));

        for(l_index = 0; l_index < p_window->active_keys.key_count; l_index++)
            p_window->active_keys.p_keys[l_index] = tmp[l_index];
        break;
    }
    
    default:
        break;
    }
}

static void clean_keys(DENGWindow *p_window, int type) {
    switch (type)
    {
    case MOUSE_BUTTON:
        free(p_window->active_keys.p_btn);
        p_window->active_keys.p_btn = malloc(sizeof(DENGMouseButton));
        p_window->active_keys.key_count = 0;
        break;
    
    case KB_KEY:
        free(p_window->active_keys.p_keys);
        p_window->active_keys.p_keys = malloc(sizeof(DENGKey));
        p_window->active_keys.key_count = 0;   
        break;
    default:
        break;
    }
}

static void handle_key_events(DENGWindow *p_window) {
    size_t key_index;
    switch (p_window->x11_handler.event.type)
    {
    case KeyPress: {
        recent_press_key = translateX11Key(XLookupKeysym(&p_window->x11_handler.event.xkey, 0));

        if(key_index = get_key_index(p_window, recent_press_key, DENG_MOUSE_BTN_UNKNOWN, KB_KEY) == p_window->active_keys.key_count)
            add_key(p_window, &recent_press_key, NULL, KB_KEY);

        break;
    }
        
    case KeyRelease: {
        recent_release_key = translateX11Key(XLookupKeysym(&p_window->x11_handler.event.xkey, 0));
        
        XEvent new_event;
        XPeekEvent(p_window->x11_handler.p_display, &new_event);

        if(!(new_event.type == KeyPress && new_event.xkey.time == p_window->x11_handler.event.xkey.time && new_event.xkey.keycode == p_window->x11_handler.event.xkey.keycode)) {
            for(int index = 0; index < p_window->active_keys.key_count; index++) {
                if(p_window->active_keys.p_keys[index] == recent_release_key) {
                    remove_key(p_window, index, KB_KEY);
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

static void handle_mouse_events(DENGWindow *p_window) {
    size_t key_index;
    switch (p_window->x11_handler.event.type)
    {
    case ButtonPress: {
        recent_press_btn = translateX11Btn(p_window->x11_handler.event.xbutton.button);

        if(key_index = get_key_index(p_window, DENG_KEY_UNKNOWN, recent_press_btn, MOUSE_BUTTON) == p_window->active_keys.btn_count)
            add_key(p_window, NULL, &recent_press_btn, MOUSE_BUTTON);
        
        break;
    }
    
    case ButtonRelease:
        recent_release_btn = translateX11Btn(p_window->x11_handler.event.xbutton.button);
                
        for(int index = 0; index < p_window->active_keys.btn_count; index++) {
            if(p_window->active_keys.p_btn[index] == recent_release_btn) {
                remove_key(p_window, index, MOUSE_BUTTON);
                break;   
            }
        }
        break;

    default:
        break;
    }
}

int is_key_active(DENGWindow *p_window, DENGKey key) {
    size_t index;
    for(index = 0; index < p_window->active_keys.key_count; index++) 
        if(key == p_window->active_keys.p_keys[index]) return 1;

    return 0;
}

int is_mouse_btn_active(DENGWindow *p_window, DENGMouseButton btn) {
    size_t index;
    for(index = 0; index < p_window->active_keys.btn_count; index++)
        if(btn == p_window->active_keys.p_btn[index]) return 1;
    
    return 0;
}

void set_mouse_coords(DENGWindow *p_window, int x, int y) {
    XWarpPointer(p_window->x11_handler.p_display, None, p_window->x11_handler.window, 0, 0, 0, 0, x, y);
}

static int setback_mouse_pointer(DENGWindow *p_window, int x, int y) {
    int is_setback = 0;

    if(x <= MAX_MOUSE_PTR_DISTANCE_FROM_BORDER)
        is_setback = 1,
        set_mouse_coords(p_window, x + MAX_MOUSE_PTR_DISTANCE_FROM_BORDER, y);

    else if(x >= p_window->width - MAX_MOUSE_PTR_DISTANCE_FROM_BORDER)
        is_setback = 1,
        set_mouse_coords(p_window, x - MAX_MOUSE_PTR_DISTANCE_FROM_BORDER, y);

    if(y <= MAX_MOUSE_PTR_DISTANCE_FROM_BORDER)
        is_setback = 1,
        set_mouse_coords(p_window, x, y + MAX_MOUSE_PTR_DISTANCE_FROM_BORDER);

    else if(y >= p_window->height - MAX_MOUSE_PTR_DISTANCE_FROM_BORDER)
        is_setback = 1,
        set_mouse_coords(p_window, x, y - MAX_MOUSE_PTR_DISTANCE_FROM_BORDER);

    return is_setback;
}

void get_mouse_pos(DENGWindow *p_window, int *p_x, int *p_y) {
    // dummy variables for x11
    Window return_window;
    int win_x, win_y, x, y;
    unsigned int mask;
    XQueryPointer(p_window->x11_handler.p_display, p_window->x11_handler.window, &return_window, &return_window, &win_x, &win_y, &x, &y, &mask);

    if(p_window->virtual_mouse_position.is_enabled) {
        
        p_window->virtual_mouse_position.is_setback = setback_mouse_pointer(p_window, x, y);
        XQueryPointer(p_window->x11_handler.p_display, p_window->x11_handler.window, &return_window, &return_window, &win_x, &win_y, &x, &y, &mask);
        p_window->virtual_mouse_position.x += p_window->virtual_mouse_position.movement_x;
        p_window->virtual_mouse_position.y += p_window->virtual_mouse_position.movement_y;

        if(!p_window->virtual_mouse_position.is_setback) {
            p_window->virtual_mouse_position.movement_x = x - p_window->virtual_mouse_position.orig_x;
            p_window->virtual_mouse_position.movement_y = y - p_window->virtual_mouse_position.orig_y;
        }

        p_window->virtual_mouse_position.orig_x = x;
        p_window->virtual_mouse_position.orig_y = y;


        *p_x = p_window->virtual_mouse_position.x;
        *p_y = p_window->virtual_mouse_position.y;
    }

    else {
        *p_x = x;
        *p_y = y;
    }
}

void set_cursor(DENGWindow *p_window, const char *file_path) {
    Cursor cursor = XcursorFilenameLoadCursor(p_window->x11_handler.p_display, realpath(file_path, NULL));
    XDefineCursor(p_window->x11_handler.p_display, p_window->x11_handler.window, cursor);
}

void set_mouse_input_mode(DENGWindow *p_window, int mouse_mode) {
    if(mouse_mode & DENG_HIDE_CURSOR) {
        set_cursor(p_window, DENG_CURSOR_HIDDEN);
        p_window->virtual_mouse_position.is_enabled = 1;
    }

    if(mouse_mode & DENG_SHOW_CURSOR) {
        set_cursor(p_window, DENG_CURSOR_MARK_SELECT);
        p_window->virtual_mouse_position.is_enabled = 0;
    }
}

void update_window(DENGWindow *p_window) {
    if(XCheckWindowEvent(p_window->x11_handler.p_display, p_window->x11_handler.window, KeyPressMask | KeyReleaseMask, &p_window->x11_handler.event));
        handle_key_events(p_window);
    
    if(XCheckWindowEvent(p_window->x11_handler.p_display, p_window->x11_handler.window, ButtonPressMask | ButtonReleaseMask, &p_window->x11_handler.event));
        handle_mouse_events(p_window);
    
    usleep(REFRESH_INTERVAL);
}

void destroy_window(DENGWindow *p_window) {
    XFreeGC(p_window->x11_handler.p_display, p_window->x11_handler.gc);
    XDestroyWindow(p_window->x11_handler.p_display, p_window->x11_handler.window);
    XCloseDisplay(p_window->x11_handler.p_display);
    free(p_window->active_keys.p_keys);
    exit(EXIT_SUCCESS);
}