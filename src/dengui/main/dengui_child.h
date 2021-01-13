#ifndef DENGUI_CHILD_H
#define DENGUI_CHILD_H

namespace dengui {
     /* Child element alignment mode */
    enum ChildAlignment {
        CHILD_ELEMENT_ALIGNMENT_HORIZONTAL  = 0,
        CHILD_ELEMENT_ALIGNMENT_LIST        = 1,
        CHILD_ELEMENT_ALIGNMENT_VERTICES    = 2,
        CHILD_ELEMENT_ALIGNMENT_FREE        = 3
    };

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
        dengMath::vec2<deng_vec_t> size;
        ChildAlignment alignment;
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
}

#endif