#define __VULKAN_UPDATE_CPP
#include <dengui/vulkan/vulkan_update.h>


namespace dengui {
    namespace vulkan {
        /***************************************/
        /********* FRAME UPDATER CLASS *********/
        /***************************************/
        __FrameUpdater::__FrameUpdater(EventInfo *p_info) 
        { m_p_info = p_info; }


        /* External asset mutex lockers and unlockers */
        void __FrameUpdater::lockAssets() {
            m_p_info->p_res_mut->lock();
        }


        void __FrameUpdater::unlockAssets() {
            m_p_info->p_res_mut->unlock();
        }

        
        /* External frame mutex lockers and unlockers */
        void __FrameUpdater::lockFrame() {
            m_p_info->p_frame_mut->lock();
            vkDeviceWaitIdle(m_p_info->p_ic->getDev());
        }

        void __FrameUpdater::unlockFrame() {
            m_p_info->p_frame_mut->unlock();
        }


        /* Reallocate vertex buffer */
        void __FrameUpdater::reallocBuffer() {
            vkDestroyBuffer (
                m_p_info->p_ic->getDev(),
                m_p_info->p_ra->getBD()->main_buffer,
                NULL
            );

            vkFreeMemory (
                m_p_info->p_ic->getDev(),
                m_p_info->p_ra->getBD()->main_buffer_memory,
                NULL
            );

            m_p_info->p_ra->mkBuffers (
                m_p_info->p_ic->getDev(),
                m_p_info->p_ic->getGpu(),
                m_p_info->p_dc->getComPool(),
                m_p_info->p_ic->getQFF().graphics_queue
            );
        }

        
        /* Update vertices for every asset between bounds */
        void __FrameUpdater::updateVerts(dengMath::vec2<deng_ui32_t> update_bounds) {
            m_p_info->p_ra->remapAssetVerts (
                m_p_info->p_ic->getDev(),
                m_p_info->p_ic->getGpu(),
                m_p_info->p_dc->getComPool(),
                m_p_info->p_ic->getQFF().graphics_queue,
                update_bounds
            );
        }


        /* Update descriptor sets with texture sampling */
        void __FrameUpdater::updateTexDS (
            deng_bool_t realloc_ds, 
            dengMath::vec2<deng_ui32_t> *p_tex_bounds
        ) {
            m_p_info->p_desc_c->updateTexDescriptors (
                m_p_info->p_ic->getDev(),
                m_p_info->sc_img_size,
                realloc_ds,
                p_tex_bounds,
                m_p_info->p_ra->getBD()
            );
        }


        /* Update newly pushed texture objects */
        void __FrameUpdater::updateTextures(dengMath::vec2<deng_ui32_t> tex_bounds) {
            m_p_info->p_ra->mkTextureImages (
                m_p_info->p_ic->getDev(),
                m_p_info->p_ic->getGpu(),
                m_p_info->p_dc->getComPool(),
                m_p_info->p_ic->getLFSupport(),
                tex_bounds,
                m_p_info->p_ic->getQFF().graphics_queue,
                m_p_info->sc_img_size
            );

            updateTexDS (
                true, 
                &tex_bounds
            );
        }


        /* Update command buffers */
        void __FrameUpdater::updateCmdBuffers() {
            m_p_info->p_dc->recordMainCmdBuffers (
                m_p_info->renderpass,
                m_p_info->ext,
                m_p_info->background,
                *m_p_info->p_ra->getBD()
            );
        }
    }
}
