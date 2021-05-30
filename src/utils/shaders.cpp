#include "glass/utils/shaders.h"

using namespace std;

string glass::common_utils_shader = 
"#ifndef __UTILS_GLSL__\n"
"#define __UTILS_GLSL__\n"
"#ifndef PI\n"
"#define PI 3.14159265358979323846\n"
"#endif\n"
"struct PointLight\n"
"{\n"
"	vec3 color;\n"
"    float brightness;\n"
"    float radius;\n"
"    float Kl;\n"
"    float Kq;\n"
"    vec3 position;\n"
"    // for shadow\n"
"    bool using_shadow;\n"
"    mat4 mat[6];\n"
"    samplerCube depth_map;\n"
"};\n"
"struct DirLight\n"
"{\n"
"    vec3 color;\n"
"    float brightness;\n"
"    vec3 direction;\n"
"    // for shadow\n"
"    bool using_shadow;\n"
"    mat4 mat[4];\n"
"    sampler2D depth_map[4];\n"
"};\n"
"struct SpotLight\n"
"{\n"
"    vec3 color;\n"
"    float brightness;\n"
"    float radius;\n"
"    float Kl;\n"
"    float Kq;\n"
"    float cutoff_angle;\n"
"    float soft_distance;\n"
"    vec3 position;\n"
"    vec3 direction;\n"
"    bool using_shadow;\n"
"    mat4 mat[6];\n"
"    samplerCube depth_map;\n"
"};\n"
"struct Material\n"
"{\n"
"    vec3 diffuse_color;\n"
"    vec3 mask_color;\n"
"    float specular_strength;\n"
"    float shininess;\n"
"    float height_factor;\n"
"    sampler2D diffuse_map;\n"
"    sampler2D specular_map;\n"
"    sampler2D reflection_map;\n"
"    sampler2D bump_map;\n"
"    sampler2D normal_map;\n"
"    bool using_diffuse_map;\n"
"    bool using_specular_map;\n"
"    bool using_reflection_map;\n"
"    bool bump_inverse;\n"
"    bool using_bump_map;\n"
"    bool using_normal_map;\n"
"};\n"
"struct Camera\n"
"{\n"
"    vec3 position;\n"
"    mat4 mat;\n"
"    mat4 screen_mat;\n"
"    mat4 projection_mat;\n"
"    mat4 view_mat;\n"
"    mat4 rotation_mat;\n"
"    float cut[5];\n"
"};\n"
"struct Model\n"
"{\n"
"    mat4 mat;\n"
"    mat3 normal_mat;\n"
"    bool is_light;\n"
"    Material material;\n"
"};\n"
"struct Fragment\n"
"{\n"
"	vec3 position;\n"
"	vec3 pos_light;\n"
"	vec3 normal;\n"
"	vec2 tex_coord;\n"
"    vec3 diffuse_color;\n"
"    float specular_strength;\n"
"    float shininess;\n"
"};\n"
"float soft_abs(float x, float t0)\n"
"{\n"
"	if(t0 == 0)\n"
"	{\n"
"		return abs(x);\n"
"	}\n"
"	float abs_x = abs(x);\n"
"	return abs_x > t0 ? abs_x : 0.5*(t0 + x*x/t0);\n"
"}\n"
"float soft_max(float a, float b, float t0)\n"
"{\n"
"	return 0.5*(a + b + soft_abs(a - b, t0));\n"
"}\n"
"float soft_min(float a, float b, float t0)\n"
"{\n"
"	return 0.5*(a + b - soft_abs(a - b, t0));\n"
"}\n"
"float soft_sign(float x, float t0)\n"
"{\n"
"	if(t0 == 0)\n"
"	{\n"
"		return x > 0 ? 1 : (x < 0 ? -1 : 0);\n"
"	}\n"
"	return x/soft_abs(x, t0);\n"
"}\n"
"float soft_step(float x, float t0)\n"
"{\n"
"    return 0.5 * soft_sign(x, t0) + 0.5;\n"
"}\n"
"float step(float x)\n"
"{\n"
"    return x > 0.0 ? 1.0 : 0.0;\n"
"}\n"
"float soft_cos_angle(vec3 v1, vec3 v2, float t0)\n"
"{\n"
"	return soft_max(dot(normalize(v1), normalize(v2)), 0, t0);\n"
"}\n"
"float cos_angle(vec3 v1, vec3 v2)\n"
"{\n"
"	return max(dot(normalize(v1), normalize(v2)), 0);\n"
"}\n"
"vec4 trunc01(vec4 v)\n"
"{\n"
"    return vec4(min(max(v.x, 0), 1), min(max(v.y, 0), 1), min(max(v.z, 0), 1), min(max(v.w, 0), 1));\n"
"}\n"
"vec4 texture01(sampler2D s, vec2 tex_coord)\n"
"{\n"
"    return trunc01(texture(s, tex_coord));\n"
"}\n"
"vec4 trunc0(vec4 v)\n"
"{\n"
"    return vec4(max(v.x, 0), max(v.y, 0), max(v.z, 0), max(v.w, 0));\n"
"}\n"
"vec4 texture0(sampler2D s, vec2 tex_coord)\n"
"{\n"
"    return trunc0(texture(s, tex_coord));\n"
"}\n"
"vec3 exposure_map(vec3 color_in, float exposure)\n"
"{\n"
"    return vec3(1.0) - exp(-color_in * exposure);\n"
"}\n"
"vec3 diffuse_color(Material material, vec2 tex_coord)\n"
"{\n"
"    vec3 diffuse = material.diffuse_color;\n"
"    if(material.using_diffuse_map)\n"
"    {\n"
"        diffuse = texture(material.diffuse_map, tex_coord).rgb;\n"
"    }\n"
"    return diffuse;\n"
"}\n"
"vec3 ambient_color(Material material, vec2 tex_coord)\n"
"{\n"
"    return 0.1 * diffuse_color(material, tex_coord);\n"
"}\n"
"float specular_strength(Material material, vec2 tex_coord)\n"
"{\n"
"    float specular = material.specular_strength;\n"
"    if(material.using_specular_map)\n"
"    {\n"
"        specular = texture(material.specular_map, tex_coord).r;\n"
"    }\n"
"    return specular;\n"
"}\n"
"#endif\n"
;

