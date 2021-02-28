#define __DENG_USE_GUI
#define __DENG_API_CORE
#include "../../headers/deng/api_core.h"

namespace dengui {
    MapEditor::MapEditor (
        EventInfo &ev_info, 
        dengUtils::StringRasterizer *p_sr,
        dengMath::vec2<deng_ui32_t> window_size
    ) {
        setupCallbacks (
            p_sr, 
            window_size
        );
        m_p_ev = new Events(ev_info);
        m_p_sr = p_sr;
        m_win_size = window_size;
        
        // Create main side window for map editor
        WindowInfo wi;
        wi.fl_b = DENG_MAP_UI_MAIN_WINDOW_FLAGS;
        wi.label = "DENG map editor";
        wi.id = "map_editor";
        wi.pc = DENG_MAP_UI_PRIMARY_COLOR;
        wi.sc = DENG_MAP_UI_SECONDARY_COLOR;
        wi.tc = DENG_MAP_UI_TERTIARY_COLOR;
        wi.pos = {0.0f, 0.0f};
        wi.size = {0.6f, 0.0f};
        wi.wt = DENG_MAP_UI_MAIN_WINDOW_TYPE;
        wi.border = DENG_MAP_UI_BORDER;

        beginWindow (
            &m_p_main_win,
            &wi,
            ev_info.deng_window_area,
            m_p_sr,
            m_p_ev,
            false
        );
        
        genTestObjects();
    }

    MapEditor::~MapEditor() {
        delete m_p_ev;
        delete m_p_main_win;
    }


    /* Create ddm element infos */
    std::vector<DropDownElementInfo> MapEditor::getDDMElemInfos() {
        /*
         * FILE
         * EDIT
         * VIEW
         * HELP
         */

        std::vector<DropDownElementInfo> out_elems(4);
        out_elems[0].label = (char*) "FILE";
        out_elems[0].is_expandable = false;
        out_elems[0].show_more_tri_color = {1.0f, 1.0f, 1.0f, 1.0f};
        out_elems[0].lmb_callback = helloCallback;
        out_elems[0].mmb_callback = NULL;
        out_elems[0].rmb_callback = NULL;
        out_elems[0].hover_callback = NULL;

        out_elems[1].label = (char*) "EDIT";
        out_elems[1].is_expandable = false;
        out_elems[1].show_more_tri_color = {1.0f, 1.0f, 1.0f, 1.0f};
        out_elems[1].lmb_callback = helloCallback;
        out_elems[1].mmb_callback = NULL;
        out_elems[1].rmb_callback = NULL;
        out_elems[1].hover_callback = NULL;

        out_elems[2].label = (char*) "VIEW";
        out_elems[2].is_expandable = false;
        out_elems[2].show_more_tri_color = {1.0f, 1.0f, 1.0f, 1.0f};
        out_elems[2].lmb_callback = helloCallback;
        out_elems[2].mmb_callback = NULL;
        out_elems[2].rmb_callback = NULL;
        out_elems[2].hover_callback = NULL;

        out_elems[3].label = (char*) "HELP";
        out_elems[3].is_expandable = false;
        out_elems[3].show_more_tri_color = {1.0f, 1.0f, 1.0f, 1.0f};
        out_elems[3].lmb_callback = ddmHelpCallback;
        out_elems[3].mmb_callback = NULL;
        out_elems[3].rmb_callback = NULL;
        out_elems[3].hover_callback = NULL;
        
        return out_elems;
    }
    

    /* Attach buttons to window for testing purposes */
    void MapEditor::genTestObjects() {
        // Generate container for push button
        ParentInfo pi = m_p_main_win->getParentInfo();
        ChildContainerInfo c_info;
        c_info.add_titlebar = true;
        c_info.titlebar_info.titlebar_height = 0.05f;
        c_info.titlebar_info.add_label = true;
        c_info.titlebar_info.label = (char*) "Hello world!";
        c_info.titlebar_info.background_color = {0.0f, 1.0f, 1.0f, 1.0f};
        c_info.titlebar_info.color = {0x00, 0x00, 0x00};

        c_info.background_color = {0.33f, 1.0f, 0.0f, 0.33f},
        c_info.border = BORDER_LIGHT;
        c_info.border_color = {0.0f, 0.0f, 0.0f, 1.0f};
        c_info.id = (char*) "container";
        c_info.origin = {0.0f, 0.0f};
        c_info.position = {0.0f, -0.5f};
        c_info.size = {1.8f, 0.5f};
        
        m_p_cont = new ChildContainer (
            pi,
            c_info,
            m_win_size,
            m_p_sr
        );

        // Generate Hello world button
        pi = m_p_cont->getContainerParentInfo();
        PushButtonInfo pb_info;
        pb_info.background_color = {0.7f, 0.7f, 0.7f, 1.0f};
        pb_info.border_color = {0.0f, 0.0f, 0.0f, 1.0f};
        pb_info.border = BORDER_LIGHT;
        pb_info.color = {0x00, 0x00, 0x00};
        pb_info.id = (char*) "Test_btn";
        pb_info.label = (char*) "Hello";
        pb_info.origin = {0.0f, 0.0f};
        pb_info.size = {0.5f, 0.2f};
        pb_info.position = {0.0f, -0.7f};
        pb_info.onClick = helloCallback;

        m_p_pb = new ChildPushButton (
            pi,
            pb_info,
            m_win_size,
            m_p_sr
        );

        m_p_cont->attachChild(m_p_pb, CHILD_ELEMENT_TYPE_PUSH_BUTTON);
        m_p_ev->pushWindowElements (
            m_p_cont->getWindowElements(), 
            NULL,
            m_p_main_win->getId(), 
            0,
            false,
            false
        );

        // Create empty sample drop down menu
        // DELETE THIS:
        deng_ObjVertData2D *active_verts = (deng_ObjVertData2D*) calloc (
            4,
            sizeof(deng_ObjVertData2D)
        );

        active_verts[0] = {-1.0f, -1.0f};
        active_verts[1] = {1.0f, -1.0f};
        active_verts[2] = {1.0f, 1.0f};
        active_verts[3] = {-1.0f, 1.0f};
        // End of DELETE THIS

        DropDownMenuInfo ddm_info;
        ddm_info.background_color = {0.7f, 0.7f, 0.7f, 1.0f};
        ddm_info.border_color = {0.0f, 0.0f, 0.0f, 1.0f};
        ddm_info.border = BORDER_LIGHT;
        ddm_info.color = {0x00, 0x00, 0x00};
        ddm_info.id = (char*) "test_menu";
        ddm_info.p_sr = m_p_sr;
        ddm_info.size = {0.3f, 0.05f};

        CursorDropDownMenu drop_menu (
            ddm_info, 
            m_win_size
        );

        std::vector<DropDownElementInfo> dde_infos = getDDMElemInfos(); 
        for(size_t i = 0; i < dde_infos.size(); i++)
            drop_menu.attachMenuElement(dde_infos[i]);

        m_p_ev->pushCursorDDM (
            "test_menu",
            drop_menu.getElems(),
            active_verts,
            4,
            false
        );
    }


    /* Event handler class instance getter */
    Events *MapEditor::getEv() { return m_p_ev; }
}
