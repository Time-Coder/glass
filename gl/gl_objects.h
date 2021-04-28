#ifndef __GL_OBJECTS_H__
#define __GL_OBJECTS_H__

#include <typeinfo>
#include <map>
#include <set>
#include <list>

#include "glsl_objects.h"
#include <GLFW/glfw3.h>
#include "exception.h"
#include "glvector.h"

#ifndef TYPE_UINT
typedef unsigned int uint;
#define TYPE_UINT
#endif

struct GLSL
{
	struct Size
	{
		uint size;
		uint align_size;
		uint glsl_size;

		Size(uint _size = 0, uint _align_size = 0, uint _glsl_size = 0) :
		size(_size), align_size(_align_size), glsl_size(_glsl_size) {}
	};

	struct Constructor
	{
		Constructor()
		{
			GLSL::init();
		}
	};

	static Constructor constructor;
	static map<string, Size> built_in_types;
	static void init();
};

class GLFW
{
private:
	static bool is_glfw_init;

public:
	static void init();
	static bool is_init();
};

class GLAD
{
private:
	static bool is_glad_init;

public:
	static void init();
	static bool is_init();
};

class VBO
{
private:
	static multiset<uint> existing_VBOs;
	static uint active_VBO;

private:
	uint _id = 0;
	uint _size = 0;

private:
	void init();

public:
	VBO();
	VBO(const VBO& vbo);
	VBO(VBO&& vbo);
	VBO& operator =(const VBO& ebo);
	VBO& operator =(VBO&& ebo);
	~VBO();
	uint id()const;
	void bind();
	void unbind()const;
	bool is_bind()const;
	void bufferData(void* ptr_value, uint n_bytes);
	bool empty()const;
	uint size()const;
	void clear();
	void malloc(uint n_bytes);
	void* ptr();
	bool apply();
	VBO clone();
};

struct VertexAttribPointer
{
	uint location = 0;
	uint vertex_size = 0;
	uint dtype = GL_FLOAT;
	uint should_normalize = GL_FALSE;
	uint stride = 0;
	uint offset = 0;

	VBO vbo;

	VertexAttribPointer(uint _location = 0, uint _vertex_size = 0, uint _dtype = GL_FLOAT, uint _should_normalize = GL_FALSE, uint _stride = 0, uint _offset = 0);
	VertexAttribPointer& operator ()(uint _vertex_size, uint _dtype = GL_FLOAT, uint _should_normalize = GL_FALSE, uint _stride = 0, uint _offset = 0);

	void interp(const VBO& _vbo);
	bool empty()const;
};

class EBO
{
private:
	static multiset<uint> existing_EBOs;
	static uint active_EBO;

private:
	uint _id = 0;
	uint _size = 0;

private:
	void init();

public:
	EBO();
	EBO(const EBO& ebo);
	EBO(EBO&& ebo);
	EBO& operator =(const EBO& ebo);
	EBO& operator =(EBO&& ebo);
	
	~EBO();
	uint id()const;
	uint size()const;
	bool empty()const;
	void bind();
	void unbind()const;
	bool is_bind()const;
	void bufferData(void* ptr_value, uint n_bytes);
	bool bufferData(const vector<uint>& indices);
	bool bufferData(const vector<uvec3>& indices);

	void clear();
	void malloc(uint n_bytes);
	void* ptr();
	bool apply();
	EBO clone();
};

class VAO
{
private:
	static multiset<uint> existing_VAOs;
	static uint active_VAO;

private:
	uint _id = 0;
	EBO _ebo;
	map<uint, VertexAttribPointer> map_VAP;
	void init();

public:
	VAO();
	VAO(const VAO& vao);
	VAO(VAO&& vao);
	VAO& operator =(const VAO& ebo);
	VAO& operator =(VAO&& ebo);
	VertexAttribPointer& operator [](uint i);
	~VAO();
	uint id()const;
	EBO ebo()const;
	void bind();
	void unbind()const;
	bool empty()const;
	void draw();
	void draw(uint n);
	void addEBO(const EBO& _ebo);
};