string glass::common_fragment_utils_shader = 
"#ifndef __FRAGMENT_UTILS__\n"
"#define __FRAGMENT_UTILS__\n"
+ glass::common_utils_shader + 
"float getBump(Material material, vec2 tex_coord)\n"
"{\n"
"    if(!material.bump_inverse)\n"
"    {\n"
"        return material.height_factor*texture01(material.bump_map, tex_coord).x - material.height_factor;\n"
"    }\n"
"    else\n"
"    {\n"
"        return material.height_factor*(1-texture01(material.bump_map, tex_coord).x) - material.height_factor;\n"
"    }\n"
"}\n"
"Fragment parallax_mapping(Fragment frag, Material material, vec3 to_camera_tangent)\n"
"{\n"
"    if(to_camera_tangent.z < 0)\n"
"    {\n"
"        discard;\n"
"    }\n"
"    int max_times = 20;\n"
"    float epsilon = 1E-6;\n"
"    float bias = 0.618;\n"
"    to_camera_tangent = normalize(to_camera_tangent);\n"
"    float sin_theta = to_camera_tangent.z;\n"
"    float lower, middle, upper;\n"
"    float L = material.height_factor / sin_theta;\n"
"    float delta = L/max_times;\n"
"    for(lower = -delta; lower >= -L; lower -= delta)\n"
"    {\n"
"        if(lower*sin_theta <= getBump(material, frag.tex_coord + lower*to_camera_tangent.xy))\n"
"        {\n"
"            break;\n"
"        }\n"
"    }\n"
"    upper = lower + delta;\n"
"    int times = 0;\n"
"    while(times <= max_times && abs(upper - lower) > epsilon)\n"
"    {\n"
"        middle = bias*upper+(1-bias)*lower;\n"
"        if(middle*sin_theta >= getBump(material, frag.tex_coord + middle*to_camera_tangent.xy))\n"
"        {\n"
"            upper = middle;\n"
"        }\n"
"        else\n"
"        {\n"
"            lower = middle;\n"
"        }\n"
"        times++;\n"
"    }\n"
"    frag.position = frag.position + getBump(material, frag.tex_coord)*frag.normal;\n"
"    vec2 origin = floor(frag.tex_coord);\n"
"    // float hl = getBump(material, frag.tex_coord+lower*to_camera_tangent.xy);\n"
"    // float hu = getBump(material, frag.tex_coord+upper*to_camera_tangent.xy);\n"
"    // middle = (hu*lower - hl*upper)/(hu-hl-(upper-lower)*sin_theta);\n"
"    // frag.tex_coord = frag.tex_coord + middle * to_camera_tangent.xy;\n"
"    frag.tex_coord = frag.tex_coord + (bias*upper+(1-bias)*lower) * to_camera_tangent.xy;\n"
"    if(frag.tex_coord.x > origin.x + 1.0 ||\n"
"       frag.tex_coord.y > origin.y + 1.0 ||\n"
"       frag.tex_coord.x < origin.x ||\n"
"       frag.tex_coord.y < origin.y)\n"
"    {\n"
"        discard;\n"
"    }\n"
"    return frag;\n"
"}\n"
"#endif\n"
;

