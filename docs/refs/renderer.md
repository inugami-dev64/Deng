---
title: Renderer 
date: 2021-04-07
---

# deng::Renderer 

C++ specification
File: renderer.h
## Description
`deng::Renderer` is used as a main class for rendering assets, handling api calls
and drawing to the window surface area.

## Constructor
Constructor for class `deng::Renderer` is following:  
```
deng::Renderer::Renderer (
    deng::WindowWrap *p_win,
    void *p_cam,
    deng_CameraType cam_type
);
```

* `p_win` must be a valid pointer to `deng::WindowWrap` instance ([link to reference](windowwrap.md))
* `p_cam` must be a valid pointer to any camera class 
(currently supported `deng::FPPCamera` and `deng::EditorCamera`)
* `cam_type` specifies the type of camera that is going to be used ([link to reference](enums/deng_cameratype.md))

## List of public members
`deng::Renderer` has following members:  

### Methods
* [submitAssets](#submitassets)
* [submitTextures](#submittextures)
* [submitTextureFile](#submittexturefile)
* [submitRendStr](#submitrendstr)
* [setHints](#sethints)
* [initRenderer](#initrenderer)
* [run](#run)

## Method specifications

### submitAssets
```
void deng::Renderer::submitAssets (
    deng_Asset *assets,
    deng_i32_t asset_c
);
```

#### Description
Submit an array of assets to renderer for drawing

#### Parameters
* `assets` must be a valid pointer to the beginning of `deng_Asset` array 
* `asset_c` must be a correct size of the array      


### submitTextures
```
void submitTextures (
    deng_Texture *textures,
    deng_i32_t tex_c
);
```

#### Description
Submit an array of textures to renderer for texture mapping assets

#### Parameters
* `textures` must be a valid pointer to the beginning of `deng_Texture` array
* `tex_c` must be a correct size of the texture array


### submitTextureFile
```
void submitTextureFile (
    std::string tex_file, 
    std::string id
);
```

#### Description
Submit texture file to read from into renderer for texture mapping

#### Parameters
* `tex_file` must be a correct texture image file name ([more info about textures](../textures.md))
* `id` will be the identifier used to identify loaded texture instance. It is recommended to use generated UUID 
for identifiers in order to avoid possible id conflicts. More info about DENG and UUIDs can be found [here](../uuid.md)


### submitRendStr
```
void submitRendStr (
    dengUtils::BitmapStr *rend_strs, 
    size_t str_c
);
```

#### Description
Submit rasterised text into renderer for drawing

#### Parameters 
* `rend_strs` must be a valid pointer to the beginning of `dengUtils::BitmapStr` array  
* `str_c` must be a correct size of the rasterised string array


### setHints
```
void setHints(deng_RendererHintBits hints);
```

#### Description
Set renderer usage hints

#### Parameters 
* `hints` specify some properties of DENG renderer ([more info about hints](#rend_hints.md))


### initRenderer
```
void initRenderer (
    deng_RendererUsageMode usage, 
    dengMath::vec4<deng_vec_t> background
);
```

#### Description
Set up the renderer for drawing 

#### Parameters
* `usage` specify the usage mode for DENG renderer ([more info about usage](#rend_usage.md))
* `background` specify the RGB color for the background (values are from 0.0f to 1.0f)


### run
```
void run();
```

#### Description
Run the main loop in the renderer
