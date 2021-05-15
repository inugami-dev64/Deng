# DENG

![logo](logo/logo_full.png)  
DENG is a cross platform high level game development framework / engine designed to keep game development process
simple and straight forward. Currently not much has been implemented yet, but the list of already implemented 
features is following: 
* 3D model renderering  
* 2D model renderering  
* 2D shape generator  
* Editor and first person camera systems  
* Game entity data registry system  
* Asset texture mapping  
* Custom asset format  
* Wavefront OBJ parser
* Point light

Features that are not yet implemented but are coming soon are following:  
* Other lighting components (directional light and global light sources)  
* OpenGL support for legacy graphics cards  
* Lua scripting layer for game scripting  
* libjpeg and libpng integration for supporting additional texture formats  
* Custom mapdata format  
* Map editor tool  


## Getting started
DENG can be built using [premake5](https://github.com/premake/premake-core) 

Additional premake flags for DENG are following:  
| Flag          | Possible values       | Description                                                                   |
| :---          | :--------------       | ----------:                                                                   |
| build-static  | \-                    | Build DENG as a static library (Windows only)                                 |
| use-modules   | \-                    | Build all dependencies from source instead of searching for their binaries    |
| sandbox-mode  | DEFAULT, VKR, NONE    | Sandbox application mode (currently only VKR and NONE work)                   |
| vk-sdk-path   | [PATH_TO_VULKAN_SDK]  | Specify the Vulkan SDK path (Windows only)                                    |

### GNU/Linux or potentially other Unix like operating systems excluding MacOS

#### Prerequisites
* Gnu toolchain (gcc, make)  
* Vulkan headers  
* Freetype library and headers (optional / can be built as a submodule)  
* Xlib
* XCursor
* Premake5


#### Building
Generating Makefiles using premake is pretty straight forward:  
`$ premake5 [OPTIONS] gmake`  

After generating the Makefiles make sure that `config` variable is specifed as either `debug_linux` or `release_linux`.
Build command should now look something like this:  
`make config=debug_linux -j[N]` for debug build or  
`make config=release_linux -j[N]` for release build


### Microsoft Windows

When using Windows to build DENG please make sure that submodules are initialized. For that you can use `git submodule init` and `git submodule update`.  
NOTE: Currently only static library builds have been tested on Windows. Please keep that in mind.

#### Prerequisites
* Visual Studio  
* Vulkan SDK  
* Premake5  

#### Building
Generating Visual Studio solution files using premake requires some additional flags, namely `use-modules`, `build-static` and `vk-sdk-path`.  
Premake command to generate solution files should now look something like this:  
`premake5 --use-modules --build-static --vk-sdk-path=[PATH_TO_VULKAN_SDK]`  

After that is done open newly generated `deng.sln` file and build the project normally using Visual Studio.
