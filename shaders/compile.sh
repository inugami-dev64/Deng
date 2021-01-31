#!/bin/bash

glslc src/2d_tex_mapped.vert -o bin/2d_tex_mapped.spv 
glslc src/2d_unmapped.vert -o bin/2d_unmapped.spv 

glslc src/3d_tex_mapped.vert -o bin/3d_tex_mapped.spv 
glslc src/3d_tex_mapped_nor.vert -o bin/3d_tex_mapped_nor.spv
glslc src/3d_unmapped.vert -o bin/3d_unmapped.spv
glslc src/3d_unmapped_nor.vert -o bin/3d_unmapped_nor.spv

# Fragment shaders
glslc src/unmapped.frag -o bin/unmapped_frag.spv
glslc src/tex_mapped.frag -o bin/tex_mapped_frag.spv