class RBO
{
private:
	static multiset<uint> existing_RBOs;
	static uint active_RBO;

private:
	uint _id = 0;
	uint _width = 0;
	uint _height = 0;
	uint _dtype = GL_DEPTH24_STENCIL8;
	
private:
	void init();

public:
	RBO();
	RBO(uint __width, uint __height, uint __dtype = GL_DEPTH24_STENCIL8);
	RBO(const RBO& rbo);
	RBO(RBO&& rbo);
	~RBO();

	RBO& operator =(const RBO& rbo);
	RBO& operator =(RBO&& rbo);

	uint id()const;
	uint width()const;
	uint height()const;
	uint dtype()const;

	void bind();
	void unbind();
	void clear();
	void malloc(uint __width, uint __height, uint __dtype = GL_DEPTH24_STENCIL8);

	Image image();
};

class FBO
{
private:
	static multiset<uint> existing_FBOs;
	static uint active_FBO;

private:
	uint _id = 0;
	uint _screen_width = 1024;
	uint _screen_height = 1024;
	uint _buffer_width = 1024;
	uint _buffer_height = 1024;
	RBO _color_RBO;
	RBO _depth_RBO;
	RBO _stencil_RBO;
	sampler2D _color_sampler;
	sampler2D _depth_sampler;
	sampler2D _stencil_sampler;

	bool is_color_RBO_enabled = false;
	bool is_depth_RBO_enabled = false;
	bool is_stencil_RBO_enabled = false;
	bool is_color_sampler_enabled = false;
	bool is_depth_sampler_enabled = false;
	bool is_stencil_sampler_enabled = false;

	bool is_applied = false;
	bool _auto_clear = true;
	bool _auto_resize = true;

private:
	void init();
	void apply();
	void _bind();
	void _unbind();

public:
	FBO(uint __screen_width = 1024, uint __screen_height = 1024, uint __buffer_width = 1024, uint __buffer_height = 1024);
	FBO(const FBO& fbo);
	FBO(FBO&& fbo);
	~FBO();

	FBO& operator =(const FBO& fbo);
	FBO& operator =(FBO&& fbo);

	uint id()const;
	uint screen_width()const;
	uint screen_height()const;
	uint buffer_width()const;
	uint buffer_height()const;
	void screen_size(uint width, uint height);
	void auto_clear(bool flag);
	void auto_resize(bool flag);

	void bind();
	void unbind();
	uint status();
	bool completed();
	void clear();

	void enable_color_sampler();
	void enable_depth_sampler();
	void enable_stencil_sampler();

	void enable_color_RBO();
	void enable_depth_RBO();
	void enable_stencil_RBO();

	sampler2D& color_sampler();
	sampler2D& depth_sampler();
	sampler2D& stencil_sampler();

	RBO& color_RBO();
	RBO& depth_RBO();
	RBO& stencil_RBO();
};

class BaseShader
{
private:
	static multiset<uint> existing_BaseShaders;

protected:
	uint _id = 0;
	uint _type = 0;
	string _filename;
	string _code;
	list<string> include_paths;
	map<uint, string> line_map;

public:
	std::set<uint> comments_set;

protected:
	void init();
	void find_comments();
	void replace_includes();
	void format_error(string& info);

public:
	BaseShader(uint __type);
	BaseShader(const BaseShader& vertex_shader);
	BaseShader(BaseShader&& vertex_shader);
	virtual ~BaseShader();
	BaseShader& operator =(const BaseShader& vertex_shader);
	BaseShader& operator =(BaseShader&& vertex_shader);

	uint id()const;
	string code()const;
	bool is_comment(int pos);
	string filename()const;
	bool empty()const;
	void compile(const string& file_name);
	void add_include_path(const string& include_path);
};

class VertexShader : public BaseShader
{
public:
	VertexShader() : BaseShader(GL_VERTEX_SHADER) {}
};

class FragmentShader : public BaseShader
{
public:
	FragmentShader() : BaseShader(GL_FRAGMENT_SHADER) {}
};

class Shader;
class Layout
{
	bool is_applied = false;

public:
	VAO vao;
	VBO vbo;
	EBO ebo;
	map<uint, GLVector> defined_layouts;

public:
	Layout();
	GLVector& operator ()(uint location);
	const GLVector& operator ()(uint location)const;
	void order(const vector<uvec3>& order_mat);
	void apply();
	void draw();
	bool empty()const;
};

