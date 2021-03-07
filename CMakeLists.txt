cmake_minimum_required(VERSION 3.10)

project(deng CXX C)
set(SANDBOX_TARGET "sandboxapp")
set(LIBDENG_TARGET "deng")
set(LIBFREETYPE_TARGET "freetype")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Source list for libfreetype
set (
    LIBFREETYPE_CORE_SRC_LIST
    modules/freetype2/src/base/ftsystem.c
    modules/freetype2/src/base/ftinit.c
    modules/freetype2/src/base/ftdebug.c
    modules/freetype2/src/base/ftbase.c
    modules/freetype2/src/base/ftbbox.c
    modules/freetype2/src/base/ftglyph.c
    modules/freetype2/src/base/ftbitmap.c
    modules/freetype2/src/base/ftwinfnt.c
    modules/freetype2/src/cff/cff.c
    modules/freetype2/src/truetype/truetype.c
    modules/freetype2/src/winfonts/winfnt.c
    modules/freetype2/src/raster/raster.c
    modules/freetype2/src/smooth/smooth.c
    modules/freetype2/src/autofit/autofit.c
    modules/freetype2/src/type1/type1.c
    modules/freetype2/src/cid/type1cid.c
    modules/freetype2/src/pfr/pfr.c
    modules/freetype2/src/psaux/psaux.c
    modules/freetype2/src/psnames/psnames.c
    modules/freetype2/src/pshinter/pshinter.c
    modules/freetype2/src/sfnt/sfnt.c
    modules/freetype2/src/bdf/bdf.c
    modules/freetype2/src/type42/type42.c
    modules/freetype2/src/pcf/pcf.c
    modules/freetype2/src/bzip2/ftbzip2.c
    modules/freetype2/src/lzw/ftlzw.c
    modules/freetype2/src/gzip/ftgzip.c
)


# Source list for libdeng excluding all platform specific sources
set (
    LIBDENG_CORE_SRC_LIST
    src/deng/camera.cpp
    src/deng/deng_math.cpp
    src/deng/key_translation.c
    src/deng/key_ev.c
    src/deng/renderer.cpp
    src/deng/window.cpp
    src/deng/vulkan/vulkan_handler.c
    src/deng/vulkan/vulkan_pipelines.cpp
    src/deng/vulkan/vulkan_qm.cpp
    src/deng/vulkan/vulkan_renderer.cpp
    src/deng/vulkan/vulkan_resources.cpp
    src/deng/vulkan/vulkan_sd.cpp
    src/utils/font.cpp
    src/utils/collision.cpp
    src/utils/timer.cpp
    src/utils/shapes.cpp
    src/utils/grid_generator.cpp
    src/dengui/vulkan/vulkan_update.cpp
    src/dengui/dengui_child.cpp
    src/dengui/dengui_events.cpp
    src/dengui/dengui_window.cpp
    src/dengui/map_editor_ui.cpp
    src/dengui/ui_callbacks.cpp
    src/common/common.c
    src/common/hashmap.c
    src/das/das_handler.c
    src/das/data_loader.c
)


# Source list for sandbox application
set (
    SANDBOX_CORE_SRC_LIST
    src/main/sandbox_3d.cpp
)

# Linux specific build configuration
# if(LINUX)
#     add_library (
#         ${LIBDENG_TARGET} SHARED
#         src/deng/camera.cpp
#         src/deng/deng_math.cpp
#         src/deng/key_translation.c
#         src/deng/key_ev.c
#         src/deng/renderer.cpp
#         src/deng/window.cpp
#         src/deng/vulkan/vulkan_handler.c
#         src/deng/vulkan/vulkan_pipelines.cpp
#         src/deng/vulkan/vulkan_qm.cpp
#         src/deng/vulkan/vulkan_renderer.cpp
#         src/deng/vulkan/vulkan_resources.cpp
#         src/deng/vulkan/vulkan_sd.cpp
#         src/deng/x11_surface.c
#         src/utils/font.cpp
#         src/utils/collision.cpp
#         src/utils/timer.cpp
#         src/utils/shapes.cpp
#         src/utils/grid_generator.cpp
#         src/dengui/vulkan/vulkan_update.cpp
#         src/dengui/dengui_child.cpp
#         src/dengui/dengui_events.cpp
#         src/dengui/dengui_window.cpp
#         src/dengui/map_editor_ui.cpp
#         src/dengui/ui_callbacks/cpp
#         src/common/common.c
#         src/common/hashmap.c
#         src/das/das_handler.c
#         src/das/data_loader.c
#     )

