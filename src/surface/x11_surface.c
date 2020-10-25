#include "deng_surface_core.h"


static time_t timer;
static struct X11Handler x11_handler;
static struct ActiveKeys active_keys;

static enum DENGKey recent_press_key;
static enum DENGKey recent_release_key;
static Bool is_held = False;

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

    XSelectInput(x11_handler.p_display, x11_handler.window, KeyPressMask | KeyReleaseMask | LeaveWindowMask | FocusChangeMask);
    x11_handler.gc = XCreateGC(x11_handler.p_display, x11_handler.window, 0, 0);
    XSetBackground(x11_handler.p_display, x11_handler.gc, BlackPixel(x11_handler.p_display, x11_handler.screen));
    XSetForeground(x11_handler.p_display, x11_handler.gc, WhitePixel(x11_handler.p_display, x11_handler.screen));
    
    active_keys.p_keys = malloc(sizeof(struct DENGKeyStatus));
    active_keys.key_count = 0;

    XClearWindow(x11_handler.p_display, x11_handler.window);
    XMapRaised(x11_handler.p_display, x11_handler.window);
}

static void add_key(struct DENGKeyStatus *p_key) {
    active_keys.key_count++;
    active_keys.p_keys = realloc(active_keys.p_keys, active_keys.key_count * sizeof(struct DENGKeyStatus));

    active_keys.p_keys[active_keys.key_count - 1] = *p_key;
}

static void remove_key(int index) {
    printf("%s\n", "removing key");
    int l_index, r_index;
    struct DENGKeyStatus tmp[active_keys.key_count--];

    for(l_index = 0, r_index = 0; l_index < active_keys.key_count; l_index++, r_index++) {
        if(r_index != index) tmp[l_index] = active_keys.p_keys[r_index];
        else { 
            r_index++;
            tmp[l_index] = active_keys.p_keys[r_index];
        }
    }

    free(active_keys.p_keys);
    active_keys.p_keys = malloc(active_keys.key_count * sizeof(struct DENGKeyStatus));

    for(l_index = 0; l_index < active_keys.key_count; l_index++)
        active_keys.p_keys[l_index] = tmp[l_index];

}

static void clean_keys() {
    free(active_keys.p_keys);
    active_keys.p_keys = malloc(sizeof(struct DENGKeyStatus));
    active_keys.key_count = 0;
}

static void handle_key_events() {
    size_t tmp_index;
    switch (x11_handler.event.type)
    {
    case KeyPress: {
        recent_press_key = translateX11KeySym(XLookupKeysym(&x11_handler.event.xkey, 0));
        XEvent new_event;
        XPeekEvent(x11_handler.p_display, &new_event);
        
        recent_release_key = translateX11KeySym(XLookupKeysym(&new_event.xkey, 0));

        struct DENGKeyStatus deng_key_status;
        deng_key_status.key_mode = DENG_KEY_MODE_PRESS;
        deng_key_status.key = recent_press_key;
        add_key(&deng_key_status);

        break;
    }
        
    case KeyRelease: {
        recent_release_key = translateX11KeySym(XLookupKeysym(&x11_handler.event.xkey, 0));
        XEvent new_event;
        XPeekEvent(x11_handler.p_display, &new_event);
        
        recent_press_key = translateX11KeySym(XLookupKeysym(&new_event.xkey, 0));

        for(int index = 0; index < active_keys.key_count; index++) {
            if(active_keys.p_keys[index].key == recent_release_key) {
                remove_key(index);
                break;
            }
        }
        break;
    }

    case FocusIn: case FocusOut:
        clean_keys();
        break;

    default:
        break;
    }

    printf("%s", "Active keys size is: ");
    printf("%ld\n", active_keys.key_count);
} 

int is_key_active(enum DENGKey key) {
    size_t index;
    for(index = 0; index < active_keys.key_count; index++) 
        if(key == active_keys.p_keys[index].key) return 1;

    return 0;
}

void update_window() {
    XNextEvent(x11_handler.p_display, &x11_handler.event);
    handle_key_events();
}

void destroy_window() {
    XFreeGC(x11_handler.p_display, x11_handler.gc);
    XDestroyWindow(x11_handler.p_display, x11_handler.window);
    XCloseDisplay(x11_handler.p_display);
    free(active_keys.p_keys);
    exit(0);
}