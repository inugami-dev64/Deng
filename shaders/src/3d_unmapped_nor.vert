#version 450
#extension GL_ARB_separate_shader_objects : enable

const uint orthographic_camera_mode3D = 0x00000001u;
const uint perspective_camera_mode3D = 0x00000002u;

layout(binding = 0) uniform UniformBufferObj {
    mat4 view;
    mat4 projection;
    uint ubo_flag_bits;
} ubo;

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec4 in_color;
layout(location = 2) in vec3 in_norm_pos;

layout(location = 0) out vec4 out_color;

void main() {
    if((ubo.ubo_flag_bits & orthographic_camera_mode3D) == orthographic_camera_mode3D)
        gl_Position = ubo.view * vec4(-in_pos[0], -in_pos[1], in_pos[2], 1.0f);
    
    else if((ubo.ubo_flag_bits & perspective_camera_mode3D) == perspective_camera_mode3D)
        gl_Position = ubo.projection * ubo.view * vec4(-in_pos[0], -in_pos[1], in_pos[2], 1.0f);

    else gl_Position = vec4(-in_pos[0], -in_pos[1], in_pos[2], 1.0f);

    out_color = in_color;   
}
