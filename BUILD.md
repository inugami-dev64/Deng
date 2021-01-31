# Building intructions

## GNU/Linux dependencies
* saucer (https://github.com/inugami-dev64/saucer)  
* xorg-server  
* xcursor  
* make  
* vulkan-headers  
* vulkan-loader  
* vulkan-validation-layers (optional)  

## Windows dependencies
### PS! There is absolutely no guarantee that this configuration would work on Windows, since I have not managed to successfully build DENG for Windows yet.
* MinGW-w64  
* Vulkan SDK  
* Vulkan validation layers (optional)  

### Enabling debug mode
For debuging purposes there are several debug macros which can be enabled before building.  Debugging allows for verbose console output. <br>
There are multiple macros for controlling debugging in src/core/deng_core.h 
*    `GENERAL_DEBUG` enables verbose logging to the console <br>
*    `CAMERA_LOCATION_DEBUG` logs the camera location to the console in vector space <br>
*    `CAMERA_MOUSE_DEBUG` logs mouse position to console (x axis: -2 to 2; y axis: -1 to 1) <br>

### Configuring build config
It is usually a good idea to check and verify if build config (build.yml) specification is compatible with your system. If building on Windows then make sure that VulkanSDK include path is correct (by default it is "C:/VulkanSDK/[VERSION]").  

### Building
Firstly a Makefile needs to be generated. For that purpose just run the following command in terminal `saucer build.yml`.
Then a simple `make` command can be used to compile DENG. 