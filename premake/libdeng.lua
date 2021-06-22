--    ____         ________     __      _        ______
--   |  _ \_      |  ______|   |  \    | |     _/  ____\
--   | | \_ \_    |  |         |   \   | |    /  _/
--   | |   \  |   |  |_____    | |\ \  | |   |  |   _____
--   | |    | |   |  ______|   | | \ \ | |   |  |  |___  |
--   | |  _/ _|   |  |         | |  \ \| |    \  \    / /
--   | |_/ _/     |  |_____    | |   \   |     \_ \__/ /
--   |____/       |________|   |_|    \__|       \____/
--
-- __________________________________________________________
-- __________________________________________________________
--                       Project DENG
--
--
-- Copyright (C) 2020 - 2021
-- This Software is licensed under Apache License as described
-- in the LICENSE.md file, which you should have recieved with
-- this distribution.
--
-- You may reproduce and distribute copies of the
-- Work or Derivative Works thereof in any medium, with or without
-- modifications, and in Source or Object form, provided that You
-- meet the following conditions:
--
-- (a) You must give any other recipients of the Work or
--     Derivative Works a copy of this License; and
--
-- (b) You must cause any modified files to carry prominent notices
--     stating that You changed the files; and
--
-- (c) You must retain, in the Source form of any Derivative Works
--     that You distribute, all copyright, patent, trademark, and
--     attribution notices from the Source form of the Work,
--     excluding those notices that do not pertain to any part of
--     the Derivative Works; and
--
-- (d) If the Work includes a "NOTICE" text file as part of its
--     distribution, then any Derivative Works that You distribute must
--     include a readable copy of the attribution notices contained
--     within such NOTICE file, excluding those notices that do not
--     pertain to any part of the Derivative Works, in at least one
--     of the following places: within a NOTICE text file distributed
--     as part of the Derivative Works; within the Source form or
--     documentation, if provided along with the Derivative Works; or,
--     within a display generated by the Derivative Works, if and
--     wherever such third-party notices normally appear. The contents
--     of the NOTICE file are for informational purposes only and
--     do not modify the License. You may add Your own attribution
--     notices within Derivative Works that You distribute, alongside
--     or as an addendum to the NOTICE text from the Work, provided
--     that such additional attribution notices cannot be construed
--     as modifying the License.
--
-- You may add Your own copyright statement to Your modifications and
-- may provide additional or different license terms and conditions
-- for use, reproduction, or distribution of Your modifications, or
-- for any such Derivative Works as a whole, provided Your use,
-- reproduction, and distribution of the Work otherwise complies with
-- the conditions stated in this License.
--

local libdeng = {}

-- Build libdeng library
function libdeng.build()
    project "deng"
        if os.istarget("windows") then
            kind "StaticLib"
        else
            kind "SharedLib"
        end
        language "C++"
        cppdialect "C++14"

        files {
            "headers/**/*.h",
            "src/common/*.c",
            "src/data/*.c",
            "src/deng/**.cpp",
            "src/deng/**.c",
            "src/imgui-layer/ui_manager.cpp",
            "src/math/camera_mat.cpp",
            "src/math/conversion.cpp",
            "src/math/projection_mat.cpp",
            "src/math/transformer.cpp",
            "src/utils/*.cpp",
            "modules/glad/src/glad.c"
        }

        removefiles { "src/deng/renderer.cpp" }

        -- Link all Windows specific dependencies
        filter "platforms:Win32"
            removefiles { "src/deng/surface/x11_surface.c" }
            links {
                "freetype",
                "imgui",
                "vulkan-1",
                "comctl32",
				"iphlpapi"
            }

        -- Link all GNU/Linux specific dependencies
        filter "platforms:Linux"
            removefiles { "src/deng/surface/win32_surface.c" }
            links {
                "freetype",
                "imgui",
                "vulkan",
                "pthread",
                "X11",
                "GL",
                "GLU",
                "Xcursor",
                "dl"
            }

        filter "configurations:Debug"
            defines { "__DEBUG" }
        filter {}
end

return libdeng
