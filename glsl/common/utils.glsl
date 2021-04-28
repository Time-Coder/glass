#ifndef __UTILS_GLSL__
#define __UTILS_GLSL__

#ifndef pi
#define pi 3.14159265358979323846
#endif

struct PointLight
{
	vec3 color;
	float Kl, Kq;
	vec3 position;
};

struct DirLight
{
    vec3 color;
    vec3 direction;
    mat4 mat;
    bool using_shadow;
    sampler2D depth_map;
};

struct Material
{
    vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
    float shininess;
    float height_factor;

    sampler2D ambient_map;
    sampler2D diffuse_map;
    sampler2D specular_map;
    sampler2D reflection_map;
    sampler2D depth_map;
    sampler2D normal_map;

    bool using_ambient_map;
    bool using_diffuse_map;
    bool using_specular_map;
    bool using_reflection_map;
    bool using_depth_map;
    bool using_normal_map;
};

struct Camera
{
    vec3 position;
    mat4 mat;
    mat4 screen_mat;
    mat4 projection_mat;
    mat4 view_mat;
    mat4 rotation_mat;
};

struct Model
{
	mat4 mat;
	mat3 normal_mat;
	Material material;
};

struct Fragment
{
	vec3 position;
	vec3 pos_light;
	vec3 normal;
	vec2 tex_coord;
};

float soft_abs(float x, float t0)
{
	if(t0 == 0)
	{
		return abs(x);
	}
	
	float abs_x = abs(x);
	return abs_x > t0 ? abs_x : 0.5*(t0 + x*x/t0);
}

float soft_max(float a, float b, float t0)
{
	return 0.5*(a + b + soft_abs(a - b, t0));
}

float soft_min(float a, float b, float t0)
{
	return 0.5*(a + b - soft_abs(a - b, t0));
}

float soft_sign(float a, float t0)
{
	if(t0 == 0)
	{
		return a > 0 ? 1 : (a < 0 ? -1 : 0);
	}
	
	return a/soft_abs(a, t0);
}

float soft_cos_angle(vec3 v1, vec3 v2, float t0)
{
	return soft_max(dot(normalize(v1), normalize(v2)), 0, t0);
}

float cos_angle(vec3 v1, vec3 v2)
{
	return max(dot(normalize(v1), normalize(v2)), 0);
}

float PCFshadow(DirLight light, Fragment frag, int half_width)
{
    frag.pos_light = 0.5 * frag.pos_light + 0.5;
    if(frag.pos_light.z > 1)
    {
        return 1;
    }
    
    // float bias = 0.005;
    float bias = max(0.005 * (1.0 + dot(normalize(frag.normal), normalize(light.direction))), 0.002);

    float sigma = half_width / 3.0 + 1E-3;
    float k = -0.5/sigma/sigma;

    vec2 texel_size = 1.0 / textureSize(light.depth_map, 0);
    float weight_sum = 0.0;
    float shadow = 0.0;
    for(int x = -half_width; x <= half_width; x++)
    {
        for(int y = -half_width; y <= half_width; y++)
        {
            float depth = texture(light.depth_map, frag.pos_light.xy + vec2(x, y) * texel_size).r;
            // float weight = exp(k*(x*x + y*y));
            float weight = 1;
            weight_sum += weight;
            shadow += weight * ((frag.pos_light.z - bias) > depth ? 0.0 : 1.0);        
        }
    }

    return shadow / weight_sum;
}

#endif