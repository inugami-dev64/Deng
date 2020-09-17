#!/bin/bash

glslc src/deng/object_shader.vert -o bin/deng/object_vert.spv
glslc src/deng/object_shader.frag -o bin/deng/object_frag.spv
glslc src/deng/specified_shader.vert -o bin//deng/specified_vert.spv
glslc src/deng/specified_shader.frag -o bin/deng/specified_frag.spv

glslc src/dengui/ui_shader.vert -o bin/dengui/ui_vert.spv
glslc src/dengui/ui_shader.frag -o bin/dengui/ui_frag.spv