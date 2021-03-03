#define __VULKAN_RESOURCES_CPP
#include <deng/vulkan/vulkan_resources.h>

namespace deng {
    namespace vulkan {
        /********************************/
        /******** HardwareSpecs *********/
        /********************************/

        /*
         * Check if requested extension is supported
         * This method gets information about supported Vulkan extensions and checks  
         * if requested extension is one of them
         */
        deng_bool_t HardwareSpecs::getExtensionSupport (
            const VkPhysicalDevice &gpu, 
            char *ext_name
        ) {
            // Count total amount of supported extensions  
            LOG("Finding support for extension " + std::string(ext_name));
            deng_ui32_t extension_count;
            vkEnumerateDeviceExtensionProperties (
                gpu, 
                nullptr, 
                &extension_count, 
                nullptr
            );
            
            // Get extensions by names
            std::vector<VkExtensionProperties> all_extensions(extension_count);
            vkEnumerateDeviceExtensionProperties (
                gpu, 
                nullptr, 
                &extension_count, 
                all_extensions.data()
            );
            
            // Iterate through all extension to find matching extension
            for(VkExtensionProperties &extension : all_extensions) {
                const char *current_extenstion_name = extension.extensionName;
                
                if(std::string(current_extenstion_name) == std::string(ext_name)) {
                    LOG("Detected support for " + std::string(current_extenstion_name));
                    return true;
                }
            }

            return false;
        }


        /*
         * Find device memory type
         * This method finds appropriate memory type by VkMemoryPropertyFlags
         */
        deng_ui32_t HardwareSpecs::getMemoryType (
            const VkPhysicalDevice &gpu, 
            deng_ui32_t type_filter, 
            VkMemoryPropertyFlags properties
        ) {
            // Get all device memory properties
            VkPhysicalDeviceMemoryProperties memory_properties;
            
            vkGetPhysicalDeviceMemoryProperties (
                gpu, 
                &memory_properties
            );

            for(deng_ui32_t index = 0; index < memory_properties.memoryTypeCount; index++) {
                if(type_filter & (1 << index) && (memory_properties.memoryTypes[index].propertyFlags & properties))
                    return index;
            }

            VK_BUFFER_ERR("failed to find suitable memory type");
        }


        /*
         * Find device score
         * Rate logical device based on its capabilities
         */
        deng_ui32_t HardwareSpecs::getDeviceScore (
            const VkPhysicalDevice &gpu, 
            std::vector<const char*> &required_extenstions
        ) {
            deng_ui32_t score = 0;
            VkPhysicalDeviceFeatures device_features;
            VkPhysicalDeviceProperties device_properties;

            vkGetPhysicalDeviceProperties (
                gpu, 
                &device_properties
            );
            
            vkGetPhysicalDeviceFeatures (
                gpu, 
                &device_features
            );

            if(device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) score += 1000;
            
            if(device_features.fillModeNonSolid) score += 500;

            score += device_properties.limits.maxImageDimension2D;
            score += device_properties.limits.maxImageDimension3D;
            score += device_properties.limits.maxMemoryAllocationCount;
            score += device_properties.limits.maxVertexOutputComponents;

            if(!device_features.geometryShader) 
                return 0;
            if(!device_features.samplerAnisotropy) 
                return 0;

            // Check if all the extension are supported by the device  
            for(const char* extension_name : required_extenstions) {
                if(!HardwareSpecs::getExtensionSupport(gpu, (char*) extension_name)) {
                    LOG("Required extension: " + std::string(extension_name) + " is not supported!");
                    return 0;
                }
            }

            return score;
        }


        /*
         * Find information about graphics device
         * This method gets information about graphics device name, Vulkan api
         * version and driver version
         */
        DeviceInfo HardwareSpecs::getGpuInfo(const VkPhysicalDevice &gpu) {
            DeviceInfo dev_info;
            VkPhysicalDeviceProperties gpu_props{};
            vkGetPhysicalDeviceProperties (
                gpu,
                &gpu_props
            );

            dev_info.driver_version = gpu_props.driverVersion;
            dev_info.vk_api_version = gpu_props.apiVersion;

            
            dev_info.gpu_name = (char*) calloc (
                DENG_MAX_GPU_NAME_SIZE,
                sizeof(char)
            );

            strcpy(dev_info.gpu_name, gpu_props.deviceName);

            return dev_info;
        }


