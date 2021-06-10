#ifndef __FRAGMENT_UTILS__
#define __FRAGMENT_UTILS__

#include "utils.glsl"

float getBump(Material material, vec2 tex_coord)
{
    if(!material.bump_inverse)
    {
        return material.height_factor*texture01(material.bump_map, tex_coord).x - material.height_factor;
    }
    else
    {
        return material.height_factor*(1-texture01(material.bump_map, tex_coord).x) - material.height_factor;
    }
}

Fragment parallax_mapping(Fragment frag, Material material, vec3 to_camera_tangent)
{
    if(to_camera_tangent.z < 0)
    {
        discard;
    }
    
    int max_times = 20;
    float epsilon = 1E-6;
    float bias = 0.618;
    
    to_camera_tangent = normalize(to_camera_tangent);
    float sin_theta = to_camera_tangent.z;

    float lower, middle, upper;
    float L = material.height_factor / sin_theta;
    float delta = L/max_times;

    for(lower = -delta; lower >= -L; lower -= delta)
    {
        if(lower*sin_theta <= getBump(material, frag.tex_coord + lower*to_camera_tangent.xy))
        {
            break;
        }
    }
    upper = lower + delta;

    int times = 0;
    while(times <= max_times && abs(upper - lower) > epsilon)
    {
        middle = bias*upper+(1-bias)*lower;
        if(middle*sin_theta >= getBump(material, frag.tex_coord + middle*to_camera_tangent.xy))
        {
            upper = middle;
        }
        else
        {
            lower = middle;
        }

        times++;
    }

    frag.position = frag.position + getBump(material, frag.tex_coord)*frag.normal;
    vec2 origin = floor(frag.tex_coord);
    
    // float hl = getBump(material, frag.tex_coord+lower*to_camera_tangent.xy);
    // float hu = getBump(material, frag.tex_coord+upper*to_camera_tangent.xy);
    // middle = (hu*lower - hl*upper)/(hu-hl-(upper-lower)*sin_theta);
    // frag.tex_coord = frag.tex_coord + middle * to_camera_tangent.xy;

    frag.tex_coord = frag.tex_coord + (bias*upper+(1-bias)*lower) * to_camera_tangent.xy;

    if(frag.tex_coord.x > origin.x + 1.0 ||
       frag.tex_coord.y > origin.y + 1.0 ||
       frag.tex_coord.x < origin.x ||
       frag.tex_coord.y < origin.y)
    {
        discard;
    }

    return frag;
}

#endif