string glass::common_lighting_phong_shader = 
"#ifndef __PHONG_GLSL__\n"
"#define __PHONG_GLSL__\n"
+ glass::common_fragment_utils_shader +
"// GLSL's bug: use array[level] will not work\n"
"// So write this macro\n"
"#define ARRAY_ACCESS(array, index) \\\n"
"((index) == 0 ? array[0] : \\\n"
"((index) == 1 ? array[1] : \\\n"
"((index) == 2 ? array[2] : array[3])))\n"
"float getCSMLevel(Fragment frag, Camera camera)\n"
"{\n"
"    vec4 pos_viewspace = camera.view_mat * vec4(frag.position, 1.0);\n"
"    float D = -pos_viewspace.z/pos_viewspace.w;\n"
"    float t0 = 0.3*(camera.cut[1]-camera.cut[0]);\n"
"	 if(D <= camera.cut[2]-t0)\n"
"    {\n"
"        return soft_step(D-camera.cut[1], t0);\n"
"    }\n"
"    else if(D <= camera.cut[3]-t0)\n"
"    {\n"
"        return 1 + soft_step(D-camera.cut[2], t0);\n"
"    }\n"
"    else\n"
"    {\n"
"        return 2 + soft_step(D-camera.cut[3], t0);\n"
"    }\n"
"    return (D <= camera.cut[1] ? 0 : (D <= camera.cut[2] ? 1 : (D <= camera.cut[3] ? 2 : 3)));\n"
"}\n"
"float _dirLightShadow(DirLight light, Fragment frag, int level, int half_width)\n"
"{\n"
"    vec4 pos_lightspace4 = ARRAY_ACCESS(light.mat, level) * vec4(frag.position, 1.0);\n"
"    vec3 pos_lightspace = pos_lightspace4.xyz / pos_lightspace4.w;\n"
"    pos_lightspace = 0.5 * pos_lightspace + 0.5;\n"
"    if(pos_lightspace.z > 1)\n"
"    {\n"
"        return 1;\n"
"    }\n"
"    float bias = max(0.005 * (1.0 + dot(normalize(frag.normal), normalize(light.direction))), 0.002);\n"
"    // float closest_depth = texture(ARRAY_ACCESS(light.depth_map, level), pos_lightspace.xy).r;\n"
"    // float shadow = (bias + closest_depth - pos_lightspace.z > 0 ? 1.0 : 0.0);\n"
"    // float shadow = soft_step(bias + closest_depth - pos_lightspace.z + 0.1, 0.1);\n"
"    // float shadow = step(bias + closest_depth - pos_lightspace.z);\n"
"    // return shadow;\n"
"    vec2 texel_size = 1.0 / textureSize(ARRAY_ACCESS(light.depth_map, level), 0);\n"
"    float weight_sum = 0.0;\n"
"    float shadow = 0.0;\n"
"    for(int x = -half_width; x <= half_width; x++)\n"
"    {\n"
"        for(int y = -half_width; y <= half_width; y++)\n"
"        {\n"
"            float closest_depth = texture(ARRAY_ACCESS(light.depth_map, level), pos_lightspace.xy + vec2(x, y) * texel_size).r;\n"
"            weight_sum += 1;\n"
"            shadow += ((pos_lightspace.z - bias) > closest_depth ? 0.0 : 1.0);        \n"
"        }\n"
"    }\n"
"    return shadow / weight_sum;\n"
"}\n"
"float dirLightShadow(DirLight light, Fragment frag, Camera camera, int half_width)\n"
"{\n"
"    float level = getCSMLevel(frag, camera);\n"
"    float shadow = _dirLightShadow(light, frag, int(level), half_width);\n"
"    if(level > float(int(level)) && int(level)+1 <= 3)\n"
"    {\n"
"        float rear = level - float(int(level));\n"
"        shadow = (1-rear)*shadow + rear*_dirLightShadow(light, frag, int(level)+1, half_width);\n"
"    }\n"
"    return shadow;\n"
"}\n"
"#define SAMPLES_COUNT 20\n"
"#define POINT_LIGHT_SHADOW_CODE(light, frag) \\\n"
"{\\\n"
"    float z_far = light.radius;\\\n"
"    vec3 light_dir = frag.position - light.position;\\\n"
"    \\\n"
"    float current_depth = length(light_dir);\\\n"
"    \\\n"
"    float bias = 0.15;\\\n"
"    float radius = (1.0 + (current_depth / z_far)) / 25.0;\\\n"
"    vec3 directions[SAMPLES_COUNT] = vec3[]\\\n"
"    (\\\n"
"       vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1),\\\n"
"       vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),\\\n"
"       vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),\\\n"
"       vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),\\\n"
"       vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)\\\n"
"    );\\\n"
"    \\\n"
"    float shadow = 0;\\\n"
"    for(int i = 0; i < SAMPLES_COUNT; i++)\\\n"
"    {\\\n"
"        float closest_depth = z_far * texture(light.depth_map, light_dir + directions[i] * radius).r;\\\n"
"        shadow += ((current_depth -  bias > closest_depth && current_depth < z_far) ? 0.0 : 1.0);\\\n"
"    }\\\n"
"    return shadow / float(SAMPLES_COUNT);\\\n"
"}\n"
"float pointLightShadow(PointLight light, Fragment frag)\n"
"POINT_LIGHT_SHADOW_CODE(light, frag)\n"
"float spotLightShadow(SpotLight light, Fragment frag)\n"
"POINT_LIGHT_SHADOW_CODE(light, frag)\n"
"vec3 PhongLighting(Fragment frag, vec3 to_light, vec3 to_camera)\n"
"{\n"
"	vec3 light_dir = -to_light;\n"
"	float cos_i = dot(to_light, frag.normal);\n"
"	float diffuse_factor = 0;\n"
"    float specular_factor = 0;\n"
"    if(cos_i > 0 && dot(to_camera, frag.normal) > 0)\n"
"    {\n"
"        diffuse_factor = cos_i;\n"
"        vec3 reflect_dir = reflect(light_dir, frag.normal);\n"
"        specular_factor = pow(max(dot(to_camera, reflect_dir), 0), frag.shininess*128);\n"
"    }\n"
"    vec3 diffuse_component = diffuse_factor * frag.diffuse_color;\n"
"    float specular_component = specular_factor * frag.specular_strength;\n"
"    return diffuse_component + specular_component;\n"
"}\n"
"vec3 PhongDirLight(Fragment frag, DirLight light, Camera camera)\n"
"{\n"
"    vec3 to_light = -normalize(light.direction);\n"
"    vec3 to_camera = normalize(camera.position - frag.position);\n"
"    vec3 phong_component = light.brightness * light.color * PhongLighting(frag, to_light, to_camera);    \n"
"    float shadow = 1;\n"
"    if(light.using_shadow)\n"
"    {\n"
"        shadow = dirLightShadow(light, frag, camera, 1);\n"
"    }\n"
"    return shadow * phong_component;\n"
"}\n"
"#define PHONG_POINT_LIGHT_CODE(frag, light, camera, shadow_func) \\\n"
"{\\\n"
"    vec3 to_light = light.position - frag.position;\\\n"
"    vec3 to_camera = normalize(camera.position - frag.position);\\\n"
"    vec3 phong_component = light.brightness * light.color * PhongLighting(frag, normalize(to_light), to_camera);\\\n"
"    float distance_to_light = length(to_light);\\\n"
"    float attenuation = 1.0 / (1.0 + distance_to_light*(light.Kl + light.Kq * distance_to_light));\\\n"
"    float shadow = 1;\\\n"
"    if(light.using_shadow)\\\n"
"    {\\\n"
"        shadow = shadow_func(light, frag);\\\n"
"    }\\\n"
"    return shadow * attenuation * phong_component;\\\n"
"}\n"
"vec3 PhongPointLight(Fragment frag, PointLight light, Camera camera)\n"
"PHONG_POINT_LIGHT_CODE(frag, light, camera, pointLightShadow)\n"
"vec3 _PhongSpotLight(Fragment frag, SpotLight light, Camera camera)\n"
"PHONG_POINT_LIGHT_CODE(frag, light, camera, spotLightShadow)\n"
"vec3 PhongSpotLight(Fragment frag, SpotLight light, Camera camera)\n"
"{\n"
"    float cos_theta = dot(normalize(frag.position - light.position), normalize(light.direction));\n"
"    float theta = acos(cos_theta);\n"
"    if(theta > light.cutoff_angle + light.soft_distance)\n"
"    {\n"
"        return vec3(0);\n"
"    }\n"
"    float factor = soft_step(light.cutoff_angle-theta, light.soft_distance);\n"
"    float aggregation = 2.0 / (1.001 - cos(light.cutoff_angle));\n"
"    return factor * aggregation * _PhongSpotLight(frag, light, camera);\n"
"}\n"
"#endif\n"
;

