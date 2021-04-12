---
title: deng\_RendererHintBits
date: 2021-04-07
---

# deng\_RendererHintBits

C / C++ specification


## Description
`deng_RendererHintBits` are used to modify renderer behaviour and / or enable
some features. 

```
typedef deng_ui16_t deng_RendererHintBits;
#define DENG_RENDERER_HINT_API_VULKAN                       0x0001u
#define DENG_RENDERER_HINT_API_OPENGL                       0x0002u
#define DENG_RENDERER_HINT_ENABLE_VSYNC                     0x0004u
#define DENG_RENDERER_HINT_SHOW_CMD_FPS_COUNTER             0x0008u
#define DENG_RENDERER_HINT_SHOW_GUI_FPS_COUNTER             0x0010u
#define DENG_RENDERER_HINT_ENABLE_VALIDATION_LAYERS         0x0020u
#define DENG_RENDERER_HINT_MSAA_MAX_HARDWARE_SUPPORTED      0x0040u
#define DENG_RENDERER_HINT_MSAA_64                          0x0080u
#define DENG_RENDERER_HINT_MSAA_32                          0x0100u
#define DENG_RENDERER_HINT_MSAA_16                          0x0200u
#define DENG_RENDERER_HINT_MSAA_8                           0x0400u
#define DENG_RENDERER_HINT_MSAA_4                           0x0800u
#define DENG_RENDERER_HINT_MSAA_2                           0x1000u
#define DENG_RENDERER_HINT_MSAA_1                           0x2000u
#define DENG_RENDERER_HINT_MIPMAP_ENABLE                    0x4000u
#define DENG_RENDERER_HINT_CUSTOM_LOOP                      0x8000u
```

* `DENG_RENDERER_HINT_API_VULKAN` -- forces renderer to use Vulkan as its running API  
* `DENG_RENDERER_HINT_API_OPENGL` -- forces renderer to use OpenGL as its running API (ignored, reserved for future use)
* `DENG_RENDERER_HINT_ENABLE_VSYNC` -- forces renderer to enable VSync
* `DENG_RENDERER_HINT_SHOW_CMD_FPS_COUNTER` -- enables console fps counter 
* `DENG_RENDERER_HINT_SHOW_GUI_FPS_COUNTER` -- enabled rendered fps counter (ignored, reserved for future use)
* `DENG_RENDERER_HINT_ENABLE_VALIDATION_LAYERS` -- enables Vulkan validation layers for usage 
(requires Vulkan API hint flag)
* `DENG_RENDERER_HINT_MSAA_MAX_HARDWARE_SUPPORTED` -- enable MSAA with as many sample levels as graphics
device supports
* `DENG_RENDERER_HINT_MSAA_64` -- enable 64 sample level MSAA 
* `DENG_RENDERER_HINT_MSAA_32` -- enable 32 sample level MSAA
* `DENG_RENDERER_HINT_MSAA_16` -- enable 16 sample level MSAA
* `DENG_RENDERER_HINT_MSAA_8` -- enable 8 sample level MSAA
* `DENG_RENDERER_HINT_MSAA_4` -- enable 4 sample level MSAA
* `DENG_RENDERER_HINT_MSAA_2` -- enable 2 sample level MSAA
* `DENG_RENDERER_HINT_MSAA_1` -- enable 1 sample level MSAA (default)
* `DENG_RENDERER_HINT_MIPMAP_ENABLE` -- enable mipmapping
* `DENG_RENDERER_HINT_CUSTOM_LOOP` -- specify custom loop method to use 
(ignored, reserved for future use)

## Notes
DENG renderer will throw runtime exeption if multiple MSAA levels, multiple graphics APIs or
if Vulkan validation layers are enabled without forced Vulkan support.
