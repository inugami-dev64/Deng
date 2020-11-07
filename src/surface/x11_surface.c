#include "deng_surface_core.h"

static void handle_key_events();
static void handle_mouse_events();
static void set_cursor(DENGWindow *p_window, const char *cursor_path, int is_library_cur);

static DENGKey recent_press_key;
static DENGKey recent_release_key;

static DENGMouseButton recent_press_btn;
static DENGMouseButton recent_release_btn;

DENGWindow *init_window(int width, int height, char *title, WindowMode window_mode) {
    DENGWindow *p_window = malloc(sizeof(DENGWindow));
    
    p_window->width = width;
    p_window->height = height;
    p_window->window_title = title;
    p_window->virtual_mouse_position.is_enabled = 0;
    p_window->virtual_mouse_position.movement_x = 0;
    p_window->virtual_mouse_position.movement_y = 0;
    p_window->virtual_mouse_position.orig_x = width / 2;
    p_window->virtual_mouse_position.orig_y = height / 2;

    p_window->x11_handler.p_display = XOpenDisplay((char*) NULL);
    p_window->x11_handler.screen = DefaultScreen(p_window->x11_handler.p_display);

    XSizeHints local_size_hints;

    if(window_mode == DENG_WINDOW_MODE_FIXED) {
        local_size_hints.flags |= (PMinSize | PMaxSize);
        local_size_hints.min_width = local_size_hints.max_width =  p_window->width;
        local_size_hints.min_height = local_size_hints.max_height = p_window->height;   
    }

    

    p_window->x11_handler.window = XCreateSimpleWindow(p_window->x11_handler.p_display, DefaultRootWindow(p_window->x11_handler.p_display), 0, 0, width, height, DEFAULT_WINDOW_BORDER, WhitePixel(p_window->x11_handler.p_display, p_window->x11_handler.screen), BlackPixel(p_window->x11_handler.p_display, p_window->x11_handler.screen));

    XSetStandardProperties(p_window->x11_handler.p_display, p_window->x11_handler.window, title, title, None, NULL, 0, NULL);
    XSetWMNormalHints(p_window->x11_handler.p_display, p_window->x11_handler.window, &local_size_hints);
    int i;

    XSelectInput(p_window->x11_handler.p_display, p_window->x11_handler.window, EVENT_MASKS);
    p_window->x11_handler.gc = XCreateGC(p_window->x11_handler.p_display, p_window->x11_handler.window, 0, 0);
    XSetBackground(p_window->x11_handler.p_display, p_window->x11_handler.gc, BlackPixel(p_window->x11_handler.p_display, p_window->x11_handler.screen));
    XSetForeground(p_window->x11_handler.p_display, p_window->x11_handler.gc, WhitePixel(p_window->x11_handler.p_display, p_window->x11_handler.screen));

    Bool supported;
    if(!XkbSetDetectableAutoRepeat(p_window->x11_handler.p_display, True, &supported))
        exit(DENG_ERR_AUTOREPEAT_DISABLE);


    XClearWindow(p_window->x11_handler.p_display, p_window->x11_handler.window);
    XMapRaised(p_window->x11_handler.p_display, p_window->x11_handler.window);

    init_key_vectors(p_window);
    p_window->mode = X11_WINDOW;

    return p_window;
}

