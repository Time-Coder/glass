#ifndef __FRAGMENT_UTILS__
#define __FRAGMENT_UTILS__

#include "utils.glsl"

vec2 parallax_mapping(vec2 tex_coord, Material material, vec3 to_camera_tangent)
{
    if(to_camera_tangent.z < 0)
    {
        discard;
    }
    to_camera_tangent = normalize(to_camera_tangent);

    float lower, middle, upper;
    float upper_max = material.height_factor/to_camera_tangent.z;
    float delta = upper_max/20.0;

    for(lower = 0, upper = delta; upper <= upper_max; lower += delta, upper += delta)
    {
        if(upper*to_camera_tangent.z >= material.height_factor*texture(material.depth_map, tex_coord - upper*to_camera_tangent.xy).x)
        {
            break;
        }
    }

    int times = 0;
    while(times <= 20 && abs(upper - lower) > 1E-6)
    {
        middle = (1-0.618)*upper+0.618*lower;
        if(middle*to_camera_tangent.z >= material.height_factor*texture(material.depth_map, tex_coord - middle*to_camera_tangent.xy).x)
        {
            upper = middle;
        }
        else
        {
            lower = middle;
        }
        times++;
    }
    vec2 result = tex_coord - ((1-0.618)*upper+0.618*lower) * to_camera_tangent.xy;
    
    vec2 origin = floor(tex_coord);
    if(result.x > origin.x + 1.0 || result.y > origin.y + 1.0 || result.x < origin.x || result.y < origin.y)
    {
        discard;
    }
    return result;
}

#endif