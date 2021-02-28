#ifndef UI_CALLBACKS_H
#define UI_CALLBACKS_H

namespace dengui {
    /* Setup dengui callbacks for use */
    void setupCallbacks (
        dengUtils::StringRasterizer *p_sr,
        dengMath::vec2<deng_ui32_t> deng_window_area
    );

    void helloCallback(WindowElement *p_elem, Events *p_ev);
    void helloCallback(DDMElement *p_elem, Events *p_ev);
    
    /* Spawn help window */
    void ddmHelpCallback(DDMElement *p_elem, Events *p_ev);
    
    /* Basic window handling callbacks */
    void minTriangleCallback(WindowElement *p_cur_elem, Events *p_ev);
    void maxTriangleCallback(WindowElement *p_cur_elem, Events *p_ev);
    void closeBtnCallback(WindowElement *p_cur_elem, Events *p_ev);
}

#endif
