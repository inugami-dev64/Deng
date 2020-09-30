#include "../core/deng_core.h"

namespace dengUI {

    Window::Window(WindowInfo &windowinfo, WindowBorderInfo borderinfo, BufferInfo &bufferinfo) {
        this->m_windowinfo = windowinfo;
        this->m_borderinfo = borderinfo;
        this->m_bufferinfo = bufferinfo;

        this->m_clickinfo.p_vertices_data = &this->m_vertices_data;
        this->m_clickinfo.p_indices_data = &this->m_indices_data;

        this->createBaseWindow();
        this->sortClickableObjects();
        
        this->m_p_collision = new PixelCollision(this->m_windowinfo.p_window, &this->m_vertices_data, &this->m_indices_data);
        this->m_p_events = new Events(this->m_windowinfo.p_window, this->m_p_collision, &this->m_vertices_data, &this->m_indices_data);
        this->m_p_handler_list = new WindowObjectHandleList(&this->m_window_objects, &this->m_p_clickable_objects, &this->m_vertices_data, &this->m_indices_data);

        for(size_t i = 0; i < this->m_p_clickable_objects.size(); i++) {
            this->m_p_collision->setupPixelRangesFromVertices(&this->m_p_clickable_objects[i]->vertices_bounds, &this->m_p_clickable_objects[i]->indices_bounds, this->m_p_clickable_objects[i]->description);
        }
        
        this->createBuffers();
    }

    Window::~Window() {
        vkDestroyBuffer(*this->m_windowinfo.p_device, *this->m_bufferinfo.p_vertices_buffer, nullptr);
        vkDestroyBuffer(*this->m_windowinfo.p_device, *this->m_bufferinfo.p_indices_buffer, nullptr);
        vkFreeMemory(*this->m_windowinfo.p_device, *this->m_bufferinfo.p_vertices_buffer_memory, nullptr);
        vkFreeMemory(*this->m_windowinfo.p_device, *this->m_bufferinfo.p_indices_buffer_memory, nullptr);
        
        delete this->m_p_handler_list;
        delete this->m_p_events;
        delete this->m_p_collision;
    }

