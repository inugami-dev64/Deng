set(CMAKE_CXX_FLAGS "-Wall -std=c++11")

# include_directories(headers)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/deng)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/deng/lib)

set(LIB_SURFACE "surface")

add_library(${LIB_SURFACE} SHARED 
    surface/x11_surface.c 
    surface/key_translation.c
    surface/key_vector.c
    surface/vulkan_handler.c
)

add_executable(${PROJECT_NAME} 
    core/camera.cpp
    core/renderer.cpp
    core/renderer_utils.cpp
    core/window.cpp

    maths/deng_math.cpp
    maths/events.cpp

    utilities/data_handler/objects_handler.cpp
    utilities/data_handler/file_manager.cpp
    utilities/map/grid_generator.cpp
    utilities/assets_handler/object_loader.cpp
    utilities/assets_handler/texture_loader.cpp
    utilities/timer/timer.cpp

    dengui/core/dengui_window.cpp
    dengui/pixel_perfect_collision/dengui_pixel_collision.cpp
    # dengui/events/dengui_events.cpp
    dengui/core/dengui_sprites.cpp
    # dengui/events/dengui_event_handles.cpp

    dengui/css_engine/css_data_manager.cpp
)


if(MANUAL_TEST MATCHES FM_FOLDER_CONTENTS_TEST)
    target_sources(${PROJECT_NAME}
        PRIVATE main/feature_testing/fm_test/fm_folder_contents_test.cpp
    )

elseif(MANUAL_TEST MATCHES FM_CSS_PROPERTY_TEST)
    target_sources(${PROJECT_NAME}
        PRIVATE main/feature_testing/fm_test/fm_css_property_test.cpp
    )

elseif(MANUAL_TEST MATCHES TYPENAME_FINDER_TEST)
    target_sources(${PROJECT_NAME} 
        PRIVATE main/feature_testing/typename_finder_test/typename_finder_test.cpp
    )

elseif(MANUAL_TEST MATCHES MATH_CONVERSION_TEST)
    target_sources(${PROJECT_NAME}
        PRIVATE main/feature_testing/math_test/math_conversion_test.cpp
    )

elseif(MANUAL_TEST MATCHES CSS_ENGINE_TITLEBAR_TEST)
    target_sources(${PROJECT_NAME}
        PRIVATE main/feature_testing/css_engine_test/css_titlebar_data_test.cpp
    )

else()
    target_sources(${PROJECT_NAME} 
        PRIVATE main/main.cpp
    )

endif()

if(UNIX)

    target_link_libraries(${PROJECT_NAME}
        PRIVATE vulkan
        PRIVATE pthread
        PRIVATE X11
        PRIVATE Xcursor
        PRIVATE dl
        PUBLIC ${LIB_SURFACE}
    )

    if(CMAKE_BUILD_TYPE MATCHES DEBUG)
        target_link_libraries(${PROJECT_NAME} 
            PRIVATE VkLayer_utils
        )

        message("Debug mode detected!")
    endif()

elseif(WIN32)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE glfw3dll
        PRIVATE opengl32
        PRIVATE vulkan-1
        PUBLIC ${PROTOBUF_LIBRARY}
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
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/dengui/css $<TARGET_FILE_DIR:${PROJECT_NAME}>/ui_styling
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/surface/cursor $<TARGET_FILE_DIR:${PROJECT_NAME}>/cursor
)