string glass::blur_fragment_shader = 
"#version 460 core\n"
+ glass::common_utils_shader + 
"out vec4 frag_color;\n"
"in vec2 tex_coord;\n"
"uniform sampler2D color_map;\n"
"uniform bool horizontal;\n"
"uniform float weights[5];\n"
"void main()\n"
"{\n"
"	vec2 tex_offset = 1.0 / textureSize(color_map, 0); // gets size of single texel\n"
"    vec3 result = texture(color_map, tex_coord).rgb * weights[0]; // current fragment's contribution\n"
"    if(horizontal)\n"
"    {\n"
"        for(int i = 1; i < 5; ++i)\n"
"        {\n"
"            result += texture(color_map, tex_coord + vec2(tex_offset.x * i, 0.0)).rgb * weights[i];\n"
"            result += texture(color_map, tex_coord - vec2(tex_offset.x * i, 0.0)).rgb * weights[i];\n"
"        }\n"
"    }\n"
"    else\n"
"    {\n"
"        for(int i = 1; i < 5; ++i)\n"
"        {\n"
"            result += texture(color_map, tex_coord + vec2(0.0, tex_offset.y * i)).rgb * weights[i];\n"
"            result += texture(color_map, tex_coord - vec2(0.0, tex_offset.y * i)).rgb * weights[i];\n"
"        }\n"
"    }\n"
"    frag_color = vec4(result, 1.0);\n"
"}\n"
;

