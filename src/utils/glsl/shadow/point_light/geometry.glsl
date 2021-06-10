#version 460 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

#include "../../common/utils.glsl"

uniform int type;
uniform SpotLight spot_light;
uniform PointLight point_light;

out vec4 frag_pos;

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // built-in variable that specifies to which face we render.
        for(int i = 0; i < 3; ++i) // for each triangle's vertices
        {
            frag_pos = gl_in[i].gl_Position;
            if(type == 0)
            {
                gl_Position = point_light.mat[face] * frag_pos;
            }
            else
            {
                gl_Position = spot_light.mat[face] * frag_pos;
            }
            EmitVertex();
        }    
        EndPrimitive();
    }
}