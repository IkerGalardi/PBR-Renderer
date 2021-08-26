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

const float PI = 3.1415926535897932384626433832795;

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
const float light_intensity = 5.0;

// Schlick's aproximation to fresnel.
// See https://en.wikipedia.org/wiki/Schlick%27s_approximation for more info.
float fresnel_schlick(float cos_theta, float F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cos_theta, 5.0);
}

// Trowbridge-Reitz GGX normal distribution function.
float distribution_ggx(vec3 N, vec3 H, float a)
{
    float a2     = a * a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

// Schlick's approximation of geometry distribution function.
float geometry_schlick_ggx(float NdotV, float k)
{
    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return nom / denom;
}

// Geometry distribution taking into account geometry obstruction (with view direction) and 
// geometry shadowing (with light direction).
float geometry_smith(vec3 N, vec3 V, vec3 L, float k)
{
    float NdotV = max(dot(V, N), 0.0);
    float NdotL = max(dot(L, N), 0.0);
    float ggx1 = geometry_schlick_ggx(NdotV, k);
    float ggx2 = geometry_schlick_ggx(NdotL, k);
	
    return ggx1 * ggx2;
}

vec3 calculate_normal_in_world_space(vec3 normal_texture) {
    vec3 texture_normalized = normal_texture * 2.0 - 1.0;

    vec3 Q1  = dFdx(v_fragment_position);
    vec3 Q2  = dFdy(v_fragment_position);
    vec2 st1 = dFdx(v_texture_coordinates);
    vec2 st2 = dFdy(v_texture_coordinates);

    vec3 N   = normalize(v_normal);
    vec3 T  = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 tbn = mat3(T, B, N);

    return tbn * texture_normalized;
}

void main()
{
    // Gather from the textures all the necessary values
    vec3 base_color = pow(texture(diffuse_texture, v_texture_coordinates).xyz, vec3(2.2));
    float roughness_value = texture(diffuse_texture, v_texture_coordinates).x;
    vec3 normal_in_tangent_space = texture(normal_texture, v_texture_coordinates).xyz;
    vec3 normal_vector = calculate_normal_in_world_space(normal_in_tangent_space);

    // Remapping of variables taken from Unreal Engine. (See Unreal Engine 4 shading documentation from Epic)
    float alpha = roughness_value * roughness_value;
    float k = pow((roughness_value + 1), 2) / 8;

    // Typical variables used when shading
    vec3 N = normalize(normal_vector);
    vec3 V = normalize(u_camera_position - v_fragment_position);
    vec3 L = normalize(light_direction);
    vec3 H = normalize(V + L);

    vec3 lambertian_diffuse = base_color / PI;

    // Calculate the cook torrance specular part.
    //          N * G * F
    // f = -------------------
    //      4 * (N*V) * (N*L)
    float fresnel_distribution = fresnel_schlick(max(dot(H, V), 0.0), 0.33);
    float geometry_distribution = geometry_smith(N, V, L, k);
    float normal_distribution = distribution_ggx(N, H, alpha);
    float numerator = normal_distribution * fresnel_distribution * geometry_distribution;
    float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
    float cook_torrance_specular = numerator / denominator;

    // Calculate fraction of specular and diffuse. Its done this way for energy conserving
    // as both specular and diffuse can't exceed 1 (unless light sources, they can exceed 1)
    float specular_fraction = fresnel_distribution;
    float diffuse_fraction = 1 - specular_fraction;

    // Calculate how much the fragment will be affected by the directional light.
    float radiance = max(dot(N, L) * light_intensity, 0.0);

    // Calculate the fractions and final lit fragment.
    vec3 final_diffuse = diffuse_fraction * lambertian_diffuse;
    float final_specular = specular_fraction * cook_torrance_specular;
    vec3 final_lit = (final_diffuse + final_specular) * radiance;

    // Correct for monitor's non linear brighness. 
    // For more information see https://learnopengl.com/Advanced-Lighting/Gamma-Correction
    vec3 gamma_corrected = pow(final_lit, vec3(1.0 / 2.2));

    out_color = vec4(gamma_corrected, 1.0);
}