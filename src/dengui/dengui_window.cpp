#include <string>
#define __DENGUI_USAGE 1
#include "../../headers/deng/api_core.h"

namespace dengui {
    /* Create new window instance */
    void beginWindow (
        Window **pp_window,
        WindowInfo *p_wi,
        dengMath::vec2<deng_ui32_t> draw_bounds,
        dengUtils::StringRasterizer *p_sr,
        Events *p_ev,
        deng_bool_t realloc_res
    ) {
        (*pp_window) = new Window (
            p_wi->wt, 
            p_wi->id, 
            p_sr,
            draw_bounds
        );
        (*pp_window)->setPos(p_wi->pos);
        (*pp_window)->setSize(p_wi->size);
        (*pp_window)->setPC(&p_wi->pc);
        (*pp_window)->setSC(&p_wi->sc);
        (*pp_window)->setTC(&p_wi->tc);

        std::vector<WindowElement> elems;
        (*pp_window)->makeWindow (
            p_wi, 
            {
                (deng_ui32_t) draw_bounds.first, 
                (deng_ui32_t) draw_bounds.second
            }
        );
        elems = (*pp_window)->getWindowElements();
        p_ev->pushWindowElements (
            elems, 
            false
        );
    }


    /* Start new DENGUI events thread */
    void beginEventHandler (
        Events *p_ev,
        std::vector<deng_Asset> *p_assets,
        std::vector<deng::TextureImageData> *p_textures,
        std::mutex *p_asset_mut,
        deng::DrawCaller *p_dc,
        deng::ResourceAllocator *p_ra,
        deng::DescriptorCreator *p_desc_c,
        VkDevice device,
        VkQueue g_queue,
        VkRenderPass renderpass,
        VkExtent2D extent,
        dengMath::vec4<float> background,
        dengMath::vec2<deng_ui32_t> draw_area
    ) {
        EventInfo ev_info;
        ev_info.background = background;
        ev_info.deng_window_area = draw_area;
        ev_info.device = device;
        ev_info.extent = extent;
        ev_info.g_queue = g_queue;
        ev_info.p_assets = p_assets;
        ev_info.p_dc = p_dc;
        ev_info.p_res_mut = p_asset_mut;
        ev_info.p_textures = p_textures;
        ev_info.renderpass = renderpass;
        p_ev = new Events(ev_info);
    }

    
    /* Initialise default values to windowinfo */
    WindowInfo::WindowInfo() {
        pc = DENGUI_DEFAULT_PRIMARY_COLOR;
        sc = DENGUI_DEFAULT_SECONDARY_COLOR;
        tc = DENGUI_DEFAULT_TERTIARY_COLOR;
        id = "Hello world!";
        wt = WINDOW_TYPE_FLOATING;
        fl_b = DENGUI_WINDOW_FLAG_NULL;
        pos = DENGUI_DEFAULT_POS;
        size = DENGUI_DEFAULT_SIZE;
    }

    
    /* Window constructor */
    Window::Window (
        WindowType wt,
        std::string id,
        dengUtils::StringRasterizer *p_sr,
        dengMath::vec2<deng_ui32_t> window_bounds
    ) : BaseWindowShapes(p_sr, window_bounds) {
        m_wt = wt;
        m_id = id;
        m_p_sr = p_sr;
    }
    

    BaseWindowShapes::BaseWindowShapes (
        dengUtils::StringRasterizer *p_sr,
        dengMath::vec2<deng_ui32_t> &window_bounds
    ) {
        m_p_sr = p_sr;
        m_window_bounds = window_bounds;
    }


