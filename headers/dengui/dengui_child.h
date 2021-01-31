#ifndef DENGUI_CHILD_H
#define DENGUI_CHILD_H

namespace dengui {
    /* Child window overlapping mode */
    enum ChildOverlapMode {
        CHILD_OVERLAP_HIDE      = 0,
        CHILD_OVERLAP_SCROLL    = 1,
        CHILD_OVERLAP_NONE      = 2
    };

    /* Information about the child object */
    struct ChildInfo {
        // Relative position of window object
        dengMath::vec2<deng_vec_t> position;
        std::string id;
        dengMath::vec2<deng_vec_t> size;
        ChildOverlapMode overlap;  
    };

    /**********************************************/
    /**********************************************/
    /*************** CHILD ELEMENTS ***************/
    /**********************************************/
    /**********************************************/
    struct PushButtonInfo {
        dengMath::vec2<deng_vec_t> position;
        dengMath::vec2<deng_vec_t> size;
        void (*onClick)(WindowElement*, Events*);
        char *label;
    };


    class ChildWindow {
    private:
        ChildInfo m_info;

    public:
        ChildWindow(ChildInfo &child_info);
    };
}

#endif