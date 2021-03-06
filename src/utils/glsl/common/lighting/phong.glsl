#ifndef __PHONG_GLSL__
#define __PHONG_GLSL__

#include "../fragment_utils.glsl"

// GLSL's limit: use array[level] will not work
// So write this macro
#define ARRAY_ACCESS(array, index) \
((index) == 0 ? (array[0]) : \
((index) == 1 ? (array[1]) : \
((index) == 2 ? (array[2]) : (array[3]))))

#define SAMPLES_COUNT 20

float getCSMLevel(Fragment frag, Camera camera)
{
    vec4 pos_viewspace = camera.view_mat * vec4(frag.position, 1.0);
    float D = -pos_viewspace.z/pos_viewspace.w;
    float t0 = 0.3*(camera.cut[1]-camera.cut[0]);
    if(D <= camera.cut[2]-t0)
    {
        return soft_step(D-camera.cut[1], t0);
    }
    else if(D <= camera.cut[3]-t0)
    {
        return 1 + soft_step(D-camera.cut[2], t0);
    }
    else
    {
        return 2 + soft_step(D-camera.cut[3], t0);
    }
}

float getClosestDepth(int level, vec2 tex_coord)
{
    float closest_depth;
    switch(level)
    {
    case 0:
        closest_depth = texture(dir_light_depth_map_zero, tex_coord).r;
        break;
    case 1:
        closest_depth = texture(dir_light_depth_map_one, tex_coord).r;
        break;
    case 2:
        closest_depth = texture(dir_light_depth_map_two, tex_coord).r;
        break;
    default:
        closest_depth = texture(dir_light_depth_map_three, tex_coord).r;
        break;
    }
    return closest_depth;
}

vec2 getTexelSize(int level)
{
    vec2 texel_size;
    switch(level)
    {
    case 0:
        texel_size = 1.0 / textureSize(dir_light_depth_map_zero, 0);
        break;
    case 1:
        texel_size = 1.0 / textureSize(dir_light_depth_map_one, 0);
        break;
    case 2:
        texel_size = 1.0 / textureSize(dir_light_depth_map_two, 0);
        break;
    case 3:
        texel_size = 1.0 / textureSize(dir_light_depth_map_three, 0);
        break;
    }
    return texel_size;
}

float _dirLightShadow(DirLight light, Fragment frag, int level, int half_width)
{
    vec4 pos_lightspace4 = ARRAY_ACCESS(light.mat, level) * vec4(frag.position, 1.0);
    vec3 pos_lightspace = 0.5*(pos_lightspace4.xyz / pos_lightspace4.w) + 0.5;

    float current_depth = pos_lightspace.z;
    float bias = max(0.005 * (1.0 + dot(normalize(frag.normal), normalize(light.direction))), 0.002);

    vec2 texel_size = getTexelSize(level);
    float weight_sum = 0.0;
    float shadow = 0.0;
    for(int x = -half_width; x <= half_width; x++)
    {
        for(int y = -half_width; y <= half_width; y++)
        {
            float closest_depth = getClosestDepth(level, pos_lightspace.xy + vec2(x, y) * texel_size);
            weight_sum += 1;
            shadow += ((current_depth - bias > closest_depth && current_depth < 1) ? 0.0 : 1.0);        
        }
    }

    return shadow / weight_sum;
}

float dirLightShadow(DirLight light, Fragment frag, Camera camera, int half_width)
{
    float level = getCSMLevel(frag, camera);

    float shadow = _dirLightShadow(light, frag, int(level), half_width);
    if(level > float(int(level)) && int(level)+1 <= 3)
    {
        float rear = level - float(int(level));
        shadow = (1-rear)*shadow + rear*_dirLightShadow(light, frag, int(level)+1, half_width);
    }

    return shadow;
}

#define POINT_LIGHT_SHADOW_CODE(light, frag) \
{\
    float z_far = light.radius;\
    vec3 light_dir = frag.position - light.position;\
    float current_depth = length(light_dir);\
    float bias = 0.15;\
    float radius = (1.0 + (current_depth / z_far)) / 25.0;\
    vec3 directions[SAMPLES_COUNT] = vec3[]\
    (\
       vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1),\
       vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),\
       vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),\
       vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),\
       vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)\
    );\
    float shadow = 0;\
    for(int i = 0; i < SAMPLES_COUNT; i++)\
    {\
        float closest_depth = z_far * texture(light.depth_map, light_dir + directions[i] * radius).r;\
        shadow += ((current_depth -  bias > closest_depth && current_depth < z_far) ? 0.0 : 1.0);\
    }\
    return shadow / float(SAMPLES_COUNT);\
}

float pointLightShadow(PointLight light, Fragment frag)
POINT_LIGHT_SHADOW_CODE(light, frag)

float spotLightShadow(SpotLight light, Fragment frag)
POINT_LIGHT_SHADOW_CODE(light, frag)

vec3 PhongLighting(Fragment frag, vec3 to_light, vec3 to_camera)
{
	vec3 light_dir = -to_light;
	float cos_i = dot(to_light, frag.normal);

	float diffuse_factor = 0;
    float specular_factor = 0;
    if(cos_i > 0 && dot(to_camera, frag.normal) > 0)
    {
        diffuse_factor = cos_i;
        vec3 reflect_dir = reflect(light_dir, frag.normal);
        specular_factor = pow(max(dot(to_camera, reflect_dir), 0), frag.shininess*128);
    }

    vec3 diffuse_component = diffuse_factor * frag.diffuse_color;
    float specular_component = specular_factor * frag.specular_strength;
    
    return diffuse_component + specular_component;
}

vec3 PhongDirLight(Fragment frag, DirLight light, Camera camera)
{
    vec3 to_light = -normalize(light.direction);
    vec3 to_camera = normalize(camera.position - frag.position);

    vec3 phong_component = light.brightness * light.color * PhongLighting(frag, to_light, to_camera);    

    float shadow = 1;
    if(light.using_shadow)
    {
        shadow = dirLightShadow(light, frag, camera, 1);
    }

    return shadow * phong_component;
}

#define PHONG_POINT_LIGHT_CODE(frag, light, camera, shadow_func) \
{\
    vec3 to_light = light.position - frag.position;\
    vec3 to_camera = normalize(camera.position - frag.position);\
    vec3 phong_component = light.brightness * light.color * PhongLighting(frag, normalize(to_light), to_camera);\
    float distance_to_light = length(to_light);\
    float attenuation = 1.0 / (1.0 + distance_to_light*(light.Kl + light.Kq * distance_to_light));\
    float shadow = 1;\
    if(light.using_shadow)\
    {\
        shadow = shadow_func(light, frag);\
    }\
    return shadow * attenuation * phong_component;\
}

vec3 PhongPointLight(Fragment frag, PointLight light, Camera camera)
PHONG_POINT_LIGHT_CODE(frag, light, camera, pointLightShadow)

vec3 _PhongSpotLight(Fragment frag, SpotLight light, Camera camera)
PHONG_POINT_LIGHT_CODE(frag, light, camera, spotLightShadow)

vec3 PhongSpotLight(Fragment frag, SpotLight light, Camera camera)
{
    float cos_theta = dot(normalize(frag.position - light.position), normalize(light.direction));
    float theta = acos(cos_theta);
    if(theta > light.cutoff_angle + light.soft_distance)
    {
        return vec3(0);
    }

    float soft_factor = soft_step(light.cutoff_angle-theta, light.soft_distance);
    float aggregation = 2.0 / (1.001 - cos(light.cutoff_angle));
    return soft_factor * aggregation * _PhongSpotLight(frag, light, camera);
}

#endif