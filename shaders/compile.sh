#!/bin/bash

glslc src/main_shader.vert -o bin/main_vert.spv
glslc src/main_shader.frag -o bin/main_frag.spv
glslc src/grid_shader.vert -o bin/grid_vert.spv
glslc src/grid_shader.frag -o bin/grid_frag.spv