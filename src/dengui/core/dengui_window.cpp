#include "../../core/deng_core.h"

namespace dengUI {

    Window::Window(WindowInfo &windowinfo, BufferInfo &bufferinfo) {
        this->m_windowinfo = windowinfo;
        this->m_bufferinfo = bufferinfo;

        this->m_p_css_data_handler = new dengCSS::CSSDataHandler(this->m_windowinfo.p_window, DENG_CSS_PATH);
        std::vector<std::string> local_css_files;
        
        dengCSS::CSSHeadData *p_local_head_css_data;
        dengCSS::CSSBodyData *p_local_body_css_data;
        dengCSS::CSSGenericObjectData *p_local_minimise_triangle_css_data;

        this->m_p_css_data_handler->getHeadData(&p_local_head_css_data);
        this->m_p_css_data_handler->getBodyData(&p_local_body_css_data);
        this->m_p_css_data_handler->getGenericObjectData(&p_local_minimise_triangle_css_data);

        this->createBaseWindow(p_local_head_css_data, p_local_body_css_data, p_local_minimise_triangle_css_data);
        this->createBuffers();
    }

    Window::~Window() {
        vkDestroyBuffer(*this->m_windowinfo.p_device, *this->m_bufferinfo.p_vertices_buffer, nullptr);
        vkDestroyBuffer(*this->m_windowinfo.p_device, *this->m_bufferinfo.p_indices_buffer, nullptr);
        vkFreeMemory(*this->m_windowinfo.p_device, *this->m_bufferinfo.p_vertices_buffer_memory, nullptr);
        vkFreeMemory(*this->m_windowinfo.p_device, *this->m_bufferinfo.p_indices_buffer_memory, nullptr);
        
        // delete this->m_p_handler_list;
        // delete this->m_p_events;
        delete this->m_p_css_data_handler;
        delete this->m_p_collision;
    }

