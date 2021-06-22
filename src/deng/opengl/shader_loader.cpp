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
 *  Name: gl_shader_loader - OpenGL shader loader
 *  Purpose: Provide an abstracted class interface for loading shaders
 *  Author: Karl-Mihkel Ott
 */ 


#define __GL_SHADER_LOADER_CPP
#include <deng/opengl/shader_loader.h>


namespace deng {
    namespace opengl {

        __gl_ShaderLoader::__gl_ShaderLoader() {
            // For each pipeline type create a set of shader objects
            for(size_t i = 0; i < m_programs.size(); i++)
                __compileShadersToProgram(static_cast<deng_ui32_t>(i));

            __prepareUniformBindings();
        }


        void __gl_ShaderLoader::__compileShadersToProgram(const deng_ui32_t index) {
            dengMath::vec2<deng_ui32_t> shaders;

            shaders.first = glCreateShader(GL_VERTEX_SHADER);
            shaders.second = glCreateShader(GL_FRAGMENT_SHADER);

            // Load shader sources
            char *vert = __loadShaderFromFile(__shader_src_files[index][0]);
            char *frag = __loadShaderFromFile(__shader_src_files[index][1]);
            glShaderSource(shaders.first, 1, &vert, NULL);
            glShaderSource(shaders.second, 1, &frag, NULL);
            
            /// Attempt to compile shaders
            glCompileShader(shaders.first);
            __checkCompileStatus(shaders.first, __shader_src_files[index][0]);
            glCompileShader(shaders.second);
            __checkCompileStatus(shaders.second, __shader_src_files[index][1]);

            free(vert);
            free(frag);

            // Create new shader programs for each pipeline type
            m_programs[index] = glCreateProgram();
            glAttachShader(m_programs[index], shaders.first);
            glAttachShader(m_programs[index], shaders.second);
            glLinkProgram(m_programs[index]);

            __checkLinkingStatus(m_programs[index], static_cast<deng_ui32_t>(index));

            // Delete all shader objects
            glDeleteShader(shaders.first);
            glDeleteShader(shaders.second);
        }


        /// Prepare bindings for uniform data
        void __gl_ShaderLoader::__prepareUniformBindings() {
            // 2D unmapped assets
            deng_ui32_t index;
            index = glGetUniformBlockIndex(m_programs[UM2D_I], "UniformData");
            glUniformBlockBinding(m_programs[UM2D_I], index, 0);

            index = glGetUniformBlockIndex(m_programs[UM2D_I], "ColorData");
            glUniformBlockBinding(m_programs[UM2D_I], index, 1);

            // 2D texture mapped assets
            index = glGetUniformBlockIndex(m_programs[UM2D_I], "UniformData");
            glUniformBlockBinding(m_programs[TM2D_I], index, 0);

            index = glGetUniformBlockIndex(m_programs[UM2D_I], "ColorData");
            glUniformBlockBinding(m_programs[TM2D_I], index, 1);

            // 3D unmapped assets
            index = glGetUniformBlockIndex(m_programs[UM2D_I], "UniformData");
            glUniformBlockBinding(m_programs[TM2D_I], index, 0);

            index = glGetUniformBlockIndex(m_programs[UM2D_I], "ColorData");
            glUniformBlockBinding(m_programs[TM2D_I], index, 1);

            index = glGetUniformBlockIndex(m_programs[UM3D_I], "LightData");
            glUniformBlockBinding(m_programs[UM3D_I], index, 2);

            // 3D texture mapped assets
            index = glGetUniformBlockIndex(m_programs[UM2D_I], "UniformData");
            glUniformBlockBinding(m_programs[TM2D_I], index, 0);

            index = glGetUniformBlockIndex(m_programs[UM2D_I], "ColorData");
            glUniformBlockBinding(m_programs[TM2D_I], index, 1);

            index = glGetUniformBlockIndex(m_programs[UM3D_I], "LightData");
            glUniformBlockBinding(m_programs[UM3D_I], index, 2);
        }


        /// Load shader data from file to a buffer
        char *__gl_ShaderLoader::__loadShaderFromFile(const char *file_name) {
            FILE *file = fopen(file_name, "rb");
            DENG_ASSERT((std::string("Failed to open file stream for file name: ") + std::string(file_name)).c_str(), file);
        
            // Find the file size
            fseek(file, 0, SEEK_END);
            size_t buf_size = ftell(file);
            fseek(file, 0, SEEK_SET);

            // Allocate memory for shader buffer
            char *buf = (char*) calloc(buf_size, sizeof(char));
            size_t res = fread(buf, sizeof(char), buf_size, file);
            DENG_ASSERT((std::string("Failed to read from file ") + std::string(file_name)).c_str(), res);

            return buf;
        }


        /// Check if the shader compilation was successful
        void __gl_ShaderLoader::__checkCompileStatus(const deng_ui32_t shader_id, const char *file_name) {
            int success;
            char log[512] = { 0 };
            glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
            
            if(!success) {
                glGetShaderInfoLog(shader_id, 512, NULL, log);
                LOG("Shader compilation error in file: " + std::string(file_name));
                LOG(std::string(log));
                exit(EXIT_FAILURE);
            }
        }


        /// Check if shader program linking was successful
        void __gl_ShaderLoader::__checkLinkingStatus(const deng_ui32_t program_id, const deng_ui32_t program_index) {
            int success;
            char log[512];
            glGetProgramiv(program_id, GL_LINK_STATUS, &success);

            if(!success) {
                glGetProgramInfoLog(program_id, 512, NULL, log);
                LOG("Failed to link " + std::string(__shader_program_names[program_index]));
                LOG(std::string(log));
                exit(EXIT_FAILURE);
            }
        }


        const deng_ui32_t __gl_ShaderLoader::getShaderProgram(const deng_ui32_t pipeline_id) {
            return m_programs[pipeline_id];
        }
    }
}