static void handle_key_events(DENGWindow *p_window) {
    size_t key_index;
    switch (p_window->x11_handler.event.type)
    {
    case KeyPress: {
        recent_press_key = translateX11Key(XLookupKeysym(&p_window->x11_handler.event.xkey, 0));

        if(key_index = get_key_index(p_window, recent_press_key, DENG_MOUSE_BTN_UNKNOWN, KB_KEY, ACTIVE_KEYS) == p_window->active_keys.key_count)
            add_key(p_window, &recent_press_key, NULL, KB_KEY, ACTIVE_KEYS);

        break;
    }
        
    case KeyRelease: {
        if(XEventsQueued(p_window->x11_handler.p_display, QueuedAfterReading)) {
            recent_release_key = translateX11Key(XLookupKeysym(&p_window->x11_handler.event.xkey, 0));

            for(int index = 0; index < p_window->active_keys.key_count; index++) {
                if(p_window->active_keys.p_keys[index] == recent_release_key) {
                    add_key(p_window, &p_window->active_keys.p_keys[index], NULL, KB_KEY, RELEASE_KEYS);
                    remove_key(p_window, index, KB_KEY, ACTIVE_KEYS);
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

        if(key_index = get_key_index(p_window, DENG_KEY_UNKNOWN, recent_press_btn, MOUSE_BUTTON, ACTIVE_KEYS) == p_window->active_keys.btn_count)
            add_key(p_window, NULL, &recent_press_btn, MOUSE_BUTTON, ACTIVE_KEYS);
        
        break;
    }
    
    case ButtonRelease:
        recent_release_btn = translateX11Btn(p_window->x11_handler.event.xbutton.button);
                
        for(int index = 0; index < p_window->active_keys.btn_count; index++) {
            if(p_window->active_keys.p_btn[index] == recent_release_btn) {
                add_key(p_window, NULL, &recent_release_key, MOUSE_BUTTON, RELEASE_KEYS);
                remove_key(p_window, index, MOUSE_BUTTON, ACTIVE_KEYS);
                break;   
            }
        }
        break;

    default:
        break;
    }
}

void set_mouse_coords(DENGWindow *p_window, int x, int y) {
    XWarpPointer(p_window->x11_handler.p_display, None, p_window->x11_handler.window, 0, 0, 0, 0, x, y);
}

void get_mouse_pos(DENGWindow *p_window, float *p_x, float *p_y, bool_t init_virtual_cursor) {
    // dummy variables for x11
    Window return_window;
    int win_x, win_y, x, y;
    unsigned int mask;
    XQueryPointer(p_window->x11_handler.p_display, p_window->x11_handler.window, &return_window, &return_window, &win_x, &win_y, &x, &y, &mask);

    if(p_window->virtual_mouse_position.is_enabled && !init_virtual_cursor) {
        
        p_window->virtual_mouse_position.movement_x = x - p_window->virtual_mouse_position.orig_x;
        p_window->virtual_mouse_position.movement_y = y - p_window->virtual_mouse_position.orig_y;

        if(x != p_window->virtual_mouse_position.orig_x || y != p_window->virtual_mouse_position.orig_y) 
            set_mouse_coords(p_window, p_window->virtual_mouse_position.orig_x, p_window->virtual_mouse_position.orig_y);
        
        p_window->virtual_mouse_position.x += p_window->virtual_mouse_position.movement_x;
        p_window->virtual_mouse_position.y += p_window->virtual_mouse_position.movement_y;

        *p_x = p_window->virtual_mouse_position.x;
        *p_y = p_window->virtual_mouse_position.y;
    }

    else {
        *p_x = (float) x;
        *p_y = (float) y;
        if(init_virtual_cursor)
            set_mouse_coords(p_window, p_window->virtual_mouse_position.orig_x, p_window->virtual_mouse_position.orig_y);
    }
}

static void set_cursor(DENGWindow *p_window, const char *file_path, bool_t is_library_cur) {
    Cursor cursor;

    switch (is_library_cur)
    {
    case 0:
        cursor = XcursorFilenameLoadCursor(p_window->x11_handler.p_display, realpath(file_path, NULL));
        break;

    case 1:
        cursor = XcursorLibraryLoadCursor(p_window->x11_handler.p_display, DENG_CURSOR_DEFAULT);
        break;
    
    default:
        break;
    }
    XDefineCursor(p_window->x11_handler.p_display, p_window->x11_handler.window, cursor);
}

void set_mouse_cursor_mode(DENGWindow *p_window, int mouse_mode) {
    if(mouse_mode & DENG_HIDE_CURSOR) {
        set_cursor(p_window, DENG_CURSOR_HIDDEN, false);
        p_window->virtual_mouse_position.is_enabled = false;
        float x, y;
        get_mouse_pos(p_window, &x, &y, true);
        p_window->virtual_mouse_position.x = x;
        p_window->virtual_mouse_position.y = y;

        p_window->virtual_mouse_position.is_enabled = true;
    }

    if(mouse_mode & DENG_SHOW_CURSOR) {
        set_cursor(p_window, DENG_CURSOR_DEFAULT, 1);
        p_window->virtual_mouse_position.is_enabled = false;
    }
}

void update_window(DENGWindow *p_window) {
    clean_keys(p_window, KB_KEY | MOUSE_BUTTON, RELEASE_KEYS);
    if(XCheckWindowEvent(p_window->x11_handler.p_display, p_window->x11_handler.window, KeyPressMask | KeyReleaseMask, &p_window->x11_handler.event));
        handle_key_events(p_window);
    
    if(XCheckWindowEvent(p_window->x11_handler.p_display, p_window->x11_handler.window, ButtonPressMask | ButtonReleaseMask, &p_window->x11_handler.event));
        handle_mouse_events(p_window);
    
    usleep(DENG_REFRESH_INTERVAL);
}

void destroy_window(DENGWindow *p_window) {
    XFreeGC(p_window->x11_handler.p_display, p_window->x11_handler.gc);
    XDestroyWindow(p_window->x11_handler.p_display, p_window->x11_handler.window);
    XCloseDisplay(p_window->x11_handler.p_display);
    if(p_window->active_keys.p_keys != NULL) free(p_window->active_keys.p_keys);
    if(p_window->active_keys.p_btn != NULL) free(p_window->active_keys.p_btn);
    if(p_window->released_keys.p_keys != NULL) free(p_window->released_keys.p_keys);
    if(p_window->released_keys.p_btn != NULL) free(p_window->released_keys.p_btn);
    exit(EXIT_SUCCESS);
}