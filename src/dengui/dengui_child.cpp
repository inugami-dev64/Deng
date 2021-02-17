#include "../../headers/deng/api_core.h"
#include <string>

namespace dengui {

    /*********************************************/
    /*********************************************/
    /******** Push button class methods **********/
    /*********************************************/
    /*********************************************/

    /* Rasterize label text and calculate vertices and indices for button */
    ChildPushButton::ChildPushButton (
        ParentInfo parent_info,
        PushButtonInfo &btn_info,
        dengMath::vec2<deng_ui32_t> window_size,
        dengUtils::StringRasterizer *p_sr
    ) {
        m_info = btn_info;
        m_p_sr = p_sr;
        m_parent_info = parent_info;
        
        for(size_t i = 0; i < strlen(m_info.id); i++) {
            if(m_info.id[i] == 0x20)
                m_info.id[i] = '_';
        }

        // Generate vertices
        dengUtils::RectangleGenerator::generateUnmappedRelRec (
            m_info.position,
            m_info.size,
            false,
            m_info.origin,
            m_parent_info.vert,
            {
                m_info.background_color.col_r,
                m_info.background_color.col_g,
                m_info.background_color.col_b,
                m_info.background_color.col_a
            },
            m_vert
        );

        m_indices.insert (
            m_indices.end(), 
            {0, 1, 2, 2, 3, 0}
        );

        // Rasterise button label
        m_label = m_p_sr->renderLabel (
            {
                m_parent_info.vert[1].vert_data.vert_x - m_parent_info.vert[0].vert_data.vert_x,
                m_parent_info.vert[3].vert_data.vert_y - m_parent_info.vert[0].vert_data.vert_y
            },
            DENGUI_PUSH_BUTTON_LABEL_PADDING, 
            {
                m_vert[0].vert_data.vert_x,
                m_vert[0].vert_data.vert_y
            },
            m_info.size, 
            m_info.origin,
            m_info.color,
            DENGUI_DEFAULT_FONT_FILE,
            m_info.label
        );

        dengUtils::BorderGenerator::generateBorders (
            m_vert,
            false, 
            0, 
            m_indices,
            {
                m_info.border_color.col_r,
                m_info.border_color.col_g,
                m_info.border_color.col_b,
                m_info.border_color.col_a
            }, 
            window_size,
            m_info.border
        );
    }


    /* Create window elements for event class */
    std::vector<WindowElement> ChildPushButton::getWindowElements() {
        std::vector<WindowElement> w_elems(2);

        // Main push button element
        w_elems[0].child_id = std::string(m_info.id);
        w_elems[0].color_mode = ELEMENT_COLOR_MODE_UNMAPPED;
        w_elems[0].parent_id = std::string(m_parent_info.id);
        w_elems[0].is_visible = true;
        w_elems[0].is_interactive = true;
        w_elems[0].unmapped_vert = m_vert;
        w_elems[0].col_vert_bounds = {0, 4};
        w_elems[0].indices = m_indices;
        w_elems[0].onLMBClickFunc = m_info.onClick;
        w_elems[0].onMMBClickFunc = NULL;
        w_elems[0].onRMBClickFunc = NULL;
        w_elems[0].onScrDownFunc = NULL;
        w_elems[0].onScrUpFunc = NULL;

        // Label element
        w_elems[1].child_id = std::string(m_info.id) + 
                              "#" + 
                              std::string(m_label.text);
        w_elems[1].color_mode = ELEMENT_COLOR_MODE_TEXTURE_MAPPED;
        w_elems[1].parent_id = std::string(m_parent_info.id);
        w_elems[1].is_visible = true;
        w_elems[1].is_interactive = true;
        w_elems[1].mapped_vert.insert (
            w_elems[1].mapped_vert.end(),
            m_label.vert_pos.begin(),
            m_label.vert_pos.end()
        );
        w_elems[1].col_vert_bounds = {0, 4};
        w_elems[1].indices.insert (
            w_elems[1].indices.end(),
            m_label.vert_indices.begin(),
            m_label.vert_indices.end()
        );
        w_elems[1].texture = m_label.tex_data;
        w_elems[1].tex_box = m_label.box_size;
        w_elems[1].onLMBClickFunc = m_info.onClick;
        w_elems[1].onMMBClickFunc = NULL;
        w_elems[1].onRMBClickFunc = NULL;
        w_elems[1].onScrDownFunc = NULL;
        w_elems[1].onScrUpFunc = NULL;

        return w_elems;
    }


    /* Return full ID of a push button */
    char *ChildPushButton::getParentID() { return m_parent_info.id; } 

    
    /****************************************/
    /****************************************/
    /******* Container class methods ********/
    /****************************************/
    /****************************************/

