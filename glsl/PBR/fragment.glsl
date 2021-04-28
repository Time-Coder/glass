#version 330 core

#include "utils.glsl"

struct PointLight
{
    vec3 color;
    float Kl, Kq;
    vec3 position;
};

in vec3 frag_pos;
in vec3 norm;
in vec4 frag_base_color;
out vec4 frag_color;
uniform vec3 camera_pos;

// material parameters
uniform float metallic;
uniform float roughness;
uniform float ao;
uniform float ambient;
uniform PointLight light;

vec3 FresnelSchlick(float cos_theta_half, vec3 frag_base_color, float metallic)
{
	vec3 F0 = mix(vec3(0.04), frag_base_color, metallic);
    return F0 + (1.0 - F0) * pow(1.0 - cos_theta_half, 5.0);
}

float DistributionGGX(float cos_theta_half, float roughness)
{
    float roughness4 = pow(roughness, 4);
    float cos_theta_half2 = cos_theta_half*cos_theta_half;

    return roughness4 / (pi * pow(1.0f - (1.0f - roughness4)*cos_theta_half2, 2));
}

float GeometrySchlickGGX(float cos_theta, float k)
{
    return cos_theta / (k + (1.0 - k)*cos_theta);
}

float GeometrySmith(float cos_theta_view, float cos_theta_in, float roughness)
{
	float k = pow(1.0f + roughness, 2) / 8.0;
    return GeometrySchlickGGX(cos_theta_view, k) * GeometrySchlickGGX(cos_theta_in, k);
}

void main()
{
    float gamma = 2.1;
    vec3 frag_color3 = frag_base_color.xyz;
    // vec3 frag_color3 = pow(frag_base_color.xyz, vec3(gamma)); // reverse gamma correction
    
    vec3 to_light = normalize(light.position - frag_pos);
    vec3 to_camera = normalize(camera_pos - frag_pos);
    vec3 half_vec = normalize(to_light + to_camera);

    float cos_theta_in = soft_max(dot(to_light, norm), 0, 0.1);
    float cos_theta_view = soft_max(dot(to_camera, norm), 0, 0.1);
    float cos_theta_half = soft_max(dot(half_vec, norm), 0, 0.1);

    float distance_to_light = length(light.position - frag_pos);
    float attenuation = 1.0 / (1.0 +
    	                       light.Kl * distance_to_light + 
                               light.Kq * (distance_to_light * distance_to_light));
    vec3 radiance = light.color * attenuation;

    float NDF = DistributionGGX(cos_theta_half, roughness);
    float G   = GeometrySmith(cos_theta_view, cos_theta_in, roughness);
    vec3  F   = FresnelSchlick(cos_theta_half, frag_color3, metallic);

    vec3 k_sepc = F;
    vec3 k_diff = (1.0 - metallic) * (1.0 - k_sepc);

    vec3 specular = NDF * G * F / (0.001f + 4.0f * cos_theta_view * cos_theta_in);              
    frag_color3 = (k_diff * frag_color3 / pi + specular) * radiance * cos_theta_in + 
                  ambient * frag_color3 * ao;

    frag_color3 = frag_color3 / (frag_color3 + 1.0f); // HDR to LDR map
    frag_color3 = pow(frag_color3, vec3(1.0/gamma)); // gamma correction
    frag_color = vec4(frag_color3, 1.0f);
}