class Uniform
{
	friend class Shader;
	Shader* parent = NULL;
	
public:
	struct Variable
	{
		string type;
		string name;
		uint size = 0;
		uint align_size = 0;
		uint padding_size = 0;
		int location = -1;
		uint texture_unit = 0;

		vector<string> atoms;

		Variable();
		Variable(const string& _type, const string& _name);
	};

	struct Struct
	{
		string name;
		vector<Variable> members;
		uint size = 0;
		uint align_size = 0;

		string host_name;
		uint host_size = 0;
		uint host_align_size = 0;
		unsigned long long host_hash = 0;

		friend ostream& operator <<(ostream& out, const Struct& structure);
		void clear();
	};

	class Reference
	{
		Uniform* parent = NULL;
		string name;

	public:
		Reference(Uniform* _parent, const string& _name);

		string type()const;
		uint size()const;
		int location()const;
		uint texture_unit()const;

		template<class DataType>
		void operator =(const DataType& value);

		Reference operator [](const string& sub_name)const;
		Reference operator [](uint index)const;
	};

private:
	map<string, Variable> original_uniform_map;
	map<string, Variable> uniform_map;
	map<string, Struct> defined_structs;
	vector<string> __parent_names;

private:
	void please_define(const string& type);
	void find_uniforms(const string& code, const std::set<uint>& comments_set);
	void find_structs(const string& code, const std::set<uint>& comments_set);
	void resolve_all_uniforms();
	void resolve_one_uniform(const string& name);
	void analyse_structs();
	void refresh();
	bool is_atom(const string& name);
	uint size_of(string type);
	uint align_size_of(string type);

public:
	Uniform(Shader* _parent);
	Uniform(Shader* _parent, const Uniform& uniform);
	Uniform(Shader* _parent, Uniform&& uniform);
	Uniform& operator =(const Uniform& uniform);
	Uniform& operator =(Uniform&& uniform);

	Reference operator [](const string& name);

	bool contains(const string& name)const;

	template<class DataType>
	void define(const string& type);

	int location(const string& name);
	
	template<class DataType>
	void set(const string& name, const DataType& value);

	void set_atom(const string& name, void* ptr_value);

	template<class DataType>
	void set_atom(const string& name, const DataType& value);
	
	void set_bool(const string& name, bool value);
	void set_uint(const string& name, uint value);
	void set_int(const string& name, int value);
	void set_float(const string& name, float value);
	void set_double(const string& name, double value);
	void set_bvec2(const string& name, const bvec2& value);
	void set_bvec3(const string& name, const bvec3& value);
	void set_bvec4(const string& name, const bvec4& value);
	void set_ivec2(const string& name, const ivec2& value);
	void set_ivec3(const string& name, const ivec3& value);
	void set_ivec4(const string& name, const ivec4& value);
	void set_uvec2(const string& name, const uvec2& value);
	void set_uvec3(const string& name, const uvec3& value);
	void set_uvec4(const string& name, const uvec4& value);
	void set_vec2(const string& name, const vec2& value);
	void set_vec3(const string& name, const vec3& value);
	void set_vec4(const string& name, const vec4& value);
	void set_dvec2(const string& name, const dvec2& value);
	void set_dvec3(const string& name, const dvec3& value);
	void set_dvec4(const string& name, const dvec4& value);
	void set_mat2(const string& name, mat2 value);
	void set_mat3(const string& name, mat3 value);
	void set_mat4(const string& name, mat4 value);
	void set_sampler2D(const string& name, sampler2D& value);
	void set_sampler2D(const string& name, uint texture_id);
	void set_samplerCube(const string& name, samplerCube& value);
	void set_samplerCube(const string& name, uint texture_id);
};

