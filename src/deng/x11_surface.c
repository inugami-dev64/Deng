#define __DENG_SURFACE_C
#include <deng/surface_window.h>

/* Static function declarations */
static void __HandleKeyEvents();
static void __HandleMouseEvents();
static void __SetCursor (
    deng_SurfaceWindow *p_win, 
    const char *cursor_path, 
    deng_bool_t is_library_cur
);

volatile sig_atomic_t deng_IsRunningVar;
static Atom atom_kill;

deng_SurfaceWindow *deng_InitVKSurfaceWindow (
    int width, 
    int height, 
    char *title, 
    deng_SurfaceWindowMode window_mode
) {
    deng_SurfaceWindow *p_win = (deng_SurfaceWindow*) malloc (
        sizeof(deng_SurfaceWindow) + 1
    );
    
    p_win->width = width;
    p_win->height = height;
    p_win->window_title = title;

    p_win->virtual_mouse_position.is_enabled = 0;
    p_win->virtual_mouse_position.movement_x = 0;
    p_win->virtual_mouse_position.movement_y = 0;
    p_win->virtual_mouse_position.orig_x = (deng_vec_t) width / 2;
    p_win->virtual_mouse_position.orig_y = (deng_vec_t) height / 2;

    p_win->x11_handler.p_display = XOpenDisplay(NULL);
    p_win->x11_handler.screen = DefaultScreen (
        p_win->x11_handler.p_display 
    );

    XSizeHints size_hints;

    if(window_mode == DENG_WINDOW_MODE_FIXED) {
        size_hints.flags |= (PMinSize | PMaxSize);
        size_hints.min_width = size_hints.max_width =  p_win->width;
        size_hints.min_height = size_hints.max_height = p_win->height;   
    }    

    p_win->x11_handler.window = XCreateSimpleWindow (
        p_win->x11_handler.p_display, 
        DefaultRootWindow(p_win->x11_handler.p_display), 
        0, 
        0, 
        width, 
        height, 
        DEFAULT_WINDOW_BORDER, 
        WhitePixel (
            p_win->x11_handler.p_display, 
            p_win->x11_handler.screen
        ), 
        BlackPixel (
            p_win->x11_handler.p_display, 
            p_win->x11_handler.screen
        )
    );

    XSetStandardProperties (
        p_win->x11_handler.p_display, 
        p_win->x11_handler.window, 
        title, 
        title, 
        None, 
        NULL, 
        0, 
        NULL
    );
    XSetWMNormalHints (
        p_win->x11_handler.p_display, 
        p_win->x11_handler.window, 
        &size_hints
    );

    XSelectInput (
        p_win->x11_handler.p_display, 
        p_win->x11_handler.window, 
        EVENT_MASKS
    );

    p_win->x11_handler.gc = XCreateGC (
        p_win->x11_handler.p_display, 
        p_win->x11_handler.window, 
        0, 
        0
    );

    XSetBackground (
        p_win->x11_handler.p_display, 
        p_win->x11_handler.gc, 
        BlackPixel (
            p_win->x11_handler.p_display, 
            p_win->x11_handler.screen
        )
    );
    XSetForeground (
        p_win->x11_handler.p_display, 
        p_win->x11_handler.gc, 
        WhitePixel (
            p_win->x11_handler.p_display, 
            p_win->x11_handler.screen
        )
    );

    Bool supported;
    if
    (
        !XkbSetDetectableAutoRepeat (
            p_win->x11_handler.p_display, 
            True, 
            &supported
        )
    ) exit(DENG_ERR_AUTOREPEAT_DISABLE);


    XClearWindow (
        p_win->x11_handler.p_display, 
        p_win->x11_handler.window
    );
    XMapRaised (
        p_win->x11_handler.p_display, 
        p_win->x11_handler.window
    );

    deng_IsRunningVar = true;
    p_win->mode = X11_WINDOW;
    
    // Set window delete atom to wm protocols
    atom_kill = XInternAtom (
        p_win->x11_handler.p_display, 
        "WM_DELETE_WINDOW", 
        True
    );
    
    XSetWMProtocols (
        p_win->x11_handler.p_display, 
        p_win->x11_handler.window, 
        &atom_kill, 
        1
    );


    // SIGTERM termination handler
    // memset(&signal_action, 0, sizeof(struct sigaction));
    // signal_action.sa_handler = __Terminate;
    // sigaction(SIGKILL, &signal_action, NULL);
    
    return p_win;
}


