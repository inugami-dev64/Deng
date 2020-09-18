#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor; 

layout(location = 0) out vec3 outColor;

void main() {
    gl_Position = ubo.projection * ubo.view * ubo.model * vec4(-inPosition[0], -inPosition[1], -inPosition[2], 1.0f);
    outColor = inColor;
}