#!/bin/bash

glslc src/2d_tex_mapped.frag -o bin/2d_tex_mapped_frag.spv
glslc src/2d_tex_mapped.vert -o bin/2d_tex_mapped_vert.spv 
glslc src/2d_unmapped.frag -o bin/2d_unmapped_frag.spv 
glslc src/2d_unmapped.vert -o bin/2d_unmapped_vert.spv 

glslc src/3d_tex_mapped.frag -o bin/3d_tex_mapped_frag.spv 
glslc src/3d_tex_mapped.vert -o bin/3d_tex_mapped_vert.spv 
glslc src/3d_unmapped.frag -o bin/3d_unmapped_frag.spv 
glslc src/3d_unmapped.vert -o bin/3d_unmapped_vert.spv 