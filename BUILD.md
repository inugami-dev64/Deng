# Building intructions (No Windows)

## GNU/Linux dependencies
* xorg-server <br>
* make <br>
* CMake <br>
* vulkan-loader <br>
* vulkan-layers (optional) <br>
* GLFW <br>

## Windows dependencies
* MinGW-w64 <br>
* CMake <br>
* vulkan compatible drivers <br>
* vulkan validation layers (optional) <br>
* GLFW <br>

### Enabling debug mode
For debuging purposes there are several debug macros which can be enabled before building. Debugging allows for verbose console output. <br>
There are multiple macros for controlling debugging in src/core/deng_core.h 
*    `GENERAL_DEBUG` enables vulkan validation layers and verbose logging to the console <br>
*    `CAMERA_LOCATION_DEBUG` logs the camera location to the console in vector space <br>
*    `CAMERA_MOUSE_DEBUG` logs mouse position to console (x axis: -4 to 4; y axis: -1 to 1) <br>
*    When creating Makefile, set CMake CMAKE_BUILD_TYPE variable value to DEBUG <br>

### Building
#### Using GNU/Linux
* `$ cd deng`
* `$ mkdir build` <br>
* `$ cmake ..` <br>
* `$ make` <br>

#### Using window
* Open cmake-gui <br>
* Specify the source code path as `C:\YOURPATH\deng` <br>
* Specify the binary directory to your build folder <br>
* Configure and select "MinGw Makefiles" for the generator <br>
* Generate build files <br>
* Open cmd <br>
* cd to build path <br>
* Build binaries using `mingw32-make` <br>