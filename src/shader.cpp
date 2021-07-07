#include "glass/Shader"
#include "glass/Layout"
#include "glass/Model"
#include "glass/samplerCube"
#include "glass/utils/helper.h"

using namespace std;

multiset<uint> BaseShader::existing_BaseShaders;
void BaseShader::init()
{
	if(_id == 0)
	{
		_id = glCreateShader(_type);
		if(_id == 0)
		{
			throw glass::RuntimeError("Failed to create BaseShader.");
		}
	#ifdef _DEBUG
		cout << "constructing BaseShader " << _id << endl;
	#endif
		existing_BaseShaders.insert(_id);
	}
	else if(existing_BaseShaders.count(_id) == 0)
	{
		throw glass::RuntimeError("BaseShader " + str::str(_id) + " has already been destructed.");
	}
}

void BaseShader::del()
{
	uint count = existing_BaseShaders.count(_id);
	if(count > 0)
	{
		multiset_pop(existing_BaseShaders, _id);
		if(count == 1)
		{
		#ifdef _DEBUG
			cout << "destructing BaseShader " << _id << endl;
		#endif
			glDeleteShader(_id);
		}
	}
}

BaseShader::BaseShader(uint __type) : _type(__type) {}

BaseShader::BaseShader(const BaseShader& base_shader) :
_id(base_shader._id),
_type(base_shader._type),
_filename(base_shader._filename),
_code(base_shader._code),
include_paths(base_shader.include_paths),
line_map(base_shader.line_map),
comments_set(base_shader.comments_set)
{
	if(existing_BaseShaders.count(_id) > 0)
	{
		existing_BaseShaders.insert(_id);
	}
}

BaseShader::BaseShader(BaseShader&& base_shader) :
_id(move(base_shader._id)),
_type(move(base_shader._type)),
_filename(move(base_shader._filename)),
_code(move(base_shader._code)),
include_paths(move(base_shader.include_paths)),
line_map(move(base_shader.line_map)),
comments_set(move(base_shader.comments_set))
{
	base_shader._id = 0;
}

BaseShader::~BaseShader()
{
	del();
}

BaseShader& BaseShader::operator =(const BaseShader& base_shader)
{
	if(this != &base_shader && _id != base_shader._id)
	{
		del();
		_id = base_shader._id;
		_type = base_shader._type;
		_filename = base_shader._filename;
		_code = base_shader._code;
		include_paths = base_shader.include_paths;
		line_map = base_shader.line_map;
		comments_set = base_shader.comments_set;

		if(existing_BaseShaders.count(_id))
		{
			existing_BaseShaders.insert(_id);
		}
	}
	return *this;
}

BaseShader& BaseShader::operator =(BaseShader&& base_shader)
{
	if(this != &base_shader)
	{
		if(_id != base_shader._id)
		{
			del();
		}
		_id = move(base_shader._id);
		_type = move(base_shader._type);
		_filename = move(base_shader._filename);
		_code = move(base_shader._code);
		include_paths = move(base_shader.include_paths);
		line_map = move(base_shader.line_map);
		comments_set = move(base_shader.comments_set);

		base_shader._id = 0;
	}
	return *this;
}

uint BaseShader::id()const
{
	return _id;
}

string BaseShader::code()const
{
	return clean_code;
}

bool BaseShader::empty()const
{
	return _code.empty();
}

void BaseShader::add_include_path(const string& include_path)
{
	include_paths.push_front(path::abspath(include_path));
}

void BaseShader::find_comments()
{
	comments_set.clear();
    size_t pos_start = 0;
    size_t pos_end = 0;
	bool should_break = false;
	while(true)
	{
		pos_start = _code.find("/*", pos_end);
		if(pos_start == string::npos)
		{
			break;
		}

		pos_end = _code.find("*/", pos_start+2);
		if(pos_end == string::npos)
		{
			pos_end = _code.size()-2;
			should_break = true;
		}
		pos_end += 2;

        for(uint i = pos_start; i < pos_end; i++)
		{
			comments_set.insert(i);
		}
		if(should_break)
		{
			break;
		}
	}

	pos_start = 0;
	pos_end = 0;
	should_break = false;
	while(true)
	{
		pos_start = _code.find("//", pos_end);
		if(pos_start == string::npos)
		{
			break;
		}

		pos_end = _code.find("\n", pos_start+2);
		if(pos_end == string::npos)
		{
			pos_end = _code.size();
			should_break = true;
		}
        for(uint i = pos_start; i < pos_end; i++)
		{
			comments_set.insert(i);
		}
		if(should_break)
		{
			break;
		}
	}
}

