---
title: started with DENG
date: 2021-04-07
---

# Getting started with DENG

Configuring DENG for your project is the first and the most important step of developing with DENG.   
This page helps to get started with setting up your very first DENG project.   


## Building DENG 
DENG can be built using CMake or Saucer (experimental). 

### Build using CMake
Builing DENG Using CMake is pretty straight forward. However if using Windows then `VULKAN_SDK_PATH` variable must be specified to the location of VulkanSDK installed on the system.
List of all available CMake variables are 

Variable | Expected value | Description
--- | --- | ---
VULKAN_SDK_PATH | [PATH TO VULKAN SDK] | Specify the path to Vulkan SDK (Windows only)
BUILD_ALL | True / False | Build all targets available (including submodules)
BUILD_DAM | True / False | Build DENG asset manager
BUILD_LIBDENG | True / False | Build libdeng
BUILD_FREETYPE | True / False | Add Freetype to build target list
BUILD_SANDBOX_APP | True / False | Add sandbox application to build target list

Building can then be done with following commands  
* `$ mkdir build`
* `$ cd build`
* `$ cmake .. [FLAGS]`

### Build using Saucer (experimental)   
Saucer is a small subproject of DENG, created for the purpose of having small and unbloated program for generating Makefiles. Currently Saucer successfully generates Makefiles for Linux only.   
Git repository for Saucer can be found [here](#https://github.com/inugami-dev64/saucer)  
Makefile for DENG can be generated with the following command  
`$ saucer build.yml`  
To generate a Makefile with sandboxapp included use
`$ saucer build_devel.yml`
Additionally target platform configuration can be specified with `-p` flag.  
All that is left to do now is simply run `make`. 

## Drawing simple 2D objects 
### Includes
In order to use DENG we need to include `deng/deng.h` header which can found in headers directory of the main repository. Before including `deng/deng.h` header it is good idea to define following preprocessor flag `DENG_ENABLE_STD_INCL`. What this does is it includes all required STL headers that are used in DENG. If this flag is not defined then all STL headers must be included manually.
```
#define DENG_ENABLE_STD_INCL
#include <deng/deng.h>
```

### Setting up renderer 
First steps of setting up DENG renderer are creating a surface window and a camera instance. 
In DENG there are currently two camera classes: `deng::EditorCamera` and `deng::FPPCamera`.
`deng::EditorCamera` creates new editor camera and `deng::FPPCamera` creates new first person
perspective camera instance.  


In this example code we are going to use first person perspective camera 
but the renderer accepts both camera types  
```
deng::WindowWrap window(1280, 720, "My awesome game!");
deng::FPPCamera fpp_cam (
    {1.0f, 1.0f, 1.0f}, 
    {0.7f, 0.7f}, 
    dengMath::Conversion::degToRad(65.0f), 
    DENG_DEFAULT_NEAR_PLANE, 
    DENG_DEFAULT_FAR_PLANE, 
    NULL, 
    NULL,
    &window
);
deng::Renderer rend(&window, &fpp_cam, DENG_CAMERA_FPP);
```

As you can probably see the sample code is using `DENG_DEFAULT_NEAR_PLANE` and `DENG_DEFAULT_FAR_PLANE` preprocessor 
definition values. These values expand to 0.1f and 25.0f accordingly.    

More info about classes used in this example:  
* `deng::FPPCamera` -- [link](refs/fppcamera.md)  
* `deng::EditorCamera` -- [link](refs/editorcamera.md)   
* `deng::Renderer` -- [link](refs/renderer.md)  
* `deng::WindowWrap` -- [link](refs/windowwrap.md)  

### Setting hints for renderer
Before we can proceed into generating 2D shapes we need to first give the renderer hints, which define how the renderer is going 
to work. The full list of all possible renderer flags can be found [here](refs/global/deng_rendererhintbits.md). Please keep
in mind that using some combinations of these flags can result runtime errors (for example: using different msaa value flags at the same time).
Check the flag reference page for more information. Flags can be submited to renderer via following method: `deng::Renderer::setHints(deng_RendererHintBits)`  
In this example we use MSAA 2x antialiasing and Vulkan as our graphics api with console FPS counter enabled.

```
deng::WindowWrap window(1280, 720, "My awesome game!");
deng::FPPCamera fpp_cam (
    {1.0f, 1.0f, 1.0f}, 
    {0.7f, 0.7f}, 
    dengMath::Conversion::degToRad(65.0f), 
    DENG_DEFAULT_NEAR_PLANE, 
    DENG_DEFAULT_FAR_PLANE, 
    NULL, 
    NULL,
    &window
);
deng::Renderer rend(&window, &fpp_cam, DENG_CAMERA_FPP);

rend.setHints (
    DENG_RENDERER_HINT_API_VULKAN |
    DENG_RENDERER_HINT_SHOW_CMD_FPS_COUNTER |
    DENG_RENDERER_HINT_MSAA_2
);
```

### Generating 2D shape assets
In DENG there is universal asset structure for both 3D and 2D assets. In order to generate 2D shapes we can use `dengUtils::TriangleGenerator`, `dengUtils::RectangleGenerator` and/or `dengUtils::CircleGenerator` classes accordingly. In this example
code we generate one circle, one triangle and one rectangle. As a first step we need to create all asset instances for each individual shape. In this example an `std::array` will be suitable for that purpose. Once the array is created we can move on to generating 2D shapes into these asset instances. For this demo application we make all assets unmapped, which means that no texture mapping
will be done. Texture mapping tutorial comes later.  
For generating assets we are going to use following methods:   
`dengUtils::TriangleGenerator::makeAbsTriangleAsset()` for triangle assets  
`dengUtils::CircleGenerator::makeAbsCircleAsset()` for circle assets and  
`dengUtils::RectangleGenerator::makeUnmappedAbsRecAsset()` for rectangle assets  
References to shape generators:  
`dengUtils::RectangleGenerator` -- [link](refs/utils/rectanglegenerator.md)  
`dengUtils::CircleGenerator` -- [link](refs/utils/circlegenerator.md)  
`dengUtils::TriangleGenerator` -- [link](refs/utils/trianglegenerator.md)  

After generating new assets we also need to explicitly specify the visibility of an asset. This can be done by changing the value of `das_Asset::is_shown` to `true`  

```
std::array<das_Asset, 3> assets;

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
Submiting the assets in DENG can be done with `deng::Renderer::submitAssets()` method   
```
...
rend.submitAssets ( 
    assets.data(),
    assets.size()
);
```

Initialising the renderer can be done by calling `deng::Renderer::initRenderer()` method. This method takes three parametres, first pointer to WindowWrap instance,
second renderer usage mode `deng_RendererUsageMode`, which in our case is specified as `DENG_RENDERER_USAGE_GAME_MODE` and third the background color. For background color 
we can create a new definition which we will call `SANDBOX_BACKGROUND`. The colors are specified as floating point RGB values. We will use purpleish color for the background in this example application, because why not.   

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

std::vector<das_Asset> assets(3);

int main() {
    deng::WindowWrap window(1280, 720, "My awesome game!");
    deng::FPPCamera fpp_cam (
        {1.0f, 1.0f, 1.0f}, 
        {0.7f, 0.7f}, 
        dengMath::Conversion::degToRad(65.0f), 
        DENG_DEFAULT_NEAR_PLANE, 
        DENG_DEFAULT_FAR_PLANE, 
        NULL, 
        NULL,
        &window
    );
    deng::Renderer rend(&window, &fpp_cam, DENG_CAMERA_FPP);

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
