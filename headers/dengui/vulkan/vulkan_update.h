#ifndef __VULKAN_UPDATE_H
#define __VULKAN_UPDATE_H

#ifdef __VULKAN_UPDATE_CPP
    #include <mutex>
    #include <vector>
    #include <array>
    #include <vulkan/vulkan.h>

    #include <deng/forward_dec.h>
    #include <common/base_types.h>
    #include <das/assets.h>

    #include <deng/deng_math.h>
    #include <deng/surface_window.h>
    #include <deng/vulkan/vulkan_qm.h>
    #include <deng/vulkan/vulkan_sd.h>
    #include <deng/vulkan/vulkan_resources.h>
    #include <deng/vulkan/vulkan_rend_infos.h>
    #include <deng/vulkan/vulkan_renderer.h>
    #include <dengui/dengui_infos.h>
#endif


namespace dengui {
    namespace vulkan {
        /* 
         * This class is used for updating command and vertex buffers as well as
         * texture descriptors on the second thread 
         */
        class __FrameUpdater {
        private:
            EventInfo *m_p_info;
        
        public:
            __FrameUpdater(EventInfo *p_info);

            /* Asset mutex lockers */
            void lockAssets();
            void unlockAssets();

            /* Frame mutex lockers */
            void lockFrame();
            void unlockFrame();

            /* 
             * External update caller methods 
             * Frame locking is required for any of these method calls to work correctly!   
             */
            void reallocBuffer();
            void updateVerts(dengMath::vec2<deng_ui32_t> remap_bounds);
            void updateTexDS (
                deng_bool_t realloc_ds, 
                dengMath::vec2<deng_ui32_t> *p_tex_bounds
            );
            void updateTextures(dengMath::vec2<deng_ui32_t> tex_bounds);
            void updateCmdBuffers();
        };
    }
}

#endif