        /**********************************/
        /******** MemoryAllocator *********/
        /**********************************/
        
        /*
         * Allocate graphics memory using Vulkan
         * This method is used to allocate memory for VkBuffer and VkImage objects
         */
        void MemoryAllocator::allocateMemory (
            const VkDevice &device, 
            const VkPhysicalDevice &gpu, 
            VkDeviceSize size,
            VkDeviceMemory &memory,  
            deng_ui32_t mem_type_bits, 
            VkMemoryPropertyFlags properties
        ) {
            VkMemoryAllocateInfo allocinfo{};
            allocinfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocinfo.allocationSize = size;
            
            allocinfo.memoryTypeIndex = HardwareSpecs::getMemoryType (
                gpu, 
                mem_type_bits, 
                properties
            );

            if(vkAllocateMemory(device, &allocinfo, nullptr, &memory) != VK_SUCCESS)
                VK_BUFFER_ERR("failed to allocate buffer memory!");
        }


        /**********************************/
        /********** ImageCreator **********/
        /**********************************/

        
        /*
         * Create new VkImageCreateInfo instance
         * This method is used to simplify VkImageViewCreateInfo creation
         */
        VkImageViewCreateInfo ImageCreator::getImageViewInfo (
            const VkImage &image, 
            VkFormat format, 
            VkImageAspectFlags aspect_flags,
            deng_ui32_t mip_levels
        ) {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = image;
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = format;

            createInfo.subresourceRange.aspectMask = aspect_flags;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = mip_levels;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;
            return createInfo;
        }



        /*
         * Create new VkImage instance
         * This method creates new VkImage instance and returns 
         * VkMemoryRequirements for this image
         */
        VkMemoryRequirements ImageCreator::makeImage (
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
        ) {
            // Set up image createinfo
            VkImageCreateInfo image_createInfo{};
            image_createInfo.sType  = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            image_createInfo.imageType = VK_IMAGE_TYPE_2D;
            image_createInfo.extent.width = width;
            image_createInfo.extent.height = height;
            image_createInfo.extent.depth = 1;
            image_createInfo.mipLevels = mip_levels;
            image_createInfo.arrayLayers = 1;
            image_createInfo.format = format;
            image_createInfo.tiling = tiling;
            image_createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            image_createInfo.usage = usage;
            image_createInfo.samples = sample_c;
            image_createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            // Create image
            if(vkCreateImage(device, &image_createInfo, nullptr, &image) != VK_SUCCESS) 
                VK_BUFFER_ERR("failed to create image!");

            // Get memory requirements for this image
            VkMemoryRequirements memory_requirement;
            vkGetImageMemoryRequirements (
                device, 
                image, 
                &memory_requirement
            );
            
            return memory_requirement;
        }


