#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "glsl_objects.h"
#include <vector>

struct Material
{
    // for glsl
	vec3 ambient_color;
    vec3 diffuse_color;
    vec3 specular_color;
    float shininess;
    float height_factor = 1.0f;

    sampler2D ambient_map;
    sampler2D diffuse_map;
    sampler2D specular_map;
    sampler2D reflection_map;
    sampler2D depth_map;
    sampler2D normal_map;

    bool using_ambient_map = false;
    bool using_diffuse_map = false;
    bool using_specular_map = false;
    bool using_reflection_map = false;
    bool using_depth_map = false;
    bool using_normal_map = false;
    // end

	Material(string name = "");
	static void ls();
	static vector<string> list();

	void set_ambient_map(const string& filename);
	void set_diffuse_map(const string& filename);
	void set_specular_map(const string& filename);
	void set_reflection_map(const string& filename);
	void set_depth_map(const string& filename, float factor = 1.0f);
	void set_normal_map(const string& filename);

    void enable_ambient_map();
    void enable_diffuse_map();
    void enable_specular_map();
    void enable_reflection_map();
    void enable_depth_map();
    void enable_normal_map();

    void disable_ambient_map();
    void disable_diffuse_map();
    void disable_specular_map();
    void disable_reflection_map();
    void disable_depth_map();
    void disable_normal_map();
};

#endif