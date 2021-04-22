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

-- Available options are following
-- [[ use-modules -- build all submodules with DENG
--                   It is recommended in Windows but usually not in GNU/Linux systems
-- [[ build-static -- build static library instead of dynamic library
--                    Recommended for micro optimization purposes, otherwise prefer dynamc libraries
-- [[ no-sandbox -- do not build any sandbox applications
--

--local helpers = require("modules/premake_helpers/helpers")
workspace "deng"
    configurations { "Debug", "Release" }
    platforms { "Win32", "Linux" }
    includedirs { "./headers" }
	architecture "x86_64"
    targetdir "build"
	pic "On"

	-- Ignore safety warnings that MSVC gives
	filter "platforms:Win32"
		defines { "_CRT_SECURE_NO_WARNINGS" }


-- Define the clean action 
if _ACTION == "clean" then
	if(package.config:sub(1,1) == '/') then
		os.execute("rm -rf *.make obj Makefile")
		os.exit()
	else
		os.execute("del /s /q deng.sln obj *.vcxproj*")
		os.exit()
	end
	print("Successfully cleaned generated project files")
end

-- Create an option to compile all submodule dependencies from source
newoption {
    trigger = "use-modules",
    description = "Use submodule dependencies instead of searching them in system path"
}

-- Create an option to specify vulkan sdk library location (Windows only)
newoption {
	trigger = "vk-sdk-path",
	description = "Specify Vulkan SDK path for Windows builds "
}


-- Check if Vulkan SDK path should and is specified
if _OPTIONS["vk-sdk-path"] then
	if package.config:sub(1,1) == '/' then
		error("Vulkan SDK path is not supposed to be specified in GNU/Linux builds")
	else
		libdirs { _OPTIONS["vk-sdk-path"] .. "\\Lib" }
		includedirs{ _OPTIONS["vk-sdk-path"] .. "\\Include" }
	end
elseif package.config:sub(1,1) == '\\' then
	error("No Vulkan SDK path specified on Windows build")
end


-- Check if all submodule build configs should be created
if _OPTIONS["use-modules"] then
	includedirs { 
		"modules/freetype/include", 
		"modules/freetype/include/freetype"
	}
    local ft = require("premake/ft")
    ft.build()
else
	filter "platforms:Win32"
		error("Please use use-modules option on Windows builds!")
	includedirs { "/usr/include/freetype2", "/usr/include/freetype2/freetype" }
end


-- Create an option to build DENG as a static library instead of shared library (Windows only)
newoption {
    trigger = "build-static",
    description = "Build libdeng as static library (Windows only)"
}

if _OPTIONS["build-static"] and package.config:sub(1,1) == '/' then
	error("Building DENG as static library is unsupported in GNU/Linux systems")
end


-- Create an option to build DENG sandboxapp using specific sandbox configuration
newoption {
    trigger = "sandbox-mode",
    value = "MODE",
    description = "Select sandbox application mode",
    allowed = {
        { "DEFAULT", "Use default Renderer class" },
        { "VKR", "Use Vulkan renderer class specifically" },
        { "NONE", "Do not build sandbox application" }
    }
}


-- Specify default sandboxapp configuration
if not _OPTIONS["sandbox-mode"] then
    _OPTIONS["sandbox-mode"] = "NONE"
elseif _OPTIONS["sandbox-mode"] ~= "NONE" then
    local sandbox_data = require("premake/sandbox_data")
    sandbox_data.datacpy();
end


-- Libdeng build configuration
local libdeng = require("premake/libdeng")
if _OPTIONS["build-static"] then
    libdeng.build(true)
else 
    libdeng.build(false)
end


-- DENG asset manager build configuration
local dam = require("premake/dam")
dam.build()


-- Sandbox application build configuration
if _OPTIONS["sandbox-mode"] == "VKR" then
    local sandbox = require("premake/vk_sandbox")
    sandbox.build()
elseif _OPTIONS["sandbox-mode"] == "DEFAULT" then
    local sandbox = require("premake/default_sandbox")
    sandbox.build()
end
