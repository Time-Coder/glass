#ifndef __GLSL_OBJECTS_H__
#define __GLSL_OBJECTS_H__

#include <vector>
#include <list>
#include <map>
#include <set>

#include "glad/glad.h"
#include "vec.h"
#include "mat.h"
#include "image.h"

class sampler2D
{
	struct Instance
	{
		uint id = 0;

		uint width = 0;
		uint height = 0;
		uint format = GL_RGB;
		uint dtype = GL_UNSIGNED_BYTE;

		uint s_wrap_type = GL_REPEAT;
		uint t_wrap_type = GL_REPEAT;
		uint min_filter_type = GL_LINEAR;
		uint mag_filter_type = GL_LINEAR;
		vec4 border_color;

		bool is_applied = false;
	};

private:
	static uint active_sampler2D;
	static set<uint> existing_sampler2Ds;
	static list<Instance> existing_instances;
	static map<string, Instance*> path_map;
	static void destruct_all();
	static bool destruct_all_is_registered;
	
private:
	Instance* instance = NULL;

private:
	void init();
	void _bind();

public:
	sampler2D();
	sampler2D(uint __width, uint __height, uint __format = GL_RGB, uint __dtype = GL_UNSIGNED_BYTE);
	sampler2D(const Image& image);
	sampler2D(const string& filename);
	sampler2D(const sampler2D& sampler);
	sampler2D(sampler2D&& sampler);
	~sampler2D();
	sampler2D& operator =(const sampler2D& sampler);
	sampler2D& operator =(sampler2D&& sampler);
	uint id()const;
	uint width()const;
	uint height()const;
	uint format()const;
	uint dtype()const;
	bool is_bind()const;

	void border_color(const vec4& color);
	void border_color(const vec3& color);
	vec4 border_color()const;

	uint wrap_s()const;
	uint wrap_t()const;
	void wrap_s(uint wrap_type);
	void wrap_t(uint wrap_type);
	void wrap(uint wrap_type_s, uint wrap_type_t = -1);
	// wrap_type can choose in
	// [GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER]

	uint filter_min()const;
	uint filter_mag()const;
	void filter_min(uint filter_type);
	void filter_mag(uint filter_type);
	void filter(uint filter_type_min, uint filter_type_mag = -1);
	// filter_type can be choose in
	// [GL_NEAREST, GL_LINEAR]
	
	void bind();
	void unbind()const;
	void addImage(const Image& image);
	void addImage(const string& filename);
	void malloc(uint _width, uint _height, uint _format = GL_RGB, uint _dtype = GL_UNSIGNED_BYTE);
};
// vec4 texture(const sampler2D& sampler, const vec2& texture_coord);

class samplerCube
{
	struct Instance
	{
		uint id = 0;
		
		uint s_wrap_type = GL_CLAMP_TO_EDGE;
		uint t_wrap_type = GL_CLAMP_TO_EDGE;
		uint r_wrap_type = GL_CLAMP_TO_EDGE;
		uint min_filter_type = GL_LINEAR;
		uint mag_filter_type = GL_LINEAR;
		vec4 border_color;

		bool is_applied = false;
	};

private:
	static uint active_samplerCube;
	static set<uint> existing_samplerCubes;
	static list<Instance> existing_instances;

	static map<string, Instance*> path_map;
	static void destruct_all();
	static bool destruct_all_is_registered;

private:
	Instance* instance = NULL;

private:
	void _bind();
	void init();

public:
	samplerCube();
	samplerCube(const string& filename);
	samplerCube(const Image& image);
	samplerCube(const string& right, const string& left,
	            const string& up   , const string& down,
	            const string& back , const string& front);
	samplerCube(const Image& right, const Image& left,
	            const Image& up   , const Image& down,
	            const Image& back , const Image& front);
	samplerCube(vector<Image>& image);
	samplerCube(const vector<string>& filename);
	samplerCube(const samplerCube& sampler);
	samplerCube(samplerCube&& sampler);
	~samplerCube();
	samplerCube& operator =(const samplerCube& sampler);
	samplerCube& operator =(samplerCube&& sampler);
	uint id()const;
	bool is_bind()const;

	void bind();
	void unbind()const;

	void addImage(const string& filename);
	void addImage(const Image& image);

	void addImages(const vector<Image>& images);
	void addImages(const vector<string>& filenames);

	void addImages(const Image& right, const Image& left,
		           const Image& up   , const Image& down,
		           const Image& back , const Image& front);

	void addImages(const string& right, const string& left,
		           const string& up   , const string& down,
		           const string& back , const string& front);

	void addRightImage(const Image& image);
	void addLeftImage(const Image& image);
	void addUpImage(const Image& image);
	void addDownImage(const Image& image);
	void addFrontImage(const Image& image);
	void addBackImage(const Image& image);

	void addRightImage(const string& image);
	void addLeftImage(const string& image);
	void addUpImage(const string& image);
	void addDownImage(const string& image);
	void addFrontImage(const string& image);
	void addBackImage(const string& image);

	void border_color(const vec4& color);
	void border_color(const vec3& color);
	vec4 border_color()const;

	uint wrap_s()const;
	uint wrap_t()const;
	uint wrap_r()const;
	void wrap_s(uint wrap_type);
	void wrap_t(uint wrap_type);
	void wrap_r(uint wrap_type);
	void wrap(uint wrap_type_s, uint wrap_type_t = -1, uint wrap_type_r = -1);
	// wrap_type can choose in
	// [GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER]

	uint filter_min()const;
	uint filter_mag()const;
	void filter_min(uint filter_type);
	void filter_mag(uint filter_type);
	void filter(uint filter_type_min, uint filter_type_mag = -1);
	// filter_type can be choose in
	// [GL_NEAREST, GL_LINEAR]
};

#endif