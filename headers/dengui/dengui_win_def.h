#ifndef __DENGUI_WIN_DEF_H
#define __DENGUI_WIN_DEF_H

// Triangle size and position
#define DENGUI_DROP_DOWN_LIST_TRIANGLE_SIZE     0.1 // vec
#define DENGUI_DROP_DOWN_LIST_TRIANGLE          { {\
                                                    {-1.0f, -1.0f}, \
                                                    {1.0f, 0.0f}, \
                                                    {-1.0f, 1.0f} \
                                                } }


// Hierarchy Level specifiers 
#define DENGUI_CHILD_PUSH_BUTTON_LEVEL          2
#define DENGUI_CHILD_CONTAINER_LEVEL            1
#define DENGUI_DROP_DOWN_LIST_LEVEL             4
// In pixels
#define DENGUI_LIGHT_BORDER_THICKNESS           1
#define DENGUI_MEDIUM_BORDER_THICKNESS          2
#define DENGUI_HEAVY_BORDER_THICKNESS           5

// Define default colors
#define DENGUI_DEFAULT_PRIMARY_COLOR            {0.0f, 1.0f, 1.0f, 0.5f}
#define DENGUI_DEFAULT_SECONDARY_COLOR          {1.0f, 1.0f, 1.0f, 1.0f}
#define DENGUI_DEFAULT_TERTIARY_COLOR           {0.0f, 0.2f, 1.0f, 1.0f}

// Define default position and size
#define DENGUI_DEFAULT_POS                      {0.0f, 0.0f}
#define DENGUI_DEFAULT_SIZE                     {0.1f, 0.1f}

// Window flags
#define DENGUI_WINDOW_FLAG_NULL                 0x00
#define DENGUI_WINDOW_FLAG_MENUBAR              0x01
#define DENGUI_WINDOW_FLAG_NO_COLLAPSE          0x02
#define DENGUI_WINDOW_FLAG_NO_MOVE              0x04
#define DENGUI_WINDOW_FLAG_NO_TITLEBAR          0x08
#define DENGUI_WINDOW_FLAG_ALWAYS_ON_TOP        0x10
#define DENGUI_WINDOW_FLAG_NO_RESIZE            0x20
#define DENGUI_WINDOW_FLAG_NO_CLOSE             0x40

// Default window parametres
#define DENGUI_TITLEBAR_HEIGHT                  0.05f
#define DENGUI_TITLEBAR_ELEM_MARGIN             0.005f
#define DENGUI_DEFAULT_LABEL_PADDING            5.0 // px
#define DENGUI_DEFAULT_FONT_FILE                "PressStart2P-Regular"

// Main window element ids
#define DENGUI_FORM_ID                          "form"
#define DENGUI_TITLEBAR_ID                      "titlebar"
#define DENGUI_MINIMISE_TRIANGLE_ID             "min_triangle"
#define DENGUI_MAXIMISE_TRIANGLE_ID             "max_triangle"
#define DENGUI_CLOSE_BTN_ID                     "close_btn"
#define DENGUI_TITLE_ID                         "title"

// Default string buttons 
#define DENGUI_CLOSE_BTN                        "[X]"
#define DENGUI_WINDOW_LEVEL                     0

#endif