#     if(BUILD_FREETYPE OR BUILD_ALL)
#         add_library (
#             ${LIBFREETYPE_TARGET} SHARED
#             modules/freetype2/src/base/ftsystem.c
#             modules/freetype2/src/base/ftinit.c
#             modules/freetype2/src/base/ftdebug.c
#             modules/freetype2/src/base/ftbase.c
#             modules/freetype2/src/base/ftbbox.c
#             modules/freetype2/src/base/ftglyph.c
#             modules/freetype2/src/base/ftbitmap.c
#             modules/freetype2/src/base/ftwinfnt.c
#             modules/freetype2/src/cff/cff.c
#             modules/freetype2/src/truetype/truetype.c
#             modules/freetype2/src/winfonts/winfnt.c
#             modules/freetype2/src/raster/raster.c
#             modules/freetype2/src/smooth/smooth.c
#         )

#         target_link_libraries (
#             ${LIBDENG_TARGET}
#             ${LIBFREETYPE_TARGET}
#             comctl32
#             vulkan-1
#         )
#     endif()

# # Windows specific build configuration
# elseif(WIN32)
#     add_library (
#         ${LIBDENG_TARGET} SHARED
#         src/deng/camera.cpp
#         src/deng/deng_math.cpp
#         src/deng/key_translation.c
#         src/deng/key_ev.c
#         src/deng/renderer.cpp
#         src/deng/window.cpp
#         src/deng/vulkan/vulkan_handler.c
#         src/deng/vulkan/vulkan_pipelines.cpp
#         src/deng/vulkan/vulkan_qm.cpp
#         src/deng/vulkan/vulkan_renderer.cpp
#         src/deng/vulkan/vulkan_resources.cpp
#         src/deng/vulkan/vulkan_sd.cpp
#         src/deng/win32_surface.c
#         src/utils/font.cpp
#         src/utils/collision.cpp
#         src/utils/timer.cpp
#         src/utils/shapes.cpp
#         src/utils/grid_generator.cpp
#         src/dengui/vulkan/vulkan_update.cpp
#         src/dengui/dengui_child.cpp
#         src/dengui/dengui_events.cpp
#         src/dengui/dengui_window.cpp
#         src/dengui/map_editor_ui.cpp
#         src/dengui/ui_callbacks.cpp
#         src/common/common.c
#         src/common/hashmap.c
#         src/das/das_handler.c
#         src/das/data_loader.c
#     )

# Prepare Windows build
function(PrepWin32 vk_sdk_path)
    # Add freetype to build list if needed
    if(BUILD_FREETYPE OR BUILD_ALL) 
        add_library (
            ${LIBFREETYPE_TARGET} SHARED
            ${LIBFREETYPE_CORE_SRC_LIST}
        )

        target_include_directories (
            ${LIBFREETYPE_TARGET} PUBLIC
            modules/freetype2/include
        )
    endif()

    # libdeng target
    add_library (
        ${LIBDENG_TARGET} STATIC
        ${LIBDENG_CORE_SRC_LIST}
        src/deng/win32_surface.c
    )

    target_include_directories (
        ${LIBDENG_TARGET}
        PUBLIC ./headers
        PUBLIC "${vk_sdk_path}/Include"
        PUBLIC modules/freetype2/include
    )

    target_link_directories (
        ${LIBDENG_TARGET}
        PUBLIC "${vk_sdk_path}/Lib"
    )

    target_link_libraries (
        ${LIBDENG_TARGET}
        ${LIBFREETYPE_TARGET}
        comctl32
        vulkan-1
    )

    # Sandbox application build 
    if(BUILD_SANDBOX_APP OR BUILD_ALL)
        add_executable (
            ${SANDBOX_TARGET}
            ${SANDBOX_CORE_SRC_LIST}
        )

        target_link_directories (
            ${SANDBOX_TARGET} 
            PUBLIC ${BINARY_DIR}
        )
        
        target_link_libraries (
            ${SANDBOX_TARGET}
            ${LIBDENG_TARGET}
            ${LIBFREETYPE_TARGET}
            comctl32
            vulkan-1
        )

        target_include_directories (
            ${SANDBOX_TARGET} PUBLIC
            ${PROJECT_SOURCE_DIR}/headers
            "${vk_sdk_path}/Include"
            modules/freetype2/include
        )

        # Copy sandbox texture and asset
        file (
            COPY ${CMAKE_CURRENT_SOURCE_DIR}/textures/viking_room.tga
            DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
        )

        file (
            COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/viking_unor.das
            DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
        )

        add_custom_command (
            TARGET ${SANDBOX_TARGET}
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${SANDBOX_TARGET}>/shaders
        )
    endif()

    # Copy sandbox texture asset also shaders
endfunction()

if(WIN32)
    if(NOT VULKAN_SDK_PATH)
        message(SEND_ERROR "No Vulkan SDK path specified")
        message("Please specify Vulkan SDK path using VULKAN_SDK_PATH variable")
        return()
    endif()
    PrepWin32(${VULKAN_SDK_PATH})
endif()