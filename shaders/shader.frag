#version 450
#extension GL_ARB_separate_shader_objects : enable

vec4 fragColor = {1.0f, 1.0f, 1.0f, 1.0f};

layout(location = 0) out vec4 outColor;

void main() {
    outColor = fragColor;
}