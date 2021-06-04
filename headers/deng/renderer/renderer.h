/*    ____         ________     __      _        ______  
 *   |  _ \_      |  ______|   |  \    | |     _/  ____\
 *   | | \_ \_    |  |         |   \   | |    /  _/   
 *   | |   \  |   |  |_____    | |\ \  | |   |  |   _____ 
 *   | |    | |   |  ______|   | | \ \ | |   |  |  |___  | 
 *   | |  _/ _|   |  |         | |  \ \| |    \  \    / /
 *   | |_/ _/     |  |_____    | |   \   |     \_ \__/ /
 *   |____/       |________|   |_|    \__|       \____/
 *               
 * __________________________________________________________
 * __________________________________________________________
 *                       Project DENG 
 *
 *
 * Copyright (C) 2020 - 2021
 * This Software is licensed under Apache License as described 
 * in the LICENSE.md file, which you should have recieved with 
 * this distribution.
 * 
 * You may reproduce and distribute copies of the
 * Work or Derivative Works thereof in any medium, with or without
 * modifications, and in Source or Object form, provided that You
 * meet the following conditions:
 *
 * (a) You must give any other recipients of the Work or
 *     Derivative Works a copy of this License; and
 *
 * (b) You must cause any modified files to carry prominent notices
 *     stating that You changed the files; and
 *
 * (c) You must retain, in the Source form of any Derivative Works
 *     that You distribute, all copyright, patent, trademark, and
 *     attribution notices from the Source form of the Work,
 *     excluding those notices that do not pertain to any part of
 *     the Derivative Works; and
 *
 * (d) If the Work includes a "NOTICE" text file as part of its
 *     distribution, then any Derivative Works that You distribute must
 *     include a readable copy of the attribution notices contained
 *     within such NOTICE file, excluding those notices that do not
 *     pertain to any part of the Derivative Works, in at least one
 *     of the following places: within a NOTICE text file distributed
 *     as part of the Derivative Works; within the Source form or
 *     documentation, if provided along with the Derivative Works; or,
 *     within a display generated by the Derivative Works, if and
 *     wherever such third-party notices normally appear. The contents
 *     of the NOTICE file are for informational purposes only and
 *     do not modify the License. You may add Your own attribution
 *     notices within Derivative Works that You distribute, alongside
 *     or as an addendum to the NOTICE text from the Work, provided
 *     that such additional attribution notices cannot be construed
 *     as modifying the License.
 *
 * You may add Your own copyright statement to Your modifications and
 * may provide additional or different license terms and conditions
 * for use, reproduction, or distribution of Your modifications, or
 * for any such Derivative Works as a whole, provided Your use,
 * reproduction, and distribution of the Work otherwise complies with
 * the conditions stated in this License.
 */ 


#ifndef __RENDERER_H
#define __RENDERER_H

#ifdef __RENDERER_CPP
    #include <vector>
    #include <thread>
    #include <mutex>
    #include <queue>
    #include <memory>
    #include <array>
    
    #include <vulkan/vulkan.h>
    #include <common/base_types.h>
    #include <common/hashmap.h>
    #include <common/uuid.h>
    #include <common/err_def.h>
    #include <data/assets.h>

    #include <math/deng_math.h>
    #include <deng/window.h>
    #include <utils/timer.h>
    #include <deng/camera.h>

    #include <deng/registry/registry.h>
    #include <utils/font.h>

#endif

#include <deng/vulkan/renderer.h>
#include <deng/renderer/asset_man.h>

#define DENG_DEFAULT_NEAR_PLANE 0.1f
#define DENG_DEFAULT_FAR_PLANE 25.0f
#define DENG_FRAME_INTERVAL 50 // microseconds

namespace deng {

    // Frame renderer type
    class Renderer;
    typedef void(*PCustomRunFunc)(Renderer&);

    /// Main renderer class
    class Renderer : public __AssetManager {   
    private:
        std::shared_ptr<vulkan::__vk_ConfigVars> m_vk_vars;
        std::shared_ptr<vulkan::__vk_Renderer> m_vk_rend;   
        dengMath::vec4<deng_vec_t> m_env_color;
        deng_RendererHintBits m_hints;
        deng_RendererHintBits m_api_bits;
        deng_bool_t m_is_init;

        Window *m_p_win;
        Camera3D *m_p_cam;
        PCustomRunFunc m_run_func = NULL;

    private:
        /// Check if the renderer is initialised for update methods and throw error if needed
        void __initCheck(const std::string &func_name);

    public:
        Renderer(deng_RendererHintBits hints, const dengMath::vec4<deng_vec_t> &env_color);

        /// Setup graphics api specific renderer from the hints given in the constructor
        void setup(Camera3D &main_cam, Window &main_win);


        /// Overwrite asset vertices to main buffer.
        /// Note that this method expects that vertices count hasn't changed,
        /// otherwise weird stuff can happen!
        void updateVerts(const dengMath::vec2<deng_ui32_t> &bounds);


        /// Replace current light sources with new ones
        void updateLighting(std::array<deng_Id, __DENG_MAX_LIGHT_SRC_COUNT> &light_srcs);


        /// Update data, before creating new frame
        void update();

        /// Begin the rendering loop
        void run();        
    };
}


#endif