string glass::blur_vertex_shader = 
"#version 460 core\n"
"layout (location = 0) in vec2 position;\n"
"out vec2 tex_coord;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(position, 0.0, 1.0);\n"
"    tex_coord = 0.5*position+0.5;\n"
"}\n"
;

string glass::gbuffer_fragment_shader = 
"#version 460 core\n"
+ glass::common_fragment_utils_shader +
"layout(location = 0) out vec4 gbuffer_diffuse_specular;\n"
"layout(location = 1) out vec4 gbuffer_position_shininess;\n"
"layout(location = 2) out vec3 gbuffer_normal;\n"
"layout(location = 3) out vec3 gbuffer_lights;\n"
"in VertexOut\n"
"{\n"
"    vec3 position;\n"
"    // vec4 pos_light;\n"
"    vec2 tex_coord;\n"
"    mat3 TBN;\n"
"    mat3 normalTBN;\n"
"    vec3 to_camera_tangent;\n"
"}frag_in;\n"
"uniform Camera camera;\n"
"uniform PointLight point_light;\n"
"uniform DirLight dir_light;\n"
"uniform Model model;\n"
"uniform samplerCube skybox_map;\n"
"Fragment createFragment()\n"
"{\n"
"    Fragment frag;\n"
"    frag.position = frag_in.position;\n"
"    // frag.pos_light = frag_in.pos_light.xyz/frag_in.pos_light.w;\n"
"    frag.normal = normalize(frag_in.TBN[2]);\n"
"    frag.tex_coord = frag_in.tex_coord;\n"
"    if(model.material.using_bump_map)\n"
"    {\n"
"        frag = parallax_mapping(frag, model.material, frag_in.to_camera_tangent);\n"
"    }\n"
"    if(model.material.using_normal_map)\n"
"    {\n"
"        frag.normal = normalize(frag_in.normalTBN * (2*texture(model.material.normal_map, frag.tex_coord).xyz-1));\n"
"    }\n"
"    else\n"
"    {\n"
"        frag.normal = normalize(frag_in.TBN[2]);\n"
"    }\n"
"    return frag;\n"
"}\n"
"void main()\n"
"{\n"
"    Fragment frag = createFragment();\n"
"    gbuffer_normal = frag.normal;\n"
"    if(model.is_light)\n"
"    {\n"
"        gbuffer_diffuse_specular = vec4(0);\n"
"        gbuffer_position_shininess = vec4(0);\n"
"        gbuffer_lights = model.material.mask_color * diffuse_color(model.material, frag.tex_coord);\n"
"    }\n"
"    else\n"
"    {\n"
"        gbuffer_diffuse_specular = vec4(diffuse_color(model.material, frag.tex_coord), specular_strength(model.material, frag.tex_coord));\n"
"        gbuffer_position_shininess.xyz = frag.position;\n"
"        gbuffer_position_shininess.w = model.material.shininess;\n"
"        gbuffer_lights = vec3(0);\n"
"    }\n"
"}\n"
;

