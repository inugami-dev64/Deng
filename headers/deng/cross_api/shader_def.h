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
 *  Name: shader_def - shader file definitions
 *  Purpose: Provide definitions for shader files
 *  Author: Karl-Mihkel Ott
 */ 


#ifndef __SHADER_DEF_H
#define __SHADER_DEF_H

// Pipeline indices
#define UM2D_I          0
#define TM2D_I          1
#define UM3D_I          2
#define TM3D_I          3
#define UI_I            4
#define PIPELINE_C      5


/// Spir-V binary shaders
// UI system shaders
#define SPIR_V_UI_VERT_SHADER                       "./shaders/bin/2d/UIV.spv"
#define SPIR_V_UI_FRAG_SHADER                       "./shaders/bin/2d/UIF.spv"

// 2D asset shaders
#define SPIR_V_UNMAPPED_VERT_SHADER_2D              "./shaders/bin/2d/UV.spv"
#define SPIR_V_UNMAPPED_FRAG_SHADER_2D              "./shaders/bin/2d/UF.spv"
#define SPIR_V_TEXTURE_MAPPED_VERT_SHADER_2D        "./shaders/bin/2d/TMV.spv"
#define SPIR_V_TEXTURE_MAPPED_FRAG_SHADER_2D        "./shaders/bin/2d/TMF.spv"

// 3D asset shaders
#define SPIR_V_UNMAPPED_VERT_SHADER_3D              "./shaders/bin/3d/UV.spv"
#define SPIR_V_UNMAPPED_FRAG_SHADER_3D              "./shaders/bin/3d/UF.spv"
#define SPIR_V_TEXTURE_MAPPED_VERT_SHADER_3D        "./shaders/bin/3d/TMV.spv"
#define SPIR_V_TEXTURE_MAPPED_FRAG_SHADER_3D        "./shaders/bin/3d/TMF.spv"


/// Array of SPIR-v binary file names for iteration purposes
static const char *__shader_bin_files[][2]  = {
    { SPIR_V_UNMAPPED_VERT_SHADER_2D, SPIR_V_UNMAPPED_FRAG_SHADER_2D },
    { SPIR_V_TEXTURE_MAPPED_VERT_SHADER_2D, SPIR_V_TEXTURE_MAPPED_FRAG_SHADER_2D },
    { SPIR_V_UNMAPPED_VERT_SHADER_3D, SPIR_V_UNMAPPED_FRAG_SHADER_3D },
    { SPIR_V_TEXTURE_MAPPED_VERT_SHADER_3D, SPIR_V_TEXTURE_MAPPED_FRAG_SHADER_3D },
    { SPIR_V_UI_VERT_SHADER, SPIR_V_UI_FRAG_SHADER }
};


/// Source GLSL shaders
// UI system shaders
#define SRC_UI_VERT_SHADER                          "./shaders/src/2d/imgui.vert"
#define SRC_UI_FRAG_SHADER                          "./shaders/src/2d/imgui.frag"

// 2D asset shaders
#define SRC_UNMAPPED_VERT_SHADER_2D                 "./shaders/src/2d/unmapped.vert"
#define SRC_UNMAPPED_FRAG_SHADER_2D                 "./shaders/src/2d/unmapped.frag"
#define SRC_TEXTURE_MAPPED_VERT_SHADER_2D           "./shaders/src/2d/tex_mapped.vert"
#define SRC_TEXTURE_MAPPED_FRAG_SHADER_2D           "./shaders/src/2d/tex_mapped.frag"

// 3D asset shaders
#define SRC_UNMAPPED_VERT_SHADER_3D                 "./shaders/src/3d/unmapped.vert"
#define SRC_UNMAPPED_FRAG_SHADER_3D                 "./shaders/src/3d/unmapped.frag"
#define SRC_TEXTURE_MAPPED_VERT_SHADER_3D           "./shaders/src/3d/tex_mapped.vert"
#define SRC_TEXTURE_MAPPED_FRAG_SHADER_3D           "./shaders/src/3d/tex_mapped.frag"


/// Array of source shader file names for iteration purposes
static const char *__shader_src_files[][2]  = {
    { SRC_UNMAPPED_VERT_SHADER_2D, SRC_UNMAPPED_FRAG_SHADER_2D },
    { SRC_TEXTURE_MAPPED_VERT_SHADER_2D, SRC_TEXTURE_MAPPED_FRAG_SHADER_2D },
    { SRC_UNMAPPED_VERT_SHADER_3D, SRC_UNMAPPED_FRAG_SHADER_3D },
    { SRC_TEXTURE_MAPPED_VERT_SHADER_3D, SRC_TEXTURE_MAPPED_FRAG_SHADER_3D },
    { SRC_UI_VERT_SHADER, SRC_UI_FRAG_SHADER }
};


/// OpenGL shader program names
static const char *__shader_program_names[] = {
    "2D unmapped shader program",
    "2D texture mapped shader program",
    "3D unmapped shader program",
    "3D texture mapped shader program",
    "UI shader program"
};

#endif
