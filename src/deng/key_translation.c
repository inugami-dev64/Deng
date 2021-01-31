#include "../../headers/deng/deng_surface_core.h"

#ifdef __linux__
/************** Xorg ************/
deng_Key translateX11Key(KeySym keysym) {
    switch (keysym)
    {   
        // Generic keys
        case XK_space:              return DENG_KEY_SPACE;
        case XK_apostrophe:         return DENG_KEY_APOSTROPHE;
        case XK_comma:              return DENG_KEY_COMMA;
        case XK_minus:              return DENG_KEY_MINUS;
        case XK_period:             return DENG_KEY_DOT;
        case XK_slash:              return DENG_KEY_SLASH;
        case XK_0:                  return DENG_KEY_0;
        case XK_1:                  return DENG_KEY_1;
        case XK_2:                  return DENG_KEY_2;
        case XK_3:                  return DENG_KEY_3;
        case XK_4:                  return DENG_KEY_4;
        case XK_5:                  return DENG_KEY_5;
        case XK_6:                  return DENG_KEY_6;
        case XK_7:                  return DENG_KEY_7;
        case XK_8:                  return DENG_KEY_8;
        case XK_9:                  return DENG_KEY_9;
        case XK_semicolon:          return DENG_KEY_SEMICOLON;
        case XK_equal:              return DENG_KEY_EQUAL;
        case XK_a:                  return DENG_KEY_A;
        case XK_b:                  return DENG_KEY_B;
        case XK_c:                  return DENG_KEY_C;
        case XK_d:                  return DENG_KEY_D;
        case XK_e:                  return DENG_KEY_E;
        case XK_f:                  return DENG_KEY_F;
        case XK_g:                  return DENG_KEY_G;
        case XK_h:                  return DENG_KEY_H;
        case XK_i:                  return DENG_KEY_I;
        case XK_j:                  return DENG_KEY_J;
        case XK_k:                  return DENG_KEY_K;
        case XK_l:                  return DENG_KEY_L;
        case XK_m:                  return DENG_KEY_M;
        case XK_n:                  return DENG_KEY_N;
        case XK_o:                  return DENG_KEY_O;
        case XK_p:                  return DENG_KEY_P;
        case XK_q:                  return DENG_KEY_Q;
        case XK_r:                  return DENG_KEY_R;
        case XK_s:                  return DENG_KEY_S;
        case XK_t:                  return DENG_KEY_T;
        case XK_u:                  return DENG_KEY_U;
        case XK_v:                  return DENG_KEY_V;
        case XK_w:                  return DENG_KEY_W;
        case XK_x:                  return DENG_KEY_X;
        case XK_y:                  return DENG_KEY_Y;
        case XK_z:                  return DENG_KEY_Z;
        
        // Function keys
        case XK_Escape:             return DENG_KEY_ESCAPE;
        case XK_Return:             return DENG_KEY_ENTER;
        case XK_Tab:                return DENG_KEY_TAB;
        case XK_BackSpace:          return DENG_KEY_BACKSPACE;
        case XK_Insert:             return DENG_KEY_INSERT;
        case XK_Delete:             return DENG_KEY_DELETE;
        case XK_Right:              return DENG_KEY_RIGHT;
        case XK_Left:               return DENG_KEY_LEFT;
        case XK_Down:               return DENG_KEY_DOWN;
        case XK_Up:                 return DENG_KEY_UP;
        case XK_Page_Up:            return DENG_KEY_PAGE_UP;
        case XK_Page_Down:          return DENG_KEY_PAGE_DOWN;
        case XK_Home:               return DENG_KEY_HOME;
        case XK_End:                return DENG_KEY_END;
        case XK_Caps_Lock:          return DENG_KEY_CAPS_LOCK;
        case XK_Scroll_Lock:        return DENG_KEY_SCROLL_LOCK;
        case XK_Num_Lock:           return DENG_KEY_NUM_LOCK;
        case XK_Print:              return DENG_KEY_PRINT_SCREEN;
        case XK_Pause:              return DENG_KEY_PAUSE;
        case XK_F1:                 return DENG_KEY_F1;
        case XK_F2:                 return DENG_KEY_F2;
        case XK_F3:                 return DENG_KEY_F3;
        case XK_F4:                 return DENG_KEY_F4;
        case XK_F5:                 return DENG_KEY_F5;
        case XK_F6:                 return DENG_KEY_F6;
        case XK_F7:                 return DENG_KEY_F7;
        case XK_F8:                 return DENG_KEY_F8;
        case XK_F9:                 return DENG_KEY_F9;
        case XK_F10:                return DENG_KEY_F10;
        case XK_F11:                return DENG_KEY_F11;
        case XK_F12:                return DENG_KEY_F12;
        case XK_F13:                return DENG_KEY_F13;
        case XK_F14:                return DENG_KEY_F14;
        case XK_F15:                return DENG_KEY_F15;
        case XK_F16:                return DENG_KEY_F16;
        case XK_F17:                return DENG_KEY_F17;
        case XK_F18:                return DENG_KEY_F18;
        case XK_F19:                return DENG_KEY_F19;
        case XK_F20:                return DENG_KEY_F20;
        case XK_F21:                return DENG_KEY_F21;
        case XK_F22:                return DENG_KEY_F22;
        case XK_F23:                return DENG_KEY_F23;
        case XK_F24:                return DENG_KEY_F24;
        case XK_F25:                return DENG_KEY_F25;
        case XK_KP_0:               return DENG_KEY_NP_0;
        case XK_KP_1:               return DENG_KEY_NP_1;
        case XK_KP_2:               return DENG_KEY_NP_2;
        case XK_KP_3:               return DENG_KEY_NP_3;
        case XK_KP_4:               return DENG_KEY_NP_4;
        case XK_KP_5:               return DENG_KEY_NP_5;
        case XK_KP_6:               return DENG_KEY_NP_6;
        case XK_KP_7:               return DENG_KEY_NP_7;
        case XK_KP_8:               return DENG_KEY_NP_8;
        case XK_KP_9:               return DENG_KEY_NP_9;
        case XK_KP_Decimal:         return DENG_KEY_NP_DECIMAL;
        case XK_KP_Divide:          return DENG_KEY_NP_DIVIDE;
        case XK_KP_Multiply:        return DENG_KEY_NP_MULTIPLY;
        case XK_KP_Add:             return DENG_KEY_NP_ADD;
        case XK_KP_Enter:           return DENG_KEY_NP_ENTER;
        case XK_KP_Equal:           return DENG_KEY_NP_EQUAL;
        case XK_Shift_L:            return DENG_KEY_LEFT_SHIFT;
        case XK_Control_L:          return DENG_KEY_LEFT_CTRL;
        case XK_Alt_L:              return DENG_KEY_LEFT_ALT;
        case XK_Super_L:            return DENG_KEY_LEFT_SUPER;
        case XK_Shift_R:            return DENG_KEY_RIGHT_SHIFT;
        case XK_Control_R:          return DENG_KEY_RIGHT_CTRL;
        case XK_Alt_R:              return DENG_KEY_RIGHT_ALT;
        case XK_Super_R:            return DENG_KEY_RIGHT_SUPER;
        case XK_Menu:               return DENG_KEY_MENU;

        default:                    return DENG_KEY_UNKNOWN;
    }
}

