#include "../core/dengcore.h"

namespace dengUI {

    Window::Window(WindowInfo &windowinfo, BufferInfo &bufferinfo) {
        this->m_windowinfo = windowinfo;
        this->m_bufferinfo = bufferinfo;

        this->createMainWindow();   
        this->createBuffers();
    }

    Window::~Window() {
        vkDestroyBuffer(*this->m_windowinfo.p_device, *this->m_bufferinfo.p_vertices_buffer, nullptr);
        vkDestroyBuffer(*this->m_windowinfo.p_device, *this->m_bufferinfo.p_indices_buffer, nullptr);
        vkFreeMemory(*this->m_windowinfo.p_device, *this->m_bufferinfo.p_vertices_buffer_memory, nullptr);
        vkFreeMemory(*this->m_windowinfo.p_device, *this->m_bufferinfo.p_indices_buffer_memory, nullptr);
        delete this->m_p_main_rectangle;
    }

    void Window::createMainWindow() {
        RectangleSpriteInfo local_rectangle_spriteinfo{};
        local_rectangle_spriteinfo.color = this->m_windowinfo.color;
        local_rectangle_spriteinfo.size = this->m_windowinfo.size;
        local_rectangle_spriteinfo.origin = this->m_windowinfo.origin;
        local_rectangle_spriteinfo.position = this->m_windowinfo.position;
        LOG("Creating new window!");

        this->m_p_main_rectangle = new RectangleSprite(&local_rectangle_spriteinfo);
        this->m_p_main_rectangle->getVertices(&this->m_vertices_data, &this->m_indices_data);
        // this->m_vertices_data = {{{-0.5f, -0.5f}, {0.5f, 0.0f, 0.0f, 0.2f}},
        //                          {{0.5f, -0.5f}, {0.5f, 0.0f, 0.0f, 0.2f}},
        //                          {{0.5f, 0.5f}, {0.5f, 0.0f, 0.0f, 0.2f}},
        //                          {{-0.5f, 0.5f}, {0.5f, 0.0f, 0.0f, 0.2f}}};

        // this->m_indices_data = {0, 1, 2, 2, 3, 0};
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
}