string glass::gbuffer_vertex_shader = 
"#version 460 core\n"
+ glass::common_utils_shader + 
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 normal;\n"
"layout (location = 2) in vec2 tex_coord;\n"
"layout (location = 3) in vec3 tangent;\n"
"layout (location = 4) in vec3 bitangent;\n"
"out VertexOut\n"
"{\n"
"	vec3 position;\n"
"	// vec4 pos_light;\n"
"	vec2 tex_coord;\n"
"	mat3 TBN;\n"
"	mat3 normalTBN;\n"
"	vec3 to_camera_tangent;\n"
"}vertex_out;\n"
"uniform DirLight dir_light;\n"
"uniform Camera camera;\n"
"uniform Model model;\n"
"void main()\n"
"{\n"
"	gl_Position = camera.mat * model.mat * vec4(position, 1.0);\n"
"	vertex_out.position = vec3(model.mat * vec4(position, 1.0));\n"
"	// vertex_out.pos_light = dir_light.mat * model.mat * vec4(position, 1.0);\n"
"	vertex_out.tex_coord = tex_coord;\n"
"	vertex_out.TBN = model.normal_mat * mat3(tangent, bitangent, normal);\n"
"	vertex_out.normalTBN = model.normal_mat * mat3(tangent/dot(tangent, tangent), bitangent/dot(bitangent, bitangent), 0.1*normal/model.material.height_factor);\n"
"	vertex_out.to_camera_tangent = inverse(vertex_out.TBN) * (camera.position - vertex_out.position);\n"
"}\n"
;

string glass::HDR_fragment_shader = 
"#version 460 core\n"
"out vec4 frag_color;\n"
"in vec2 tex_coord;\n"
"uniform sampler2D luminance_texture;\n"
"uniform sampler2D frame_image;\n"
"void main()\n"
"{\n"
"	float luminance = texture(luminance_texture, vec2(0.5, 0.5)).r;\n"
"	vec3 frag_color3 = texture(frame_image, tex_coord).rgb;\n"
"	frag_color3 = vec3(1.0) - exp(-frag_color3 / (0.5+pow(luminance, 1.0/3)));\n"
"	// frag_color3 = pow(frag_color3, vec3(1.0/2.2));\n"
"	// float alpha = 1;\n"
"	// frag_color3 = alpha*frag_color3/(1+luminance);\n"
"	// frag_color3 = frag_color3/(1+frag_color3);\n"
"	frag_color = vec4(frag_color3, 1.0);\n"
"}\n"
;

string glass::HDR_vertex_shader = 
"#version 460 core\n"
"layout (location = 0) in vec2 position;\n"
"out vec2 tex_coord;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(position, 0.0, 1.0);\n"
"    tex_coord = 0.5*position+0.5;\n"
"}\n"
;

