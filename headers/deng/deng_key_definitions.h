#ifndef DENG_KEY_DEFINITIONS_H
#define DENG_KEY_DEFINITIONS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum deng_MouseMode {
    DENG_MOUSE_MODE_VIRTUAL         = 0,
    DENG_MOUSE_MODE_CURSOR_VISIBLE  = 1
} deng_MouseMode;

typedef enum deng_Key {
    DENG_KEY_UNKNOWN        =   -1,
    DENG_KEY_SPACE          =   32,
    DENG_KEY_APOSTROPHE     =   39,
    DENG_KEY_COMMA          =   44,
    DENG_KEY_MINUS          =   45,
    DENG_KEY_DOT            =   46,
    DENG_KEY_SLASH          =   47,
    DENG_KEY_0              =   48,
    DENG_KEY_1              =   49,
    DENG_KEY_2              =   50,
    DENG_KEY_3              =   51,
    DENG_KEY_4              =   52,
    DENG_KEY_5              =   53,
    DENG_KEY_6              =   54,
    DENG_KEY_7              =   55,
    DENG_KEY_8              =   56,
    DENG_KEY_9              =   57,
    DENG_KEY_SEMICOLON      =   59,
    DENG_KEY_EQUAL          =   61,
    DENG_KEY_A              =   65,
    DENG_KEY_B              =   66,
    DENG_KEY_C              =   67,
    DENG_KEY_D              =   68,
    DENG_KEY_E              =   69,
    DENG_KEY_F              =   70,
    DENG_KEY_G              =   71,
    DENG_KEY_H              =   72,
    DENG_KEY_I              =   73,
    DENG_KEY_J              =   74, 
    DENG_KEY_K              =   75,
    DENG_KEY_L              =   76,
    DENG_KEY_M              =   77,
    DENG_KEY_N              =   78,
    DENG_KEY_O              =   79,
    DENG_KEY_P              =   80,
    DENG_KEY_Q              =   81,
    DENG_KEY_R              =   82,
    DENG_KEY_S              =   83,
    DENG_KEY_T              =   84,
    DENG_KEY_U              =   85,
    DENG_KEY_V              =   86,
    DENG_KEY_W              =   87,
    DENG_KEY_X              =   88,
    DENG_KEY_Y              =   89,
    DENG_KEY_Z              =   90,

    // Function keys
    DENG_KEY_ESCAPE         =   256,
    DENG_KEY_ENTER          =   257,
    DENG_KEY_TAB            =   258,
    DENG_KEY_BACKSPACE      =   259,
    DENG_KEY_INSERT         =   260,
    DENG_KEY_DELETE         =   261,
    DENG_KEY_RIGHT          =   262,
    DENG_KEY_LEFT           =   263,
    DENG_KEY_DOWN           =   264,
    DENG_KEY_UP             =   265,
    DENG_KEY_PAGE_UP        =   266,
    DENG_KEY_PAGE_DOWN      =   267,
    DENG_KEY_HOME           =   268,
    DENG_KEY_END            =   269,
    DENG_KEY_CAPS_LOCK      =   280,
    DENG_KEY_SCROLL_LOCK    =   281,
    DENG_KEY_NUM_LOCK       =   282,
    DENG_KEY_PRINT_SCREEN   =   283,
    DENG_KEY_PAUSE          =   284,
    DENG_KEY_F1             =   290,
    DENG_KEY_F2             =   291,
    DENG_KEY_F3             =   292,
    DENG_KEY_F4             =   293,
    DENG_KEY_F5             =   294,
    DENG_KEY_F6             =   295,
    DENG_KEY_F7             =   296,
    DENG_KEY_F8             =   297,
    DENG_KEY_F9             =   298,
    DENG_KEY_F10            =   299,
    DENG_KEY_F11            =   300,
    DENG_KEY_F12            =   301,
    DENG_KEY_F13            =   302,
    DENG_KEY_F14            =   303,
    DENG_KEY_F15            =   304,
    DENG_KEY_F16            =   305,
    DENG_KEY_F17            =   306,
    DENG_KEY_F18            =   307,
    DENG_KEY_F19            =   308,
    DENG_KEY_F20            =   309,
    DENG_KEY_F21            =   310,
    DENG_KEY_F22            =   311,
    DENG_KEY_F23            =   312,
    DENG_KEY_F24            =   313,
    DENG_KEY_F25            =   314,
    DENG_KEY_NP_0           =   320,
    DENG_KEY_NP_1           =   321,
    DENG_KEY_NP_2           =   322,
    DENG_KEY_NP_3           =   323,
    DENG_KEY_NP_4           =   324,
    DENG_KEY_NP_5           =   325,
    DENG_KEY_NP_6           =   326,
    DENG_KEY_NP_7           =   327,
    DENG_KEY_NP_8           =   328,
    DENG_KEY_NP_9           =   329,
    DENG_KEY_NP_DECIMAL     =   330,
    DENG_KEY_NP_DIVIDE      =   331,
    DENG_KEY_NP_MULTIPLY    =   332,
    DENG_KEY_NP_SUBTRACT    =   333,
    DENG_KEY_NP_ADD         =   334,
    DENG_KEY_NP_ENTER       =   335,
    DENG_KEY_NP_EQUAL       =   336,
    DENG_KEY_LEFT_SHIFT     =   340,
    DENG_KEY_LEFT_CTRL      =   341,
    DENG_KEY_LEFT_ALT       =   342,
    DENG_KEY_LEFT_SUPER     =   343,
    DENG_KEY_RIGHT_SHIFT    =   344,
    DENG_KEY_RIGHT_CTRL     =   345,
    DENG_KEY_RIGHT_ALT      =   346,
    DENG_KEY_RIGHT_SUPER    =   347,
    DENG_KEY_MENU           =   348
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
