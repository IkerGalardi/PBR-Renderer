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
out vec3 v_fragment_position;

void main()
{
    // Variables to pass to fragment shader
    v_normal = in_normal;
    v_texture_coordinates = in_texture_coords;
    
    // Position of fragment (pixel) on world space. This is going to be used to
    // calculate the specular part of phong shading.
    v_fragment_position = vec3(u_model * vec4(in_position, 1.0));

    gl_Position = u_proj * u_view * u_model * vec4(in_position, 1.0);
}

#shader frag
#version 330 core

const float PI = 3.1416;

in vec3 v_normal;
in vec2 v_texture_coordinates;
in vec3 v_fragment_position;

uniform vec3 u_camera_position;

out vec4 out_color;

uniform sampler2D diffuse_texture;
uniform sampler2D roughness_texture;
uniform sampler2D normal_texture;

const float ambient_color = 0.3;
const vec3 light_direction = vec3(1.0, 1.0, 0.0);

float fresnel_schlick(float cos_theta, float F0) {
    return F0 + (1.0 - F0) * pow(max(1.0 - cos_theta, 0.0), 5.0);
}

float distribution_ggx(vec3 N, vec3 H, float a)
{
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float geometry_schlick_ggx(float NdotV, float k)
{
    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return nom / denom;
}
  
float geometry_smith(vec3 N, vec3 V, vec3 L, float k)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = geometry_schlick_ggx(NdotV, k);
    float ggx2 = geometry_schlick_ggx(NdotL, k);
	
    return ggx1 * ggx2;
}

void main()
{
    // Gather from the textures all the necessary values
    vec3 base_color = texture(diffuse_texture, v_texture_coordinates).xyz;
    float roughness_value = texture(diffuse_texture, v_texture_coordinates).x;
    vec3 normal_vector = texture(normal_texture, v_texture_coordinates).xyz;

    vec3 N = normalize(v_normal);
    vec3 V = normalize(u_camera_position - v_fragment_position);
    vec3 L = normalize(light_direction);
    vec3 H = normalize(V + L);

    float cos_theta = max(dot(H, V), 0.0);
    float fresnel_distribution = fresnel_schlick(max(dot(H, V), 0.0), 0.33);
    float geometry_distribution = geometry_smith(N, V, L, roughness_value);
    float normal_distribution = distribution_ggx(N, H, roughness_value);

    float numerator = normal_distribution * fresnel_distribution * geometry_distribution;
    float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);

    float specular_fraction = fresnel_distribution;
    float diffuse_fraction = 1 - specular_fraction;

    float cook_torrance_specular = numerator / denominator;

    vec3 final_fragment = diffuse_fraction * base_color + 
                          specular_fraction * cook_torrance_specular;

    out_color = vec4(final_fragment, 1.0);
}