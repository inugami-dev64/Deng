#include "deng_key_definitions.h"
#include "deng_surface_core.h"

static deng_Key recent_press_key;
static deng_Key recent_release_key;

static deng_MouseButton recent_press_btn;
static deng_MouseButton recent_release_btn;

static deng_SurfaceWindow *p_recent_window;

static bool_t is_exit = false;
static bool_t is_window_deleted = false;
static size_t key_index;

deng_SurfaceWindow *deng_InitVKSurfaceWindow (
    int width, 
    int height, 
    char *title, 
    deng_SurfaceWindowMode window_mode
) {
    deng_SurfaceWindow *p_window = (deng_SurfaceWindow*) malloc(sizeof(deng_SurfaceWindow));

    p_window->width = width;
    p_window->height = height;
    p_window->window_title = title;
    p_window->virtual_mouse_position.is_enabled = 0;
    p_window->virtual_mouse_position.movement_x = 0;
    p_window->virtual_mouse_position.movement_y = 0;
    p_window->virtual_mouse_position.orig_x = width / 2;
    p_window->virtual_mouse_position.orig_y = height / 2;

    deng_InitKeyData(p_window);
    
    p_window->win32_handler.p_window = (WNDCLASS*) calloc(1, sizeof(WNDCLASS));
    p_window->win32_handler.p_hwnd = (HWND*) calloc(1, sizeof(HWND));
    p_window->win32_handler.p_message = (MSG*) calloc(1, sizeof(MSG));

    p_window->win32_handler.p_window->hInstance = GetModuleHandle(NULL);
    p_window->win32_handler.p_window->lpszClassName = DENG_WIN32_CLASS_NAME;
    p_window->win32_handler.p_window->hCursor = LoadCursor(NULL, IDC_ARROW);
    p_window->win32_handler.p_window->hbrBackground = (HBRUSH) COLOR_WINDOW;
    p_window->win32_handler.p_window->lpfnWndProc = win32_message_handler;
    
    p_window->active_keys.key_count = 0;
    p_window->active_keys.p_keys = (deng_Key*) malloc(sizeof(deng_Key));
    p_window->active_keys.btn_count = 0;
    p_window->active_keys.p_btn = (deng_MouseButton*) malloc(sizeof(deng_MouseButton));

    p_window->released_keys.key_count = 0;
    p_window->released_keys.p_keys = (deng_Key*) malloc(sizeof(deng_Key));
    p_window->released_keys.p_btn = (deng_MouseButton*) malloc(sizeof(deng_MouseButton));

    RegisterClass(p_window->win32_handler.p_window);
    LPCTSTR win_class = DENG_WIN32_CLASS_NAME;
    DWORD window_style;

    switch (window_mode)
    {
    case DENG_WINDOW_MODE_FIXED:
        window_style = WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX;
        break;

    case DENG_WINDOW_MODE_FULL_SCREEN:
        p_window->width = GetSystemMetrics(SM_CXSCREEN);
        p_window->height = GetSystemMetrics(SM_CYSCREEN);
        window_style = WS_POPUP | WS_VISIBLE;
        break;

    default:
        return NULL;
        break;
    }
    
    *p_window->win32_handler.p_hwnd = CreateWindowEx(0, win_class, title, window_style, CW_USEDEFAULT, CW_USEDEFAULT, p_window->width, p_window->height, NULL, NULL, p_window->win32_handler.p_window->hInstance, NULL);

    if(*p_window->win32_handler.p_hwnd == NULL) {
        printf("%s\n", "Failed to initialize win32 window!");
        exit(1);
    }

    p_window->win32_handler.rids[0].usUsagePage = 0x01;
    p_window->win32_handler.rids[0].usUsage = 0x02;
    p_window->win32_handler.rids[0].dwFlags = 0;
    p_window->win32_handler.rids[0].hwndTarget = *p_window->win32_handler.p_hwnd;

    p_window->win32_handler.rids[1].usUsagePage = 0x01;
    p_window->win32_handler.rids[1].usUsage = 0x06;
    p_window->win32_handler.rids[1].dwFlags = 0;
    p_window->win32_handler.rids[1].hwndTarget = *p_window->win32_handler.p_hwnd;

    if(!RegisterRawInputDevices(p_window->win32_handler.rids, 2, sizeof(p_window->win32_handler.rids[0]))) {
        printf("%ld\n", GetLastError());
        fflush(stdout);
        perror("Failed to register rid devices!");
    }

    p_window->win32_handler.mouse_x_pos = 0.0f;
    p_window->win32_handler.mouse_y_pos = 0.0f;

    p_window->mode = WIN32_WINDOW;

    return p_window;
}