class Model;
class sampler2D;
class Shader
{
	friend class Uniform;

private:
	static multiset<uint> existing_Shaders;
	static uint active_Shader;

private:
	uint _id = 0;
	bool linked = false;
	bool enable_warning = true;
	uint available_texture_unit = 1; // for class Uniform to use
	
private:
	void init();
	void find_layouts(const string& code, const std::set<uint>& comments_set);
	
public:
	VertexShader vertex_shader;
	FragmentShader fragment_shader;
	Uniform uniform;
	map<uint, string> layout_map;

public:
	Shader();
	Shader(const string& vertex_code, const string& fragment_code);
	Shader(const Shader& shader_program);
	Shader(Shader&& shader_program);
	~Shader();
	Shader& operator =(const Shader& shader_program);
	Shader& operator =(Shader&& shader_program);
	uint id()const;
	void compile_vertex_code(const string& vertex_code);
	void compile_fragment_code(const string& fragment_code);
	void link();
	void use()const;
	void unuse()const;
	bool is_using()const;
	void draw(Layout& layout);
	void draw(Model& model);
	void warning_on();
	void warning_off();
};

//*************************************************************************
// Implementation of all templates
//*************************************************************************

template<class DataType>
void Uniform::set(const string& name, const DataType& value)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}

	if(is_atom(name))
	{
		set_atom(name, value);
		return;
	}

	string type = uniform_map[name].type;
	please_define(type);

	if(defined_structs.count(type))
	{
		unsigned long long host_hash = defined_structs[type].host_hash;
		string host_name = defined_structs[type].host_name;
		if(host_hash != 0 && typeid(DataType).hash_code() != host_hash)
		{
			throw TypeError("Error when setting " + type + " uniform variable " + name + ":\nGLSL struct " + type + " is defined as host struct " + host_name + ".\nBut host type " + type_name(DataType) + " is used now.");
		}
	}

	if(sizeof(DataType) < uniform_map[name].size)
	{
		throw TypeError("Error when setting " + type + " uniform variable " + name + ":\nCannot convert " + type_name(DataType) + " to " + type + ".");
	}

	unsigned char* ptr = (unsigned char*)(&(value));
	vector<string>& atoms = uniform_map[name].atoms;
	for(auto atom = atoms.begin(); atom != atoms.end(); atom++)
	{
		// if(name == "test_test")
		// {
		// 	printf("%s: %d\n", atom->c_str(), (void*)ptr);
		// }
		set_atom(*atom, (void*)(ptr));
		ptr += uniform_map[*atom].size + uniform_map[*atom].padding_size;
	}
	// cout << endl;
}

template<class DataType>
void Uniform::define(const string& type)
{
	if(defined_structs.count(type))
	{
		defined_structs[type].host_size = sizeof(DataType);
		if(defined_structs[type].host_size < defined_structs[type].size)
		{
			defined_structs[type].host_size = 0;
			throw TypeError("Cannot convert host struct " + type_name(DataType) + " to GLSL struct " + type + ":\nhost struct's size should be larger than GLSL struct's.");
		}

		defined_structs[type].host_align_size = alignof(DataType);
		if(defined_structs[type].host_align_size < defined_structs[type].align_size)
		{
			defined_structs[type].host_align_size = 0;
			throw TypeError("Cannot convert host struct " + type_name(DataType) + " to GLSL struct " + type + ":\nhost struct's align size should be larger than GLSL struct's.");
		}

		defined_structs[type].host_hash = typeid(DataType).hash_code();
		defined_structs[type].host_name = type_name(DataType);

		refresh();
	}
	else
	{
		throw TypeError("uniform struct " + type + " is not defined in GLSL code.");
	}
}

