#version 450
#extension GL_ARB_separate_shader_objects : enable

const uint OrthographcCameraMode2D = 0x00000004u;

layout(binding = 0) uniform UniformData {
    mat4 view;
    mat4 projection;
    uint ubo_flag_bits;
} ubo;

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec4 in_color;
layout(location = 2) in uint in_hier;
layout(location = 0) out vec4 out_color;

void main() {
    // Check how to handle vertex
    if((ubo.ubo_flag_bits & OrthographcCameraMode2D) == OrthographcCameraMode2D)
        gl_Position = ubo.view * vec4(in_position, float(in_hier), 1.0f);

    else gl_Position = vec4(in_position, 0.0f, 1.0f);
    
    out_color = in_color;
}