deng_MouseButton translateX11Btn(unsigned int button) {
    switch (button)
    {
        case Button1: return DENG_MOUSE_BTN_1;
        case Button2: return DENG_MOUSE_BTN_2;
        case Button3: return DENG_MOUSE_BTN_3;
        case Button4: return DENG_MOUSE_SCROLL_UP;
        case Button5: return DENG_MOUSE_SCROLL_DOWN;
        
        default: return DENG_MOUSE_BTN_UNKNOWN;
    }
}

#endif


/********* WIN32 API ***********/
#ifdef _WIN32

deng_Key translateWIN32Key(deng_ui16_t key_code) {
    switch(key_code) {
        case VK_SPACE:              return DENG_KEY_SPACE;
        case VK_OEM_7:              return DENG_KEY_APOSTROPHE;
        case VK_OEM_COMMA:          return DENG_KEY_COMMA;
        case VK_OEM_MINUS:          return DENG_KEY_MINUS;
        case VK_OEM_PERIOD:         return DENG_KEY_DOT;
        case VK_OEM_2:              return DENG_KEY_SLASH;
        case 0x30:                  return DENG_KEY_0;
        case 0x31:                  return DENG_KEY_1;
        case 0x32:                  return DENG_KEY_2;
        case 0x33:                  return DENG_KEY_3;
        case 0x34:                  return DENG_KEY_4;
        case 0x35:                  return DENG_KEY_5;
        case 0x36:                  return DENG_KEY_6;
        case 0x37:                  return DENG_KEY_7;
        case 0x38:                  return DENG_KEY_8;
        case 0x39:                  return DENG_KEY_9;
        case VK_OEM_1:              return DENG_KEY_SEMICOLON;
        case VK_OEM_PLUS:           return DENG_KEY_EQUAL;
        case 0x41:                  return DENG_KEY_A;
        case 0x42:                  return DENG_KEY_B;
        case 0x43:                  return DENG_KEY_C;
        case 0x44:                  return DENG_KEY_D;
        case 0x45:                  return DENG_KEY_E;
        case 0x46:                  return DENG_KEY_F;
        case 0x47:                  return DENG_KEY_G;
        case 0x48:                  return DENG_KEY_H;
        case 0x49:                  return DENG_KEY_I;
        case 0x4A:                  return DENG_KEY_J;
        case 0x4B:                  return DENG_KEY_K;
        case 0x4C:                  return DENG_KEY_L;
        case 0x4D:                  return DENG_KEY_M;
        case 0x4E:                  return DENG_KEY_N;
        case 0x4F:                  return DENG_KEY_O;
        case 0x50:                  return DENG_KEY_P;
        case 0x51:                  return DENG_KEY_Q;
        case 0x52:                  return DENG_KEY_R;
        case 0x53:                  return DENG_KEY_S;
        case 0x54:                  return DENG_KEY_T;
        case 0x55:                  return DENG_KEY_U;
        case 0x56:                  return DENG_KEY_V;
        case 0x57:                  return DENG_KEY_W;
        case 0x58:                  return DENG_KEY_X;
        case 0x59:                  return DENG_KEY_Y;
        case 0x5A:                  return DENG_KEY_Z;
        
        // Function keys
        case VK_ESCAPE:             return DENG_KEY_ESCAPE;
        case VK_RETURN:             return DENG_KEY_ENTER;
        case VK_TAB:                return DENG_KEY_TAB;
        case VK_BACK:               return DENG_KEY_BACKSPACE;
        case VK_INSERT:             return DENG_KEY_INSERT;
        case VK_DELETE:             return DENG_KEY_DELETE;
        case VK_RIGHT:              return DENG_KEY_RIGHT;
        case VK_LEFT:               return DENG_KEY_LEFT;
        case VK_DOWN:               return DENG_KEY_DOWN;
        case VK_UP:                 return DENG_KEY_UP;
        case VK_PRIOR:              return DENG_KEY_PAGE_UP;
        case VK_NEXT:               return DENG_KEY_PAGE_DOWN;
        case VK_HOME:               return DENG_KEY_HOME;
        case VK_END:                return DENG_KEY_END;
        case VK_CAPITAL:            return DENG_KEY_CAPS_LOCK;
        case VK_SCROLL:             return DENG_KEY_SCROLL_LOCK;
        case VK_NUMLOCK:            return DENG_KEY_NUM_LOCK;
        case VK_SNAPSHOT:           return DENG_KEY_PRINT_SCREEN;
        case VK_PAUSE:              return DENG_KEY_PAUSE;
        case VK_F1:                 return DENG_KEY_F1;
        case VK_F2:                 return DENG_KEY_F2;
        case VK_F3:                 return DENG_KEY_F3;
        case VK_F4:                 return DENG_KEY_F4;
        case VK_F5:                 return DENG_KEY_F5;
        case VK_F6:                 return DENG_KEY_F6;
        case VK_F7:                 return DENG_KEY_F7;
        case VK_F8:                 return DENG_KEY_F8;
        case VK_F9:                 return DENG_KEY_F9;
        case VK_F10:                return DENG_KEY_F10;
        case VK_F11:                return DENG_KEY_F11;
        case VK_F12:                return DENG_KEY_F12;
        case VK_F13:                return DENG_KEY_F13;
        case VK_F14:                return DENG_KEY_F14;
        case VK_F15:                return DENG_KEY_F15;
        case VK_F16:                return DENG_KEY_F16;
        case VK_F17:                return DENG_KEY_F17;
        case VK_F18:                return DENG_KEY_F18;
        case VK_F19:                return DENG_KEY_F19;
        case VK_F20:                return DENG_KEY_F20;
        case VK_F21:                return DENG_KEY_F21;
        case VK_F22:                return DENG_KEY_F22;
        case VK_F23:                return DENG_KEY_F23;
        case VK_F24:                return DENG_KEY_F24;
        case VK_NUMPAD0:            return DENG_KEY_NP_0;
        case VK_NUMPAD1:            return DENG_KEY_NP_1;
        case VK_NUMPAD2:            return DENG_KEY_NP_2;
        case VK_NUMPAD3:            return DENG_KEY_NP_3;
        case VK_NUMPAD4:            return DENG_KEY_NP_4;
        case VK_NUMPAD5:            return DENG_KEY_NP_5;
        case VK_NUMPAD6:            return DENG_KEY_NP_6;
        case VK_NUMPAD7:            return DENG_KEY_NP_7;
        case VK_NUMPAD8:            return DENG_KEY_NP_8;
        case VK_NUMPAD9:            return DENG_KEY_NP_9;
        case VK_DECIMAL:            return DENG_KEY_NP_DECIMAL;
        case VK_DIVIDE:             return DENG_KEY_NP_DIVIDE;
        case VK_MULTIPLY:           return DENG_KEY_NP_MULTIPLY;
        case VK_SUBTRACT:           return DENG_KEY_NP_SUBTRACT;
        case VK_ADD:                return DENG_KEY_NP_ADD;
        case VK_LSHIFT:             return DENG_KEY_LEFT_SHIFT;
        case VK_LCONTROL:           return DENG_KEY_LEFT_CTRL;
        case VK_CONTROL:            return DENG_KEY_LEFT_CTRL;
        case VK_LMENU:              return DENG_KEY_LEFT_ALT;
        case VK_LWIN:               return DENG_KEY_LEFT_SUPER;
        case VK_RSHIFT:             return DENG_KEY_RIGHT_SHIFT;
        case VK_RCONTROL:           return DENG_KEY_RIGHT_CTRL;
        case VK_RMENU:              return DENG_KEY_RIGHT_ALT;
        case VK_RWIN:               return DENG_KEY_RIGHT_SUPER;
        case VK_MENU:               return DENG_KEY_MENU;
        default:                    return DENG_KEY_UNKNOWN;
    }
}

deng_MouseButton translateWIN32Btn(UINT msg) {
    switch(msg) {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:          return DENG_MOUSE_BTN_1;

        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:          return DENG_MOUSE_BTN_2;

        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:          return DENG_MOUSE_BTN_3;
        
        default:                    return DENG_MOUSE_BTN_UNKNOWN;   
    }
}

#endif