    /* Add text element to window */
    void BaseWindowShapes::addRelText (
        dengMath::vec2<deng_vec_t> pos,
        deng_vec_t text_size,
        dengMath::vec2<deng_ui32_t> draw_bounds,
        RectangleOrigin rec_origin,
        dengUtils::BitmapStr text,
        dengMath::vec3<unsigned char> color,
        VERT_UNMAPPED_2D *form_vert,
        std::vector<VERT_MAPPED_2D> &vert,
        std::vector<deng_ui32_t> &indices,
        std::vector<deng_ui8_t> &tex,
        dengMath::vec2<deng_i32_t> &tex_size
    ) {
        m_p_sr->newVecStr (
            text,
            DENGUI_DEFAULT_FONT_FILE,
            text_size,
            color,
            {0.0f, 0.0f},
            {-1.0f, -1.0f}
        );
        
        dengMath::vec2<deng_vec_t> rec_size;
        rec_size.first = text.vert_pos[1].vert_data.vert_x - text.vert_pos[0].vert_data.vert_x;  
        rec_size.second = text.vert_pos[3].vert_data.vert_y - text.vert_pos[0].vert_data.vert_y; 
        
        switch (rec_origin)
        {
        case REC_ORIGIN_VERTEX_TOP_LEFT:
            dengUtils::RectangleGenerator::generateMappedRelRec (
                pos,
                rec_size,
                true,
                {-1.0f, -1.0f},
                form_vert,
                vert
            );
            break;

        case REC_ORIGIN_VERTEX_TOP_RIGHT:
            dengUtils::RectangleGenerator::generateMappedRelRec (
                pos,
                rec_size,
                true,
                {1.0f, -1.0f},
                form_vert,
                vert
            );
            break;

        case REC_ORIGIN_VERTEX_BOTTOM_LEFT:
            dengUtils::RectangleGenerator::generateMappedRelRec (
                pos,
                rec_size,
                true,
                {-1.0f, 1.0f},
                form_vert,
                vert
            );
            break;

        case REC_ORIGIN_VERTEX_BOTTOM_RIGHT:
            dengUtils::RectangleGenerator::generateMappedRelRec (
                pos,
                rec_size,
                true,
                {1.0f, 1.0f},
                form_vert,
                vert
            );
            break;
        
        default:
            break;
        }

        indices = {0, 1, 2, 2, 3, 0};
        tex.insert (
            tex.end(),
            text.tex_data.begin(),
            text.tex_data.end()
        );
        
        tex_size = text.box_size;
    }


    /*************************************************************/
    /*************************************************************/
    /********** Main window elements creation methods ************/
    /*************************************************************/
    /*************************************************************/
    
    /* Main form creation method */
    void Window::mkForm(WindowInfo *p_wi) {
        dengMath::vec2<deng_vec_t> tmp_pos;
        dengMath::vec2<deng_vec_t> tmp_size;
        m_win_elems.resize(m_win_elems.size() + 1);

        switch (p_wi->wt)
        {
        case WINDOW_TYPE_FLOATING:
            // Create floating window rectangle
            tmp_pos = p_wi->pos;
            tmp_size = p_wi->size;
            break;

        case WINDOW_TYPE_STATIC_BOTTOM:
            tmp_pos.first = -1.0f;
            tmp_pos.second = 1.0f - p_wi->size.second;
            tmp_size.first = 2.0f;
            tmp_size.second = p_wi->size.second;
            break;

        case WINDOW_TYPE_STATIC_TOP:
            tmp_pos.first = -1.0f;
            tmp_pos.second = -1.0f;
            tmp_size.first = 2.0f;
            tmp_size.second = p_wi->size.second;
            break;

        case WINDOW_TYPE_STATIC_LEFT:
            tmp_pos.first = -1.0f;
            tmp_pos.second = -1.0f;
            tmp_size.first = p_wi->size.first;
            tmp_size.second = 2.0f;
            break;

        case WINDOW_TYPE_STATIC_RIGHT:
            tmp_pos.first = 1.0f - p_wi->size.first;
            tmp_pos.second = -1.0f;
            tmp_size.first = p_wi->size.first;
            tmp_size.second = 2.0f;
            break;
        
        default:
            break;
        }

        dengUtils::RectangleGenerator::generateUnmappedAbsRec (
            tmp_pos,
            tmp_size,
            {-1.0f, -1.0f},
            p_wi->pc,
            m_win_elems[m_win_elems.size() - 1].unmapped_vert
        );

        m_win_elems[m_win_elems.size() - 1].indices = {0, 1, 2, 2, 3, 0};

        m_win_elems[m_win_elems.size() - 1].parent_id = p_wi->id;
        m_win_elems[m_win_elems.size() - 1].child_id = DENGUI_FORM_ID;
        m_win_elems[m_win_elems.size() - 1].is_visible = true;
        m_win_elems[m_win_elems.size() - 1].is_interactive = false;
        m_win_elems[m_win_elems.size() - 1].base_vert_size = (deng_i32_t) m_win_elems[m_win_elems.size() - 1].unmapped_vert.size();
        m_win_elems[m_win_elems.size() - 1].base_indices_size = (deng_i32_t) m_win_elems[m_win_elems.size() - 1].indices.size();
        m_win_elems[m_win_elems.size() - 1].color_mode = ELEMENT_COLOR_MODE_UNMAPPED;
        m_win_elems[m_win_elems.size() - 1].onLMBClickFunc = NULL;
        m_win_elems[m_win_elems.size() - 1].onMMBClickFunc = NULL;
        m_win_elems[m_win_elems.size() - 1].onRMBClickFunc = NULL;
        m_win_elems[m_win_elems.size() - 1].onScrUpFunc = NULL;
        m_win_elems[m_win_elems.size() - 1].onScrDownFunc = NULL;
    }


