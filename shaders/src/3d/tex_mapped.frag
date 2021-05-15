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


#version 450
#extension GL_ARB_separate_shader_objects : enable


const int max_light_src = 8;

layout(binding = 2) uniform UboLightData {
    // Data for simple point light
    vec3 pos;
    float intensity;

    vec4 ambient;
    uint light_src_c;
} ld;

layout(binding = 3) uniform sampler2D tex_sampler;


// Fragment input data
layout(location = 0) in vec4 in_color;
layout(location = 1) in vec2 in_tex;
layout(location = 2) in vec3 in_pos;
layout(location = 3) in vec3 in_normal;
layout(location = 4) in flat uint in_is_unmapped;

// Output fragment
layout(location = 0) out vec4 out_color;

void main() {
    vec4 dif_color;

    // Check if color or texture mapping should be used
    //if(in_is_unmapped == 1)
        //dif_color = in_color;
    dif_color = ld.ambient * texture(tex_sampler, in_tex);

    out_color = dif_color;

    // For each light source apply lambertian shading
    //for(uint i = 0; i < ld.light_src_c; i++) {
        //float cos_theta = dot(normalize(vec3(ld.pos[i]) - in_pos), 
            //in_normal);
        //out_color += dif_color * ld.intensity[i / 4][i % 4] * max(0, cos_theta);
    //}
    
    float cos_theta = dot(normalize(vec3(ld.pos) - in_pos), 
        normalize(in_normal));
    out_color += dif_color * ld.intensity * max(0, cos_theta);
}
