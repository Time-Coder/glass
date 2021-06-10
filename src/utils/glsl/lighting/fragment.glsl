#version 460 core

#include "../common/lighting/phong.glsl"

out vec4 frag_color;
in vec2 tex_coord;

uniform int type;

uniform sampler2D base_image;
uniform sampler2D gbuffer_diffuse_specular;
uniform sampler2D gbuffer_position_shininess;
uniform sampler2D gbuffer_normal;

uniform PointLight point_light;
uniform DirLight dir_light;
uniform SpotLight spot_light;

uniform Camera camera;

Fragment createFragment()
{
    Fragment frag;

    vec4 position_shininess = texture(gbuffer_position_shininess, tex_coord);
    frag.position = position_shininess.xyz;
    // frag.pos_light = frag_in.pos_light.xyz/frag_in.pos_light.w;

    // frag.tex_coord = frag_in.tex_coord.st/frag_in.tex_coord.q;
    // if(model.material.using_depth_map)
    // {
    //     frag.tex_coord = parallax_mapping(frag.tex_coord, model.material, frag_in.to_camera_tangent);
    // }

    frag.normal = normalize(texture(gbuffer_normal, tex_coord).xyz);
    // if(model.material.using_normal_map)
    // {
    //     frag.normal = normalize(frag_in.normalTBN * (2*texture(model.material.normal_map, frag.tex_coord).xyz-1));
    // }

    vec4 diffuse_specular = texture0(gbuffer_diffuse_specular, tex_coord);
    frag.diffuse_color = diffuse_specular.rgb;
    frag.specular_strength = diffuse_specular.a;
    frag.shininess = position_shininess.w;

    return frag;
}

void main()
{
	Fragment frag = createFragment();

    vec3 frag_color3 = vec3(0);
    switch(type)
    {
    case 0: // shadering ambient
        frag_color3 = 0.1 * frag.diffuse_color;
        break;

    case 1: // shadering direction light
        frag_color3 = texture0(base_image, tex_coord).rgb + PhongDirLight(frag, dir_light, camera);
        break;

    case 2: // shadering point light
        frag_color3 = texture0(base_image, tex_coord).rgb + PhongPointLight(frag, point_light, camera);
        break;

    case 3: // shadering spot light
        frag_color3 = texture0(base_image, tex_coord).rgb + PhongSpotLight(frag, spot_light, camera);
        break;
    }

    frag_color = vec4(frag_color3, 1.0);
}