    void Window::createBaseWindow() {
        this->m_window_objects.resize(5);
        deng::vec4<deng::vec2<float>> local_rectangle_vertices;
        deng::vec3<deng::vec2<float>> local_triangle_vertices;
        RectangleInfo local_rectangle_info{};
        TriangleInfo local_triangle_info{};

        // main window
        local_rectangle_vertices.first = {this->m_windowinfo.position.first - (this->m_windowinfo.origin.first * this->m_windowinfo.size.first), this->m_windowinfo.position.second - (this->m_windowinfo.origin.second * this->m_windowinfo.size.second) + this->m_borderinfo.titlebar_height};
        local_rectangle_vertices.second = {this->m_windowinfo.position.first + (this->m_windowinfo.size.first - (this->m_windowinfo.origin.first * this->m_windowinfo.size.first)), this->m_windowinfo.position.second - (this->m_windowinfo.origin.second * this->m_windowinfo.size.second) + this->m_borderinfo.titlebar_height};
        local_rectangle_vertices.third = {this->m_windowinfo.position.first + (this->m_windowinfo.size.first - (this->m_windowinfo.origin.first * this->m_windowinfo.size.first)), this->m_windowinfo.position.second + (this->m_windowinfo.size.second - (this->m_windowinfo.origin.second * this->m_windowinfo.size.second))};
        local_rectangle_vertices.fourth = {this->m_windowinfo.position.first - (this->m_windowinfo.origin.first * this->m_windowinfo.size.first), this->m_windowinfo.position.second + (this->m_windowinfo.size.second - (this->m_windowinfo.origin.second * this->m_windowinfo.size.second))};

        local_rectangle_info.p_color = &this->m_windowinfo.color;
        local_rectangle_info.p_vertices = &local_rectangle_vertices;
        local_rectangle_info.p_vertices_data = &this->m_vertices_data;
        local_rectangle_info.p_indices_data = &this->m_indices_data;
        local_rectangle_info.enable_fill = DENG_TRUE;
        local_rectangle_info.sequence_id = DENGUI_WINDOW_LAYER_TYPE_MAIN_WINDOW;

        this->m_window_objects[0].description = "main window";
        this->m_window_objects[0].is_clickable = DENG_FALSE;
        this->m_window_objects[0].is_drawn = DENG_TRUE;
        
        ShapeVerticesDataCreator::getRectangleVertices(&local_rectangle_info, &this->m_window_objects[0]);
        LOG("Successfully initialised dengui window main rectangle vertices!");

        // window borders and titlebar
            // titlebar
        local_rectangle_vertices.first = {this->m_vertices_data[this->m_window_objects[0].vertices_bounds.first].position_vec.first, this->m_windowinfo.position.second - (this->m_windowinfo.origin.second * this->m_windowinfo.size.second)};
        local_rectangle_vertices.second = {this->m_vertices_data[this->m_window_objects[0].vertices_bounds.first + 1].position_vec.first, this->m_windowinfo.position.second - (this->m_windowinfo.origin.second * this->m_windowinfo.size.second)};
        local_rectangle_vertices.third = {this->m_vertices_data[this->m_window_objects[0].vertices_bounds.first + 1].position_vec.first, local_rectangle_vertices.second.second + this->m_borderinfo.titlebar_height};
        local_rectangle_vertices.fourth = {this->m_vertices_data[this->m_window_objects[0].vertices_bounds.first].position_vec.first, local_rectangle_vertices.second.second + this->m_borderinfo.titlebar_height};

        local_rectangle_info.p_color = &this->m_windowinfo.color;
        local_rectangle_info.p_vertices = &local_rectangle_vertices;
        local_triangle_info.sequence_id = DENGUI_WINDOW_LAYER_TYPE_MAIN_WINDOW;

        this->m_window_objects[1].description = "titlebar";
        this->m_window_objects[1].is_clickable = DENG_FALSE;
        this->m_window_objects[1].is_drawn = DENG_TRUE;

        ShapeVerticesDataCreator::getRectangleVertices(&local_rectangle_info, &this->m_window_objects[1]);

            // titlebar borders
        local_rectangle_vertices.first = {this->m_vertices_data[this->m_window_objects[1].vertices_bounds.first].position_vec.first, this->m_vertices_data[this->m_window_objects[1].vertices_bounds.first].position_vec.second};
        local_rectangle_vertices.second = {this->m_vertices_data[this->m_window_objects[1].vertices_bounds.first + 1].position_vec.first, this->m_vertices_data[this->m_window_objects[1].vertices_bounds.first + 1].position_vec.second};
        local_rectangle_vertices.third = {this->m_vertices_data[this->m_window_objects[1].vertices_bounds.first + 2].position_vec.first, this->m_vertices_data[this->m_window_objects[1].vertices_bounds.first + 2].position_vec.second + this->m_borderinfo.thickness};
        local_rectangle_vertices.fourth = {this->m_vertices_data[this->m_window_objects[1].vertices_bounds.first + 3].position_vec.first, this->m_vertices_data[this->m_window_objects[1].vertices_bounds.first + 3].position_vec.second + this->m_borderinfo.thickness};

        local_rectangle_info.p_color = &this->m_borderinfo.border_color;
        local_rectangle_info.p_vertices = &local_rectangle_vertices;
        local_rectangle_info.enable_fill = DENG_FALSE;
        local_rectangle_info.border_offset_mode = DENGUI_NON_FILLED_BORDER_OFFSET_MODE_IN;
        local_rectangle_info.border_thickness = this->m_borderinfo.thickness;
        local_rectangle_info.sequence_id = DENGUI_WINDOW_LAYER_TYPE_BORDERS_AND_TITLEBAR;

        this->m_window_objects[2].description = "titlebar borders";
        this->m_window_objects[2].is_clickable = DENG_FALSE;
        this->m_window_objects[2].is_drawn = DENG_TRUE;

        ShapeVerticesDataCreator::getRectangleVertices(&local_rectangle_info, &this->m_window_objects[2]);

            // window borders
        local_rectangle_vertices.first = {this->m_vertices_data[this->m_window_objects[0].vertices_bounds.first].position_vec.first, this->m_vertices_data[this->m_window_objects[0].vertices_bounds.first].position_vec.second};
        local_rectangle_vertices.second = {this->m_vertices_data[this->m_window_objects[0].vertices_bounds.first + 1].position_vec.first, this->m_vertices_data[this->m_window_objects[0].vertices_bounds.first + 1].position_vec.second};
        local_rectangle_vertices.third = {this->m_vertices_data[this->m_window_objects[0].vertices_bounds.first + 2].position_vec.first, this->m_vertices_data[this->m_window_objects[0].vertices_bounds.first + 2].position_vec.second};
        local_rectangle_vertices.fourth = {this->m_vertices_data[this->m_window_objects[0].vertices_bounds.first + 3].position_vec.first, this->m_vertices_data[this->m_window_objects[0].vertices_bounds.first + 3].position_vec.second};

        local_rectangle_info.p_vertices = &local_rectangle_vertices;
        local_rectangle_info.p_color = &this->m_borderinfo.border_color;

        this->m_window_objects[3].description = "window borders";
        this->m_window_objects[3].is_clickable = DENG_FALSE;
        this->m_window_objects[3].is_drawn = DENG_TRUE;

        ShapeVerticesDataCreator::getRectangleVertices(&local_rectangle_info, &this->m_window_objects[3]);
        LOG("Successfully initialised dengui window border border vertices!");

            // minimizing triangle
        local_triangle_vertices.first = {this->m_vertices_data[this->m_window_objects[1].vertices_bounds.first].position_vec.first + this->m_borderinfo.thickness + DENGUI_MINIMIZE_TRIANGLE_OFFSET, this->m_vertices_data[this->m_window_objects[1].vertices_bounds.first].position_vec.second + this->m_borderinfo.thickness + DENGUI_MINIMIZE_TRIANGLE_OFFSET};
        local_triangle_vertices.second = {local_triangle_vertices.first.first + (this->m_borderinfo.titlebar_height - this->m_borderinfo.thickness - DENGUI_MINIMIZE_TRIANGLE_OFFSET), local_triangle_vertices.first.second};
        local_triangle_vertices.third = {(local_triangle_vertices.first.first + local_triangle_vertices.second.first) / 2, this->m_vertices_data[this->m_window_objects[1].vertices_bounds.first].position_vec.second + (this->m_borderinfo.titlebar_height - DENGUI_MINIMIZE_TRIANGLE_OFFSET)};

        // local_triangle_vertices.first.second += (local_triangle_vertices.third.second - local_triangle_vertices.second.second);
        // local_triangle_vertices.third.second -= (local_triangle_vertices.third.second - local_triangle_vertices.second.second);
        // local_triangle_vertices.second.second == local_triangle_vertices.first.second;

        local_triangle_info.p_vertices = &local_triangle_vertices;
        local_triangle_info.p_color = &this->m_borderinfo.minimizing_triangle_color;
        local_triangle_info.p_vertices_data = &this->m_vertices_data;
        local_triangle_info.p_indices_data = &this->m_indices_data;
        local_triangle_info.sequence_id = DENGUI_WINDOW_LAYER_TYPE_BORDERS_AND_TITLEBAR;

        // MinimiseHandle local_minimise_handle();

        this->m_window_objects[4].description = "minimising triangle";
        this->m_window_objects[4].is_clickable = DENG_TRUE;
        this->m_window_objects[4].handle_id = 0;
        this->m_window_objects[4].is_drawn = DENG_TRUE;
        // this->m_window_objects_info[4].p_click_handler_struct = ;

        ShapeVerticesDataCreator::getTriangleVertices(&local_triangle_info, &this->m_window_objects[4]);
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

    // void Window::updateBuffers() {
    //     VkDeviceSize local_size = static_cast<VkDeviceSize>(sizeof(this->m_vertices_data[0]) * this->m_vertices_data.size());
        
    //     this->m_bufferinfo.p_buffer_create_func(this->m_windowinfo.p_device, this->m_windowinfo.p_gpu, &local_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->m_bufferinfo.p_staging_buffer, this->m_bufferinfo.p_staging_buffer_memory, nullptr);
        
    // }

    void Window::sortClickableObjects() {
        for(size_t i = 0; i < this->m_window_objects.size(); i++) {
            if(this->m_window_objects[i].is_clickable == DENG_TRUE)
                this->m_p_clickable_objects.push_back(&this->m_window_objects[i]);
        }
    }

    void Window::getClickableObjects(std::vector<WindowObject*> *p_clickable_objects) {
        p_clickable_objects->clear();
        p_clickable_objects = &this->m_p_clickable_objects;
    }

    void Window::update() {
        for(size_t i = 0; i < this->m_p_clickable_objects.size(); i++) {
            this->m_clickinfo.vertices_bounds = this->m_p_clickable_objects[i]->vertices_bounds;
            this->m_clickinfo.indices_bounds = this->m_p_clickable_objects[i]->indices_bounds;
            this->m_clickinfo.p_is_clicked = &this->m_p_clickable_objects[i]->is_clicked;
            this->m_clickinfo.description = this->m_p_clickable_objects[i]->description;

            this->m_p_events->checkForClicks(&this->m_clickinfo);
            
            if(*this->m_clickinfo.p_is_clicked)
                this->m_p_handler_list->callClickHandler(this->m_p_clickable_objects[i]->handle_id);
        }
    }
}