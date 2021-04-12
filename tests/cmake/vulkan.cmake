#    ____         ________     __      _        ______  
#   |  _ \_      |  ______|   |  \    | |     _/  ____\
#   | | \_ \_    |  |         |   \   | |    /  _/   
#   | |   \  |   |  |_____    | |\ \  | |   |  |   _____ 
#   | |    | |   |  ______|   | | \ \ | |   |  |  |___  | 
#   | |  _/ _|   |  |         | |  \ \| |    \  \    / /
#   | |_/ _/     |  |_____    | |   \   |     \_ \__/ /
#   |____/       |________|   |_|    \__|       \____/
#               
# __________________________________________________________
# __________________________________________________________
#                       Project DENG 
#
#
# Copyright (C) 2020 - 2021
# This Software is licensed under Apache License as described 
# in the LICENSE.md file, which you should have recieved with 
# this distribution.
# 
# You may reproduce and distribute copies of the
# Work or Derivative Works thereof in any medium, with or without
# modifications, and in Source or Object form, provided that You
# meet the following conditions:
#
# (a) You must give any other recipients of the Work or
#     Derivative Works a copy of this License and
#
# (b) You must cause any modified files to carry prominent notices
#     stating that You changed the files and
#
# (c) You must retain, in the Source form of any Derivative Works
#     that You distribute, all copyright, patent, trademark, and
#     attribution notices from the Source form of the Work,
#     excluding those notices that do not pertain to any part of
#     the Derivative Works and
#
# (d) If the Work includes a "NOTICE" text file as part of its
#     distribution, then any Derivative Works that You distribute must
#     include a readable copy of the attribution notices contained
#     within such NOTICE file, excluding those notices that do not
#     pertain to any part of the Derivative Works, in at least one
#     of the following places: within a NOTICE text file distributed
#     as part of the Derivative Works within the Source form or
#     documentation, if provided along with the Derivative Works or,
#     within a display generated by the Derivative Works, if and
#     wherever such third-party notices normally appear. The contents
#     of the NOTICE file are for informational purposes only and
#     do not modify the License. You may add Your own attribution
#     notices within Derivative Works that You distribute, alongside
#     or as an addendum to the NOTICE text from the Work, provided
#     that such additional attribution notices cannot be construed
#     as modifying the License.
#
# You may add Your own copyright statement to Your modifications and
# may provide additional or different license terms and conditions
# for use, reproduction, or distribution of Your modifications, or
# for any such Derivative Works as a whole, provided Your use,
# reproduction, and distribution of the Work otherwise complies with
# the conditions stated in this License.

set(VULKAN_TEST_TARGET "vk_rnd_test")
set (
    VULKAN_TEST_SOURCES
    src/deng/surface/x11_surface.c
    src/deng/surface/key_translation.c
    src/deng/surface/key_ev.c
    src/deng/window.cpp
    src/deng/camera/cam_base.cpp
    src/deng/camera/ed_cam.cpp
    src/deng/camera/fpp_cam.cpp
    src/deng/camera.cpp
    src/deng/vulkan/vulkan_dc.cpp
    src/deng/vulkan/vulkan_desc_c.cpp
    src/deng/vulkan/vulkan_handler.c
    src/deng/vulkan/vulkan_ic.cpp
    src/deng/vulkan/vulkan_pipelines.cpp
    src/deng/vulkan/vulkan_qm.cpp
    src/deng/vulkan/vulkan_ra.cpp
    src/deng/vulkan/vulkan_renderer.cpp
    src/deng/vulkan/vulkan_resources.cpp
    src/deng/vulkan/vulkan_scc.cpp
    src/deng/vulkan/vulkan_sd.cpp
    src/math/camera_mat.cpp
    src/math/conversion.cpp
    src/math/projection_mat.cpp
    src/math/transformer.cpp
    src/utils/timer.cpp
    src/utils/shape_gen.cpp
    src/common/common.c
    src/common/hashmap.c
    src/common/uuid.c
    src/das/das_loader.c
    src/das/tex_loader.c
    tests/vulkan/vulkan_test.cpp
)


add_executable (
    ${VULKAN_TEST_TARGET}
    ${VULKAN_TEST_SOURCES}
)


target_include_directories (
    ${VULKAN_TEST_TARGET}
    PUBLIC ./headers
    PUBLIC /usr/include/freetype2
    PUBLIC /usr/include/freetype2/include
)


target_link_libraries (
    ${VULKAN_TEST_TARGET}
    pthread
    vulkan
    X11
    Xcursor
    dl
)


add_custom_command (
    TARGET ${VULKAN_TEST_TARGET}
    PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${VULKAN_TEST_TARGET}>/assets
)

add_custom_command (
    TARGET ${VULKAN_TEST_TARGET}
    PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${VULKAN_TEST_TARGET}>/shaders
)
