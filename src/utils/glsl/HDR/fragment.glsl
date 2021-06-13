#version 430 core

out vec4 frag_color;
in vec2 tex_coord;

uniform sampler2D luminance_texture;
uniform sampler2D _;
uniform sampler2D frame_image;

void main()
{
	float luminance = texture(luminance_texture, vec2(0.5, 0.5)).r;

	vec3 frag_color3 = texture(frame_image, tex_coord).rgb;
	frag_color3 = vec3(1.0) - exp(-frag_color3 / (0.5+pow(luminance, 1.0/3)));
	// frag_color3 = pow(frag_color3, vec3(1.0/2.2));

	// float alpha = 1;
	// frag_color3 = alpha*frag_color3/(1+luminance);
	// frag_color3 = frag_color3/(1+frag_color3);

	frag_color = vec4(frag_color3, 1.0);
}