template<class DataType>
void Uniform::set_atom(const string& name, const DataType& value)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}

	string type = uniform_map[name].type;

	if(type == "bool")
	{
		if(same_type(DataType, bool)) set_bool(name, force_cast<bool>(value));
		else if(same_type(DataType, _Bit_reference)) set_bool(name, (bool)force_cast<_Bit_reference>(value));
		else if(same_type(DataType, uint)) set_bool(name, force_cast<uint>(value));
		else if(same_type(DataType, int)) set_bool(name, force_cast<int>(value));
		else if(same_type(DataType, float)) set_bool(name, force_cast<float>(value));
		else if(same_type(DataType, double)) set_bool(name, force_cast<double>(value));
		else throw TypeError("Error when setting bool uniform variable " + name + ":\nCannot convert " + type_name(DataType) + " to bool.");
	}
	else if(type == "uint")
	{
		if(same_type(DataType, bool)) set_uint(name, force_cast<bool>(value));
		else if(same_type(DataType, _Bit_reference)) set_uint(name, (bool)force_cast<_Bit_reference>(value));
		else if(same_type(DataType, uint)) set_uint(name, force_cast<uint>(value));
		else if(same_type(DataType, int)) set_uint(name, force_cast<int>(value));
		else if(same_type(DataType, float)) set_uint(name, force_cast<float>(value));
		else if(same_type(DataType, double)) set_uint(name, force_cast<double>(value));
		else throw TypeError("Error when setting uint uniform variable " + name + ":\nCannot convert " + type_name(DataType) + " to uint.");
	}
	else if(type == "int")
	{
		if(same_type(DataType, bool)) set_int(name, force_cast<bool>(value));
		else if(same_type(DataType, _Bit_reference)) set_int(name, (bool)force_cast<_Bit_reference>(value));
		else if(same_type(DataType, uint)) set_int(name, force_cast<uint>(value));
		else if(same_type(DataType, int)) set_int(name, force_cast<int>(value));
		else if(same_type(DataType, float)) set_int(name, force_cast<float>(value));
		else if(same_type(DataType, double)) set_int(name, force_cast<double>(value));
		else throw TypeError("Error when setting int uniform variable " + name + ":\nCannot convert " + type_name(DataType) + " to int.");
	}
	else if(type == "float")
	{
		if(same_type(DataType, bool)) set_float(name, force_cast<bool>(value));
		else if(same_type(DataType, _Bit_reference)) set_float(name, (bool)force_cast<_Bit_reference>(value));
		else if(same_type(DataType, uint)) set_float(name, force_cast<uint>(value));
		else if(same_type(DataType, int)) set_float(name, force_cast<int>(value));
		else if(same_type(DataType, float)) set_float(name, force_cast<float>(value));
		else if(same_type(DataType, double)) set_float(name, force_cast<double>(value));
		else throw TypeError("Error when setting float uniform variable " + name + ":\nCannot convert " + type_name(DataType) + " to float.");
	}
	else if(type == "double")
	{
		if(same_type(DataType, bool)) set_double(name, force_cast<bool>(value));
		else if(same_type(DataType, _Bit_reference)) set_double(name, (bool)force_cast<_Bit_reference>(value));
		else if(same_type(DataType, uint)) set_double(name, force_cast<uint>(value));
		else if(same_type(DataType, int)) set_double(name, force_cast<int>(value));
		else if(same_type(DataType, float)) set_double(name, force_cast<float>(value));
		else if(same_type(DataType, double)) set_double(name, force_cast<double>(value));
		else throw TypeError("Error when setting double uniform variable " + name + ":\nCannot convert " + type_name(DataType) + " to double.");
	}
	else if(type == "bvec2")
	{
		if(same_type(DataType, bvec2)) set_bvec2(name,         force_cast<bvec2>(value));
		else if(same_type(DataType, ivec2)) set_bvec2(name, (bvec2)(force_cast<ivec2>(value)));
		else if(same_type(DataType, uvec2)) set_bvec2(name, (bvec2)(force_cast<uvec2>(value)));
		else if(same_type(DataType,  vec2)) set_bvec2(name, (bvec2)(force_cast< vec2>(value)));
		else if(same_type(DataType, dvec2)) set_bvec2(name, (bvec2)(force_cast<dvec2>(value)));
		else throw TypeError("Error when setting bvec2 uniform variable " + name + ":\nCannot convert " + type_name(DataType) + " to bvec2.");
	}
	else if(type == "bvec3")
	{
		if(same_type(DataType, bvec3)) set_bvec3(name,         force_cast<bvec3>(value));
		else if(same_type(DataType, ivec3)) set_bvec3(name, (bvec3)(force_cast<ivec3>(value)));
		else if(same_type(DataType, uvec3)) set_bvec3(name, (bvec3)(force_cast<uvec3>(value)));
		else if(same_type(DataType,  vec3)) set_bvec3(name, (bvec3)(force_cast< vec3>(value)));
		else if(same_type(DataType, dvec3)) set_bvec3(name, (bvec3)(force_cast<dvec3>(value)));
		else throw TypeError("Error when setting bvec3 uniform variable " + name + ":\nCannot convert " + type_name(DataType) + " to bvec3.");
	}
	else if(type == "bvec4")
	{
		if(same_type(DataType, bvec4)) set_bvec4(name,         force_cast<bvec4>(value));
		else if(same_type(DataType, ivec4)) set_bvec4(name, (bvec4)(force_cast<ivec4>(value)));
		else if(same_type(DataType, uvec4)) set_bvec4(name, (bvec4)(force_cast<uvec4>(value)));
		else if(same_type(DataType,  vec4)) set_bvec4(name, (bvec4)(force_cast< vec4>(value)));
		else if(same_type(DataType, dvec4)) set_bvec4(name, (bvec4)(force_cast<dvec4>(value)));
		else throw TypeError("Error when setting bvec4 uniform variable " + name + ":\nCannot convert " + type_name(DataType) + " to bvec4.");
	}
	else if(type == "ivec2")
	{
		if(same_type(DataType, bvec2)) set_ivec2(name, (ivec2)(force_cast<bvec2>(value)));
		else if(same_type(DataType, ivec2)) set_ivec2(name,         force_cast<ivec2>(value));
		else if(same_type(DataType, uvec2)) set_ivec2(name, (ivec2)(force_cast<uvec2>(value)));
		else if(same_type(DataType,  vec2)) set_ivec2(name, (ivec2)(force_cast< vec2>(value)));
		else if(same_type(DataType, dvec2)) set_ivec2(name, (ivec2)(force_cast<dvec2>(value)));
		else throw TypeError("Error when setting ivec2 uniform variable " + name + ":\nCannot convert " + type_name(DataType) + " to ivec2.");
	}
	else if(type == "ivec3")
	{
		if(same_type(DataType, bvec3)) set_ivec3(name, (ivec3)(force_cast<bvec3>(value)));
		else if(same_type(DataType, ivec3)) set_ivec3(name,         force_cast<ivec3>(value));
		else if(same_type(DataType, uvec3)) set_ivec3(name, (ivec3)(force_cast<uvec3>(value)));
		else if(same_type(DataType,  vec3)) set_ivec3(name, (ivec3)(force_cast< vec3>(value)));
		else if(same_type(DataType, dvec3)) set_ivec3(name, (ivec3)(force_cast<dvec3>(value)));
		else throw TypeError("Error when setting ivec3 uniform variable " + name + ":\nCannot convert " + type_name(DataType) + " to ivec3.");
	}
	else if(type == "ivec4")
	{
		if(same_type(DataType, bvec4)) set_ivec4(name, (ivec4)(force_cast<bvec4>(value)));
		else if(same_type(DataType, ivec4)) set_ivec4(name,         force_cast<ivec4>(value));
		else if(same_type(DataType, uvec4)) set_ivec4(name, (ivec4)(force_cast<uvec4>(value)));
		else if(same_type(DataType,  vec4)) set_ivec4(name, (ivec4)(force_cast< vec4>(value)));
		else if(same_type(DataType, dvec4)) set_ivec4(name, (ivec4)(force_cast<dvec4>(value)));
		else throw TypeError("Error when setting ivec4 uniform variable " + name + ":\nCannot convert " + type_name(DataType) + " to ivec4.");
	}
	else if(type == "uvec2")
	{
		if(same_type(DataType, bvec2)) set_uvec2(name, (uvec2)(force_cast<bvec2>(value)));
		else if(same_type(DataType, ivec2)) set_uvec2(name, (uvec2)(force_cast<ivec2>(value)));
		else if(same_type(DataType, uvec2)) set_uvec2(name,         force_cast<uvec2>(value));
		else if(same_type(DataType,  vec2)) set_uvec2(name, (uvec2)(force_cast< vec2>(value)));
		else if(same_type(DataType, dvec2)) set_uvec2(name, (uvec2)(force_cast<dvec2>(value)));
		else throw TypeError("Error when setting uvec2 uniform variable " + name + ":\nCannot convert " + type_name(DataType) + " to uvec2.");
	}
	else if(type == "uvec3")
	{
		if(same_type(DataType, bvec3)) set_uvec3(name, (uvec3)(force_cast<bvec3>(value)));
		else if(same_type(DataType, ivec3)) set_uvec3(name, (uvec3)(force_cast<ivec3>(value)));
		else if(same_type(DataType, uvec3)) set_uvec3(name,         force_cast<uvec3>(value));
		else if(same_type(DataType,  vec3)) set_uvec3(name, (uvec3)(force_cast< vec3>(value)));
		else if(same_type(DataType, dvec3)) set_uvec3(name, (uvec3)(force_cast<dvec3>(value)));
		else throw TypeError("Error when setting uvec3 uniform variable " + name + ":\nCannot convert " + type_name(DataType) + " to uvec3.");
	}
	else if(type == "uvec4")
	{
		if(same_type(DataType, bvec4)) set_uvec4(name, (uvec4)(force_cast<bvec4>(value)));
		else if(same_type(DataType, ivec4)) set_uvec4(name, (uvec4)(force_cast<ivec4>(value)));
		else if(same_type(DataType, uvec4)) set_uvec4(name,         force_cast<uvec4>(value));
		else if(same_type(DataType,  vec4)) set_uvec4(name, (uvec4)(force_cast< vec4>(value)));
		else if(same_type(DataType, dvec4)) set_uvec4(name, (uvec4)(force_cast<dvec4>(value)));
		else throw TypeError("Error when setting uvec4 uniform variable " + name + ":\nCannot convert " + type_name(DataType) + " to uvec4.");
	}
	else if(type == "vec2")
	{
		if(same_type(DataType, bvec2)) set_vec2(name, (vec2)(force_cast<bvec2>(value)));
		else if(same_type(DataType, ivec2)) set_vec2(name, (vec2)(force_cast<ivec2>(value)));
		else if(same_type(DataType, uvec2)) set_vec2(name, (vec2)(force_cast<uvec2>(value)));
		else if(same_type(DataType,  vec2)) set_vec2(name,        force_cast< vec2>(value));
		else if(same_type(DataType, dvec2)) set_vec2(name, (vec2)(force_cast<dvec2>(value)));
		else throw TypeError("Error when setting vec2 uniform variable " + name + ":\nCannot convert " + type_name(DataType) + " to vec2.");
	}
	else if(type == "vec3")
	{
		if(same_type(DataType, bvec3)) set_vec3(name, (vec3)(force_cast<bvec3>(value)));
		else if(same_type(DataType, ivec3)) set_vec3(name, (vec3)(force_cast<ivec3>(value)));
		else if(same_type(DataType, uvec3)) set_vec3(name, (vec3)(force_cast<uvec3>(value)));
		else if(same_type(DataType,  vec3)) set_vec3(name,        force_cast< vec3>(value));
		else if(same_type(DataType, dvec3)) set_vec3(name, (vec3)(force_cast<dvec3>(value)));
		else throw TypeError("Error when setting vec3 uniform variable " + name + ":\nCannot convert " + type_name(DataType) + " to vec3.");
	}
	else if(type == "vec4")
	{
		if(same_type(DataType, bvec4)) set_vec4(name, (vec4)(force_cast<bvec4>(value)));
		else if(same_type(DataType, ivec4)) set_vec4(name, (vec4)(force_cast<ivec4>(value)));
		else if(same_type(DataType, uvec4)) set_vec4(name, (vec4)(force_cast<uvec4>(value)));
		else if(same_type(DataType,  vec4)) set_vec4(name,        force_cast< vec4>(value));
		else if(same_type(DataType, dvec4)) set_vec4(name, (vec4)(force_cast<dvec4>(value)));
		else throw TypeError("Error when setting vec4 uniform variable " + name + ":\nCannot convert " + type_name(DataType) + " to vec4.");
	}
	else if(type == "dvec2")
	{
		if(same_type(DataType, bvec2)) set_dvec2(name, (dvec2)(force_cast<bvec2>(value)));
		else if(same_type(DataType, ivec2)) set_dvec2(name, (dvec2)(force_cast<ivec2>(value)));
		else if(same_type(DataType, uvec2)) set_dvec2(name, (dvec2)(force_cast<uvec2>(value)));
		else if(same_type(DataType,  vec2)) set_dvec2(name, (dvec2)(force_cast< vec2>(value)));
		else if(same_type(DataType, dvec2)) set_dvec2(name,         force_cast<dvec2>(value));
		else throw TypeError("Error when setting dvec2 uniform variable " + name + ":\nCannot convert " + type_name(DataType) + " to dvec2.");
	}
	else if(type == "dvec3")
	{
		if(same_type(DataType, bvec3)) set_dvec3(name, (dvec3)(force_cast<bvec3>(value)));
		else if(same_type(DataType, ivec3)) set_dvec3(name, (dvec3)(force_cast<ivec3>(value)));
		else if(same_type(DataType, uvec3)) set_dvec3(name, (dvec3)(force_cast<uvec3>(value)));
		else if(same_type(DataType,  vec3)) set_dvec3(name, (dvec3)(force_cast< vec3>(value)));
		else if(same_type(DataType, dvec3)) set_dvec3(name,         force_cast<dvec3>(value));
		else throw TypeError("Error when setting dvec3 uniform variable " + name + ":\nCannot convert " + type_name(DataType) + " to dvec3.");
	}
	else if(type == "dvec4")
	{
		if(same_type(DataType, bvec4)) set_dvec4(name, (dvec4)(force_cast<bvec4>(value)));
		else if(same_type(DataType, ivec4)) set_dvec4(name, (dvec4)(force_cast<ivec4>(value)));
		else if(same_type(DataType, uvec4)) set_dvec4(name, (dvec4)(force_cast<uvec4>(value)));
		else if(same_type(DataType,  vec4)) set_dvec4(name, (dvec4)(force_cast< vec4>(value)));
		else if(same_type(DataType, dvec4)) set_dvec4(name,         force_cast<dvec4>(value));
		else throw TypeError("Error when setting dvec4 uniform variable " + name + ":\nCannot convert " + type_name(DataType) + " to dvec4.");
	}
	else if(type == "mat2")
	{
		if(same_type(DataType, mat2)) set_mat2(name, force_cast<mat2>(value));
		else throw TypeError("Error when setting mat2 uniform variable " + name + ":\nCannot convert " + type_name(DataType) + " to mat2.");
	}
	else if(type == "mat3")
	{
		if(same_type(DataType, mat3)) set_mat3(name, force_cast<mat3>(value));
		else throw TypeError("Error when setting mat3 uniform variable " + name + ":\nCannot convert " + type_name(DataType) + " to mat3.");
	}
	else if(type == "mat4")
	{
		if(same_type(DataType, mat4)) set_mat4(name, force_cast<mat4>(value));
		else throw TypeError("Error when setting mat4 uniform variable " + name + ":\nCannot convert " + type_name(DataType) + " to mat4.");
	}
	else if(type == "sampler2D")
	{
		if(same_type(DataType, sampler2D)) set_sampler2D(name, force_cast<sampler2D>(value));
		else if(same_type(DataType, bool)) set_sampler2D(name, (uint)(force_cast<bool>(value)));
		else if(same_type(DataType, int)) set_sampler2D(name, (uint)(force_cast<int>(value)));
		else if(same_type(DataType, uint)) set_sampler2D(name, force_cast<uint>(value));
		else if(same_type(DataType, float)) set_sampler2D(name, (uint)(force_cast<float>(value)));
		else if(same_type(DataType, double)) set_sampler2D(name, (uint)(force_cast<double>(value)));
		else throw TypeError("Error when setting sampler2D uniform variable " + name + ":\nCannot convert " + type_name(DataType) + " to sampler2D.");
	}
	else if(type == "samplerCube")
	{
		if(same_type(DataType, samplerCube)) set_samplerCube(name, force_cast<samplerCube>(value));
		else if(same_type(DataType, bool)) set_samplerCube(name, (uint)(force_cast<bool>(value)));
		else if(same_type(DataType, int)) set_samplerCube(name, (uint)(force_cast<int>(value)));
		else if(same_type(DataType, uint)) set_samplerCube(name, force_cast<uint>(value));
		else if(same_type(DataType, float)) set_samplerCube(name, (uint)(force_cast<float>(value)));
		else if(same_type(DataType, double)) set_samplerCube(name, (uint)(force_cast<double>(value)));
		else throw TypeError("Error when setting samplerCube uniform variable " + name + ":\nCannot convert " + type_name(DataType) + " to samplerCube.");
	}
	else
	{
		throw TypeError("Uniform variable " + name + " is not an atom variable.");
	}
}

template<class DataType>
void Uniform::Reference::operator =(const DataType& value)
{
	parent->set(name, value);
}

#endif