LRESULT CALLBACK win32_message_handler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    switch(msg) {  
        case WM_CLOSE:
            deng_DestroyWindow(p_recent_window);
            break;

        case WM_DESTROY:
            return 0;

        case WM_SYSKEYDOWN: 
        case WM_KEYDOWN:
            recent_press_key = translateWIN32Key((deng_ui16_t) wparam);

            if((key_index = deng_FindKeyIndex(p_recent_window, recent_press_key, DENG_MOUSE_BTN_UNKNOWN, KB_KEY, ACTIVE_KEYS)) == p_recent_window->active_keys.key_count)
                deng_RegisterKeyEvent(p_recent_window, &recent_press_key, NULL, KB_KEY, ACTIVE_KEYS);
            break;

        case WM_SYSKEYUP: case WM_KEYUP: case WM_IME_KEYUP: 
            recent_release_key = translateWIN32Key((deng_ui16_t) wparam);

            for(int index = 0; index < p_recent_window->active_keys.key_count; index++) {
                if(p_recent_window->active_keys.p_keys[index] == recent_release_key) {
                    deng_RegisterKeyEvent(p_recent_window, &recent_release_key, NULL, KB_KEY, RELEASE_KEYS);
                    deng_ClearKeyEvent(p_recent_window, index, KB_KEY, ACTIVE_KEYS);
                }
            }
            break;

        case WM_LBUTTONDOWN: case WM_MBUTTONDOWN: case WM_RBUTTONDOWN:
            recent_press_btn = translateWIN32Btn(msg);

            if((key_index = deng_FindKeyIndex(p_recent_window, DENG_KEY_UNKNOWN, recent_press_btn, MOUSE_BUTTON, ACTIVE_KEYS)) == p_recent_window->active_keys.btn_count)
                deng_RegisterKeyEvent(p_recent_window, NULL, &recent_press_btn, MOUSE_BUTTON, ACTIVE_KEYS);
            break;

        case WM_LBUTTONUP: case WM_MBUTTONUP: case WM_RBUTTONUP: 
            recent_release_btn = translateWIN32Btn(msg);

            for(int index = 0; index < p_recent_window->active_keys.btn_count; index++) {
                if(p_recent_window->active_keys.p_btn[index] == recent_release_btn) {
                    deng_RegisterKeyEvent(p_recent_window, NULL, &recent_release_btn, MOUSE_BUTTON, RELEASE_KEYS);
                    deng_ClearKeyEvent(p_recent_window, index, MOUSE_BUTTON, ACTIVE_KEYS);
                }
            }
            break;
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

void deng_UpdateWindow(deng_SurfaceWindow *p_window) {
    deng_CleanKeyEvents (
        p_window, 
        KB_KEY | MOUSE_BUTTON, 
        RELEASE_KEYS
    );
    ShowWindow (
        *p_window->win32_handler.p_hwnd, 
        SW_NORMAL
    );

    while(PeekMessageW(p_window->win32_handler.p_message, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(p_window->win32_handler.p_message);
        DispatchMessage(p_window->win32_handler.p_message);
    }
    
    if(p_recent_window != p_window) p_recent_window = p_window;  

    if(is_exit) {
        is_window_deleted = true;
        deng_DestroyWindow(p_window);
    }

    usleep(DENG_REFRESH_INTERVAL);
}

void deng_SetMouseCoords (
    deng_SurfaceWindow *p_window, 
    int x, 
    int y
) {
    SetCursorPos(x, y);
    if(!p_window->virtual_mouse_position.is_enabled) {
        p_window->win32_handler.mouse_x_pos = (LONG) x;
        p_window->win32_handler.mouse_y_pos = (LONG) y;
    }
}

void deng_SetMouseCursorMode (
    deng_SurfaceWindow *p_window, 
    deng_MouseMode mouse_mode
) {
    switch(mouse_mode) 
    {
    case DENG_MOUSE_MODE_VIRTUAL:
        ShowCursor(false);
        p_window->virtual_mouse_position.is_enabled = false;

        float x, y;
        deng_GetMousePos(p_window, &x, &y, true);
        p_window->virtual_mouse_position.x = x;
        p_window->virtual_mouse_position.y = y;

        p_window->virtual_mouse_position.is_enabled = true;
        break;

    case DENG_MOUSE_MODE_CURSOR_VISIBLE:
        ShowCursor(true);
        p_window->virtual_mouse_position.is_enabled = false;
        break;
    }
}

void deng_GetMousePos (
    deng_SurfaceWindow *p_window, 
    float *p_x, 
    float *p_y, 
    bool_t init_virtual_cursor
) {
    POINT point;
    RECT rect;

    if
    (
        GetCursorPos(&point) && 
        GetWindowRect(*p_window->win32_handler.p_hwnd, &rect)
    ) {
        p_window->virtual_mouse_position.orig_x = (float) (rect.left + (LONG) (p_window->width / 2));
        p_window->virtual_mouse_position.orig_y = (float) (rect.top + (LONG) (p_window->height / 2));
    }
    
    if(p_window->virtual_mouse_position.is_enabled && !init_virtual_cursor) {
        p_window->virtual_mouse_position.movement_x = ((float) point.x) - p_window->virtual_mouse_position.orig_x;
        p_window->virtual_mouse_position.movement_y = ((float) point.y) - p_window->virtual_mouse_position.orig_y;

        if
        (
            (point.x - rect.left) != p_window->virtual_mouse_position.orig_x || 
            (point.y - rect.top) != p_window->virtual_mouse_position.orig_y
        ) {
            deng_SetMouseCoords (
                p_window, 
                p_window->virtual_mouse_position.orig_x, 
                p_window->virtual_mouse_position.orig_y
            );
        }

        p_window->virtual_mouse_position.x += p_window->virtual_mouse_position.movement_x;
        p_window->virtual_mouse_position.y += p_window->virtual_mouse_position.movement_y;

        *p_x = p_window->virtual_mouse_position.x;
        *p_y = p_window->virtual_mouse_position.y;
    }

    else {
        *p_x = (float) p_window->win32_handler.mouse_x_pos;
        *p_y = (float) p_window->win32_handler.mouse_y_pos;
        if(init_virtual_cursor) {
            deng_SetMouseCoords (
                p_window, 
                p_window->virtual_mouse_position.orig_x, 
                p_window->virtual_mouse_position.orig_y
            );
        }
    }

}

void deng_DestroyWindow(deng_SurfaceWindow *p_window) {
    deng_FreeKeyData(p_window);
    if(!is_window_deleted) {
        free(p_window->win32_handler.p_window);
        free(p_window->win32_handler.p_hwnd);
        free(p_window->win32_handler.p_message);
    }
    exit(EXIT_SUCCESS);
}

bool_t deng_IsRunning(deng_SurfaceWindow *p_window) {
    return GetMessage(p_window->win32_handler.p_message, NULL, 0, 0);
}