        /*
         * Transition VkImage from one layout to another
         * This method uses VkImageMemoryBarrier to transition image layout to new_layout
         */
        void ImageCreator::transitionImageLayout (
            const VkDevice &device, 
            const VkImage &image, 
            const VkCommandPool &commandpool, 
            const VkQueue &g_queue, 
            VkFormat format, 
            VkImageLayout old_layout, 
            VkImageLayout new_layout,
            deng_ui32_t mip_levels
        ) {
            // Begin recording commandbuffer
            VkCommandBuffer tmp_commandbuffer;
            CommandBufferRecorder::beginCommandBufferSingleCommand (
                device, 
                commandpool, 
                &tmp_commandbuffer
            );

            // Set up image memory barrier struct
            VkImageMemoryBarrier memory_barrier{};
            memory_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            memory_barrier.oldLayout = old_layout;
            memory_barrier.newLayout = new_layout;
            memory_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            memory_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            memory_barrier.image = image;
            memory_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            memory_barrier.subresourceRange.baseMipLevel = 0;
            memory_barrier.subresourceRange.levelCount = mip_levels;
            memory_barrier.subresourceRange.baseArrayLayer = 0;
            memory_barrier.subresourceRange.layerCount = 1;

            // Set up pipeline stage flags
            VkPipelineStageFlags src_stage;
            VkPipelineStageFlags dst_stage;
                    
            if(old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
                memory_barrier.srcAccessMask = 0;
                memory_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

                src_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                dst_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            }

            else if(old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
                memory_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                memory_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

                src_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                dst_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            }

            else 
                VK_BUFFER_ERR("invalid image layout transition");
        
            // Change image layout
            vkCmdPipelineBarrier (
                tmp_commandbuffer, 
                src_stage, 
                dst_stage, 
                0, 
                0, 
                nullptr, 
                0, 
                nullptr, 
                1, 
                &memory_barrier
            );

            CommandBufferRecorder::endCommandBufferSingleCommand (
                device, 
                g_queue, 
                commandpool, 
                &tmp_commandbuffer
            );
        }


        /*
         * Copy VkBuffer to VkImage instance
         * This method copies data from src_buffer to dst_image
         */
        void ImageCreator::copyBufferToImage (
            const VkDevice &device, 
            const VkCommandPool &commandpool, 
            const VkQueue &g_queue, 
            const VkBuffer &src_buffer, 
            const VkImage &dst_image, 
            deng_ui32_t width, 
            deng_ui32_t height
        ) {
            // Begin recording commandbuffer
            VkCommandBuffer tmp_commandbuffer;
            CommandBufferRecorder::beginCommandBufferSingleCommand (
                device, 
                commandpool, 
                &tmp_commandbuffer
            );

            // Set up buffer image copy struct
            VkBufferImageCopy copy_region{};
            copy_region.bufferOffset = 0;
            copy_region.bufferRowLength = 0;
            copy_region.bufferImageHeight = 0;
            copy_region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            copy_region.imageSubresource.mipLevel = 0;
            copy_region.imageSubresource.baseArrayLayer = 0;
            copy_region.imageSubresource.layerCount = 1;
            copy_region.imageOffset = {0, 0, 0};
            copy_region.imageExtent = {width, height, 1};
            
            // Call Vulkan buffer to image copy handler
            vkCmdCopyBufferToImage (
                tmp_commandbuffer, 
                src_buffer, 
                dst_image, 
                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                1, 
                &copy_region
            );

            // End recording commandbuffer
            CommandBufferRecorder::endCommandBufferSingleCommand (
                device, 
                g_queue, 
                commandpool, 
                &tmp_commandbuffer
            );
        }


        /**********************************/
        /********* BufferCreator **********/
        /**********************************/
        
        /*
         * Create new VkBuffer instance
         * This method creates new VkImage instance and returns 
         * memory allocation requirements for this image
         */
        VkMemoryRequirements BufferCreator::makeBuffer (
            const VkDevice &device, 
            const VkPhysicalDevice &gpu, 
            VkDeviceSize size, 
            VkBufferUsageFlags usage, 
            VkBuffer &buffer
        ) {
            // Set up buffer createinfo struct 
            VkBufferCreateInfo buffer_createInfo{};
            buffer_createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            buffer_createInfo.size = size;
            buffer_createInfo.usage = usage;
            buffer_createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            // Call Vulkan buffer creation handler 
            if(vkCreateBuffer(device, &buffer_createInfo, nullptr, &buffer) != VK_SUCCESS)
                VK_BUFFER_ERR("Failed to create a buffer!");

            // Get memory requirements for the buffer
            VkMemoryRequirements memory_requirement;
            vkGetBufferMemoryRequirements (
                device, 
                buffer, 
                &memory_requirement
            );

            return memory_requirement;
        }


