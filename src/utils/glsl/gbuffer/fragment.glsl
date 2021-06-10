#version 430 core

#include "../common/fragment_utils.glsl"

layout(location = 0) out vec4 gbuffer_diffuse_specular;
layout(location = 1) out vec4 gbuffer_position_shininess;
layout(location = 2) out vec3 gbuffer_normal;
layout(location = 3) out vec3 gbuffer_lights;

in VertexOut
{
    vec3 position;
    vec2 tex_coord;
    mat3 TBN;
    mat3 normalTBN;
    vec3 to_camera_tangent;
}frag_in;

uniform Camera camera;
uniform PointLight point_light;
uniform DirLight dir_light;
uniform Model model;
uniform samplerCube skybox_map;

Fragment createFragment()
{
    Fragment frag;

    frag.position = frag_in.position;

    frag.normal = normalize(frag_in.TBN[2]);

    frag.tex_coord = frag_in.tex_coord;
    if(model.material.using_bump_map)
    {
        frag = parallax_mapping(frag, model.material, frag_in.to_camera_tangent);
    }

    if(model.material.using_normal_map)
    {
        frag.normal = normalize(frag_in.normalTBN * (2*texture(model.material.normal_map, frag.tex_coord).xyz-1));
    }
    else
    {
        frag.normal = normalize(frag_in.TBN[2]);
    }

    return frag;
}

void main()
{
    Fragment frag = createFragment();


    gbuffer_normal = frag.normal;
    if(model.is_light)
    {
        gbuffer_diffuse_specular = vec4(0);
        gbuffer_position_shininess = vec4(0);

        gbuffer_lights = model.material.mask_color * diffuse_color(model.material, frag.tex_coord);
    }
    else
    {
        gbuffer_diffuse_specular = vec4(diffuse_color(model.material, frag.tex_coord), specular_strength(model.material, frag.tex_coord));
        gbuffer_position_shininess.xyz = frag.position;
        gbuffer_position_shininess.w = model.material.shininess;

        gbuffer_lights = vec3(0);
    }
}