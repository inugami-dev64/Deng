#include "../../core/deng_core.h" 

namespace dengUI {
    
    Events::Events(deng::Window *p_window, PixelCollision *p_pixel_collision, std::vector<dengUtils::UIVerticesData> *p_vertices_data, std::vector<uint16_t> *p_indices_data) {
        this->m_p_window = p_window;
        this->m_p_pixel_collision = p_pixel_collision;
        this->m_p_vertices_data = p_vertices_data;
        this->m_p_indices_data = p_indices_data;
    }

    void Events::updateBuffers(ClickInfo *p_clickinfo) {
        std::vector<dengUtils::UIVerticesData> local_vertices_data = *p_clickinfo->p_vertices_data;
        std::vector<uint16_t> local_indices_data = *p_clickinfo->p_indices_data;
        
        for(size_t i = 0; i < p_clickinfo->p_window_objects->size(); i++) {
            dengMath::vec2<size_t> local_indices_boundaries;
            dengMath::vec2<size_t> local_vertices_boundaries;
            
            if(!(*p_clickinfo->p_window_objects)[i].is_drawn) {
                for(size_t remove_index = (*p_clickinfo->p_window_objects)[i].vertices_bounds.first; remove_index < (*p_clickinfo->p_window_objects)[i].vertices_bounds.second; remove_index++) 
                    local_vertices_data[remove_index].color_vec = {-1.0f, -1.0f, -1.0f, -1.0f};

                for(size_t remove_index = (*p_clickinfo->p_window_objects)[i].indices_bounds.first; remove_index < (*p_clickinfo->p_window_objects)[i].indices_bounds.second; remove_index++) 
                    local_indices_data[remove_index] = -1;
            }
        }

        // clean the vectors
        // for(size_t i = 0; i < local_vertices_data.size(); i++) {
        //     if(local_vertices_data[i].color_vec.first == -1.0f && local_vertices_data[i].color_vec.second == -1.0f && local_vertices_data[i].color_vec.third == -1.0f && local_vertices_data[i].color_vec.fourth == -1.0f) {
        //         local_vertices_data.erase(local_vertices_data.begin() + i);
        //     }
        // }

        // for(size_t i = 0; i < local_indices_data.size(); i++) {
        //     if(local_indices_data[i] == -1) {
        //         local_indices_data.erase(local_indices_data.begin() + i);
        //     }
        // }
        
        VkDeviceSize local_size = static_cast<VkDeviceSize>(sizeof((*p_clickinfo->p_vertices_data)[0]) * p_clickinfo->p_vertices_data->size());
        
        p_clickinfo->p_bufferinfo->p_buffer_create_func(p_clickinfo->p_windowinfo->p_device, p_clickinfo->p_windowinfo->p_gpu, &local_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, p_clickinfo->p_bufferinfo->p_staging_buffer, p_clickinfo->p_bufferinfo->p_staging_buffer_memory, nullptr);
        p_clickinfo->p_bufferinfo->p_buffer_memory_populate_func(p_clickinfo->p_windowinfo->p_device, p_clickinfo->p_windowinfo->p_gpu, &local_size, local_vertices_data.data(), p_clickinfo->p_bufferinfo->p_staging_buffer, p_clickinfo->p_bufferinfo->p_staging_buffer_memory);
        p_clickinfo->p_bufferinfo->p_buffer_copy_func(p_clickinfo->p_windowinfo->p_device, p_clickinfo->p_bufferinfo->p_commandpool, p_clickinfo->p_bufferinfo->p_graphics_queue, p_clickinfo->p_bufferinfo->p_staging_buffer, p_clickinfo->p_bufferinfo->p_vertices_buffer, &local_size);
        
        vkDestroyBuffer(*p_clickinfo->p_windowinfo->p_device, *p_clickinfo->p_bufferinfo->p_staging_buffer, nullptr);
        vkFreeMemory(*p_clickinfo->p_windowinfo->p_device, *p_clickinfo->p_bufferinfo->p_staging_buffer_memory, nullptr);

        local_size = static_cast<VkDeviceSize>(sizeof((*p_clickinfo->p_indices_data)[0]) * p_clickinfo->p_indices_data->size());
        p_clickinfo->p_bufferinfo->p_buffer_create_func(p_clickinfo->p_windowinfo->p_device, p_clickinfo->p_windowinfo->p_gpu, &local_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, p_clickinfo->p_bufferinfo->p_staging_buffer, p_clickinfo->p_bufferinfo->p_staging_buffer_memory, nullptr);
        p_clickinfo->p_bufferinfo->p_buffer_memory_populate_func(p_clickinfo->p_windowinfo->p_device, p_clickinfo->p_windowinfo->p_gpu, &local_size, local_indices_data.data(), p_clickinfo->p_bufferinfo->p_staging_buffer, p_clickinfo->p_bufferinfo->p_staging_buffer_memory);
        p_clickinfo->p_bufferinfo->p_buffer_copy_func(p_clickinfo->p_windowinfo->p_device, p_clickinfo->p_bufferinfo->p_commandpool, p_clickinfo->p_bufferinfo->p_graphics_queue, p_clickinfo->p_bufferinfo->p_staging_buffer, p_clickinfo->p_bufferinfo->p_indices_buffer, &local_size);
    
        vkDestroyBuffer(*p_clickinfo->p_windowinfo->p_device, *p_clickinfo->p_bufferinfo->p_staging_buffer, nullptr);
        vkFreeMemory(*p_clickinfo->p_windowinfo->p_device, *p_clickinfo->p_bufferinfo->p_staging_buffer_memory, nullptr);
    }

    void Events::checkForClicks(ClickInfo *p_clickinfo, WindowObjectHandleList *p_handle_list) {
        if(this->m_p_window->getInputMode() == DENG_INPUT_NONMOVEMENT) {
            dengMath::vec2<double> local_mouse_positions;

            glfwGetCursorPos(this->m_p_window->getWindow(), &local_mouse_positions.first, &local_mouse_positions.second);
            if(this->m_p_pixel_collision->isCollided(p_clickinfo->description, {static_cast<size_t>(local_mouse_positions.first), static_cast<size_t>(local_mouse_positions.second)})) {
                int local_mouse_button_status = glfwGetMouseButton(this->m_p_window->getWindow(), GLFW_MOUSE_BUTTON_1);

                switch (local_mouse_button_status)
                {
                case GLFW_PRESS: {
                    if(!(*p_clickinfo->p_is_clicked)) {
                        p_handle_list->callClickHandler(p_clickinfo->handle_id);
                        this->updateBuffers(p_clickinfo);
                        *p_clickinfo->p_is_clicked = DENG_TRUE;
                    }
                    break;
                }

                case GLFW_RELEASE:
                    if(*p_clickinfo->p_is_clicked) {
                        *p_clickinfo->p_is_clicked = DENG_FALSE;
                    }
                    break;
                
                default:
                    break;
                }
            }    
        }
    }
};