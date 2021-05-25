#version 460 core

out float luminance;
in vec2 tex_coord;

uniform int type;
uniform sampler2D frame_image;

void main()
{
	switch(type)
	{
	case 0:
		// luminance = log(0.001 +dot(vec3(0.299, 0.587, 0.114), texture(frame_image, tex_coord).rgb));
		luminance = dot(vec3(0.299, 0.587, 0.114), texture(frame_image, tex_coord).rgb);
		break;
	case 1:
		luminance = texture(frame_image, tex_coord).r;
		break;
	case 2:
		// luminance = exp(texture(frame_image, tex_coord).r);
		luminance = texture(frame_image, tex_coord).r;
		break;
	}
}