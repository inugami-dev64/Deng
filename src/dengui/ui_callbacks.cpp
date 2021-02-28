#include "../../headers/deng/api_core.h"

namespace dengui {
    static dengUtils::StringRasterizer *__p_sr;
    static dengMath::vec2<deng_ui32_t> __deng_win_area;
    std::vector<Window*> __callback_windows; 


    /* Setup all global variables needed for callbacks */
    void setupCallbacks (
        dengUtils::StringRasterizer *p_sr,
        dengMath::vec2<deng_ui32_t> deng_win_area
    ) {
        __p_sr = p_sr;
        __deng_win_area = deng_win_area;
    }


    /* Testing callback for adding new window elements */
    void helloCallback (
        WindowElement *p_elem, 
        Events *p_ev
    ) {
        printf("Hello world!\n");
    }

    /* Test callback for drop down menus */
    void helloCallback (
        DDMElement *p_elem, 
        Events *p_ev
    ) {
        printf("Hello world!\n");
    }


    /* Trigger help window generation */
    void ddmHelpCallback (
        DDMElement *p_elem, 
        Events *p_ev
    ) {
        WindowInfo wi;
        wi.border = BORDER_LIGHT;
        wi.fl_b = DENGUI_WINDOW_FLAG_NO_COLLAPSE;
        wi.id = "help_form";
        wi.label = "Help";
        wi.pc = DENGUI_DEFAULT_PRIMARY_COLOR;
        wi.sc = DENGUI_DEFAULT_SECONDARY_COLOR;
        wi.tc = DENGUI_DEFAULT_TERTIARY_COLOR;
        wi.pos = {0.0f, 0.0f};
        wi.size = {0.5f, 0.5f};
        
        Window *win;
        beginWindow (
            &win,
            &wi,
            __deng_win_area,
            __p_sr,
            p_ev,
            true
        );
        
        __callback_windows.push_back(win);
        LOG("Window make end");
    }

    
    /*************************************************/
    /*************************************************/
    /******** Base window handling callbacks *********/
    /*************************************************/
    /*************************************************/
    void minTriangleCallback (
        WindowElement *p_cur_elem,
        Events *p_ev
    ) {
        size_t l_index;
        std::vector<deng_Asset> *p_assets;
        std::vector<WindowElement> *p_elems;
        p_elems = p_ev->getWinElems(p_cur_elem->parent_id);
        if(!p_elems)
            return;
        
        dengMath::vec2<deng_ui32_t> asset_bounds;

        p_ev->frame_upd.lockAssets();
        p_ev->toggleWindow(p_cur_elem->parent_id);
        p_assets = p_ev->getAssets();
        asset_bounds = p_ev->getWindowAssetBounds(p_cur_elem->parent_id);    

        // Unhide maximise triangle element

        for(l_index = 0; l_index < p_elems->size(); l_index++) {
            if(p_elems->at(l_index).child_id == DENGUI_MAXIMISE_TRIANGLE_ID) {
                p_assets->at(l_index + asset_bounds.first).is_shown = true;
                break;
            }
        }
        
        p_ev->frame_upd.unlockAssets();

        p_ev->frame_upd.lockFrame();
        p_ev->frame_upd.updateCmdBuffers();
        p_ev->frame_upd.unlockFrame();
    }


    void maxTriangleCallback (
        WindowElement *p_cur_elem,
        Events *p_ev
    ) {
        size_t l_index;
        std::vector<deng_Asset> *p_assets;
        std::vector<WindowElement> *p_elems;
        p_elems = p_ev->getWinElems(p_cur_elem->parent_id);
        if(!p_elems) return;
        
        dengMath::vec2<deng_ui32_t> asset_bounds;

        p_ev->frame_upd.lockAssets();
        p_ev->toggleWindow(p_cur_elem->parent_id);
        p_assets = p_ev->getAssets();
        asset_bounds = p_ev->getWindowAssetBounds(p_cur_elem->parent_id);    

        // Unhide maximise triangle element
        for(l_index = 0; l_index < p_elems->size(); l_index++) {
            if(p_elems->at(l_index).child_id == DENGUI_MAXIMISE_TRIANGLE_ID) {
                p_assets->at(l_index + asset_bounds.first).is_shown = false;
                break;
            }
        }
        
        p_ev->frame_upd.unlockAssets();

        p_ev->frame_upd.lockFrame();
        p_ev->frame_upd.updateCmdBuffers();
        p_ev->frame_upd.unlockFrame();
    }


    void closeBtnCallback (
        WindowElement *p_cur_elem,
        Events *p_ev
    ) {
        /* Remove all window elements */
        p_ev->frame_upd.lockFrame();
        p_ev->windowAssetRmSync(p_cur_elem->parent_id);
        p_ev->checkWindowElements();
        p_ev->frame_upd.reallocBuffer();
        p_ev->frame_upd.updateTexDS(false, NULL);
        p_ev->frame_upd.updateCmdBuffers();
        p_ev->frame_upd.unlockFrame();
    }
}