static void __HandleKeyEvents(deng_SurfaceWindow *p_win) {
    deng_Key key;
    switch (p_win->x11_handler.event.type)
    {
    case KeyPress: {
        key = translateX11Key (
            XLookupKeysym (
                &p_win->x11_handler.event.xkey, 
                0
            )
        );

        deng_RegisterKeyEvent (
            key, 
            DENG_MOUSE_BTN_UNKNOWN, 
            DENG_INPUT_TYPE_KB, 
            DENG_INPUT_EVENT_TYPE_ACTIVE 
        );

        break;
    }
        
    case KeyRelease: {
        if(XEventsQueued(p_win->x11_handler.p_display, QueuedAfterReading)) {
            key = translateX11Key (
                XLookupKeysym (
                    &p_win->x11_handler.event.xkey, 
                    0
                )
            );

            deng_RegisterKeyEvent (
                key,
                DENG_MOUSE_BTN_UNKNOWN,
                DENG_INPUT_TYPE_KB,
                DENG_INPUT_EVENT_TYPE_RELEASED
            );
        }
        break;
    }

    default:
        break;
    }
} 

static void __HandleMouseEvents(deng_SurfaceWindow *p_win) {
    deng_MouseButton btn = DENG_MOUSE_BTN_UNKNOWN;
    switch (p_win->x11_handler.event.type)
    {
    case ButtonPress: {
        btn = translateX11Btn(p_win->x11_handler.event.xbutton.button);
        deng_RegisterKeyEvent (
            DENG_KEY_UNKNOWN, 
            btn, 
            DENG_INPUT_TYPE_MOUSE, 
            DENG_INPUT_EVENT_TYPE_ACTIVE
        );
        
        break;
    }
    
    case ButtonRelease:
        btn = translateX11Btn(p_win->x11_handler.event.xbutton.button);
        
        deng_RegisterKeyEvent (
            DENG_KEY_UNKNOWN,
            btn,
            DENG_INPUT_TYPE_MOUSE,
            DENG_INPUT_EVENT_TYPE_RELEASED
        );
        break;

    default:
        break;
    }
}

void deng_SetMouseCoords (
    deng_SurfaceWindow *p_win, 
    int x, 
    int y
) {
    XWarpPointer (
        p_win->x11_handler.p_display, 
        None, 
        p_win->x11_handler.window, 
        0, 
        0, 
        0, 
        0, 
        x, 
        y
    );
}

void deng_GetMousePos (
    deng_SurfaceWindow *p_win, 
    deng_vec_t *p_x, 
    deng_vec_t *p_y, 
    deng_bool_t init_virtual_cursor
) {
    // dummy variables for x11
    Window return_window;
    int win_x, win_y, x, y;
    unsigned int mask;
    XQueryPointer (
        p_win->x11_handler.p_display, 
        p_win->x11_handler.window, 
        &return_window, 
        &return_window, 
        &win_x, 
        &win_y, 
        &x, 
        &y, 
        &mask
    );

    if(p_win->virtual_mouse_position.is_enabled && !init_virtual_cursor) {
        p_win->virtual_mouse_position.movement_x = x - p_win->virtual_mouse_position.orig_x;
        p_win->virtual_mouse_position.movement_y = y - p_win->virtual_mouse_position.orig_y;

        if
        (
            x != p_win->virtual_mouse_position.orig_x || 
            y != p_win->virtual_mouse_position.orig_y
        )   deng_SetMouseCoords (
                p_win, 
                p_win->virtual_mouse_position.orig_x, 
                p_win->virtual_mouse_position.orig_y
            );
        
        p_win->virtual_mouse_position.x += p_win->virtual_mouse_position.movement_x;
        p_win->virtual_mouse_position.y += p_win->virtual_mouse_position.movement_y;

        *p_x = p_win->virtual_mouse_position.x;
        *p_y = p_win->virtual_mouse_position.y;
    }

    else {
        *p_x = (deng_vec_t) x;
        *p_y = (deng_vec_t) y;
        if(init_virtual_cursor)
            deng_SetMouseCoords (
                p_win, 
                p_win->virtual_mouse_position.orig_x, 
                p_win->virtual_mouse_position.orig_y
            );
    }
}