string glass::lighting_fragment_shader = 
"#version 460 core\n"
+ glass::common_utils_shader + 
"out vec4 frag_color;\n"
"in vec2 tex_coord;\n"
"uniform int type;\n"
"uniform sampler2D base_image;\n"
"uniform sampler2D gbuffer_diffuse_specular;\n"
"uniform sampler2D gbuffer_position_shininess;\n"
"uniform sampler2D gbuffer_normal;\n"
"uniform PointLight point_light;\n"
"uniform DirLight dir_light;\n"
"uniform SpotLight spot_light;\n"
"uniform Camera camera;\n"
+ glass::common_lighting_phong_shader +
"Fragment createFragment()\n"
"{\n"
"    Fragment frag;\n"
"    vec4 position_shininess = texture(gbuffer_position_shininess, tex_coord);\n"
"    frag.position = position_shininess.xyz;\n"
"    // frag.pos_light = frag_in.pos_light.xyz/frag_in.pos_light.w;\n"
"    // frag.tex_coord = frag_in.tex_coord;\n"
"    // if(model.material.using_depth_map)\n"
"    // {\n"
"    //     frag.tex_coord = parallax_mapping(frag.tex_coord, model.material, frag_in.to_camera_tangent);\n"
"    // }\n"
"    frag.normal = normalize(texture(gbuffer_normal, tex_coord).xyz);\n"
"    // if(model.material.using_normal_map)\n"
"    // {\n"
"    //     frag.normal = normalize(frag_in.normalTBN * (2*texture(model.material.normal_map, frag.tex_coord).xyz-1));\n"
"    // }\n"
"    vec4 diffuse_specular = texture0(gbuffer_diffuse_specular, tex_coord);\n"
"    frag.diffuse_color = diffuse_specular.rgb;\n"
"    frag.specular_strength = diffuse_specular.a;\n"
"    frag.shininess = position_shininess.w;\n"
"    return frag;\n"
"}\n"
"void main()\n"
"{\n"
"	Fragment frag = createFragment();\n"
"    vec3 frag_color3 = vec3(0);\n"
"    switch(type)\n"
"    {\n"
"    case 0: // shadering ambient\n"
"        frag_color3 = 0.1 * frag.diffuse_color;\n"
"        break;\n"
"    case 1: // shadering direction light\n"
"        frag_color3 = texture0(base_image, tex_coord).rgb + PhongDirLight(frag, dir_light, camera);\n"
"        break;\n"
"    case 2: // shadering point light\n"
"        frag_color3 = texture0(base_image, tex_coord).rgb + PhongPointLight(frag, point_light, camera);\n"
"        break;\n"
"    case 3: // shadering spot light\n"
"        frag_color3 = texture0(base_image, tex_coord).rgb + PhongSpotLight(frag, spot_light, camera);\n"
"        break;\n"
"    }\n"
"    frag_color = vec4(frag_color3, 1.0);\n"
"}\n"
;

string glass::lighting_vertex_shader = 
"#version 460 core\n"
"layout (location = 0) in vec2 position;\n"
"out vec2 tex_coord;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(position, 0.0, 1.0);\n"
"    tex_coord = 0.5*position+0.5;\n"
"}\n"
;

string glass::luminance_fragment_shader = 
"#version 460 core\n"
"out float luminance;\n"
"in vec2 tex_coord;\n"
"uniform int type;\n"
"uniform sampler2D frame_image;\n"
"void main()\n"
"{\n"
"	switch(type)\n"
"	{\n"
"	case 0:\n"
"		// luminance = log(0.001 +dot(vec3(0.299, 0.587, 0.114), texture(frame_image, tex_coord).rgb));\n"
"		luminance = dot(vec3(0.299, 0.587, 0.114), texture(frame_image, tex_coord).rgb);\n"
"		break;\n"
"	case 1:\n"
"		luminance = texture(frame_image, tex_coord).r;\n"
"		break;\n"
"	case 2:\n"
"		// luminance = exp(texture(frame_image, tex_coord).r);\n"
"		luminance = texture(frame_image, tex_coord).r;\n"
"		break;\n"
"	}\n"
"}\n"
;

string glass::luminance_vertex_shader = 
"#version 460 core\n"
"layout (location = 0) in vec2 position;\n"
"out vec2 tex_coord;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(position, 0.0, 1.0);\n"
"    tex_coord = 0.5*position+0.5;\n"
"}\n"
;

