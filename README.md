# DENG

![logo](logo/logo_full.png)  
DENG is a cross platform high level game engine designed to be easy to use. Currently not much has 
been implemented, but the list of already implemented features is following: 
* 3D model renderering  
* 2D model renderering  
* 2D shape generator  
* Editor and first person camera systems  
* Game entity data registry system  
* Asset texture mapping  
* Custom asset format  

Features that are not yet implemented but are coming soon are following:  
* Collection of lighting components (point light, directional light and global light sources)  
* OpenGL support for legacy graphics cards  
* Lua scripting layer for game scripting  
* libjpeg and libpng integration for supporting additional texture formats  
* Custom mapdata format  
* Map editor tool  


## Building
DENG can be built using [premake5](https://github.com/premake/premake-core) 

Additional premake flags for DENG are following:  
| Flag          | Possible values       | Description                                                                   |
| :---          | :--------------       | ----------:                                                                   |
| build-static  | \-                    | Build DENG as a static library                                                |
| use-modules   | \-                    | Build all dependencies from source instead of searching for their binaries    |
| sandbox-mode  | DEFAULT, VKR, NONE    | Sandbox application mode (currently only VKR and NONE work)                   |

### Unix like operating system
In Unix like systems that are using GNU/Make and gcc you can just use: 
`$ premake5 [FLAGS] gmake`
