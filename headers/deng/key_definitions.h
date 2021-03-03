#ifndef DENG_KEY_DEFINITIONS_H
#define DENG_KEY_DEFINITIONS_H

#define DENG_KEY_COUNT 113
#define DENG_MOUSE_BTN_COUNT 5

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __KEY_TRANSLATION_C
    #include <X11/XKBlib.h>
    #include <X11/keysym.h>
#endif

typedef enum deng_MouseMode {
    DENG_MOUSE_MODE_VIRTUAL         = 0,
    DENG_MOUSE_MODE_CURSOR_VISIBLE  = 1
} deng_MouseMode;


typedef enum deng_Key {
    DENG_KEY_UNKNOWN        =   -1,
    DENG_KEY_SPACE          =   0,
    DENG_KEY_APOSTROPHE     =   1,
    DENG_KEY_COMMA          =   2,
    DENG_KEY_MINUS          =   3,
    DENG_KEY_DOT            =   4,
    DENG_KEY_SLASH          =   5,
    DENG_KEY_0              =   6,
    DENG_KEY_1              =   7,
    DENG_KEY_2              =   8,
    DENG_KEY_3              =   9,
    DENG_KEY_4              =   10,
    DENG_KEY_5              =   11,
    DENG_KEY_6              =   12,
    DENG_KEY_7              =   13,
    DENG_KEY_8              =   14,
    DENG_KEY_9              =   15,
    DENG_KEY_SEMICOLON      =   16,
    DENG_KEY_EQUAL          =   17,
    DENG_KEY_A              =   18,
    DENG_KEY_B              =   19,
    DENG_KEY_C              =   20,
    DENG_KEY_D              =   21,
    DENG_KEY_E              =   22,
    DENG_KEY_F              =   23,
    DENG_KEY_G              =   24,
    DENG_KEY_H              =   25,
    DENG_KEY_I              =   26,
    DENG_KEY_J              =   27, 
    DENG_KEY_K              =   28,
    DENG_KEY_L              =   29,
    DENG_KEY_M              =   30,
    DENG_KEY_N              =   31,
    DENG_KEY_O              =   32,
    DENG_KEY_P              =   33,
    DENG_KEY_Q              =   34,
    DENG_KEY_R              =   35,
    DENG_KEY_S              =   36,
    DENG_KEY_T              =   37,
    DENG_KEY_U              =   38,
    DENG_KEY_V              =   39,
    DENG_KEY_W              =   40,
    DENG_KEY_X              =   41,
    DENG_KEY_Y              =   42,
    DENG_KEY_Z              =   43,
    DENG_KEY_ESCAPE         =   44,
    DENG_KEY_ENTER          =   45,
    DENG_KEY_TAB            =   46,
    DENG_KEY_BACKSPACE      =   47,
    DENG_KEY_INSERT         =   48,
    DENG_KEY_DELETE         =   49,
    DENG_KEY_RIGHT          =   50,
    DENG_KEY_LEFT           =   51,
    DENG_KEY_DOWN           =   52,
    DENG_KEY_UP             =   53,
    DENG_KEY_PAGE_UP        =   54,
    DENG_KEY_PAGE_DOWN      =   55,
    DENG_KEY_HOME           =   56,
    DENG_KEY_END            =   57,
    DENG_KEY_CAPS_LOCK      =   58,
    DENG_KEY_SCROLL_LOCK    =   59,
    DENG_KEY_NUM_LOCK       =   60,
    DENG_KEY_PRINT_SCREEN   =   61,
    DENG_KEY_PAUSE          =   62,
    DENG_KEY_F1             =   63,
    DENG_KEY_F2             =   64,
    DENG_KEY_F3             =   65,
    DENG_KEY_F4             =   66,
    DENG_KEY_F5             =   67,
    DENG_KEY_F6             =   68,
    DENG_KEY_F7             =   69,
    DENG_KEY_F8             =   70,
    DENG_KEY_F9             =   71,
    DENG_KEY_F10            =   72,
    DENG_KEY_F11            =   73,
    DENG_KEY_F12            =   74,
    DENG_KEY_F13            =   75,
    DENG_KEY_F14            =   76,
    DENG_KEY_F15            =   77,
    DENG_KEY_F16            =   78,
    DENG_KEY_F17            =   79,
    DENG_KEY_F18            =   80,
    DENG_KEY_F19            =   81,
    DENG_KEY_F20            =   82,
    DENG_KEY_F21            =   83,
    DENG_KEY_F22            =   84,
    DENG_KEY_F23            =   85,
    DENG_KEY_F24            =   86,
    DENG_KEY_F25            =   87,
    DENG_KEY_NP_0           =   88,
    DENG_KEY_NP_1           =   89,
    DENG_KEY_NP_2           =   90,
    DENG_KEY_NP_3           =   91,
    DENG_KEY_NP_4           =   92,
    DENG_KEY_NP_5           =   93,
    DENG_KEY_NP_6           =   94,
    DENG_KEY_NP_7           =   95,
    DENG_KEY_NP_8           =   96,
    DENG_KEY_NP_9           =   97,
    DENG_KEY_NP_DECIMAL     =   98,
    DENG_KEY_NP_DIVIDE      =   99,
    DENG_KEY_NP_MULTIPLY    =   100,
    DENG_KEY_NP_SUBTRACT    =   101,
    DENG_KEY_NP_ADD         =   102,
    DENG_KEY_NP_ENTER       =   103,
    DENG_KEY_NP_EQUAL       =   104,
    DENG_KEY_LEFT_SHIFT     =   105,
    DENG_KEY_LEFT_CTRL      =   106,
    DENG_KEY_LEFT_ALT       =   107,
    DENG_KEY_LEFT_SUPER     =   108,
    DENG_KEY_RIGHT_SHIFT    =   109,
    DENG_KEY_RIGHT_CTRL     =   110,
    DENG_KEY_RIGHT_ALT      =   111,
    DENG_KEY_RIGHT_SUPER    =   112,
    DENG_KEY_MENU           =   113 
} deng_Key;


typedef enum deng_MouseButton {
    DENG_MOUSE_BTN_UNKNOWN = -1,
    DENG_MOUSE_BTN_1 = 0,
    DENG_MOUSE_BTN_2 = 1,
    DENG_MOUSE_BTN_3 = 2,
    DENG_MOUSE_SCROLL_DOWN = 3,
    DENG_MOUSE_SCROLL_UP = 4
} deng_MouseButton;


#ifdef __linux__
    deng_Key translateX11Key(KeySym keysym);
    deng_MouseButton translateX11Btn(unsigned int button);
#endif

#ifdef WIN32
    deng_Key translateWIN32Key(deng_ui16_t key_code);
    deng_MouseButton translateWIN32Btn(UINT msg);
#endif

#ifdef __cplusplus
}
#endif

#endif