    /* Add titlebar to window */
    void Window::mkTitlebar (
        WindowInfo *p_wi,
        VERT_UNMAPPED_2D *form_vert
    ) {
        dengMath::vec2<deng_vec_t> tmp_pos;
        dengMath::vec2<deng_vec_t> tmp_size;

        tmp_pos.first = -1.0f;
        tmp_pos.second = -1.0f;
        tmp_size.first = 2.0f;
        tmp_size.second = DENGUI_TITLEBAR_HEIGHT;
        m_win_elems.resize(m_win_elems.size() + 1);

        dengUtils::RectangleGenerator::generateUnmappedRelRec (
            tmp_pos,
            tmp_size,
            false,
            {-1.0f, -1.0},
            form_vert,
            p_wi->tc,
            m_win_elems[m_win_elems.size() - 1].unmapped_vert
        );
        m_win_elems[m_win_elems.size() - 1].indices = {0, 1, 2, 2, 3, 0};

        m_win_elems[m_win_elems.size() - 1].parent_id = p_wi->id;
        m_win_elems[m_win_elems.size() - 1].child_id = DENGUI_TITLEBAR_ID;
        m_win_elems[m_win_elems.size() - 1].is_visible = true;
        m_win_elems[m_win_elems.size() - 1].is_interactive = false;
        m_win_elems[m_win_elems.size() - 1].base_vert_size = (deng_i32_t) m_win_elems[m_win_elems.size() - 1].unmapped_vert.size();
        m_win_elems[m_win_elems.size() - 1].base_indices_size = (deng_i32_t) m_win_elems[m_win_elems.size() - 1].indices.size();
        m_win_elems[m_win_elems.size() - 1].color_mode = ELEMENT_COLOR_MODE_UNMAPPED;
        m_win_elems[m_win_elems.size() - 1].onLMBClickFunc = NULL;
        m_win_elems[m_win_elems.size() - 1].onMMBClickFunc = NULL;
        m_win_elems[m_win_elems.size() - 1].onRMBClickFunc = NULL;
        m_win_elems[m_win_elems.size() - 1].onScrDownFunc = NULL;
        m_win_elems[m_win_elems.size() - 1].onScrUpFunc = NULL;
    }


