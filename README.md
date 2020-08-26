# DimentionalENG (Deng)
Deng is a project to make cross platform game engine based on vulkan! <br>
Currently developing renderer <br>
<br>

In order to compile and run the engine with Vulkan validation layers enabled <br>
    - Download Vulkan SDK (https://www.lunarg.com/vulkan-sdk/) <br>
    - Copy libVkLayer_khronos_validation.so and libVkLayer_utils.so to dependencies/lib/debug <br>
    - Link -lVkLayer_utils and -lVkLayer_khronos_validation to makefile <br>
    - Use either ``` $ make run ``` or ```$ VK_LAYER_PATH=dependencies/explicit_layer.d ./deng ``` <br>
    
In order to compile and run the engine without validation layers <br>
    - In renderer.h change ``` #define DEBUG 1 ``` to ``` #define DEBUG 0 ``` <br>
    - Run ``` $ make precompile ``` and then ``` $ make ``` <br>
    
Link to Liberty Statue model used as a example: https://free3d.com/3d-model/statue-of-liberty-73656.html <br> 
    
PS! I haven't tried to compile and run it on windows yet! <br>

 