        /*
         * Copy data to graphics memory
         * This method copies size bytes of data from src_data to buf_mem using vkMapMemory
         */
        void BufferCreator::cpyToBufferMem (
            const VkDevice &device, 
            VkDeviceSize size, 
            void *src_data, 
            const VkDeviceMemory &buf_mem, 
            VkDeviceSize offset
        ) {
            void *buf;
            vkMapMemory (
                device, 
                buf_mem, 
                offset, 
                size, 
                0, 
                &buf
            );
                memcpy (
                    buf, 
                    src_data, 
                    size
                );

            vkUnmapMemory (
                device, 
                buf_mem
            );
        }


        /*
         * Copy buffer to other buffer
         * This method copies data from src_buffer to dst_buffer
         */
        void BufferCreator::copyBufferToBuffer (
            const VkDevice &device, 
            const VkCommandPool &commandpool, 
            const VkQueue &g_queue, 
            const VkBuffer &src_buffer, 
            const VkBuffer &dst_buffer, 
            VkDeviceSize size, 
            VkDeviceSize offset
        ) {
            // Begin recording commandbuffer
            VkCommandBuffer tmp_commandbuffer;
            CommandBufferRecorder::beginCommandBufferSingleCommand (
                device, 
                commandpool, 
                &tmp_commandbuffer
            );

            // Set up copy region 
            VkBufferCopy copy_region{};
            copy_region.srcOffset = 0;
            copy_region.dstOffset = offset;
            copy_region.size = size;
            
            // Call Vulkan buffer copy handler
            vkCmdCopyBuffer (
                tmp_commandbuffer, 
                src_buffer, 
                dst_buffer, 
                1, 
                &copy_region
            );
            
            // End recording commandbuffer
            CommandBufferRecorder::endCommandBufferSingleCommand (
                device, 
                g_queue, 
                commandpool, 
                &tmp_commandbuffer
            );
        }


        /******************************************/
        /********* CommandBufferRecorder **********/
        /******************************************/


        /*
         * Start recording commandbuffer
         * This method allocates and begins recording commandbuffers meant for single use
         */
        void CommandBufferRecorder::beginCommandBufferSingleCommand (
            VkDevice device, 
            VkCommandPool commandpool, 
            VkCommandBuffer *p_commandbuffer
        ) {
            // Set up commandbuffer allocation info
            VkCommandBufferAllocateInfo commandbuffer_allocinfo{};
            commandbuffer_allocinfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            commandbuffer_allocinfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            commandbuffer_allocinfo.commandPool = commandpool;
            commandbuffer_allocinfo.commandBufferCount = 1;

            // Allocate commandbuffers
            vkAllocateCommandBuffers (
                device, 
                &commandbuffer_allocinfo, 
                p_commandbuffer
            );
            
            // Set up command buffer begin info
            VkCommandBufferBeginInfo commandbuffer_begininfo{};
            commandbuffer_begininfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            commandbuffer_begininfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

            // Begin commandbuffers
            if(vkBeginCommandBuffer(*p_commandbuffer, &commandbuffer_begininfo) != VK_SUCCESS)
                VK_RES_ERR("failed to start command recording buffer!");
        }


        /* 
         * Finish recording commandbuffer
         * This method end commandbuffer recording, submits it into graphics queue
         * and frees commandbuffers
         */
        void CommandBufferRecorder::endCommandBufferSingleCommand (
            VkDevice device, 
            VkQueue g_queue, 
            VkCommandPool commandpool, 
            VkCommandBuffer *p_commandbuffer
        ) {
            // Call Vulkan command buffer end handler
            vkEndCommandBuffer(*p_commandbuffer);

            // Set up submit info 
            VkSubmitInfo submitinfo{};
            submitinfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitinfo.commandBufferCount = 1;
            submitinfo.pCommandBuffers = p_commandbuffer;
            
            // Submit to the graphics queue
            vkQueueSubmit (
                g_queue, 
                1, 
                &submitinfo, 
                VK_NULL_HANDLE
            );
            vkQueueWaitIdle(g_queue);

            // Cleanup
            vkFreeCommandBuffers (
                device, 
                commandpool, 
                1, 
                p_commandbuffer
            );
        }
    }
}
