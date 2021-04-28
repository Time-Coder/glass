#version 330 core

#include "../common/lighting/phong.glsl"

in VertexOut
{
    vec3 position;
    vec4 pos_light;
    vec4 tex_coord;
    mat3 TBN;
    mat3 normalTBN;
    vec3 to_camera_tangent;
}frag_in;

out vec4 frag_color;

uniform Camera camera;
uniform PointLight point_light;
uniform DirLight dir_light;
uniform Model model;
uniform samplerCube skybox_map;
uniform int type;

Fragment createFragment()
{
    Fragment frag;

    frag.position = frag_in.position;
    frag.pos_light = frag_in.pos_light.xyz/frag_in.pos_light.w;

    frag.tex_coord = frag_in.tex_coord.st/frag_in.tex_coord.q;
    // if(model.material.using_depth_map)
    // {
    //     frag.tex_coord = parallax_mapping(frag.tex_coord, model.material, frag_in.to_camera_tangent);
    // }

    frag.normal = normalize(frag_in.TBN[2]);
    // if(model.material.using_normal_map)
    // {
    //     frag.normal = normalize(frag_in.normalTBN * (2*texture(model.material.normal_map, frag.tex_coord).xyz-1));
    // }

    return frag;
}

void main()
{
    Fragment frag = createFragment();

    vec3 frag_color3;
    if(type == 0)
    {
        frag_color3 = ambient_color(model.material, frag.tex_coord) + PhongDirLight(dir_light, frag, model.material, camera);
    }
    else
    {
        frag_color3 = diffuse_color(model.material, frag.tex_coord);
    }
    frag_color = vec4(frag_color3, 1.0);
}