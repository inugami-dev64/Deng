#ifndef FORWARD_DEC_H
#define FORWARD_DEC_H

/* Header file for all forward class and struct declarations */

namespace deng {
    // deng/deng_math.h
    struct vec4;
    struct vec3;
    struct vec2;

    class mat2;
    class mat3;
    class mat4;

    // deng/renderer.h
    struct VulkanInstanceInfo;
    struct VulkanSwapChainInfo;
    struct VulkanDescriptorInfo;
    struct VulkanResourceInfo;
    class InstanceCreator;
    class SwapChainCreator;
    class DescriptorCreator;
    class ResourceAllocator;
    class DrawCaller;
    class Renderer;

    // deng/window.h
    class WindowWrap;

    // deng/camera.h
    struct CameraBaseValues;
    struct EventBaseValues;
    class FPPCameraEv;
    class FPPCamera;

    // deng/rend_helpers.h
    struct BufferData;
    struct TextureImageData;
    struct HardwareSpecs;
    struct BufferCreator;
    struct CommandBufferRecorder;
    class QueueFamilyFinder;
    struct PipelineData;
    struct PipelineCreateinfoSpecifiers;
    struct VulkanDeviceInfo;
    class PipelineCreator;
    class SwapChainDetails;
}

namespace dengui {
    // dengui/dengui_child.h
    struct ParentInfo;
    struct PushButtonInfo;
    class ChildPushButton;
    
    // dengui/dengui_events.h
    struct MouseInputInfo;
    struct EventInfo;
    class Events;

    // dengui/dengui_window.h
    struct WindowUpdateInfo;
    struct SharedWindowUpdateInfos;
    struct WindowElement;
    struct WindowInfo;
    struct WindowShapeInfo;
    class BaseWindowShapes;
    class Window;
}

#endif
