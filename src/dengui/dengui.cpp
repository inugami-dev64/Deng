#include "../core/dengcore.h"

namespace dengUI {

    Window::Window(WindowInfo &p_windowinfo) {
        this->m_windowinfo = p_windowinfo;
        this->createMainWindow();   
        this->createBuffers();
    }

    Window::~Window() {
        vkDestroyBuffer(*this->m_windowinfo.p_device, *this->m_windowinfo.p_vertices_buffer, nullptr);
        vkDestroyBuffer(*this->m_windowinfo.p_device, *this->m_windowinfo.p_indices_buffer, nullptr);
        vkFreeMemory(*this->m_windowinfo.p_device, *this->m_windowinfo.p_vertices_buffer_memory, nullptr);
        vkFreeMemory(*this->m_windowinfo.p_device, *this->m_windowinfo.p_indices_buffer_memory, nullptr);
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
        LOG("Seg test!");
    }

    void Window::createBuffers() {
        LOG("Creating buffers for window!");
        VkBufferCreateInfo local_vertices_buffer_createinfo{};
        local_vertices_buffer_createinfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        local_vertices_buffer_createinfo.size = static_cast<VkDeviceSize>(this->m_vertices_data.size());
        local_vertices_buffer_createinfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        local_vertices_buffer_createinfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        VkBufferCreateInfo local_indices_buffer_createinfo{};
        local_indices_buffer_createinfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        local_indices_buffer_createinfo.size = static_cast<VkDeviceSize>(this->m_indices_data.size());
        local_indices_buffer_createinfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        local_indices_buffer_createinfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        LOG("seg test!");
        if(vkCreateBuffer(*this->m_windowinfo.p_device, &local_vertices_buffer_createinfo, nullptr, this->m_windowinfo.p_vertices_buffer) != VK_SUCCESS) {
            ERR("Failed to create ui vertices' buffer!");
        }

        if(vkCreateBuffer(*this->m_windowinfo.p_device, &local_indices_buffer_createinfo, nullptr, this->m_windowinfo.p_indices_buffer) != VK_SUCCESS) {
            ERR("Failed to create ui indices buffer!");
        }
        
        VkMemoryRequirements local_vertices_memory_requirements;
        vkGetBufferMemoryRequirements(*this->m_windowinfo.p_device, *this->m_windowinfo.p_vertices_buffer, &local_vertices_memory_requirements);

        VkMemoryRequirements local_indices_memory_requirements;
        vkGetBufferMemoryRequirements(*this->m_windowinfo.p_device, *this->m_windowinfo.p_indices_buffer, &local_indices_memory_requirements);

        deng::HardwareSpecs local_hardware_specs;

        VkMemoryAllocateInfo local_vertices_memory_allocinfo{};
        local_vertices_memory_allocinfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        local_vertices_memory_allocinfo.allocationSize = local_vertices_memory_requirements.size;
        local_vertices_memory_allocinfo.memoryTypeIndex = local_hardware_specs.getMemoryType(*this->m_windowinfo.p_gpu, local_vertices_memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        VkMemoryAllocateInfo local_indices_memory_allocinfo{};
        local_indices_memory_allocinfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        local_indices_memory_allocinfo.allocationSize = local_vertices_memory_requirements.size;
        local_vertices_memory_requirements.memoryTypeBits = local_hardware_specs.getMemoryType(*this->m_windowinfo.p_gpu, local_indices_memory_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        if(vkAllocateMemory(*this->m_windowinfo.p_device, &local_vertices_memory_allocinfo, nullptr, this->m_windowinfo.p_vertices_buffer_memory) != VK_SUCCESS) {
            ERR("Failed to allocate memory for window vertices buffer!");
        }

        if(vkAllocateMemory(*this->m_windowinfo.p_device, &local_indices_memory_allocinfo, nullptr, this->m_windowinfo.p_indices_buffer_memory) != VK_SUCCESS) {
            ERR("Failed to allocate memory for window indices buffer!");
        }

        // this->m_vertices_data = {{{-0.5, -0.5, 0.0}, {0.1, 0.0, 0.0, 1.0}},
        //                           {{0.5, -0.5, 0.0}, {0.1, 0.0, 0.0, 1.0}},
        //                           {{0.5, 0.5, 0.0}, {0.1, 0.0, 0.0, 1.0}},
        //                           {{-0.5, 0.5, 0.0}, {0.1, 0.0, 0.0, 1.0}}};

        // this->m_indices_data = {0, 1, 2, 2, 3, 0};

        vkBindBufferMemory(*this->m_windowinfo.p_device, *this->m_windowinfo.p_vertices_buffer, *this->m_windowinfo.p_vertices_buffer_memory, 0);
        vkBindBufferMemory(*this->m_windowinfo.p_device, *this->m_windowinfo.p_indices_buffer, *this->m_windowinfo.p_indices_buffer_memory, 0);

        void *local_data;
        vkMapMemory(*this->m_windowinfo.p_device, *this->m_windowinfo.p_vertices_buffer_memory, 0, static_cast<VkDeviceSize>(this->m_vertices_data.size()), 0, &local_data);
            memcpy(local_data, this->m_vertices_data.data(), this->m_vertices_data.size());
        vkUnmapMemory(*this->m_windowinfo.p_device, *this->m_windowinfo.p_vertices_buffer_memory);

        local_data = nullptr;

        vkMapMemory(*this->m_windowinfo.p_device, *this->m_windowinfo.p_indices_buffer_memory, 0, static_cast<VkDeviceSize>(this->m_indices_data.size()), 0, &local_data);
            memcpy(local_data, this->m_indices_data.data(), this->m_indices_data.size());
        vkUnmapMemory(*this->m_windowinfo.p_device, *this->m_windowinfo.p_indices_buffer_memory);
    }

    uint32_t Window::getVerticesCount() {
        return static_cast<uint32_t>(this->m_indices_data.size());
    }
}