# DimentionalENG (Deng)

Deng is a cross platform game engine based on vulkan! <br> 
It is currently in early development stage! <br>
<br>

## Object and texture formats
### Model formats
* .obj - currently supported <br>
* .stl - support will be added soon <br>
<br>

### Texture formats
* .bmp - only uncompressed supported <br>
* .tga - only uncompressed supported <br>
* .png - support will be added soon <br>
* .jpg - support will be added soon <br>
<br>

## Compiling

#### In order to compile and run the engine in release mode <br>
`$ mkdir build ` <br>
`$ cd build` <br>
`$ cmake ..` <br>
`$ make` <br>
`$ cd deng` <br>
`$ chmod +x run_debug.sh` <br>
`$ ./run_debug` <br>
 
#### In order to compile and run the engine in debug mode with Vulkan validation layers enabled <br>
* Download Vulkan SDK (https://www.lunarg.com/vulkan-sdk/) <br>
* Copy libVkLayer_khronos_validation.so and libVkLayer_utils.so to dependencies/lib/debug <br>
* Change ```#define DEBUG 0``` to ```#define DEBUG 1``` in renderer.h
* Build and run the program: <br> 
`$ mkdir build ` <br>
`$ cd build` <br>
`$ cmake -DCMAKE_BUILD_TYPE=Debug ..` <br>
`$ make` <br>
`$ cd deng` <br>
`$ ./deng` <br>
    
Link to Liberty Statue model used as a example: https://free3d.com/3d-model/statue-of-liberty-73656.html <br> 
    
PS! I haven't managed yet to successfully compile and run this program on windows! <br>

 

 
