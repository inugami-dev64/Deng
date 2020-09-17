set(CMAKE_CXX_FLAGS "-Wall -std=c++11")
include_directories(headers)
include_directories(../dependencies/src)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/deng)

if(UNIX)
    link_directories(../dependencies/lib/unix)
elseif(WIN32)
    link_directories(../dependencies/lib/windows)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    link_directories(../dependencies/lib/debug)
endif()

add_executable(${PROJECT_NAME}
    core/camera.cpp
    core/renderer.cpp
    core/rendererutils.cpp
    core/window.cpp

    maths/dengmath.cpp
    maths/events.cpp
    maths/objectshandler.cpp

    utilities/filemanager.cpp
    utilities/gridgenerator.cpp
    utilities/objloader.cpp
    utilities/textureloader.cpp
    utilities/timer.cpp

    dengui/dengui.cpp
    dengui/sprites.cpp

    main.cpp
)

if(UNIX)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE glfw3
        PRIVATE vulkan
        PRIVATE pthread
        PRIVATE X11
        PRIVATE dl
    )

    if(CMAKE_BUILD_TYPE MATCHES Debug)
        target_link_libraries(${PROJECT_NAME} 
            PRIVATE VkLayer_utils
        )

        message("Debug mode detected!")

        file(APPEND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/run_debug.sh "#!/bin/bash\n")
        file(APPEND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/run_debug.sh "VK_LAYER_PATH=../../dependencies/explicit_layer.d ./deng")
    endif()

elseif(WIN32)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE glfw3dll
        PRIVATE opengl32
        PRIVATE vulkan-1
    )

    if(CMAKE_BUILD_TYPE MATCHES Debug) 
        target_link_libraries(${PROJECT_NAME}
            PRIVATE VkLayer_utils
        )
    endif()

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/dependencies/lib/windows/glfw3.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )

endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/objects $<TARGET_FILE_DIR:${PROJECT_NAME}>/objects
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/textures $<TARGET_FILE_DIR:${PROJECT_NAME}>/textures
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/config $<TARGET_FILE_DIR:${PROJECT_NAME}>/config
)