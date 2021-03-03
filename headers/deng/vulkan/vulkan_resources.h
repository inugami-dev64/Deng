#ifndef __VULKAN_RESOURCES_H
#define __VULKAN_RESOURCES_H

#define DENG_MAX_GPU_NAME_SIZE 256

#ifdef __VULKAN_RESOURCES_CPP
    #include <vector>
    #include <string>
    #include <string.h>
    #include <stdexcept>
    #include <vulkan/vulkan.h>

    #include <common/base_types.h>
    #include <common/err_def.h>
    #include <das/assets.h>
#endif
namespace deng {
    namespace vulkan {
        /*
         * Store information about graphics device
         * This struct is used to store information about graphics device properties
         */
        struct DeviceInfo {
            deng_ui32_t driver_version;
            deng_ui32_t vk_api_version;
            char *gpu_name;
        };


        /*
         * Get information about graphics hardware
         * This struct contains methods to get information about graphics hardware
         */
        struct HardwareSpecs {
            /*
             * Check if requested extension is supported
             * This method gets information about supported Vulkan extensions and checks  
             * if requested extension is one of them
             */
            static deng_bool_t getExtensionSupport (
                const VkPhysicalDevice &gpu, 
                char *ext_name
            );


            /*
             * Find device memory type
             * This method finds appropriate memory type by VkMemoryPropertyFlags
             */
            static deng_ui32_t getMemoryType (
                const VkPhysicalDevice &gpu, 
                deng_ui32_t type_filter, 
                VkMemoryPropertyFlags properties
            );


            /*
             * Find device score
             * Rate logical device based on its capabilities
             */
            static deng_ui32_t getDeviceScore (
                const VkPhysicalDevice &device, 
                std::vector<const char*> &exts
            );


            /*
             * Find information about graphics device
             * This method gets information about graphics device name, Vulkan api
             * version and driver version
             */
            static DeviceInfo getGpuInfo(const VkPhysicalDevice &gpu);
        };


        /* 
         * Struct for storing buffers
         * This struct contains all buffers and their memory objects for
         * vulkan renderer
         */
        struct BufferData {
            VkBuffer staging_buffer;
            VkDeviceMemory staging_buffer_memory;

            VkBuffer main_buffer;
            VkDeviceMemory main_buffer_memory;
            
            std::vector<deng_ObjColorData> colors;

            std::vector<VkBuffer> mat_uniform_buffers;
            std::vector<VkDeviceMemory> mat_uniform_buffer_mem;
        };

        
        /* 
         * Struct for texture images
         * This struct contains textures and their Vulkan image buffers, views, samplers 
         * and memory objects
         */
        struct TextureImage {
            deng_Texture texture;
            VkImage image;
            VkImageView image_view;
            VkSampler sampler;
            VkDeviceMemory image_mem;
            std::vector<VkDescriptorSet> descriptor_sets;
        };

        
        /* 
         * Contains method for memory allocation 
         * This struct is inherited to ImageCreator and BufferCreator structs
         */
        struct MemoryAllocator {
            /*
             * Allocate graphics memory using Vulkan
             * This method is used to allocate memory for VkBuffer and VkImage objects
             */
            static void allocateMemory (
                const VkDevice &device, 
                const VkPhysicalDevice &gpu,  
                VkDeviceSize size, 
                VkDeviceMemory &memory,
                deng_ui32_t mem_type_bits, 
                VkMemoryPropertyFlags properties
            );
        };



        /* 
         * Container for all VkImage handling methods
         */
        struct ImageCreator : MemoryAllocator {
            /*
             * Create new VkImageCreateInfo instance
             * This method is used to simplify VkImageViewCreateInfo creation
             */
            static VkImageViewCreateInfo getImageViewInfo (
                const VkImage &image, 
                VkFormat format, 
                VkImageAspectFlags aspect_flags,
                deng_ui32_t mip_levels
            );

            /*
             * Create new VkImage instance
             * This method creates new VkImage instance and returns 
             * memory allocation requirements for this image
             */
            static VkMemoryRequirements makeImage (
                const VkDevice &device, 
                const VkPhysicalDevice &gpu, 
                VkImage &image, 
                deng_ui32_t width, 
                deng_ui32_t height, 
                deng_ui32_t mip_levels,
                VkFormat format, 
                VkImageTiling tiling, 
                VkImageUsageFlags usage,
                VkSampleCountFlagBits sample_c
            );

            /*
             * Transition VkImage from one layout to another
             * This method uses VkImageMemoryBarrier to transition image layout to new_layout
             */
            static void transitionImageLayout (
                const VkDevice &device, 
                const VkImage &image, 
                const VkCommandPool &commandpool, 
                const VkQueue &g_queue, 
                VkFormat format, 
                VkImageLayout old_layout, 
                VkImageLayout new_layout,
                deng_ui32_t mip_levels
            ); 

            /*
             * Copy VkBuffer to VkImage instance
             * This method copies data from src_buffer to dst_image
             */
            static void copyBufferToImage (
                const VkDevice &device, 
                const VkCommandPool &commandpool, 
                const VkQueue &g_queue, 
                const VkBuffer &src_buffer, 
                const VkImage &dst_image, 
                deng_ui32_t width, 
                deng_ui32_t height
            );
        };

        
        /* 
         * Container for buffer handling methods
         */
        struct BufferCreator : MemoryAllocator {
            /*
             * Create new VkBuffer instance
             * This method creates new VkImage instance and returns 
             * memory allocation requirements for this image
             */
            static VkMemoryRequirements makeBuffer (
                const VkDevice &device, 
                const VkPhysicalDevice &gpu, 
                VkDeviceSize size, 
                VkBufferUsageFlags usage, 
                VkBuffer &buffer
            );
            
            /*
             * Copy data to graphics memory
             * This method copies size bytes of data from src_data to buf_mem using vkMapMemory
             */
            static void cpyToBufferMem (
                const VkDevice &device, 
                VkDeviceSize size, 
                void *src_data, 
                const VkDeviceMemory &buf_mem, 
                VkDeviceSize offset
            );
        
            /*
             * Copy buffer to other buffer
             * This method copies data from src_buffer to dst_buffer
             */
            static void copyBufferToBuffer (
                const VkDevice &device, 
                const VkCommandPool &commandpool, 
                const VkQueue &g_queue, 
                const VkBuffer &src_buffer, 
                const VkBuffer &dst_buffer, 
                VkDeviceSize size, 
                VkDeviceSize offset
            );
        };


        /* 
         * Container for single use command buffers' allocation and deallocation
         */
        struct CommandBufferRecorder {
            /*
             * Start recording commandbuffer
             * This method allocates and begins recording commandbuffers meant for single use
             */
            static void beginCommandBufferSingleCommand (
                VkDevice device, 
                VkCommandPool commandpool, 
                VkCommandBuffer *p_commandbuffer
            );

            /* 
             * Finish recording commandbuffer
             * This method end commandbuffer recording, submits it into graphics queue
             * and frees commandbuffers
             */
            static void endCommandBufferSingleCommand (
                VkDevice device, 
                VkQueue graphics_queue, 
                VkCommandPool commandpool, 
                VkCommandBuffer *p_commandBuffer
            );
        };
    }
}

#endif
