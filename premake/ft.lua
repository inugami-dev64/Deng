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
--     Derivative Works a copy of this License and
--
-- (b) You must cause any modified files to carry prominent notices
--     stating that You changed the files and
--
-- (c) You must retain, in the Source form of any Derivative Works
--     that You distribute, all copyright, patent, trademark, and
--     attribution notices from the Source form of the Work,
--     excluding those notices that do not pertain to any part of
--     the Derivative Works and
--
-- (d) If the Work includes a "NOTICE" text file as part of its
--     distribution, then any Derivative Works that You distribute must
--     include a readable copy of the attribution notices contained
--     within such NOTICE file, excluding those notices that do not
--     pertain to any part of the Derivative Works, in at least one
--     of the following places: within a NOTICE text file distributed
--     as part of the Derivative Works within the Source form or
--     documentation, if provided along with the Derivative Works or,
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


local ft = {}

-- Create freetype library build config
function ft.build()
    project "freetype"
        kind "StaticLib"
        language "C"
        defines { "FT2_BUILD_LIBRARY" }

        includedirs { "modules/freetype/include" }
        files {
            "modules/freetype/src/base/ftinit.c",
            "modules/freetype/src/base/ftdebug.c",
            "modules/freetype/src/base/ftbase.c",
            "modules/freetype/src/base/ftbbox.c",
            "modules/freetype/src/base/ftglyph.c",
            "modules/freetype/src/base/ftbitmap.c",
            "modules/freetype/src/base/ftwinfnt.c",
            "modules/freetype/src/cff/cff.c",
            "modules/freetype/src/truetype/truetype.c",
            "modules/freetype/src/winfonts/winfnt.c",
            "modules/freetype/src/raster/raster.c",
            "modules/freetype/src/smooth/smooth.c",
            "modules/freetype/src/autofit/autofit.c",
            "modules/freetype/src/type1/type1.c",
            "modules/freetype/src/cid/type1cid.c",
            "modules/freetype/src/pfr/pfr.c",
            "modules/freetype/src/sdf/sdf.c",
            "modules/freetype/src/psaux/psaux.c",
            "modules/freetype/src/psnames/psnames.c",
            "modules/freetype/src/pshinter/pshinter.c",
            "modules/freetype/src/sfnt/sfnt.c",
            "modules/freetype/src/bdf/bdf.c",
            "modules/freetype/src/type42/type42.c",
            "modules/freetype/src/pcf/pcf.c",
            "modules/freetype/src/bzip2/ftbzip2.c",
            "modules/freetype/src/lzw/ftlzw.c",
            "modules/freetype/src/gzip/ftgzip.c"
        }

    symbols "Off"
    optimize "On"

	filter "platforms:Linux"
		defines { 
            "HAVE_FCNTL_H",
            "HAVE_UNISTD_H"
		}
		files { "modules/freetype/builds/unix/ftsystem.c" }

	filter "platforms:Win32"
		removefiles { "modules/freetype/src/base/ftdebug.c" }
		files { "modules/freetype/builds/windows/ftsystem.c" }
end

return ft