    /* Add minimize triangle to the window */
    void Window::mkMinimiseTriangle (
        WindowInfo *p_wi,
        VERT_UNMAPPED_2D *form_vert
    ) {
        std::vector<dengMath::vec2<deng_vec_t>> rel_tri_pos(3);
        dengMath::vec2<deng_vec_t> tmp_pos;
        dengMath::vec2<deng_vec_t> tmp_size;

        m_win_elems.resize(m_win_elems.size() + 1);
        switch (p_wi->wt)
        {
        case WINDOW_TYPE_FLOATING:
        case WINDOW_TYPE_STATIC_BOTTOM:
        case WINDOW_TYPE_STATIC_TOP:
            rel_tri_pos[0] = {-1.0f, -1.0f};
            rel_tri_pos[1] = {1.0f, -1.0f};
            rel_tri_pos[2] = {0.0f, 1.0f};
            break;

        case WINDOW_TYPE_STATIC_RIGHT:
            rel_tri_pos[0] = {-1.0f, -1.0f};
            rel_tri_pos[1] = {-1.0f, 1.0f};
            rel_tri_pos[2] = {1.0f, 0.0f};
            break;
        
        case WINDOW_TYPE_STATIC_LEFT:
            rel_tri_pos[0] = {1.0f, -1.0f};
            rel_tri_pos[1] = {1.0f, 1.0f};
            rel_tri_pos[2] = {-1.0f, 0.0f};
            break;
        
        default:
            break;
        }

        tmp_pos.first = -1.0f + (2 * DENGUI_TITLEBAR_ELEM_MARGIN);
        tmp_pos.second = -1.0f + DENGUI_TITLEBAR_ELEM_MARGIN;
        tmp_size.first = DENGUI_TITLEBAR_HEIGHT - (2 * DENGUI_TITLEBAR_ELEM_MARGIN);
        tmp_size.second = DENGUI_TITLEBAR_HEIGHT - (2 * DENGUI_TITLEBAR_ELEM_MARGIN);

        // Add minimizable triangle
        dengUtils::TriangleGenerator::generateRelTriangle (
            m_win_elems[m_win_elems.size() - 1].unmapped_vert,
            form_vert,
            tmp_pos,
            tmp_size,
            {-1.0f, -1.0f},
            p_wi->sc,
            true,
            rel_tri_pos
        );
        m_win_elems[m_win_elems.size() - 1].indices = {0, 1, 2};
        // addGenTriangle (
        //     tmp_pos, 
        //     tmp_size,
        //     rel_tri_pos,
        //     true,
        //     REC_ORIGIN_VERTEX_TOP_LEFT,
        //     *(deng_ObjColorData*) &p_wi->sc,
        //     form_vert,
        //     m_win_elems[m_win_elems.size() - 1].unmapped_vert,
        //     m_win_elems[m_win_elems.size() - 1].indices
        // );

        m_win_elems[m_win_elems.size() - 1].parent_id = p_wi->id;
        m_win_elems[m_win_elems.size() - 1].child_id = DENGUI_MINIMISE_TRIANGLE_ID;
        m_win_elems[m_win_elems.size() - 1].is_visible = true;
        m_win_elems[m_win_elems.size() - 1].is_interactive = true;
        m_win_elems[m_win_elems.size() - 1].base_vert_size = (deng_i32_t) m_win_elems[m_win_elems.size() - 1].unmapped_vert.size();
        m_win_elems[m_win_elems.size() - 1].base_indices_size = (deng_i32_t) m_win_elems[m_win_elems.size() - 1].indices.size();
        m_win_elems[m_win_elems.size() - 1].color_mode = ELEMENT_COLOR_MODE_UNMAPPED;
        m_win_elems[m_win_elems.size() - 1].onLMBClickFunc = minTriangleCallback;
        m_win_elems[m_win_elems.size() - 1].onMMBClickFunc = NULL;
        m_win_elems[m_win_elems.size() - 1].onRMBClickFunc = NULL;
        m_win_elems[m_win_elems.size() - 1].onScrUpFunc = NULL;
        m_win_elems[m_win_elems.size() - 1].onScrDownFunc = NULL;
    }


