#ifndef __SHADERS_H__
#define __SHADERS_H__

#include <string>

namespace glass
{
extern std::string blur_fragment_shader;
extern std::string blur_vertex_shader;
extern std::string common_fragment_utils_shader;
extern std::string common_lighting_phong_shader;
extern std::string common_utils_shader;
extern std::string gbuffer_fragment_shader;
extern std::string gbuffer_vertex_shader;
extern std::string HDR_fragment_shader;
extern std::string HDR_vertex_shader;
extern std::string lighting_fragment_shader;
extern std::string lighting_vertex_shader;
extern std::string luminance_fragment_shader;
extern std::string luminance_vertex_shader;
extern std::string merge_fragment_shader;
extern std::string merge_vertex_shader;
extern std::string shadow_dir_light_fragment_shader;
extern std::string shadow_dir_light_vertex_shader;
extern std::string shadow_point_light_fragment_shader;
extern std::string shadow_point_light_geometry_shader;
extern std::string shadow_point_light_vertex_shader;
extern std::string skybox_fragment_shader;
extern std::string skybox_vertex_shader;
};

#endif