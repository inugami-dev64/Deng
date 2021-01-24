# Getting started with DENG

Configuring DENG for your project is the first and the most important step of developing with DENG.   
This page helps to get started with setting up your very first DENG project.   

## Table of contents
1. [Building DENG](#building-deng)    
    1.1 [Build using Saucer](#build-using-saucer)  
2. [Drawing simple 2D objects](#drawing-simple-2d-objects)    
    2.1 [Includes](#includes)  
    2.2 [Creating WindowWrap and Renderer instances](#creating-windowwrap-and-renderer-instances)  
    2.3 [Setting hints for renderer](#setting-hints-for-renderer)  
    2.4 [Generating 2D shape assets](#generating-2d-shape-assets)  
    2.5 [Submit assets and finish renderer initialisation](#submit-assets-and-finish-renderer-initialisation)  
    2.6 [Final Code](#final-code)  
3. [Creating 3D assets](#3d-asset)    
    3.1 [DAM](#dam)  
    3.2 [Loading assets into DENG renderer](#model-loading)  
4. [Creating main function for map editor](#map-editor)  
5. [Creating main function for asset editor](#asset-editor)  


## Building DENG 
DENG supports multiple ways of building binaries. However as a general rule of thumb it is recommended to use
Saucer for GNU/Linux target platforms and Visual Studio for Windows. CMake can also be used but it is not recommended, since
the CMake scripts are not regularly updated and only MinGW compiler is supported on Windows. 

### Build using Saucer    
Saucer is a small subproject of DENG, created for the purpose of simplifying the Makefile generation.   
Git repository for Saucer can be found [here](#https://github.com/inugami-dev64/saucer)  
Makefile can be generated with the following command  
`$ saucer build.yml`  
Additionally target platform configuration can be specified with `-p` flag.  
All that is left to do now is simply run `make`. Once the building has finished successfully build/ directory should have
appeared. Inside build/deng/ you should find dam executable and inside build/deng/lib you should find
libdeng(.so/.dll) and libdas(.so/.dll). These are the main libraries that need to be linked with DENG project.  

## Drawing simple 2D objects 
### Includes
In order to use DENG we need to include `api_core.h` header which can be usually found in deng/headers/deng/ directory of the 
main repository.
```
#include <deng/api_core.h>
```

### Creating WindowWrap and Renderer instances
First steps of setting up any graphics library are usually related to creating surface window and setting up the main loop.
In DENG the process is pretty similar, exept that for now we don't need to worry about the main loop, since by default it is handled
by DENG API itself. Firstly [deng::WindowWrap](#../refs/deng/WindowWrap) instance needs to be created. This instance allows for
window surface creation. Secondly we need to create a renderer instance, which in DENG is called [deng::Renderer](#../refs/deng/Renderer).  

```
deng::WindowWrap window(1280, 720, "My awesome game!");
deng::Renderer rend;
```

### Setting hints for renderer
Before we can proceed into generating 2D shapes we need to first give the renderer hints, which define how the renderer is going 
to work. The full list of all possible renderer flags can be found [here](#../refs/global/deng_RendererHintBits). Please keep
in mind that using some combinations of these flags can result runtime errors (for example: using different msaa value flags at the same time).
Check the flag reference page for more information. Flags can be submited to renderer via following method: `deng::Renderer::setHints(deng_RendererHintBits)`  
In this example we use MSAA 2x antialiasing and Vulkan as our graphics api with FPS counter enabled.  

```
deng::WindowWrap window(1280, 720, "My awesome game!");
deng::Renderer rend;

rend.setHints (
    DENG_RENDERER_HINT_API_VULKAN |
    DENG_RENDERER_HINT_SHOW_FPS_COUNTER |
    DENG_RENDERER_HINT_MSAA_2
);
```

### Generating 2D shape assets
In DENG there is universal asset struct for both 3D and 2D assets. In order to generate 2D shapes we can use [dengUtils::TriangleGenerator](#../refs/dengutils/TriangleGenerator.md),
[dengUtils::RectangleGenerator](#../refs/dengutils/RectangleGenerator.md) and/or [dengUtils::CircleGenerator](#../refs/dengutils/CircleGenerator.md) classes accordingly. In this example
code we generate one circle, one triangle and one rectangle. As a first step we need to create asset instances for each individual shape. In this example an array will be suitable for that  
purpose. Once the array is created we can move on to generating 2D shapes into these asset instances. For this demo application we make all assets unmapped, which means that no texture mapping
will be done. Texture mapping comes in later chapter.  
For generating asset we are going to use following methods:   
[dengUtils::TriangleGenerator::makeAbsTriangleAsset()](#../refs/dengUtils/TriangleGenerator#makeAbsTriangleAsset) for triangle asset  
[dengUtils::CircleGenerator::makeAbsCircleAsset](#../refs/dengUtils/CircleGenerator#makeAbsCircleAsset) for circle asset  and
[dengUtils::RectangleGenerator::makeUnmappedAbsRecAsset()](#../refs/dengUtils/RectangleGenerator#makeUnmappedAbsRecAsset) for rectangle asset  
After generating new asset we also need to explicitly specify the visibility of an asset. This can be done by changing the value of `deng_Asset::is_shown` to `true`  

```
std::array<deng_Asset, 3> assets;

dengUtils::TriangleGenerator tri_gen(window.getSize());
dengUtils::RectangleGenerator rec_gen(window.getSize());
dengUtils::CircleGenerator cir_gen(window.getSize());


assets[0] = tri_gen.makeAbsTriangleAsset (
    "triangle",
    NULL,
    {-1.0f, 0.0f},
    {0.67, 0.67},
    {-1.0f, 0.0f},
    {1.0f, 0.0f, 0.0f, 1.0f},
    {{-1.0f, 1.0f}, {0.0f, -1.0f}, {1.0f, 1.0f}},
    0,
    {0.0f, 0.0f, 0.0f, 0.0f}
);
assets[0].is_shown = true;

assets[1] = cir_gen.makeAbsCircleAsset (
    "circle",
    NULL,
    {0.0f, 0.0f},
    0.33f,
    {0.0f, 1.0f, 0.0f, 1.0f},
    0,
    {0.0f, 0.0f, 0.0f, 0.0f}
);
assets[1].is_shown = true;

assets[2] = rec_gen.makeUnmappedAbsRecAsset (
    "rectangle",
    NULL,
    {0.33f, 0.0f},
    {0.67f, 0.67f},
    {-1.0f, 0.0f},
    {0.0f, 0.0f, 1.0f, 1.0f},
    0,
    {0.0f, 0.0f, 0.0f, 0.0f}
);
assets[2].is_shown = true;
```

### Submit assets and finish renderer initialisation
Now that 2D shape assets are generated we can finilise the renderer initialisation by first submitting the assets, then calling renderer initialisation method and finally calling the main run method.  
Submiting assets in DENG can be done with [deng::Renderer::submitAssets()](#../refs/deng/Renderer#submitAssets) method   
```
...
rend.submitAssets ( 
    assets.data(),
    assets.size()
);
```

Initialising the renderer can be done by calling [deng::Renderer::initRenderer()](#../refs/Renderer#initRenderer) method. This method takes three parametres, first pointer to WindowWrap instance,
second renderer usage mode ([deng_RendererUsageMode](#../refs/deng_RendererUsageMode)), which in our case is specified as DENG_RENDERER_USAGE_GAME_MODE and third background color. For background color 
we can create a new definition which we will call SANDBOX_BACKGROUND. The colors are specified as floating point RGB values.  

```
#include <deng/api_core.h>
#define SANDBOX_BACKGROUND {0.1411765f, 0.0431372f, 0.2313725f, 1.0f}
```

Now all that is left to do is call renderer initilisation and run methods. Run method takes no parametres and needs to be called 
when default main loop is needed.

```
...
rend.initRenderer (
    &window,
    DENG_RENDERER_USAGE_GAME_MODE,
    SANDBOX_BACKGROUND
);

rend.run();
```

### Final code
Code should now look something like this.

```
#include <deng/api_core.h>
#define SANDBOX_BACKGROUND {0.1411765f, 0.0431372f, 0.2313725f, 1.0f}

std::vector<deng_Asset> assets(3);

int main() {
    deng::WindowWrap window(1280, 720, "My awesome game!");
    deng::Renderer rend;

    rend.setHints (
        DENG_RENDERER_HINT_API_VULKAN | 
        DENG_RENDERER_HINT_SHOW_FPS_COUNTER |
        DENG_RENDERER_HINT_MSAA_2
        &window
    );

    dengUtils::TriangleGenerator tri_gen(window.getSize());
    dengUtils::CircleGenerator cir_gen(window.getSize());
    dengUtils::RectangleGenerator rec_gen(window.getSize());

    assets[0] = tri_gen.makeAbsTriangleAsset (
        "triangle",
        NULL,
        {-1.0f, 0.0f},
        {0.67, 0.67},
        {-1.0f, 0.0f},
        {1.0f, 0.0f, 0.0f, 1.0f},
        {{-1.0f, 1.0f}, {0.0f, -1.0f}, {1.0f, 1.0f}},
        0,
        {0.0f, 0.0f, 0.0f, 0.0f}
    );
    assets[0].is_shown = true;

    assets[1] = cir_gen.makeAbsCircleAsset (
        "circle",
        NULL,
        {0.0f, 0.0f},
        0.33f,
        {0.0f, 1.0f, 0.0f, 1.0f},
        0,
        {0.0f, 0.0f, 0.0f, 0.0f}
    );
    assets[1].is_shown = true;

    assets[2] = rec_gen.makeUnmappedAbsRecAsset (
        "rectangle",
        NULL,
        {0.33f, 0.0f},
        {0.67f, 0.67f},
        {-1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 1.0f},
        0,
        {0.0f, 0.0f, 0.0f, 0.0f}
    );
    assets[2].is_shown = true;

    rend.submitAssets (
        assets.data(),
        assets.size()
    );

    rend.initRenderer (
        &window,
        DENG_RENDERER_USAGE_GAME_MODE,
        SANDBOX_BACKGROUND
    );

    rend.run();

    return 0;
}
```