    void Window::getWindowVertices(const dengUIWindowVerticesType &vertices_type, dengCSS::CSSHeadData *p_css_head_data, dengCSS::CSSBodyData *p_css_body_data, dengCSS::CSSGenericObjectData *p_css_minimise_triangle_data) {
        RectangleInfo *p_local_rectangle_info;
        TriangleInfo *p_local_triangle_info;
        WindowInfo *p_local_window_info;
        BorderInfo local_window_border_info;
        
        switch (vertices_type)
        {
        case DENGUI_WINDOW_VERTICES_TYPE_BODY:
            p_local_rectangle_info = new RectangleInfo;

            p_local_rectangle_info->p_indices = &this->m_base_window_vertices.window_main_indices;
            p_local_rectangle_info->p_vertices = &this->m_base_window_vertices.window_main_vertices;
            p_local_rectangle_info->rectangle_color = p_css_body_data->background_color;
            p_local_rectangle_info->rectangle_size = {p_css_body_data->width, p_css_body_data->height};

            p_local_rectangle_info->margin_data.margin_top = 0.0f;
            p_local_rectangle_info->margin_data.margin_right = 0.0f;
            p_local_rectangle_info->margin_data.margin_bottom = 0.0f;
            p_local_rectangle_info->margin_data.margin_left = 0.0f;

            if(p_css_body_data->p_general_border_info == nullptr && p_css_body_data->p_specified_border_info == nullptr) {
                p_local_rectangle_info->p_vertices->resize(4);

                local_window_border_info.p_css_general_border_info = nullptr;
                local_window_border_info.p_css_specified_border_info = nullptr;
                ShapeVerticesDataCreator::createRectangle(&this->m_windowinfo, &p_local_rectangle_info, &local_window_border_info, DENGUI_BORDER_DRAW_MODE_NO_BORDER, DENGUI_SIZE_UNIT_PIXELS, 1);
            }

            else if(p_css_body_data->p_general_border_info != nullptr && p_css_body_data->p_specified_border_info == nullptr) {
                p_local_rectangle_info->p_vertices->resize(12);

                local_window_border_info.p_css_general_border_info = p_css_body_data->p_general_border_info;
                local_window_border_info.p_css_specified_border_info = nullptr;
                ShapeVerticesDataCreator::createRectangle(&this->m_windowinfo, &p_local_rectangle_info, &local_window_border_info, DENGUI_BORDER_DRAW_MODE_GENERIC_BORDER, DENGUI_SIZE_UNIT_PIXELS, 1);
            }

            else if(p_css_body_data->p_general_border_info == nullptr && p_css_body_data->p_specified_border_info != nullptr) {
                p_local_rectangle_info->p_vertices->resize(20);

                local_window_border_info.p_css_general_border_info = nullptr; 
                local_window_border_info.p_css_specified_border_info = p_css_body_data->p_specified_border_info;

                ShapeVerticesDataCreator::createRectangle(&this->m_windowinfo, &p_local_rectangle_info, &local_window_border_info, DENGUI_BORDER_DRAW_MODE_SPECIFIED_BORDER, DENGUI_SIZE_UNIT_PIXELS, 1);
            }

            delete p_local_rectangle_info;
            break;

        case DENGUI_WINDOW_VERTICES_TYPE_HEAD:
            p_local_rectangle_info = new RectangleInfo;
            p_local_window_info = new WindowInfo;
            *p_local_window_info = this->m_windowinfo;

            p_local_rectangle_info->p_indices = &this->m_base_window_vertices.window_head_indices;
            p_local_rectangle_info->p_vertices = &this->m_base_window_vertices.window_head_vertices;
            p_local_rectangle_info->rectangle_color = p_css_head_data->background_color;
            p_local_rectangle_info->rectangle_size = {p_css_body_data->width, p_css_head_data->height};
            
            p_local_rectangle_info->margin_data.margin_top = 0.0f;
            p_local_rectangle_info->margin_data.margin_right = 0.0f;
            p_local_rectangle_info->margin_data.margin_bottom = 0.0f;
            p_local_rectangle_info->margin_data.margin_left = 0.0f;
            
            p_local_window_info->position.second = this->m_windowinfo.position.second - (this->m_windowinfo.origin.second + 1) / 2 * dengMath::Conversion::pixelSizeToVector2DSize(p_css_body_data->height, this->m_windowinfo.p_window->getSize(), DENG_COORD_AXIS_Y);
            p_local_window_info->position.second -= (1 - ((this->m_windowinfo.origin.second + 1) / 2)) * dengMath::Conversion::pixelSizeToVector2DSize(p_local_rectangle_info->rectangle_size.second, this->m_windowinfo.p_window->getSize(), DENG_COORD_AXIS_Y);  
            if((p_css_head_data->p_general_border_info == nullptr && p_css_head_data->p_specified_border_info == nullptr) ||
            (p_css_head_data->p_general_border_info != nullptr && p_css_head_data->p_specified_border_info != nullptr)) {
                p_local_rectangle_info->p_vertices->resize(4);

                local_window_border_info.p_css_general_border_info = nullptr;
                local_window_border_info.p_css_specified_border_info = nullptr;
                
                ShapeVerticesDataCreator::createRectangle(p_local_window_info, &p_local_rectangle_info, &local_window_border_info, DENGUI_BORDER_DRAW_MODE_NO_BORDER, DENGUI_SIZE_UNIT_PIXELS, 1);
            }

            else if(p_css_head_data->p_general_border_info != nullptr && p_css_head_data->p_specified_border_info == nullptr) {
                p_local_rectangle_info->p_vertices->resize(12);

                local_window_border_info.p_css_general_border_info = p_css_head_data->p_general_border_info;
                local_window_border_info.p_css_specified_border_info = nullptr;

                ShapeVerticesDataCreator::createRectangle(p_local_window_info, &p_local_rectangle_info, &local_window_border_info, DENGUI_BORDER_DRAW_MODE_GENERIC_BORDER, DENGUI_SIZE_UNIT_PIXELS, 1);
            }

            else if(p_css_head_data->p_general_border_info == nullptr && p_css_head_data->p_specified_border_info != nullptr) {
                p_local_rectangle_info->p_vertices->resize(24);

                local_window_border_info.p_css_general_border_info = nullptr; 
                local_window_border_info.p_css_specified_border_info = p_css_head_data->p_specified_border_info;

                ShapeVerticesDataCreator::createRectangle(p_local_window_info, &p_local_rectangle_info, &local_window_border_info, DENGUI_BORDER_DRAW_MODE_SPECIFIED_BORDER, DENGUI_SIZE_UNIT_PIXELS, 1);
            }

            delete p_local_window_info;
            delete p_local_rectangle_info;
            break;

        case DENGUI_WINDOW_VERTICES_TYPE_TRIANGLE:
            p_local_triangle_info = new TriangleInfo;
            p_local_window_info = new WindowInfo;
            *p_local_window_info = this->m_windowinfo;

            p_local_triangle_info->p_indices = &this->m_base_window_vertices.window_minimise_triangle_indices;
            p_local_triangle_info->p_vertices = &this->m_base_window_vertices.window_minimise_triangle_vertices;
            p_local_triangle_info->triangle_color = p_css_minimise_triangle_data->background_color;

            p_local_triangle_info->margin_data.margin_top = p_css_minimise_triangle_data->margin_data.margin_top;
            p_local_triangle_info->margin_data.margin_right = p_css_minimise_triangle_data->margin_data.margin_right;
            p_local_triangle_info->margin_data.margin_bottom = p_css_minimise_triangle_data->margin_data.margin_bottom;
            p_local_triangle_info->margin_data.margin_left = p_css_minimise_triangle_data->margin_data.margin_left;

            p_local_triangle_info->vertices_relativity_infos.first = {DENG_COORD_AXIS_X, -1.0f, 0};
            p_local_triangle_info->vertices_relativity_infos.second = {DENG_COORD_AXIS_X, 1.0f, 0};
            p_local_triangle_info->vertices_relativity_infos.third = {DENG_COORD_AXIS_X, 0.0f, 3};

            p_local_triangle_info->surround_rectangle_size.first = this->m_base_window_vertices.window_head_vertices[3].position_vec.second - this->m_base_window_vertices.window_head_vertices[0].position_vec.second - dengMath::Conversion::pixelSizeToVector2DSize(p_css_minimise_triangle_data->margin_data.margin_left, p_local_window_info->p_window->getSize(), DENG_COORD_AXIS_X);
            p_local_triangle_info->surround_rectangle_size.second = this->m_base_window_vertices.window_head_vertices[3].position_vec.second - this->m_base_window_vertices.window_head_vertices[0].position_vec.second - dengMath::Conversion::pixelSizeToVector2DSize(p_css_minimise_triangle_data->margin_data.margin_top + p_css_minimise_triangle_data->margin_data.margin_bottom, p_local_window_info->p_window->getSize(), DENG_COORD_AXIS_Y);

            p_local_window_info->position.first = this->m_base_window_vertices.window_head_vertices[0].position_vec.first + (p_local_window_info->origin.first + 1) / 2 * p_local_triangle_info->surround_rectangle_size.first;
            p_local_window_info->position.second = this->m_base_window_vertices.window_head_vertices[1].position_vec.second + (p_local_window_info->origin.second + 1) / 2 * p_local_triangle_info->surround_rectangle_size.second;

            if((p_css_minimise_triangle_data->p_general_border_info == nullptr && p_css_minimise_triangle_data->p_specified_border_info == nullptr) ||
            (p_css_minimise_triangle_data->p_general_border_info != nullptr && p_css_minimise_triangle_data->p_specified_border_info != nullptr)) {
                p_local_triangle_info->p_vertices->resize(4);

                local_window_border_info.p_css_general_border_info = nullptr;
                local_window_border_info.p_css_specified_border_info = nullptr;
                
                ShapeVerticesDataCreator::createTriangle(p_local_window_info, &p_local_triangle_info, &local_window_border_info, DENGUI_BORDER_DRAW_MODE_NO_BORDER, DENGUI_SIZE_UNIT_VECTOR_UNITS, 2);
            }

            else if(p_css_minimise_triangle_data->p_general_border_info != nullptr && p_css_minimise_triangle_data->p_specified_border_info == nullptr) {
                p_local_triangle_info->p_vertices->resize(9);

                local_window_border_info.p_css_general_border_info = p_css_minimise_triangle_data->p_general_border_info;
                local_window_border_info.p_css_specified_border_info = nullptr;

                ShapeVerticesDataCreator::createTriangle(p_local_window_info, &p_local_triangle_info, &local_window_border_info, DENGUI_BORDER_DRAW_MODE_GENERIC_BORDER, DENGUI_SIZE_UNIT_VECTOR_UNITS, 2);
            }

            else if(p_css_minimise_triangle_data->p_general_border_info == nullptr && p_css_minimise_triangle_data->p_specified_border_info != nullptr) {
                p_local_triangle_info->p_vertices->resize(24);

                local_window_border_info.p_css_general_border_info = nullptr; 
                local_window_border_info.p_css_specified_border_info = p_css_minimise_triangle_data->p_specified_border_info;

                ShapeVerticesDataCreator::createTriangle(p_local_window_info, &p_local_triangle_info, &local_window_border_info, DENGUI_BORDER_DRAW_MODE_SPECIFIED_BORDER, DENGUI_SIZE_UNIT_VECTOR_UNITS, 2);
            }

            delete p_local_triangle_info;
            delete p_local_window_info;
            break;
        
        default:
            break;
        }
    }

