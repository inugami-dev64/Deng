#ifndef DENG_SURFACE_CORE_H
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

#ifdef __linux__
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #include <X11/Xos.h>
    #include <X11/Xcursor/Xcursor.h>
    #include <X11/XKBlib.h>
    #include <X11/keysym.h>
#endif

#include "deng_key_definitions.h"
#include "deng_surface_window.h"

#endif