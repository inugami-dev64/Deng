#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 projection;
} ubo;

layout(binding = 1) uniform UniformColor {
    vec4 objColor;
} ucolor;

layout(location = 0) in vec3 inPosition;
layout(location = 1) out vec4 outColor;

void main() {
    gl_Position = ubo.projection * ubo.view * vec4(inPosition, 1.0f);
    outColor = ucolor.objColor;
}