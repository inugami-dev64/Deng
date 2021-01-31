#version 450
#extension GL_ARB_separate_shader_objects : enable

const uint OrthographicCameraMode3D = 0x00000001u;
const uint PerspectiveCameraMode3D = 0x00000002u;

layout(binding = 0) uniform UniformBufferObj {
    mat4 view;
    mat4 projection;
    uint ubo_flag_bits;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexPosition;

layout(location = 1) out vec2 fragTexPos;

void main() {
    if((ubo.ubo_flag_bits & OrthographicCameraMode3D) == OrthographicCameraMode3D)
        gl_Position = ubo.view * vec4(-inPosition[0], -inPosition[1], inPosition[2], 1.0f);

    else if((ubo.ubo_flag_bits & PerspectiveCameraMode3D) == PerspectiveCameraMode3D)
        gl_Position = ubo.projection * ubo.view * vec4(-inPosition[0], -inPosition[1], inPosition[2], 1.0f);

    else gl_Position = vec4(inPosition[0], -inPosition[1], inPosition[2], 1.0f);
    
    fragTexPos = inTexPosition;
}