    void Window::createBaseWindow(dengCSS::CSSHeadData *p_head_css_data, dengCSS::CSSBodyData *p_body_css_data, dengCSS::CSSGenericObjectData *p_minimise_triangle_css_data) {
        size_t index;
        this->getWindowVertices(DENGUI_WINDOW_VERTICES_TYPE_BODY, nullptr, p_body_css_data, nullptr);

        this->m_vertices_data = this->m_base_window_vertices.window_main_vertices;
        this->m_indices_data = this->m_base_window_vertices.window_main_indices;

        this->getWindowVertices(DENGUI_WINDOW_VERTICES_TYPE_HEAD, p_head_css_data, p_body_css_data, nullptr);
        
        for(index = 0; index < this->m_base_window_vertices.window_head_indices.size(); index++)
            this->m_base_window_vertices.window_head_indices[index] += static_cast<uint16_t>(this->m_vertices_data.size());

        this->m_vertices_data.insert(this->m_vertices_data.end(), this->m_base_window_vertices.window_head_vertices.begin(), this->m_base_window_vertices.window_head_vertices.end());
        this->m_indices_data.insert(this->m_indices_data.end(), this->m_base_window_vertices.window_head_indices.begin(), this->m_base_window_vertices.window_head_indices.end());

        this->getWindowVertices(DENGUI_WINDOW_VERTICES_TYPE_TRIANGLE, p_head_css_data, p_body_css_data, p_minimise_triangle_css_data);

        for(index = 0; index < this->m_base_window_vertices.window_minimise_triangle_indices.size(); index++)
            this->m_base_window_vertices.window_minimise_triangle_indices[index] += static_cast<uint16_t>(this->m_vertices_data.size());

        this->m_vertices_data.insert(this->m_vertices_data.end(), this->m_base_window_vertices.window_minimise_triangle_vertices.begin(), this->m_base_window_vertices.window_minimise_triangle_vertices.end());
        this->m_indices_data.insert(this->m_indices_data.end(), this->m_base_window_vertices.window_minimise_triangle_indices.begin(), this->m_base_window_vertices.window_minimise_triangle_indices.end());
    }

