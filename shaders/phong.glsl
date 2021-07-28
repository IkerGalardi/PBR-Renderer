#shader vert
#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texture_coords;

uniform mat4 u_proj;
uniform mat4 u_view;
uniform mat4 u_model;

out vec3 v_normal;
out vec2 v_texture_coordinates;

void main()
{
    // Variables to pass to fragment shader
    v_normal = in_normal;
    v_texture_coordinates = in_texture_coords;

    gl_Position = u_proj * u_view * u_model * vec4(in_position, 1.0);
}

#shader frag
#version 330 core

in vec3 v_normal;
in vec2 v_texture_coordinates;

out vec4 out_color;

uniform sampler2D diffuse_texture;
uniform sampler2D roughness_texture;
uniform sampler2D normal_texture;

const float ambient_color = 0.3;
const vec3 light_direction = vec3(0.0, 1.0, 0.0);

void main()
{
    vec3 base_color = texture(diffuse_texture, v_texture_coordinates).xyz;
    float roughness_value = texture(diffuse_texture, v_texture_coordinates).x;
    vec3 normal_vector = texture(normal_texture, v_texture_coordinates).xyz;

    // Normalize light direction and normal not to mess with lighting
    vec3 normalized_normal = normalize(v_normal);
    vec3 normalized_light = normalize(light_direction);

    float light_value = max(dot(normalized_light, normalized_normal), 0.0);

    vec3 final_color = ambient_color * base_color + light_value * base_color;
    out_color = vec4(min(final_color, vec3(1.0)), 1.0f);
} 