    ChildContainer::ChildContainer (
        ParentInfo parent_info,
        ChildContainerInfo info,
        dengMath::vec2<deng_ui32_t> window_size,
        dengUtils::StringRasterizer *p_sr
    ) {
        m_info = info;
        m_parent_info = parent_info;
        m_window_size = window_size;
        m_p_sr = p_sr;

        for(size_t i = 0; i < strlen(m_info.id); i++) {
            if(m_info.id[i] == 0x20)
                m_info.id[i] = '_';
        }

        // Generate container vertices 
        dengUtils::RectangleGenerator::generateUnmappedRelRec (
            m_info.position, 
            m_info.size,
            false,
            m_info.origin,
            m_parent_info.vert,
            {
                m_info.background_color.col_r,
                m_info.background_color.col_g,
                m_info.background_color.col_b,
                m_info.background_color.col_a
            },
            m_vert
        );

        printf (
            "VERTS_1: {%f, %f}, {%f, %f}, {%f, %f}, {%f, %f}\n",
            m_vert[0].vert_data.vert_x,
            m_vert[0].vert_data.vert_y,
            m_vert[1].vert_data.vert_x,
            m_vert[1].vert_data.vert_y,
            m_vert[2].vert_data.vert_x,
            m_vert[2].vert_data.vert_y,
            m_vert[3].vert_data.vert_x,
            m_vert[3].vert_data.vert_y
        );

        m_indices.insert (
            m_indices.end(),
            {0, 1, 2, 2, 3, 0}
        ); 

        // Generate container borders
        dengUtils::BorderGenerator::generateBorders (
            m_vert, 
            false, 
            0, 
            m_indices, 
            {
                m_info.border_color.col_r,
                m_info.border_color.col_g,
                m_info.border_color.col_b,
                m_info.border_color.col_a
            },
            m_window_size, 
            m_info.border
        );

        m_cont_vert_bounds = {0, (deng_ui32_t) m_vert.size()};
        printf (
            "VERT_BOUNDS: %d, %d\n", 
            m_cont_vert_bounds.first,
            m_cont_vert_bounds.second
        );
        m_cont_ind_bounds = {0, (deng_ui32_t) m_indices.size()};
        printf (
            "IND_BOUNDS: %d, %d\n", 
            m_cont_ind_bounds.first,
            m_cont_ind_bounds.second
        );

        // Check if titlebar is specified
        if(m_info.add_titlebar)
            mkTitlebar();
    }


    /* Create titlebar for container */
    void ChildContainer::mkTitlebar() {
        // Generate titlebar vertices
        deng_ui32_t vert_offset = (deng_ui32_t) m_vert.size();
        deng_ui32_t ind_offset = (deng_ui32_t) m_indices.size();

        deng_vec_t rel_height = dengMath::Conversion::findRelSize (
            m_vert[3].vert_data.vert_y - m_vert[0].vert_data.vert_y, 
            m_info.titlebar_info.titlebar_height
        );
        
        dengUtils::RectangleGenerator::generateUnmappedRelRec (
            {-1.0f, -1.0f},
            {2.0f, rel_height},
            false,
            {-1.0f, 1.0f},
            m_vert.data(),
            {
                m_info.titlebar_info.background_color.col_r,
                m_info.titlebar_info.background_color.col_g,
                m_info.titlebar_info.background_color.col_b,
                m_info.titlebar_info.background_color.col_a
            },
            m_vert
        );

        m_indices.insert (
            m_indices.end(),
            {0, 1, 2, 2, 3, 0}
        );

        dengUtils::BorderGenerator::generateBorders (
            m_vert,
            false,
            vert_offset,
            m_indices,
            {
                m_info.border_color.col_r,
                m_info.border_color.col_g,
                m_info.border_color.col_b,
                m_info.border_color.col_a
            },
            m_window_size,
            m_info.border
        );

        m_titlebar_vert_bounds = {vert_offset, (deng_ui32_t) m_vert.size()};
        m_titlebar_ind_bounds = {ind_offset, (deng_ui32_t) m_indices.size()};

        LOG (
            "TITLEBAR_VERT_BOUNDS: " + 
            std::to_string(m_titlebar_vert_bounds.first) +
            ", " +
            std::to_string(m_titlebar_vert_bounds.second)
        );

        LOG (
            "TITLEBAR_IND_BOUNDS: " + 
            std::to_string(m_titlebar_ind_bounds.first) +
            ", " +
            std::to_string(m_titlebar_ind_bounds.second)
        );

        // Generate title label if needed
        if(m_info.titlebar_info.add_label) {
            m_label = m_p_sr->renderLabel (
                {
                    m_parent_info.vert[1].vert_data.vert_x - m_parent_info.vert[0].vert_data.vert_x,
                    m_parent_info.vert[3].vert_data.vert_y - m_parent_info.vert[0].vert_data.vert_y
                },
                DENGUI_PUSH_BUTTON_LABEL_PADDING,
                {
                    m_vert[m_titlebar_vert_bounds.first].vert_data.vert_x,
                    m_vert[m_titlebar_vert_bounds.first].vert_data.vert_y
                },
                {
                    m_info.size.first,
                    m_info.titlebar_info.titlebar_height
                },
                m_info.origin,
                m_info.titlebar_info.color,
                DENGUI_DEFAULT_FONT_FILE,
                m_info.titlebar_info.label
            );
        }
    }


