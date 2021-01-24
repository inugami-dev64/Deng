#ifndef FORWARD_DEC_H
#define FORWARD_DEC_H

/* Header file for all forward class and struct declarations */

namespace deng {
    // core/renderer.h
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

    // core/window.h
    class WindowWrap;

    // core/camera.h
    class Camera;

    // core/rend_helpers.h
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

    // 
}

#endif