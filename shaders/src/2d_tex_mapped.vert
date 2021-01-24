#version 450
#extension GL_ARB_separate_shader_objects : enable

const uint OrthographicCameraMode2D = 0x00000004u;
const uint NoCameraMode2D = 0x00000008u;

layout(binding = 0) uniform UniformData {
    mat4 view;
    mat4 projection;
    uint ubo_flag_bits;
} ubo;

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_tex_position;

layout(location = 1) out vec2 frag_tex_position;

void main() {
    if((ubo.ubo_flag_bits & OrthographicCameraMode2D) == OrthographicCameraMode2D)
        gl_Position = ubo.view * vec4(in_position, 0.0f, 1.0f);
    
    else gl_Position = vec4(in_position, 0.0f, 1.0f);
        
    frag_tex_position = in_tex_position;
}