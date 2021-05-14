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
 * ----------------------------------------------------------------
 *  Name: ubo - Uniform buffer objects
 *  Purpose: Provide structures for uniform buffer data to align them for
 *  shaders
 *  Author: Karl-Mihkel Ott
 */ 

namespace deng {
    namespace vulkan {
        
        /// Structure for passing uniform lighting data right to the shader
        struct __vk_UniformLightSource {
            deng_vec_t intensity;
            dengMath::vec3<deng_vec_t> pos;
        };


        /// Structure for containing information about 3D transformations
        struct __vk_UniformObjectTransform {
            dengMath::mat4<deng_vec_t> transform;
            dengMath::mat4<deng_vec_t> view;
        };
 

        /// Structure for containing information about 2D transformations
        struct __vk_UniformObjectTransform2D {
            dengMath::mat3<deng_vec_t> view;
        };


        /// Structure for storing uniform color data per each asset
        struct __vk_UniformColorData {
            dengMath::vec4<deng_vec_t> color;
            deng_ui32_t ignore_transform;
            deng_ui32_t is_unmapped;
        };


        /// Light data uniform structure
        struct __vk_UniformLightData {
            __vk_UniformLightSource light_srcs[ 32 ];
            dengMath::vec4<deng_vec_t> ambient;
            deng_ui32_t light_src_c;
        };

    }
}