    /* Get info about titlebar type parent */
    ParentInfo ChildContainer::getTitlebarParentInfo() {
        ParentInfo out_pi;
        out_pi.id = (char*) calloc (
            strlen(m_info.id) + strlen(m_parent_info.id) + 11,
            sizeof(char)
        );

        sprintf (
            out_pi.id,
            "%s#%s#titlebar",
            m_parent_info.id,
            m_info.id
        );

        out_pi.vert = &m_vert[m_titlebar_vert_bounds.first];

        return out_pi;
    }


    /* Get info about main container type parent */
    ParentInfo ChildContainer::getContainerParentInfo() {
        ParentInfo out_pi;
        out_pi.id = (char*) calloc (
            strlen(m_parent_info.id) + strlen(m_info.id) + 2,   
            sizeof(char)
        );

        sprintf (
            out_pi.id,
            "%s#%s",
            m_parent_info.id,
            m_info.id
        );

        out_pi.vert = &m_vert[m_cont_vert_bounds.first];
        return out_pi;
    }

    
    /* Attach child object to container 
     * Titlebar elements should be specified with following id: 
     * "[PARENT]#[CONTAINER]#titlebar#[ID]"
     * Container elements should be specified with following id:
     * "[PARENT]#[CONTAINER]#[ID]"
     */
    void ChildContainer::attachChild (
        void *elem,
        ChildElementType elem_type
    ) {
        char *child_parent_id = NULL;    
        switch(elem_type) 
        {
        case CHILD_ELEMENT_TYPE_PUSH_BUTTON:
            child_parent_id = ((ChildPushButton*) elem)->getParentID();
            break;

        case CHILD_ELEMENT_TYPE_CONTAINER:
            child_parent_id = ((ChildContainer*) elem)->getParentID();
            break;

        default:
            break;
        }

        if(!child_parent_id) {
            WARNME("(ChildContainer::attachChild()) No parent specified");
            return;
        }

        char *full_id = (char*) calloc (
            strlen(m_info.id) + strlen(m_parent_info.id) + 2,
            sizeof(char)
        );
        
        sprintf (
            full_id,
            "%s#%s",
            m_parent_info.id,
            m_info.id
        );

        if(!strcmp(child_parent_id, full_id))
            m_cont_child_elems.push_back(std::make_pair(elem, elem_type));

        else if (
            strcmp(child_parent_id, full_id) > 0 && 
            strcmp(child_parent_id + strlen(full_id), "titlebar")
        ) m_titlebar_child_elems.push_back(std::make_pair(elem, elem_type));
    }


    /* Return parent id of child container */
    char *ChildContainer::getParentID() { return m_parent_info.id; }