static void __SetCursor (
    deng_SurfaceWindow *p_win, 
    const char *file_path, 
    deng_bool_t is_lib_cur
) {
    Cursor cursor;

    if(!is_lib_cur) {
        cursor = XcursorFilenameLoadCursor (
            p_win->x11_handler.p_display, 
            file_path
        );
    }
    else {
        cursor = XcursorLibraryLoadCursor (
            p_win->x11_handler.p_display, 
            "dnd_none"
        );
    }
    
    XDefineCursor (
        p_win->x11_handler.p_display, 
        p_win->x11_handler.window, 
        cursor
    );
}


/* Error callback function for xlib */
int __ErrorHandler (
    Display *display,
    XErrorEvent *err_ev
) {
    printf("Loop end\n");
    deng_IsRunningVar = false;

    return EXIT_FAILURE;
}


void deng_SetMouseCursorMode (
    deng_SurfaceWindow *p_win, 
    deng_MouseMode mouse_mode
) {
    switch(mouse_mode) 
    {
    case DENG_MOUSE_MODE_VIRTUAL:
        __SetCursor (
            p_win, 
            p_win->virtual_mouse_position.cursor, 
            false
        );
        p_win->virtual_mouse_position.is_enabled = false;
        deng_vec_t x, y;
        deng_GetMousePos (
            p_win, 
            &x, 
            &y, 
            true
        );
        p_win->virtual_mouse_position.x = x;
        p_win->virtual_mouse_position.y = y;

        p_win->virtual_mouse_position.is_enabled = true;
        break;

    case DENG_MOUSE_MODE_CURSOR_VISIBLE: 
        __SetCursor (
            p_win, 
            DENG_CURSOR_DEFAULT, 
            1
        );
        p_win->virtual_mouse_position.is_enabled = false;
        break;
    }
}

/* Update the window and recieve new key presses if needed */
void deng_UpdateWindow(deng_SurfaceWindow *p_win) {
    // Check for exit event
    if
    (
        XCheckTypedWindowEvent (
            p_win->x11_handler.p_display,
            p_win->x11_handler.window, 
            ClientMessage, 
            &p_win->x11_handler.event
        )
    ) {
        deng_IsRunningVar = false;
        return;
    }

    deng_UnreleaseKeys();
    
    if
    (
        XCheckWindowEvent (
            p_win->x11_handler.p_display, 
            p_win->x11_handler.window, 
            KeyPressMask | KeyReleaseMask, 
            &p_win->x11_handler.event
        )
    ) __HandleKeyEvents(p_win);
    
    if
    (
        XCheckWindowEvent (
            p_win->x11_handler.p_display, 
            p_win->x11_handler.window, 
            ButtonPressMask | ButtonReleaseMask, 
            &p_win->x11_handler.event
        )
    ) __HandleMouseEvents(p_win);

    usleep(DENG_REFRESH_INTERVAL);
}

void deng_DestroyWindow(deng_SurfaceWindow *p_win) {
    XFreeGC (
        p_win->x11_handler.p_display, 
        p_win->x11_handler.gc
    );
    XDestroyWindow (
        p_win->x11_handler.p_display, 
        p_win->x11_handler.window
    );
}

deng_bool_t deng_IsRunning() {
    return deng_IsRunningVar;
}
