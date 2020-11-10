# Building intructions

## GNU/Linux dependencies
* xorg-server <br>
* xcursor <br>
* make <br>
* vulkan-headers <br>
* vulkan-loader <br>
* vulkan-layers (optional) <br>

## Windows dependencies
* MinGW-w64 <br>
* Vulkan SDK <br>
* Vulkan validation layers (optional) <br>

### Enabling debug mode
For debuging purposes there are several debug macros which can be enabled before building. Debugging allows for verbose console output. <br>
There are multiple macros for controlling debugging in src/core/deng_core.h 
*    `GENERAL_DEBUG` enables vulkan validation layers and verbose logging to the console <br>
*    `CAMERA_LOCATION_DEBUG` logs the camera location to the console in vector space <br>
*    `CAMERA_MOUSE_DEBUG` logs mouse position to console (x axis: -2 to 2; y axis: -1 to 1) <br>

### Configuring Makefile
It is usually a good idea to check and verify if Makefile variables are set correctly. <br> Makefiles can be found in make/windows and make/linux folders accordingly. By default Windows Makefile expects Vulkan SDK to be installed in C:\VulkanSDK\$(VULKANSDK_VERSION) <br>
Vulkan SDK path can be specified with VULKANSDK_PATH variable and the version of it can be specified with VULKANSDK_VERSION variable. <br>
Other relevant variable might BUILD_DIR, which specifies the build directory. (if directory doesn't exist, it will be created)   

### Building
In GNU/Linux, open terminal, navigate to make/linux and run `$ make` <br>
In Windows open cmd, navigate to make\windows and run `... make\windows> mingw32-make`

### Cleaning binaries
Makefiles have two kind of cleanup options `make clean` and `make clean_exec` <br>
`make clean` removes make/obj folder where all object files are located <br>  
`make clean_exec` removes make/deng folder where executable and other program data is located <br>