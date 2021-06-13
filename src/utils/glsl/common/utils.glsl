#ifndef __UTILS_GLSL__
#define __UTILS_GLSL__

#ifndef PI
#define PI 3.14159265358979323846
#endif

struct PointLight
{
	vec3 color;
    float brightness;
    float radius;
    float Kl;
    float Kq;
    vec3 position;

    // for shadow
    bool using_shadow;
    mat4 mat[6];
    samplerCube depth_map;
};

struct DirLight
{
    vec3 color;
    float brightness;
    vec3 direction;

    // for shadow
    bool using_shadow;
    mat4 mat[4];
    sampler2D _[4];
};

struct SpotLight
{
    vec3 color;
    float brightness;
    float radius;
    float Kl;
    float Kq;
    float cutoff_angle;
    float soft_distance;
    vec3 position;
    vec3 direction;
    bool using_shadow;
    mat4 mat[6];
    samplerCube depth_map;
};

struct Material
{
    vec3 diffuse_color;
    vec3 mask_color;
    float specular_strength;
    float shininess;
    float height_factor;

    sampler2D diffuse_map;
    sampler2D specular_map;
    sampler2D reflection_map;
    sampler2D bump_map;
    sampler2D normal_map;

    bool using_diffuse_map;
    bool using_specular_map;
    bool using_reflection_map;
    bool bump_inverse;
    bool using_bump_map;
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
    float cut[5];
};

struct Model
{
    mat4 mat;
    mat3 normal_mat;
    bool is_light;
    Material material;
};

struct Fragment
{
	vec3 position;
	vec3 pos_light;
	vec3 normal;
	vec2 tex_coord;

    vec3 diffuse_color;
    float specular_strength;
    float shininess;
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

float soft_sign(float x, float t0)
{
	if(t0 == 0)
	{
		return x > 0 ? 1 : (x < 0 ? -1 : 0);
	}
	
	return x/soft_abs(x, t0);
}

float soft_step(float x, float t0)
{
    return 0.5 * soft_sign(x, t0) + 0.5;
}

float step(float x)
{
    return x > 0.0 ? 1.0 : 0.0;
}

float soft_cos_angle(vec3 v1, vec3 v2, float t0)
{
	return soft_max(dot(normalize(v1), normalize(v2)), 0, t0);
}

float cos_angle(vec3 v1, vec3 v2)
{
	return max(dot(normalize(v1), normalize(v2)), 0);
}

vec4 trunc01(vec4 v)
{
    return vec4(min(max(v.x, 0), 1), min(max(v.y, 0), 1), min(max(v.z, 0), 1), min(max(v.w, 0), 1));
}

vec4 texture01(sampler2D s, vec2 tex_coord)
{
    return trunc01(texture(s, tex_coord));
}

vec4 trunc0(vec4 v)
{
    return vec4(max(v.x, 0), max(v.y, 0), max(v.z, 0), max(v.w, 0));
}

vec4 texture0(sampler2D s, vec2 tex_coord)
{
    return trunc0(texture(s, tex_coord));
}

vec3 exposure_map(vec3 color_in, float exposure)
{
    return vec3(1.0) - exp(-color_in * exposure);
}

vec3 diffuse_color(Material material, vec2 tex_coord)
{
    vec3 diffuse = material.diffuse_color;
    if(material.using_diffuse_map)
    {
        diffuse = texture(material.diffuse_map, tex_coord).rgb;
    }
    return diffuse;
}

vec3 ambient_color(Material material, vec2 tex_coord)
{
    return 0.1 * diffuse_color(material, tex_coord);
}

float specular_strength(Material material, vec2 tex_coord)
{
    float specular = material.specular_strength;
    if(material.using_specular_map)
    {
        specular = texture(material.specular_map, tex_coord).r;
    }
    return specular;
}

#endif