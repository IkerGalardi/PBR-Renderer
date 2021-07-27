#shader vert
#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texture_coords;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec4 v_test;

void main()
{
    gl_Position = proj * view * model * vec4(in_position, 1.0);
}

#shader frag
#version 330 core

in vec4 v_test;

out vec4 out_color;

void main()
{
    out_color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
} 