void BaseShader::replace_includes()
{
    int pos_include_start = 0;
    int pos_filename_start = 0;
    int pos_include_end = 0;
    int pos_filename_end = 0;
    uint n_lines = str::lines(_code);
    for(uint i = 1; i <= n_lines; i++)
	{
		line_map[i] = _filename + ":" + str::str(i) + ": error: ";
	}
	while(true)
	{
		find_comments();

		pos_include_start = 0;
		while(true)
		{
			pos_include_start = _code.find("#include", pos_include_start);
            if((size_t)pos_include_start == string::npos)
			{
				return;
			}
			if(comments_set.count(pos_include_start))
			{
				pos_include_start += 8;
			}
			else
			{
				break;
			}
		}

		pos_filename_start = pos_include_start + 8;
		str::skip_space(_code, pos_filename_start);
		if(_code[pos_filename_start] != '<' && _code[pos_filename_start] != '\"')
		{
			throw glass::SyntaxError(line_map[str::line_number(_code, pos_filename_start)] + "#include file name must be enveloped by <...> or \"...\"");
		}
		char end_char = _code[pos_filename_start] == '<' ? '>' : '\"';
		pos_filename_start++;
		str::skip_space(_code, pos_filename_start);
		pos_filename_end = _code.find(end_char, pos_filename_start);
        if((size_t)pos_filename_end == string::npos)
		{
			throw glass::SyntaxError(line_map[str::line_number(_code, pos_filename_start)] + "#include file name must be enveloped by <...> or \"...\"");
		}
		pos_include_end = pos_filename_end + 1;
		pos_filename_end--;
		str::skip_space_reverse(_code, pos_filename_end);
		string filename = _code.substr(pos_filename_start, pos_filename_end+1-pos_filename_start);
		bool found = false;

		for(auto it = include_paths.begin(); it != include_paths.end(); it++)
		{
			if(!path::isdir(*it))
			{
				continue;
			}
			string full_name = *it + "/" + filename;
			if(path::isfile(full_name))
			{
				string include_content = path::cat(full_name);
				uint include_lines = str::lines(include_content);
				_code.replace(pos_include_start, pos_include_end-pos_include_start, include_content);
				uint include_line_number = str::line_number(_code, pos_include_start);
				uint include_line_end = include_line_number + include_lines;
				n_lines = str::lines(_code);

				map<uint, string> old_line_map = line_map;
                for(uint i = include_line_end; i <= n_lines; i++)
				{
					line_map[i] = old_line_map[i - include_lines + 1];
				}
                for(uint i = include_line_number; i < include_line_end; i++)
				{
					line_map[i] = full_name + ":" + str::str(i-include_line_number+1) + ": error: ";
				}
				found = true;
				include_paths.push_front(path::dirname(full_name));
				break;
			}
		}
		if(!found)
		{
			throw glass::SyntaxError(line_map[str::line_number(_code, pos_filename_start)] + "File \"" + filename + "\" is not exist.");
		}
	}
}

void BaseShader::format_error(string& info)
{
	int pos = 0;
	while(true)
	{
		int pos_error = info.find("ERROR: ", pos);
        if((size_t)pos_error == string::npos)
		{
			break;
		}
		pos = pos_error + 7;
		int pos_line_start = info.find(":", pos) + 1;
		int pos_line_end = info.find(":", pos_line_start);
		uint n_line = str::str2<uint>(info.substr(pos_line_start, pos_line_end-pos_line_start));
		info.replace(pos_error, pos_line_end+2-pos_error, line_map[n_line]);
	}

	info = str::replace(info, "syntax error syntax error", "syntax error");

	pos = info.size()-1;
	while(pos >= 0)
	{
		if(info[pos] == ' ' || info[pos] == '\t' || info[pos] == '\n' || info[pos] == '\r')
		{
			pos--;
		}
		else
		{
			pos++;
			break;
		}
	}
	info.erase(pos);
}