    /* Create maximise triangle */
    void Window::mkMaximiseTriangle (
        WindowInfo *p_wi,
        VERT_UNMAPPED_2D *form_vert
    ) {
        dengMath::vec2<deng_vec_t> tmp_pos;
        dengMath::vec2<deng_vec_t> tmp_size;
        std::vector<dengMath::vec2<deng_vec_t>> rel_tri_pos(3);
        tmp_size.first = DENGUI_TITLEBAR_HEIGHT - (2 * DENGUI_TITLEBAR_ELEM_MARGIN);
        tmp_size.second = DENGUI_TITLEBAR_HEIGHT - (2 * DENGUI_TITLEBAR_ELEM_MARGIN);
        tmp_pos.first = 1.0f - DENGUI_TITLEBAR_ELEM_MARGIN;
        tmp_pos.second = -1.0f + DENGUI_TITLEBAR_ELEM_MARGIN;

        // Add maximise triangle
        m_win_elems.resize(m_win_elems.size() + 1);
        switch (p_wi->wt)
        {
        case WINDOW_TYPE_FLOATING:
        case WINDOW_TYPE_STATIC_TOP:
        case WINDOW_TYPE_STATIC_BOTTOM:
            rel_tri_pos[0] = {0.0f, -1.0f};
            rel_tri_pos[1] = {1.0f, 1.0f};
            rel_tri_pos[2] = {-1.0f, 1.0f};
            break;
        
        case WINDOW_TYPE_STATIC_RIGHT:
            rel_tri_pos[0] = {-1.0f, 0.0f};
            rel_tri_pos[1] = {1.0f, -1.0f};
            rel_tri_pos[2] = {1.0f, 1.0f};
            break; 
        
        case WINDOW_TYPE_STATIC_LEFT:
            rel_tri_pos[0] = {-1.0f, -1.0f};
            rel_tri_pos[1] = {1.0f, 0.0f};
            rel_tri_pos[2] = {-1.0f, 1.0f};
            break;

        default:
            break;
        }

        dengUtils::TriangleGenerator::generateRelTriangle (
            m_win_elems[m_win_elems.size() - 1].unmapped_vert,
            form_vert,
            tmp_pos,
            tmp_size,
            {1.0f, -1.0f},
            p_wi->sc,
            true,
            rel_tri_pos
        );

        m_win_elems[m_win_elems.size() - 1].indices = {0, 1, 2};
        m_win_elems[m_win_elems.size() - 1].parent_id = p_wi->id;
        m_win_elems[m_win_elems.size() - 1].child_id = DENGUI_MAXIMISE_TRIANGLE_ID;
        m_win_elems[m_win_elems.size() - 1].is_visible = false;
        m_win_elems[m_win_elems.size() - 1].is_interactive = true;
        m_win_elems[m_win_elems.size() - 1].base_vert_size = (deng_i32_t) m_win_elems[m_win_elems.size() - 1].unmapped_vert.size();
        m_win_elems[m_win_elems.size() - 1].base_indices_size = (deng_i32_t) m_win_elems[m_win_elems.size() - 1].indices.size();
        m_win_elems[m_win_elems.size() - 1].color_mode = ELEMENT_COLOR_MODE_UNMAPPED;
        m_win_elems[m_win_elems.size() - 1].onLMBClickFunc = dengui::maxTriangleCallback;
        m_win_elems[m_win_elems.size() - 1].onMMBClickFunc = NULL;
        m_win_elems[m_win_elems.size() - 1].onRMBClickFunc = NULL;
        m_win_elems[m_win_elems.size() - 1].onScrUpFunc = NULL;
        m_win_elems[m_win_elems.size() - 1].onScrDownFunc = NULL;
    }


    /* Create close button for the window */
    void Window::mkClose (
        dengMath::vec4<deng_vec_t> color,
        dengMath::vec2<deng_ui32_t> deng_win_bounds,
        std::string parent,
        VERT_UNMAPPED_2D *form_vert 
    ) {
        dengUtils::BitmapStr str;
        dengMath::vec2<deng_vec_t> tmp_pos;
        dengMath::vec2<deng_vec_t> window_size;
        window_size.first = form_vert[1].vert_data.vert_x - form_vert[0].vert_data.vert_x;
        window_size.second = form_vert[3].vert_data.vert_y - form_vert[0].vert_data.vert_y;

        str.text = DENGUI_CLOSE_BTN;
        tmp_pos.first = -1.0f + (DENGUI_TITLEBAR_ELEM_MARGIN / window_size.first * 2);
        tmp_pos.second = -1.0f + (DENGUI_TITLEBAR_ELEM_MARGIN / window_size.second * 2);
        
        m_win_elems.resize(m_win_elems.size() + 1);
        // Add closing button as a text box
        addRelText (
            tmp_pos,
            DENGUI_TITLEBAR_HEIGHT - DENGUI_TITLEBAR_ELEM_MARGIN,
            deng_win_bounds,
            REC_ORIGIN_VERTEX_TOP_RIGHT,
            str,
            {(unsigned char) (color.first * 255), (unsigned char) (color.second * 255), (unsigned char) (color.third * 255)},
            form_vert,
            m_win_elems[m_win_elems.size() - 1].mapped_vert,
            m_win_elems[m_win_elems.size() - 1].indices,
            m_win_elems[m_win_elems.size() - 1].texture,
            m_win_elems[m_win_elems.size() - 1].tex_box
        );

        m_win_elems[m_win_elems.size() - 1].parent_id = parent;
        m_win_elems[m_win_elems.size() - 1].child_id = DENGUI_CLOSE_BTN_ID;
        m_win_elems[m_win_elems.size() - 1].is_visible = true;
        m_win_elems[m_win_elems.size() - 1].is_interactive = true;
        m_win_elems[m_win_elems.size() - 1].base_vert_size = (deng_i32_t) m_win_elems[m_win_elems.size() - 1].unmapped_vert.size();
        m_win_elems[m_win_elems.size() - 1].base_indices_size = (deng_i32_t) m_win_elems[m_win_elems.size() - 1].indices.size();
        m_win_elems[m_win_elems.size() - 1].color_mode = ELEMENT_COLOR_MODE_TEXTURE_MAPPED;
        m_win_elems[m_win_elems.size() - 1].onLMBClickFunc = closeBtnCallback;
        m_win_elems[m_win_elems.size() - 1].onMMBClickFunc = NULL;
        m_win_elems[m_win_elems.size() - 1].onRMBClickFunc = NULL;
        m_win_elems[m_win_elems.size() - 1].onScrUpFunc = NULL;
        m_win_elems[m_win_elems.size() - 1].onScrDownFunc = NULL;
    }


