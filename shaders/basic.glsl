#shader vert
#version 330 core

layout (location = 0) in vec3 in_position;

uniform mat4 view_projection_matrix;
uniform mat4 model_matrix;

void main()
{
    gl_Position = view_projection_matrix * model_matrix * vec4(in_position, 1.0);
}

#shader frag
#version 330 core

out vec4 out_color;

void main()
{
    out_color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
} 