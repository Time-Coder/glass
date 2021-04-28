#ifndef __PHONG_GLSL__
#define __PHONG_GLSL__

#include "../fragment_utils.glsl"

vec3 diffuse_color(Material material, vec2 tex_coord)
{
	vec3 diffuse = material.diffuse_color;
    if(material.using_diffuse_map)
    {
        diffuse = texture(material.diffuse_map, tex_coord).rgb;
    }
    return diffuse;
}

vec3 ambient_color(Material material, vec2 tex_coord)
{
	vec3 ambient = material.ambient_color;
    if(material.using_ambient_map)
    {
        ambient = texture(material.ambient_map, tex_coord).xyz;
    }
    else if(material.using_diffuse_map)
    {
        ambient = 0.15 * diffuse_color(material, tex_coord);
    }
    return ambient;
}

vec3 specular_color(Material material, vec2 tex_coord)
{
	vec3 specular = material.specular_color;
    if(material.using_specular_map)
    {
        specular = texture(material.specular_map, tex_coord).rgb;
    }
    return specular;
}

vec3 PhongLighting(vec3 to_light, vec3 normal, vec3 to_camera, Material material, vec2 tex_coord)
{
	vec3 light_dir = -to_light;
	float cos_i = dot(to_light, normal);

	float diffuse_factor = 0;
    float specular_factor = 0;
    if(cos_i > 0 && dot(to_camera, normal) > 0)
    {
        diffuse_factor = cos_i;
        vec3 reflect_dir = reflect(light_dir, normal);
        specular_factor = pow(max(dot(to_camera, reflect_dir), 0), material.shininess*128);
    }

    vec3 diffuse_component = diffuse_factor * diffuse_color(material, tex_coord);
    vec3 specular_component = specular_factor * specular_color(material, tex_coord);
    
    return diffuse_component + specular_component;
}

vec3 PhongDirLight(DirLight light, Fragment frag, Material material, Camera camera)
{
    vec3 to_light = -normalize(light.direction);
    vec3 to_camera = normalize(camera.position - frag.position);

    vec3 phong_component = PhongLighting(to_light, frag.normal, to_camera, material, frag.tex_coord);    
    
    float shadow = 1;
    if(light.using_shadow)
    {
        shadow = PCFshadow(light, frag, 1);
    }

    return shadow * phong_component;
}

vec3 PhongPointLight(PointLight light, Fragment frag, Material material, Camera camera)
{
	vec3 to_light = normalize(light.position - frag.position);
    vec3 to_camera = normalize(camera.position - frag.position);

    vec3 phong_component = PhongLighting(to_light, frag.normal, to_camera, material, frag.tex_coord);

    float distance_to_light = length(to_light);
    float attenuation = 1.0 / (1.0 + distance_to_light*(light.Kl + light.Kq * distance_to_light));

    return attenuation * phong_component;
}

#endif