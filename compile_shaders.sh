#!/bin/bash

glslc --target-env=vulkan1.2 -fshader-stage=frag -o shaders/bin/flatcolor.fragment.sprv shaders/flatcolor.fragment.glsl
glslc --target-env=vulkan1.2 -fshader-stage=vert -o shaders/bin/triangle.vertex.sprv shaders/triangle.vertex.glsl