void BaseShader::compile(const string& filename)
{
	init();

	if(!path::isfile(filename))
	{
		_filename = filename;
		add_include_path(".");
		_code = filename;
	}
	else
	{
		_filename = path::relpath(filename);

		add_include_path(".");
		add_include_path(path::dirname(path::abspath(filename)));

		_code = path::cat(filename);
	}

	replace_includes();
	clean_code = str::delete_C_style_comments(_code);

	char* value = new char[(_code.size()+1)*sizeof(char)];
	strcpy(value, _code.c_str());

	glShaderSource(_id, 1, &(value), NULL);
    delete[] value;
	glCompileShader(_id);

	int success;
	char infoLog[8192];
	glGetShaderiv(_id, GL_COMPILE_STATUS, &success);
	if(!success)
	{
	    glGetShaderInfoLog(_id, 8192, NULL, infoLog);
	    _code.clear();
	    string info = string(infoLog);
	    format_error(info);
	    throw glass::SyntaxError(info);
	}
}

multiset<uint> Shader::existing_Shaders;
uint Shader::active_Shader = 0;

void Shader::init()
{
	if(_id == 0)
	{
		_id = glCreateProgram();
		if(_id == 0)
		{
			throw glass::Exception("Failed to create Shader!");
		}
	#ifdef _DEBUG
		cout << "constructing Shader " << _id << endl;
	#endif
		existing_Shaders.insert(_id);
	}
	else if(existing_Shaders.count(_id) == 0)
	{
		throw glass::RuntimeError("Shader " + str::str(_id) + " has already been destructed.");
	}
}

void Shader::del()
{
	uint count = existing_Shaders.count(_id);
	if(count > 0)
	{
		multiset_pop(existing_Shaders, _id);
		if(count == 1)
		{
		#ifdef _DEBUG
			cout << "destructing Shader " << _id << endl;
		#endif
			unuse();
			glDeleteProgram(_id);
		}
	}
}

void Shader::find_layouts(const string& code)
{
	// layout (location = 2) in vec2 texture_coord;
	int pos = 0;
	while(true)
	{
		pos = code.find("layout", pos);
        if((size_t)pos == string::npos)
		{
			return;
		}
		if(!(pos == 0 || (pos-1 >= 0          && (code[pos-1] == '\n' || code[pos-1] == '\t' || code[pos-1] == ' '))) || 
           !(             (size_t)pos+6 < code.size() && (code[pos+6] == '\n' || code[pos+6] == '\t' || code[pos+6] == ' ' || code[pos+6] == '(')))
		{
			pos += 6;
			continue;
		}
		pos = code.find("(", pos) + 1;
		string var = str::get_var(code, pos);
		if(var.find("location") == string::npos)
		{
			continue;
		}

		int pos_number_start = code.find("=", pos) + 1;
		int pos_number_end = code.find(")", pos_number_start);
		uint location = str::str2<uint>(str::strip(code.substr(pos_number_start, pos_number_end-pos_number_start)));
		int pos_end = code.find(";", pos_number_end);
		pos = pos_end - 1;
		str::skip_space_reverse(code, pos);
		str::skip_valid_reverse(code, pos);
		str::skip_space_reverse(code, pos);
		int pos_type_end = pos + 1;
		str::skip_valid_reverse(code, pos);
		int pos_type_start = pos + 1;
		string type_str = code.substr(pos_type_start, pos_type_end-pos_type_start);
		layout_map[location] = type_str;
		pos = pos_end + 1;
	}
}

Shader::Shader() : uniform(this) {}

Shader::Shader(const Shader& shader) :
_id(shader._id),
linked(shader.linked),
enable_warning(shader.enable_warning),
available_texture_unit(shader.available_texture_unit),
vertex_shader(shader.vertex_shader),
fragment_shader(shader.fragment_shader),
uniform(this, shader.uniform),
layout_map(shader.layout_map)
{
	if(existing_Shaders.count(_id) > 0)
	{
		existing_Shaders.insert(_id);
	}
}

Shader::Shader(Shader&& shader) :
_id(move(shader._id)),
linked(move(shader.linked)),
enable_warning(move(shader.enable_warning)),
available_texture_unit(move(shader.available_texture_unit)),
vertex_shader(move(shader.vertex_shader)),
fragment_shader(move(shader.fragment_shader)),
uniform(this, move(shader.uniform)),
layout_map(move(shader.layout_map))
{
	shader._id = 0;
}

Shader& Shader::operator =(const Shader& shader)
{
	if(this != &shader && _id != shader._id)
	{
		del();

		_id = shader._id;
		linked = shader.linked;
		enable_warning = shader.enable_warning;
		available_texture_unit = shader.available_texture_unit;
		vertex_shader = shader.vertex_shader;
		fragment_shader = shader.fragment_shader;
		uniform = shader.uniform;
		layout_map = shader.layout_map;

		if(existing_Shaders.count(_id) > 0)
		{
			existing_Shaders.insert(_id);
		}
	}
	return *this;
}