    void Window::createBuffers() {
        VkDeviceSize local_size = static_cast<VkDeviceSize>(sizeof(this->m_vertices_data[0]) * this->m_vertices_data.size());

        // vertices buffer
        this->m_bufferinfo.p_buffer_create_func(this->m_windowinfo.p_device, this->m_windowinfo.p_gpu, &local_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->m_bufferinfo.p_staging_buffer, this->m_bufferinfo.p_staging_buffer_memory, nullptr);
        this->m_bufferinfo.p_buffer_memory_populate_func(this->m_windowinfo.p_device, this->m_windowinfo.p_gpu, &local_size, this->m_vertices_data.data(), this->m_bufferinfo.p_staging_buffer, this->m_bufferinfo.p_staging_buffer_memory);

        this->m_bufferinfo.p_buffer_create_func(this->m_windowinfo.p_device, this->m_windowinfo.p_gpu, &local_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, this->m_bufferinfo.p_vertices_buffer, this->m_bufferinfo.p_vertices_buffer_memory, nullptr);
        this->m_bufferinfo.p_buffer_copy_func(this->m_windowinfo.p_device, this->m_bufferinfo.p_commandpool, this->m_bufferinfo.p_graphics_queue, this->m_bufferinfo.p_staging_buffer, this->m_bufferinfo.p_vertices_buffer, &local_size);

        vkDestroyBuffer(*this->m_windowinfo.p_device, *this->m_bufferinfo.p_staging_buffer, nullptr);
        vkFreeMemory(*this->m_windowinfo.p_device, *this->m_bufferinfo.p_staging_buffer_memory, nullptr);

        // indices buffer
        local_size = static_cast<VkDeviceSize>(sizeof(this->m_indices_data[0]) * this->m_indices_data.size());

        this->m_bufferinfo.p_buffer_create_func(this->m_windowinfo.p_device, this->m_windowinfo.p_gpu, &local_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->m_bufferinfo.p_staging_buffer, this->m_bufferinfo.p_staging_buffer_memory, nullptr);
        this->m_bufferinfo.p_buffer_memory_populate_func(this->m_windowinfo.p_device, this->m_windowinfo.p_gpu, &local_size, this->m_indices_data.data(), this->m_bufferinfo.p_staging_buffer, this->m_bufferinfo.p_staging_buffer_memory);

        this->m_bufferinfo.p_buffer_create_func(this->m_windowinfo.p_device, this->m_windowinfo.p_gpu, &local_size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, this->m_bufferinfo.p_indices_buffer, this->m_bufferinfo.p_indices_buffer_memory, nullptr);
        this->m_bufferinfo.p_buffer_copy_func(this->m_windowinfo.p_device, this->m_bufferinfo.p_commandpool, this->m_bufferinfo.p_graphics_queue, this->m_bufferinfo.p_staging_buffer, this->m_bufferinfo.p_indices_buffer, &local_size);

        vkDestroyBuffer(*this->m_windowinfo.p_device, *this->m_bufferinfo.p_staging_buffer, nullptr);
        vkFreeMemory(*this->m_windowinfo.p_device, *this->m_bufferinfo.p_staging_buffer_memory, nullptr);

        *this->m_bufferinfo.p_indices_size = static_cast<uint32_t>(this->m_indices_data.size());
    }

    // void Window::sortClickableObjects() {
    //     for(size_t i = 0; i < this->m_window_objects.size(); i++) {
    //         if(this->m_window_objects[i].is_clickable == DENG_TRUE)
    //             this->m_p_clickable_objects.push_back(&this->m_window_objects[i]);
    //     }
    // }

   // void Window::getClickableObjects(std::vector<WindowObject*> *p_clickable_objects) {
   //     p_clickable_objects->clear();
   //     p_clickable_objects = &this->m_p_clickable_objects;
   // }

    void Window::update() {
        // for(size_t i = 0; i < this->m_p_clickable_objects.size(); i++) {
        //     this->m_clickinfo.vertices_bounds = this->m_p_clickable_objects[i]->vertices_bounds;
        //     this->m_clickinfo.indices_bounds = this->m_p_clickable_objects[i]->indices_bounds;
        //     this->m_clickinfo.p_is_clicked = &this->m_p_clickable_objects[i]->is_clicked;
        //     this->m_clickinfo.description = this->m_p_clickable_objects[i]->description;
        //     this->m_clickinfo.handle_id = this->m_p_clickable_objects[i]->handle_id;

        //     this->m_p_events->checkForClicks(&this->m_clickinfo, this->m_p_handler_list);
        // }
    }
}