    /* Rasterise and create the title string vertices */
    void Window::mkTitle (
        WindowInfo *p_wi,
        dengMath::vec2<deng_ui32_t> deng_win_bounds,
        VERT_UNMAPPED_2D *form_vert, 
        dengMath::vec3<unsigned char> color
    ) {
        dengMath::vec2<deng_vec_t> tmp_pos;
        dengMath::vec2<deng_vec_t> titlebar_size;
        dengUtils::BitmapStr str;
        str.text = p_wi->id.c_str();
        
        deng_vec_t font_size = DENGUI_TITLEBAR_HEIGHT - (2 * DENGUI_TITLEBAR_ELEM_MARGIN);

        // Trim the raster string into suitable size
        deng_vec_t box_width = 0;
        char *title = m_p_sr->strRasterWidthTrim (
            str.text,
            DENGUI_DEFAULT_FONT_FILE,
            font_size,
            form_vert[1].vert_data.vert_x - form_vert[0].vert_data.vert_x - (4 * DENGUI_TITLEBAR_HEIGHT),
            deng_win_bounds,
            &box_width
        );

        titlebar_size.first = form_vert[1].vert_data.vert_x - form_vert[0].vert_data.vert_x;
        titlebar_size.second = (DENGUI_TITLEBAR_HEIGHT / (form_vert[3].vert_data.vert_y - form_vert[0].vert_data.vert_y)) + 1.0f;
        
        tmp_pos.first = -box_width / titlebar_size.first;
        tmp_pos.second = -1.0f + DENGUI_TITLEBAR_ELEM_MARGIN / titlebar_size.second;
        
        LOG (
            "TITLE_POS: " +
            std::to_string(tmp_pos.first) + 
            ", " +
            std::to_string(tmp_pos.second)
        );

        // Rasterise the string
        str.text = title;
        m_win_elems.resize(m_win_elems.size() + 1);
        addRelText (
            tmp_pos,
            font_size,
            deng_win_bounds,
            REC_ORIGIN_VERTEX_TOP_LEFT,
            str,
            color,
            form_vert,
            m_win_elems[m_win_elems.size() - 1].mapped_vert,
            m_win_elems[m_win_elems.size() - 1].indices,
            m_win_elems[m_win_elems.size() - 1].texture,
            m_win_elems[m_win_elems.size() - 1].tex_box
        );
       
        LOG (
            "TITLE_VERT: " +
            std::to_string(m_win_elems[m_win_elems.size() - 1].mapped_vert[0].vert_data.vert_x) + 
            ", " +
            std::to_string(m_win_elems[m_win_elems.size() - 1].mapped_vert[0].vert_data.vert_y)
        );

        LOG (
            "TITLE_VERT: " +
            std::to_string(m_win_elems[m_win_elems.size() - 1].mapped_vert[1].vert_data.vert_x) + 
            ", " +
            std::to_string(m_win_elems[m_win_elems.size() - 1].mapped_vert[1].vert_data.vert_y)
        );

        LOG (
            "TITLE_VERT: " +
            std::to_string(m_win_elems[m_win_elems.size() - 1].mapped_vert[2].vert_data.vert_x) + 
            ", " +
            std::to_string(m_win_elems[m_win_elems.size() - 1].mapped_vert[2].vert_data.vert_y)
        );

        LOG (
            "TITLE_VERT: " +
            std::to_string(m_win_elems[m_win_elems.size() - 1].mapped_vert[3].vert_data.vert_x) + 
            ", " +
            std::to_string(m_win_elems[m_win_elems.size() - 1].mapped_vert[3].vert_data.vert_y)
        );

        m_win_elems[m_win_elems.size() - 1].child_id = DENGUI_TITLE_ID;
        m_win_elems[m_win_elems.size() - 1].color_mode = ELEMENT_COLOR_MODE_TEXTURE_MAPPED;
        m_win_elems[m_win_elems.size() - 1].is_interactive = false;
        m_win_elems[m_win_elems.size() - 1].is_visible = true;
        m_win_elems[m_win_elems.size() - 1].base_vert_size = (deng_i32_t) m_win_elems[m_win_elems.size() - 1].unmapped_vert.size();
        m_win_elems[m_win_elems.size() - 1].base_indices_size = (deng_i32_t) m_win_elems[m_win_elems.size() - 1].indices.size();
        m_win_elems[m_win_elems.size() - 1].onLMBClickFunc = NULL;
        m_win_elems[m_win_elems.size() - 1].onMMBClickFunc = NULL;
        m_win_elems[m_win_elems.size() - 1].onRMBClickFunc = NULL;
        m_win_elems[m_win_elems.size() - 1].onScrDownFunc = NULL;
        m_win_elems[m_win_elems.size() - 1].onScrUpFunc = NULL;
        m_win_elems[m_win_elems.size() - 1].parent_id = p_wi->id;
    }