Shader& Shader::operator =(Shader&& shader)
{
	if(this != &shader)
	{
		if(_id != shader._id)
		{
			del();
		}

		_id = move(shader._id);
		linked = move(shader.linked);
		enable_warning = move(shader.enable_warning);
		available_texture_unit = move(shader.available_texture_unit);
		vertex_shader = move(shader.vertex_shader);
		fragment_shader = move(shader.fragment_shader);
		uniform = move(shader.uniform);
		layout_map = move(shader.layout_map);
	}
	return *this;
}

Shader::~Shader()
{
	del();
}

uint Shader::id()const
{
	return _id;
}

void Shader::compile(Shader::ShaderType type, const string& code)
{
	switch(type)
	{
		case Shader::VERTEX: vertex_shader.compile(code); break;
		case Shader::FRAGMENT: fragment_shader.compile(code); break;
		case Shader::GEOMETRY: geometry_shader.compile(code); break;
	}
}

void Shader::link()
{
	if(vertex_shader.empty())
	{
		throw glass::RuntimeError("Please compile vertex shader before link!");
	}
	if(fragment_shader.empty())
	{
		throw glass::RuntimeError("Please compile fragment shader before link!");
	}

	init();
	glAttachShader(_id, vertex_shader.id());
	glAttachShader(_id, fragment_shader.id());
	if(!geometry_shader.empty())
	{
		glAttachShader(_id, geometry_shader.id());
	}
	glLinkProgram(_id);

	int  success;
	char infoLog[8192];
	glGetProgramiv(_id, GL_LINK_STATUS, &success);
	if(!success)
	{
	    glGetProgramInfoLog(_id, 8192, NULL, infoLog);
	    throw glass::Exception(infoLog);
	}
	find_layouts(vertex_shader.code());

	uniform.find_structs(vertex_shader.code());
	uniform.find_structs(fragment_shader.code());

	uniform.find_uniforms(vertex_shader.code());
	uniform.find_uniforms(fragment_shader.code());

	uniform.find_uniform_blocks(vertex_shader.code());
	uniform.find_uniform_blocks(fragment_shader.code());

	uniform.refresh();

	linked = true;

	for(auto it = uniform.uniform_map.begin(); it != uniform.uniform_map.end(); it++)
	{
		if(it->second.type == "sampler2D")
		{
			uniform[it->second.name] = sampler2D::blackSampler();
		}
	}
}

void Shader::use()const
{
	if(!linked)
	{
		throw glass::Exception("Need to link Shader before using!");
	}
	glUseProgram(_id);
	active_Shader = _id;
}

void Shader::unuse()const
{
	if(active_Shader == _id)
	{
		glUseProgram(0);
		active_Shader = 0;
	}
}

bool Shader::isUsing()const
{
	return (existing_Shaders.count(_id) > 0 && active_Shader == _id);
}

void Shader::draw(Layout& layout, VAO::DrawType type)
{
	for(auto it = layout_map.begin(); it != layout_map.end(); it++)
	{
		if(enable_warning && layout.defined_layouts.count(it->first) == 0)
		{
			cout << "Warning: layout(location = " + str::str(it->first) + ") in " + layout_map[it->first] + " is not passed." << endl;
		}
		else if(layout(it->first).dtype() != layout_map[it->first])
		{
			throw glass::TypeError("Error when setting layout(location = " + str::str(it->first) + ") in " + layout_map[it->first] + ":\nCannot convert " + layout(it->first).dtype() + " to " + layout_map[it->first] + ".");
		}
	}

	if(enable_warning)
	{
		for(auto it = layout.defined_layouts.begin(); it != layout.defined_layouts.end(); it++)
		{
			if(layout_map.count(it->first) == 0)
			{
				cout << "Warning: layout(location = " + str::str(it->first) + ") in " + layout(it->first).dtype() + " is not used." << endl;
			}
		}
	}
	
	use();
	layout.draw(type);
}

void Shader::draw(Model& model)
{
	vector<string> keys = model.keys();
	if(!(model.layout().empty()))
	{
		if(uniform.contains("model") && uniform["model"].type() == "Model")
		{
			uniform["model.material"] = model.__material;
		}
		draw(model.layout());
	}
	for(auto it = keys.begin(); it != keys.end(); it++)
	{
		draw(model[*it]);
	}
}

void Shader::warningOn()
{
	enable_warning = true;
}

void Shader::warningOff()
{
	enable_warning = false;
}
