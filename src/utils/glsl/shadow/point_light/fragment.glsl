#version 330 core

in vec4 frag_pos;

#include "../../Common/utils.glsl"

uniform int type;
uniform PointLight point_light;
uniform SpotLight spot_light;

void main()
{
	if(type == 0) // point light
	{
    	gl_FragDepth = length(frag_pos.xyz - point_light.position) / point_light.radius;
	}
	else // spot light
	{
		gl_FragDepth = length(frag_pos.xyz - spot_light.position) / spot_light.radius;
	}
}