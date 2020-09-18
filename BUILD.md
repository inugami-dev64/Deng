# Building intructions

## Linux, BSD and other unix like operating systems
### Dependencies
* xorg-server <br>
* make <br>
* cmake <br>
* vulkan compatible drivers <br><br>

### Build with validation layers and debug mode enabled
* Download Vulkan SDK (https://vulkan.lunarg.com/sdk/home) <br>
* Create folder named `debug` to `YOURPATH/Deng/dependencies/lib/` <br>
* Copy libVkLayer_khronos_validation.so and libVkLayer_utils.a to `YOURPATH/Deng/dependencies/lib/debug/` <br>
* In src/core/dengcore.h change `#define GENERIC_DEBUG 0` to `#define GENERIC_DEBUG 1` <br>
* Build the program using cmake: <br>
    `$ cd Deng` <br>
    `$ mkdir build` <br>
    `$ cd build` <br>
    `$ cmake -DCMAKE_BUILD_TYPE=Debug ..` <br>
    `$ make` <br>
* Run the program using generated script: <br>
    `$ cd deng` <br>
    `$ chmod +x run_debug.sh` <br>
    `$ ./run_debug.sh` <br><br>

### Build with validtation layers and debug mode disabled
* Build the program using cmake: <br>
    `$ cd Deng` <br>
    `$ mkdir build` <br>
    `$ cd build` <br>
    `$ cmake ..` <br>
    `$ make` <br>
* Run the binaries with following commands: <br>
    `$ cd deng` <br>
    `$ ./deng` <br>

## Windows
### Dependencies
* mingw-w64 <br>
* cmake <br>
* vulkan compatible drivers <br>

### Build with validation layers and debug mode enabled
* Download Vulkan SDK (https://vulkan.lunarg.com/sdk/home) <br>
* Create folder named `debug` to `C:\YOURPATH\Deng\dependencies\lib\` <br>
* Copy VkLayer_utils.lib to `C:\YOURPATH\Deng\dependencies\lib\debug\` <br>
* In src/core/dengcore.h change `#define GENERIC_DEBUG 0` to `#define GENERIC_DEBUG 1` <br>
* Create a build folder <br>
* Open cmake-gui <br>
* Specify the source code path as `C:\YOURPATH\Deng` <br>
* Specify the binary directory to your build folder <br>
* For CMAKE_BUILD_TYPE parameter type `Debug` <br>
* Configure and select "MinGw Makefiles" for the generator <br>
* Generate build files <br>
* Open cmd <br>
* cd to build path <br>
* Build binaries using `mingw32-make` <br>

### Build with validation layers and debug mode disabled
* Create a build folder <br>
* Open cmake-gui <br>
* Specify the source code path as `C:\YOURPATH\Deng` <br>
* Specify the binary directory to your build folder <br>
* Configure and select "MinGw Makefiles" for the generator <br>
* Generate build files <br>
* Open cmd <br>
* cd to build path <br>
* Build binaries using `mingw32-make` <br>