    /* Return all Window elements related to the container */
    std::vector<WindowElement> ChildContainer::getWindowElements() {
        std::vector<WindowElement> out_vec(1);
        std::vector<VERT_UNMAPPED_2D> vert_vec (
            m_vert.cbegin() + m_cont_vert_bounds.first,
            m_vert.cbegin() + m_cont_vert_bounds.second
        );

        std::vector<deng_ui32_t> ind_vec (
            m_indices.cbegin() + m_cont_ind_bounds.first,
            m_indices.cbegin() + m_cont_ind_bounds.second
        );

        out_vec[0].child_id = std::string(m_info.id);
        out_vec[0].color_mode = ELEMENT_COLOR_MODE_UNMAPPED;
        out_vec[0].parent_id = std::string(m_parent_info.id);
        out_vec[0].is_visible = true;
        out_vec[0].is_interactive = false;
        out_vec[0].unmapped_vert = vert_vec;
        out_vec[0].col_vert_bounds = {0, 0};
        out_vec[0].indices = ind_vec;
        out_vec[0].onLMBClickFunc = NULL;
        out_vec[0].onMMBClickFunc = NULL;
        out_vec[0].onRMBClickFunc = NULL;
        out_vec[0].onScrUpFunc = NULL;
        out_vec[0].onScrDownFunc = NULL;
            
        std::vector<WindowElement> tmp_elems;
        for(deng_ui32_t l_index = 0; l_index < m_cont_child_elems.size(); l_index++) {
            switch(m_cont_child_elems[l_index].second) 
            {
            case CHILD_ELEMENT_TYPE_PUSH_BUTTON:
                tmp_elems = ((ChildPushButton*) m_cont_child_elems[l_index].first)->getWindowElements();
                break;

            case CHILD_ELEMENT_TYPE_CONTAINER:
                tmp_elems = ((ChildContainer*) m_cont_child_elems[l_index].first)->getWindowElements();
                break;

            default:
                break;
            }

            out_vec.insert (
                out_vec.end(),
                tmp_elems.begin(),
                tmp_elems.end()
            );
        }
        
        // Add titlebar elements
        if(m_info.add_titlebar) {
            vert_vec = std::vector<VERT_UNMAPPED_2D> (
                m_vert.cbegin() + m_titlebar_vert_bounds.first,
                m_vert.cbegin() + m_titlebar_vert_bounds.second
            );

            ind_vec = std::vector<deng_ui32_t> (
                m_indices.cbegin() + m_titlebar_ind_bounds.first,
                m_indices.cbegin() + m_titlebar_ind_bounds.second
            );

            out_vec.resize(out_vec.size() + 1);
            out_vec[out_vec.size() - 1].child_id = "titlebar";
            out_vec[out_vec.size() - 1].color_mode = ELEMENT_COLOR_MODE_UNMAPPED;
            out_vec[out_vec.size() - 1].parent_id = std::string(m_parent_info.id) + "#" + std::string(m_info.id);
            out_vec[out_vec.size() - 1].is_visible = true;
            out_vec[out_vec.size() - 1].is_interactive = false;
            out_vec[out_vec.size() - 1].unmapped_vert = vert_vec;
            out_vec[out_vec.size() - 1].col_vert_bounds = {0, 0};
            out_vec[out_vec.size() - 1].indices = ind_vec;
            out_vec[out_vec.size() - 1].onLMBClickFunc = NULL;
            out_vec[out_vec.size() - 1].onMMBClickFunc = NULL;
            out_vec[out_vec.size() - 1].onRMBClickFunc = NULL;
            out_vec[out_vec.size() - 1].onScrUpFunc = NULL;
            out_vec[out_vec.size() - 1].onScrDownFunc = NULL;

            if(m_info.titlebar_info.add_label) {
                out_vec.resize(1 + out_vec.size());
                out_vec[out_vec.size() - 1].child_id = m_info.titlebar_info.label;
                out_vec[out_vec.size() - 1].color_mode = ELEMENT_COLOR_MODE_TEXTURE_MAPPED;
                out_vec[out_vec.size() - 1].parent_id = std::string(m_parent_info.id) + "#" + std::string(m_info.id) + "#titlebar";
                out_vec[out_vec.size() - 1].is_visible = true;
                out_vec[out_vec.size() - 1].is_interactive = false;
                out_vec[out_vec.size() - 1].mapped_vert.insert (
                    out_vec[out_vec.size() - 1].mapped_vert.end(),
                    m_label.vert_pos.begin(),
                    m_label.vert_pos.end()
                );

                out_vec[out_vec.size() - 1].texture = m_label.tex_data;
                out_vec[out_vec.size() - 1].indices.insert (
                    out_vec[out_vec.size() - 1].indices.end(),   
                    m_label.vert_indices.begin(),
                    m_label.vert_indices.end()
                );
                out_vec[out_vec.size() - 1].tex_box = m_label.box_size;
                out_vec[out_vec.size() - 1].col_vert_bounds = {0, 0};
                out_vec[out_vec.size() - 1].onLMBClickFunc = NULL;
                out_vec[out_vec.size() - 1].onMMBClickFunc = NULL;
                out_vec[out_vec.size() - 1].onRMBClickFunc = NULL;
                out_vec[out_vec.size() - 1].onScrUpFunc = NULL;
                out_vec[out_vec.size() - 1].onScrDownFunc = NULL;
            }

            for(deng_ui32_t l_index = 0; l_index < m_titlebar_child_elems.size(); l_index++) {
                switch(m_cont_child_elems[l_index].second) 
                {
                case CHILD_ELEMENT_TYPE_PUSH_BUTTON:
                    tmp_elems = ((ChildPushButton*) m_cont_child_elems[l_index].first)->getWindowElements();
                    break;

                case CHILD_ELEMENT_TYPE_CONTAINER:
                    tmp_elems = ((ChildContainer*) m_cont_child_elems[l_index].first)->getWindowElements();
                    break;

                default:
                    break;
                }

                out_vec.insert (
                    out_vec.end(),
                    tmp_elems.begin(),
                    tmp_elems.end()
                );
            }
        }

        return out_vec;
    }
}