string glass::merge_fragment_shader = 
"#version 460 core\n"
+ glass::common_utils_shader + 
"out vec4 frag_color;\n"
"in vec2 tex_coord;\n"
"uniform sampler2D color_without_lights;\n"
"uniform sampler2D color_lights_clear;\n"
"uniform sampler2D color_lights_blur;\n"
"uniform sampler2D skybox_map;\n"
"void main()\n"
"{\n"
"	vec4 color_lights = texture0(color_lights_clear, tex_coord);\n"
"	vec4 color_clear = texture0(color_without_lights, tex_coord) + color_lights;\n"
"	float factor = 1-0.9*soft_step(length(color_lights.rgb)-0.1, 0.1);\n"
"    frag_color = color_clear + factor * texture0(color_lights_blur, tex_coord) + texture0(skybox_map, tex_coord);\n"
"}\n"
;

string glass::merge_vertex_shader = 
"#version 460 core\n"
"layout (location = 0) in vec2 position;\n"
"out vec2 tex_coord;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(position, 0.0, 1.0);\n"
"    tex_coord = 0.5*position+0.5;\n"
"}\n"
;

string glass::shadow_dir_light_fragment_shader = 
"#version 460 core\n"
"void main()\n"
"{\n"
"    // gl_FragDepth = gl_FragCoord.z;\n"
"}\n"
;

string glass::shadow_dir_light_vertex_shader = 
"#version 460 core\n"
"layout (location = 0) in vec3 position;\n"
"uniform mat4 light_mat;\n"
"uniform mat4 model_mat;\n"
"void main()\n"
"{\n"
"    gl_Position = light_mat * model_mat * vec4(position, 1.0f);\n"
"}\n"
;

string glass::shadow_point_light_fragment_shader = 
"#version 330 core\n"
"in vec4 frag_pos;\n"
+ glass::common_utils_shader +
"uniform int type;\n"
"uniform PointLight point_light;\n"
"uniform SpotLight spot_light;\n"
"void main()\n"
"{\n"
"	if(type == 0) // point light\n"
"	{\n"
"    	gl_FragDepth = length(frag_pos.xyz - point_light.position) / point_light.radius;\n"
"	}\n"
"	else // spot light\n"
"	{\n"
"		gl_FragDepth = length(frag_pos.xyz - spot_light.position) / spot_light.radius;\n"
"	}\n"
"}\n"
;

string glass::shadow_point_light_geometry_shader = 
"#version 460 core\n"
"layout (triangles) in;\n"
"layout (triangle_strip, max_vertices=18) out;\n"
+ glass::common_utils_shader +
"uniform int type;\n"
"uniform SpotLight spot_light;\n"
"uniform PointLight point_light;\n"
"out vec4 frag_pos;\n"
"void main()\n"
"{\n"
"    for(int face = 0; face < 6; ++face)\n"
"    {\n"
"        gl_Layer = face; // built-in variable that specifies to which face we render.\n"
"        for(int i = 0; i < 3; ++i) // for each triangle's vertices\n"
"        {\n"
"            frag_pos = gl_in[i].gl_Position;\n"
"            if(type == 0)\n"
"            {\n"
"                gl_Position = point_light.mat[face] * frag_pos;\n"
"            }\n"
"            else\n"
"            {\n"
"                gl_Position = spot_light.mat[face] * frag_pos;\n"
"            }\n"
"            EmitVertex();\n"
"        }    \n"
"        EndPrimitive();\n"
"    }\n"
"}\n"
;

string glass::shadow_point_light_vertex_shader = 
"#version 460 core\n"
"layout (location = 0) in vec3 position;\n"
"uniform mat4 model_mat;\n"
"void main()\n"
"{\n"
"    gl_Position = model_mat * vec4(position, 1.0);\n"
"}\n"
;

string glass::skybox_fragment_shader = 
"#version 460 core\n"
"layout (location = 4) out vec3 skybox_color;\n"
"in vec3 tex_coord;\n"
"uniform samplerCube skybox_map;\n"
"void main()\n"
"{    \n"
"    skybox_color = texture(skybox_map, tex_coord).rgb;\n"
"}\n"
;

string glass::skybox_vertex_shader = 
"#version 460 core\n"
"layout (location = 0) in vec3 position;\n"
"out vec3 tex_coord;\n"
+ glass::common_utils_shader + 
"uniform Camera camera;\n"
"void main()\n"
"{\n"
"    tex_coord = position;\n"
"    vec4 pos = camera.projection_mat * camera.rotation_mat * vec4(position, 1.0);\n"
"    gl_Position = pos.xyww;\n"
"}\n"
;