    /* Calculate all window vertices information */
    void Window::makeWindow (
        WindowInfo *p_wi,
        dengMath::vec2<deng_ui32_t> draw_bounds
    ) {
        VERT_UNMAPPED_2D *form_vert;
        // Create main window form
        mkForm(p_wi);
        dengUtils::BorderGenerator::generateBorders (
            m_win_elems[m_win_elems.size() - 1].unmapped_vert,
            false,
            0,
            m_win_elems[m_win_elems.size() - 1].indices,
            {0.0f, 0.0f, 0.0f, 1.0f},
            draw_bounds,
            p_wi->border
        );
        form_vert = m_win_elems[m_win_elems.size() - 1].unmapped_vert.data();

        // Add titlebar to window
        if(!(p_wi->fl_b & DENGUI_WINDOW_FLAG_NO_TITLEBAR)) {
            mkTitlebar (
                p_wi, 
                form_vert
            );
            
            dengUtils::BorderGenerator::generateBorders (
                m_win_elems[m_win_elems.size() - 1].unmapped_vert,
                false,
                0,
                m_win_elems[m_win_elems.size() - 1].indices,
                {0.0f, 0.0f, 0.0f, 1.0f},
                draw_bounds,
                p_wi->border
            );

            mkTitle (
                p_wi,
                draw_bounds,
                form_vert,
                {(unsigned char) (p_wi->sc.first * 255), (unsigned char) (p_wi->sc.second * 255), (unsigned char) (p_wi->sc.third * 255)}  
            );

            dengUtils::BorderGenerator::generateBorders (
                m_win_elems[m_win_elems.size() - 1].unmapped_vert,
                false,
                0,
                m_win_elems[m_win_elems.size() - 1].indices,
                {0.0f, 0.0f, 0.0f, 1.0f},
                draw_bounds,
                p_wi->border
            );


            // Add minimise and maximise triangles if needed
            if(!(p_wi->fl_b & DENGUI_WINDOW_FLAG_NO_COLLAPSE)) {
                mkMinimiseTriangle(p_wi, form_vert);
                mkMaximiseTriangle(p_wi, form_vert);
            }

            // Add close button if needed ([X])
            if(!(p_wi->fl_b & DENGUI_WINDOW_FLAG_NO_CLOSE)) {
                mkClose (
                    p_wi->sc, 
                    draw_bounds,
                    p_wi->id, 
                    form_vert
                );
            }
        }
    }


    // Window class getters 
    dengMath::vec4<float> Window::getPC() { return m_primary_color; }
    dengMath::vec4<float> Window::getSC() { return m_secondary_color; }
    dengMath::vec4<float> Window::getTC() { return m_tertiary_color; }

    std::vector<WindowElement> Window::getWindowElements() { return m_win_elems; }
    std::string Window::getId() { return m_id; }

    // Window class setters
    void Window::setPC(dengMath::vec4<float> *p_pc) { m_primary_color = *p_pc; }
    void Window::setSC(dengMath::vec4<float> *p_sc) { m_secondary_color = *p_sc; }
    void Window::setTC(dengMath::vec4<float> *p_tc) { m_tertiary_color = *p_tc; }
    void Window::setPos(dengMath::vec2<float> &pos) { m_pos = pos; }
    void Window::setSize(dengMath::vec2<float> &size) { m_size = size; }
}
