#include "sprites.h"

namespace dengUI {

    struct WindowInfo {
        deng::vec2<float> size;
        deng::vec2<float> origin;
        deng::vec2<float> position;
        deng::vec4<float> color;

        VkDevice *p_device;
        VkPhysicalDevice *p_gpu;
        
        std::string vert_shader_path;
        std::string frag_shader_path;

        std::string window_name;
        deng::FileManager *p_file_manager;
    };

    struct BufferInfo {
        VkBuffer *p_vertices_buffer;
        VkDeviceMemory *p_vertices_buffer_memory;

        VkBuffer *p_indices_buffer;
        VkDeviceMemory *p_indices_buffer_memory;

        uint32_t *p_indices_size;

        VkBuffer *p_staging_buffer;
        VkDeviceMemory *p_staging_buffer_memory;

        VkCommandPool *p_commandpool;
        VkQueue *p_graphics_queue;

        BufferCreateFunc p_buffer_create_func;
        BufferMemoryPopulateFunc p_buffer_memory_populate_func;
        BufferCopyFunc p_buffer_copy_func;
    };
    
    class Window {
    private:
        std::vector<UIVerticesData> m_vertices_data;
        std::vector<uint16_t> m_indices_data;
        WindowInfo m_windowinfo;
        BufferInfo m_bufferinfo; 

        RectangleSprite *m_p_main_rectangle;
        // RectangleSprite *m_p_taskbar_rectangle;

    private: 
        void createMainWindow();
        void createBuffers();

    public:
        Window(WindowInfo &p_windowinfo, BufferInfo &p_bufferinfo);
        ~Window();
        void update();
        // uint32_t getVerticesCount();
    };
}