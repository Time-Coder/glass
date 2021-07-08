#include <cmath>

#include "glass/Uniform"
#include "glass/Shader"
#include "glass/UBO"
#include "glass/mat"
#include "glass/sampler2D"
#include "glass/samplerCube"

using namespace std;

const uint Uniform::Bindable::offset = sizeof(std::list<Uniform::Reference>);

void Uniform::Bindable::bind(Uniform::Reference& ref)
{
	for(auto it = refs.begin(); it != refs.end(); it++)
	{
		if(*it == ref)
		{
			return;
		}
	}
	refs.push_back(ref);
}

void Uniform::Bindable::unbind(const Uniform::Reference& ref)
{
	for(auto it = refs.begin(); it != refs.end(); it++)
	{
		if(*it == ref)
		{
			refs.erase(it);
			return;
		}
	}
}

void Uniform::Bindable::sync()
{
	for(auto it = refs.begin(); it != refs.end(); it++)
	{
		unsigned char* ptr = (unsigned char*)(this);
		if(it->parent->defined_structs.count(it->parent->uniform_map[it->name].type) == 0 ||
		   it->parent->defined_structs[it->parent->uniform_map[it->name].type].host_hash == 0)
		{
			ptr += Uniform::Bindable::offset;
		}
        for(uint i = 0; i < it->parent->uniform_map[it->name].atoms.size(); i++)
		{
			std::string child_name = it->parent->uniform_map[it->name].atoms[i];
			
			ptr += it->parent->uniform_map[child_name].offset;
			it->parent->set_atom(child_name, (void*)ptr);
			ptr += it->parent->uniform_map[child_name].size + it->parent->uniform_map[child_name].padding_size;
		}
	}
}

Uniform::Block::Block() {}

// Uniform::Block::Block(Uniform* _parent, const string& _name, const string& _instance_name) :
// parent(_parent), name(_name), instance_name(_instance_name) {}

Uniform::Block::Block(const string& _name, const string& _instance_name) :
name(_name), instance_name(_instance_name) {}

// void Uniform::Block::bind(uint point)
// {
// 	glUniformBlockBinding(parent->parent->id(), index, point);
// 	binding_point = point;
// }

// void Uniform::Block::bind(const UBO& ubo)
// {
// 	// if(ubo.block != *this)
// 	// {
// 	// 	cout << "Warning: uniform block " << name << "'s structure is not same as " << ubo.block.name << endl;
// 	// }
// 	bind(ubo.binding_point());
// }

bool Uniform::Block::empty()const
{
	return (size == 0 || index == -1);
}

bool Uniform::Block::contains(const string& member_name)const
{
	for(vector<Uniform::Variable>::const_iterator it = members.begin(); it != members.end(); it++)
	{
		if(it->name == member_name)
		{
			return true;
		}
	}
	return false;
}

Uniform::Variable& Uniform::Block::operator [](const string& member_name)
{
	for(vector<Uniform::Variable>::iterator it = members.begin(); it != members.end(); it++)
	{
		if(it->name == member_name)
		{
			return *it;
		}
	}
	members.push_back(Uniform::Variable(member_name));
	return members.back();
}

const Uniform::Variable& Uniform::Block::operator [](const string& member_name)const
{
	for(vector<Uniform::Variable>::const_iterator it = members.begin(); it != members.end(); it++)
	{
		if(it->name == member_name)
		{
			return *it;
		}
	}
	throw glass::KeyError(member_name + " is not a member of Uniform Block " + name);
}

bool Uniform::BlockMap::Reference::operator >=(const Uniform::BlockMap::Reference& block)
{
	if(size() < block.size() || instance()->members.size() < block.instance()->members.size())
	{
		return false;
	}

    for(uint i = 0; i < block.instance()->members.size() && i < instance()->members.size(); i++)
	{
		if(block.instance()->members[i].location != instance()->members[i].location || block.instance()->members[i].type != instance()->members[i].type)
		{
			return false;
		}
	}
	return true;
}

bool Uniform::BlockMap::Reference::operator <=(const Uniform::BlockMap::Reference& block)
{
	if(size() > block.size() || instance()->members.size() > block.instance()->members.size())
	{
		return false;
	}

    for(uint i = 0; i < block.instance()->members.size(); i++)
	{
		if(block.instance()->members[i].location != instance()->members[i].location || block.instance()->members[i].type != instance()->members[i].type)
		{
			return false;
		}
	}
	return true;
}

bool Uniform::BlockMap::Reference::operator !=(const Uniform::BlockMap::Reference& block)
{
	if(size() != block.size() || instance()->members.size() != block.instance()->members.size())
	{
		return true;
	}

    for(uint i = 0; i < instance()->members.size(); i++)
	{
		if(block.instance()->members[i].location != instance()->members[i].location || block.instance()->members[i].type != instance()->members[i].type)
		{
			return true;
		}
	}
	return false;
}

bool Uniform::BlockMap::Reference::operator ==(const Uniform::BlockMap::Reference& block)
{
	if(size() != block.size() || instance()->members.size() != block.instance()->members.size())
	{
		return false;
	}

    for(uint i = 0; i < instance()->members.size(); i++)
	{
		if(block.instance()->members[i].location != instance()->members[i].location || block.instance()->members[i].type != instance()->members[i].type)
		{
			return false;
		}
	}
	return true;
}

ostream& operator <<(ostream& out, const Uniform::BlockMap::Reference& block)
{
	out << "uniform " << block.name << " // index = " << block.index() << endl;
	out << "{" << endl;
	for(auto member = block.instance()->members.begin(); member != block.instance()->members.end(); member++)
	{
		out << "    " << member->type << " " << member->name << ";";
		if(member->location != -1)
		{
			 out << " // offset = " << member->location << endl;
		}
		else
		{
			out << endl;
		}
	}
	out << "}" << block.instance_name() << ";";

	return out;
}

void Uniform::find_uniforms(const string& code)
{
	int pos = 0;
	while(true)
	{
		pos = code.find("uniform", pos);
        if((size_t)pos == string::npos)
		{
			return;
		}
		if(!(pos == 0 || (pos-1 >= 0          && (code[pos-1] == ' ' || code[pos-1] == '\n' || code[pos-1] == '\t'))) ||
           !(             (size_t)pos+7 < code.size() && (code[pos+7] == ' ' || code[pos+7] == '\n' || code[pos+7] == '\t')))
		{
			pos += 7;
			continue;
		}

		int pos_start = pos + 8;
		int pos_semicolon = code.find(";", pos_start);
		int pos_equal = code.find("=", pos_start);
		int pos_end = pos_semicolon;
        if((size_t)pos_equal != string::npos && pos_equal < pos_semicolon)
		{
			pos_end = pos_equal;
		}
		string type = str::get_word(code, pos_start);
		string all_names = code.substr(pos_start, pos_end-pos_start);
		if(all_names.find("{") != string::npos)
		{
			pos += 7;
			continue;
		}

		vector<string> names = str::split(all_names, ",");
		for(auto it = names.begin(); it != names.end(); it++)
		{
			string name = str::format_var_name(*it);
			int pos_left_brace = name.find("[");
            if((size_t)pos_left_brace == string::npos)
			{
				original_uniform_map[name] = Uniform::Variable(type, name);
			}
			else
			{
				string type_ext = name.substr(pos_left_brace);
				name = name.substr(0,pos_left_brace);
				original_uniform_map[name] = Uniform::Variable(type+type_ext, name);
			}
		}
		pos = pos_end + 1;
	}
}

void Uniform::resolve_all_uniforms()
{
	uniform_map.clear();
	for(auto it = original_uniform_map.begin(); it != original_uniform_map.end(); it++)
	{
		uniform_map[it->first] = Uniform::Variable(it->second.type, it->first);
		__parent_names.clear();
		__parent_names.push_back(it->first);
		resolve_one_uniform(it->first);
	}
	__parent_names.clear();
}

void Uniform::resolve_all_uniform_blocks()
{
	uniform_block_map.clear();
	for(auto block = original_uniform_block_map.begin(); block != original_uniform_block_map.end(); block++)
	{
		uniform_block_map[block->first] = Block(block->first, block->second.instance_name);
		uniform_block_map[block->first].index = glGetUniformBlockIndex(parent->id(), block->first.c_str());
		glGetActiveUniformBlockiv(parent->id(), uniform_block_map[block->first].index, GL_UNIFORM_BLOCK_DATA_SIZE, &(uniform_block_map[block->first].size));
		for(auto member = block->second.members.begin(); member != block->second.members.end(); member++)
		{
			uniform_block_map[block->first][member->name] = Uniform::Variable(member->type, member->name);
			__parent_names.clear();
			__parent_names.push_back(member->name);
			resolve_one_uniform_block_member(block->first, member->name);
		}
	}
	__parent_names.clear();
}

void Uniform::resolve_one_uniform_block_member(const string& block_name, const string& block_member_name)
{
	string type = uniform_block_map[block_name][block_member_name].type;
	uniform_block_map[block_name][block_member_name].align_size = align_size_of(type);
	uniform_block_map[block_name][block_member_name].size = size_of(type);
	// uniform_block_map[block_name].members[block_member_name].offset = offset_of(type);

	int pos_left_brace = type.find("[");
    if((size_t)pos_left_brace == string::npos)
	{
		if(defined_structs.count(type) == 0)
		{
			const char* c_str_block_member_name = (uniform_block_map[block_name].name + "." + block_member_name).c_str();
			int index = glGetProgramResourceIndex(parent->id(), GL_UNIFORM, c_str_block_member_name);
			GLenum props[] = {GL_ARRAY_STRIDE, GL_OFFSET};
			GLint values[2] = {};
			glGetProgramResourceiv(parent->id(), GL_UNIFORM, index, 2, props, 2, NULL, values);
			uniform_block_map[block_name][block_member_name].location = values[1];

			if(block_member_name.find("[") != string::npos)
			{
				uniform_block_map[block_name][block_member_name].location += str::array_length(block_member_name) * values[0];
			}

			for(string parent_name : __parent_names)
			{
				uniform_block_map[block_name][parent_name].atoms.push_back(block_member_name);
			}
			return;
		}
		else
		{
			for(auto member = defined_structs[type].members.begin(); member != defined_structs[type].members.end(); member++)
			{
				string member_name = block_member_name + "." + member->name;
				uniform_block_map[block_name][member_name] = Uniform::Variable(member->type, member_name);
				uniform_block_map[block_name][member_name].offset = member->offset;
				uniform_block_map[block_name][member_name].size = member->size;
				uniform_block_map[block_name][member_name].padding_size = member->padding_size;
				__parent_names.push_back(member_name);
				resolve_one_uniform_block_member(block_name, member_name);
				__parent_names.pop_back();
			}
		}
	}
	else
	{
		int n = str::array_length(type);
		type = str::base_type(type);
		uint size = size_of(type);

		for(int i = 0; i < n; i++)
		{
			string element_name = block_member_name + "[" + str::str(i) + "]";
			uniform_block_map[block_name][element_name] = Uniform::Variable(type, element_name);
			uniform_block_map[block_name][element_name].offset = uniform_block_map[block_name][block_member_name].offset;
			uniform_block_map[block_name][element_name].size = size;
			uniform_block_map[block_name][element_name].padding_size = (i != n-1 ? 0 : uniform_block_map[block_name][block_member_name].padding_size);
			__parent_names.push_back(element_name);
			resolve_one_uniform_block_member(block_name, element_name);
			__parent_names.pop_back();
		}
	}
}

void Uniform::resolve_one_uniform(const string& name)
{
	string type = uniform_map[name].type;
	uniform_map[name].align_size = align_size_of(type);
	uniform_map[name].size = size_of(type);
	// uniform_map[name].offset = offset_of(type);

	int pos_left_brace = type.find("[");
    if((size_t)pos_left_brace == string::npos) // not an array
	{
		if(defined_structs.count(type) == 0) // is built-in type
		{
			uniform_map[name].location = glGetUniformLocation(parent->id(), name.c_str());
			for(string parent_name : __parent_names)
			{
				uniform_map[parent_name].atoms.push_back(name);
			}
			return;
		}
		else // is struct
		{
			for(auto member = defined_structs[type].members.begin(); member != defined_structs[type].members.end(); member++)
			{
				string member_name = name + "." + member->name;
				uniform_map[member_name] = Uniform::Variable(member->type, member_name);
				uniform_map[member_name].size = member->size;
				uniform_map[member_name].offset = member->offset;
				uniform_map[member_name].padding_size = member->padding_size;
				__parent_names.push_back(member_name);
				resolve_one_uniform(member_name);
				__parent_names.pop_back();
			}
		}
	}
	else // is an array
	{
		int n = str::array_length(type);
		type = str::base_type(type);
		uint size = size_of(type);

		for(int i = 0; i < n; i++)
		{
			string element_name = name + "[" + str::str(i) + "]";
			uniform_map[element_name] = Uniform::Variable(type, element_name);
			uniform_map[element_name].offset = uniform_map[name].offset;
			uniform_map[element_name].size = size;
			uniform_map[element_name].padding_size = (i != n-1 ? 0 : uniform_map[name].padding_size);
			__parent_names.push_back(element_name);
			resolve_one_uniform(element_name);
			__parent_names.pop_back();
		}
	}
}

uint Uniform::align_size_of(string type)
{
	if(uniform_map.count(type))
	{
		type = uniform_map[type].type;
	}

	if(type.find("[") == string::npos)
	{
		if(GLSL::built_in_types.count(type))
		{
			return GLSL::built_in_types[type].align_size;
		}
		if(!(defined_structs.count(type)))
		{
			throw glass::TypeError("Unknown uniform type: " + type);
		}
		if(defined_structs[type].align_size != 0)
		{
			return defined_structs[type].align_size;
		}

		uint max_align_size = 0;
		for(auto member = defined_structs[type].members.begin();
			member != defined_structs[type].members.end(); member++)
		{
			member->align_size = align_size_of(member->type);
			if(member->align_size > max_align_size)
			{
				max_align_size = member->align_size;
			}
		}

		if(defined_structs[type].host_align_size != 0 && defined_structs[type].host_align_size < max_align_size)
		{
			defined_structs[type].host_align_size = 0;
			throw glass::TypeError("Cannot convert host type " + type + " to GLSL type " + type);
		}

		defined_structs[type].align_size = max(max_align_size, defined_structs[type].host_align_size);
		
		return defined_structs[type].align_size;
	}
	else
	{
		return align_size_of(str::base_type(type));
	}
}

uint Uniform::offset_of(string type)
{
	if(uniform_map.count(type)) // convert variable name to type name
	{
		type = uniform_map[type].type;
	}

	type = str::base_type(type);
	if(GLSL::built_in_types.count(type))
	{
		return 0;
	}
	if(!(defined_structs.count(type)))
	{
		throw glass::TypeError("Unknown uniform type: " + type);
	}
	return defined_structs[type].offset;
}

uint Uniform::size_of(string type)
{
	if(uniform_map.count(type)) // convert variable name to type name
	{
		type = uniform_map[type].type;
	}

	if(type.find("[") == string::npos) // not an array type
	{
		if(GLSL::built_in_types.count(type))
		{
			return GLSL::built_in_types[type].size;
		}
		if(!(defined_structs.count(type)))
		{
			throw glass::TypeError("Unknown uniform type: " + type);
		}
		if(defined_structs[type].size != 0)
		{
			return defined_structs[type].size;
		}

		uint n_members = defined_structs[type].members.size();
		uint offset = 0, old_offset = 0;
		uint max_align_size = 0;
        for(uint i = 0; i < n_members; i++)
		{
			string current_type = defined_structs[type].members[i].type;

			uint current_align_size = align_size_of(current_type);
			if(current_align_size > max_align_size)
			{
				max_align_size = current_align_size;
			}

			offset = ceil((double)offset/(double)current_align_size) * current_align_size;
			
			if(i != 0)
			{
				defined_structs[type].members[i-1].padding_size = offset - old_offset;
			}
			defined_structs[type].members[i].offset = offset_of(current_type);
			defined_structs[type].members[i].size = size_of(current_type);
			offset += defined_structs[type].members[i].size;
			old_offset = offset;
		}
		defined_structs[type].members[0].offset += defined_structs[type].offset;
		uint size = ceil((double)offset/(double)max_align_size) * max_align_size;
		if(defined_structs[type].host_size != 0 && size > defined_structs[type].host_size)
		{
			defined_structs[type].host_size = 0;
			throw glass::TypeError("Cannot convert host type " + type + " to GLSL type " + type);
		}
		defined_structs[type].size = max(size, defined_structs[type].host_size);
		defined_structs[type].members[n_members-1].padding_size = defined_structs[type].size - offset - defined_structs[type].members[0].offset;
		return defined_structs[type].size;
	}
	else // array type
	{
		return size_of(str::base_type(type))*str::array_length(type);
	}
}

void Uniform::Struct::clear()
{
	name.clear();
	members.clear();
	size = 0;
	align_size = 0;

	host_size = 0;
	host_align_size = 0;
	host_hash = 0;
}

void Uniform::find_structs(const string& code)
{
	Struct structure;
	int pos = 0;
	while(true)
	{
		pos = code.find("struct", pos);
        if((size_t)pos == string::npos)
		{
			return;
		}
		if(!(pos == 0 || (pos-1 >= 0          && (code[pos-1] == '\n' || code[pos-1] == ' ' || code[pos-1] == '\t'))) ||
           !(             (size_t)pos+6 < code.size() && (code[pos+6] == '\n' || code[pos+6] == ' ' || code[pos+6] == '\t')))
		{
			pos += 6;
			continue;
		}

		pos += 6;
		structure.name = str::get_word(code, pos);
		str::get_word(code, pos); // == '{'
		while(true)
		{
			string type = str::get_word(code, pos);
			if(type[0] == '}')
			{
				break;
			}
			int pos_semi = code.find(";", pos);
			vector<string> names = str::split(code.substr(pos, pos_semi-pos), ",");
			for(auto it = names.begin(); it != names.end(); it++)
			{
				string name = str::format_var_name(*it);
				int pos_left_brace = name.find("[");
                if((size_t)pos_left_brace == string::npos)
				{
					structure.members.push_back(Uniform::Variable(type, name));
				}
				else
				{
					string type_ext = name.substr(pos_left_brace);
					name = name.substr(0, pos_left_brace);
					structure.members.push_back(Uniform::Variable(type+type_ext, name));
				}
			}
			pos = pos_semi + 1;
		}
		structure.size = 0;
		defined_structs[structure.name] = structure;
		structure.clear();
	}
}

void Uniform::find_uniform_blocks(const string& code)
{
	int pos = 0;
	while(true)
	{
		pos = code.find("uniform", pos);
        if((size_t)pos == string::npos)
		{
			return;
		}
		if(!(pos == 0 || (pos-1 >= 0          && (code[pos-1] == '\n' || code[pos-1] == ' ' || code[pos-1] == '\t'))) ||
           !(             (size_t)pos+7 < code.size() && (code[pos+7] == '\n' || code[pos+7] == ' ' || code[pos+7] == '\t' || code[pos+7] == '(')))
		{
			pos += 7;
			continue;
		}
		pos += 7;

		int pos_end = code.find(";", pos);
		string all_names = code.substr(pos, pos_end-pos);
		if(all_names.find("{") == string::npos)
		{
			pos += 7;
			continue;
		}

		pos_end = code.find("}", pos);
        if((size_t)pos_end == string::npos)
		{
			return;
		}
		pos_end++;
		string instance_name = str::get_var(code, pos_end);

		string block_name = str::get_word(code, pos);
		original_uniform_block_map[block_name] = Block(block_name, instance_name);
		str::get_word(code, pos); // == '{'
		while(true)
		{
			string type = str::get_var(code, pos);
			if(type.empty())
			{
				break;
			}
			int pos_semi = code.find(";", pos);
			vector<string> names = str::split(code.substr(pos, pos_semi-pos), ",");
			for(auto it = names.begin(); it != names.end(); it++)
			{
				string name = str::format_var_name(*it);
				int pos_left_brace = name.find("[");
                if((size_t)pos_left_brace == string::npos)
				{
					original_uniform_block_map[block_name][name] = Uniform::Variable(type, name);
				}
				else
				{
					string type_ext = name.substr(pos_left_brace);
					name = name.substr(0, pos_left_brace);
					original_uniform_block_map[block_name][name] = Uniform::Variable(type+type_ext, name);
				}
			}
			pos = pos_semi + 1;
		}
	}
}

void Uniform::analyse_structs()
{
	for(auto it = defined_structs.begin(); it != defined_structs.end(); it++)
	{
		it->second.align_size = align_size_of(it->first);
		it->second.size = size_of(it->first);
	}
}

void Uniform::refresh()
{
	if(!need_refresh)
	{
		return;
	}

	for(auto it = defined_structs.begin(); it != defined_structs.end(); it++)
	{
		it->second.align_size = 0;
		it->second.size = 0;
	}
	analyse_structs();
	resolve_all_uniforms();
	resolve_all_uniform_blocks();
	need_refresh = false;
}

Uniform::Variable::Variable(){}
Uniform::Variable::Variable(const string& _name) :
name(_name) {}
Uniform::Variable::Variable(const string& _type, const string& _name) :
type(_type), name(_name) {}

Uniform::Reference::Reference(Uniform* _parent, const string& _name) :
parent(_parent), name(_name) {}

string Uniform::Reference::type()const
{
	return parent->uniform_map[name].type;
}

uint Uniform::Reference::size()const
{
	return parent->uniform_map[name].size;
}

int Uniform::Reference::location()const
{
	return parent->uniform_map[name].location;
}

uint Uniform::Reference::texture_unit()const
{
	return parent->uniform_map[name].texture_unit;
}

Uniform::Reference Uniform::Reference::operator [](const string& member_name)const
{
	return Uniform::Reference(parent, name + "." + member_name);
}

Uniform::Reference Uniform::Reference::operator [](uint index)const
{
	return Uniform::Reference(parent, name + "[" + str::str(index) + "]");
}

bool Uniform::Reference::operator ==(const Uniform::Reference& ref)const
{
	return ((unsigned char*)parent == (unsigned char*)ref.parent && name == ref.name);
}

bool Uniform::Reference::operator !=(const Uniform::Reference& ref)const
{
	return ((unsigned char*)parent != (unsigned char*)ref.parent || name != ref.name);
}

bool Uniform::is_atom(const string& name)
{
	return (uniform_map.count(name) && uniform_map[name].atoms.size() == 1 && uniform_map[name].atoms[0] == name);
}

Uniform::Uniform(Shader* _parent) : parent(_parent) {}

Uniform::Uniform(Shader* _parent, const Uniform& uniform) :
parent(_parent),
original_uniform_map(uniform.original_uniform_map),
uniform_map(uniform.uniform_map),
original_uniform_block_map(uniform.original_uniform_block_map),
uniform_block_map(uniform.uniform_block_map),
defined_structs(uniform.defined_structs)
{}

Uniform::Uniform(Shader* _parent, Uniform&& uniform) :
parent(_parent),
original_uniform_map(move(uniform.original_uniform_map)),
uniform_map(move(uniform.uniform_map)),
original_uniform_block_map(move(uniform.original_uniform_block_map)),
uniform_block_map(move(uniform.uniform_block_map)),
defined_structs(move(uniform.defined_structs))
{}

Uniform& Uniform::operator =(const Uniform& uniform)
{
	if(&uniform == this)
	{
		return *this;
	}
	original_uniform_map = uniform.original_uniform_map;
	original_uniform_block_map = uniform.original_uniform_block_map;
	uniform_map = uniform.uniform_map;
	uniform_block_map = uniform.uniform_block_map;
	defined_structs = uniform.defined_structs;
	__parent_names = uniform.__parent_names;

	return *this;
}

Uniform& Uniform::operator =(Uniform&& uniform)
{
	if(&uniform == this)
	{
		return *this;
	}
	original_uniform_map = move(uniform.original_uniform_map);
	uniform_map = move(uniform.uniform_map);
	original_uniform_block_map = move(uniform.original_uniform_block_map);
	uniform_block_map = move(uniform.uniform_block_map);
	defined_structs = move(uniform.defined_structs);
	__parent_names = move(uniform.__parent_names);

	return *this;
}

Uniform::Reference Uniform::operator [](const string& name)
{
	if(!(uniform_map.count(name)))
	{
		throw glass::KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	return Uniform::Reference(this, name);
}

bool Uniform::contains(const string& name)const
{
	return (uniform_map.count(name) > 0);
}

void Uniform::please_define(const string& type)
{
	string basetype = str::base_type(type);
	if(type.find("[") == string::npos)
	{
		if(defined_structs.count(basetype))
		{
			for(auto member = defined_structs[basetype].members.begin(); member != defined_structs[basetype].members.end(); member++)
			{
				if(defined_structs.count(member->type) && defined_structs[member->type].host_hash == 0)
				{
					throw glass::RuntimeError("Please call Uniform::define<"+member->type+">() before assigning value.");
				}
				please_define(member->type);
			}
		}
	}
	else
	{
		if(defined_structs.count(basetype) && defined_structs[basetype].host_hash == 0)
		{
			throw glass::RuntimeError("Please call Uniform::define<"+basetype+">() before assigning value.");
		}
		please_define(basetype);
	}
}

int Uniform::location(const string& name)
{
	return glGetUniformLocation(parent->id(), name.c_str());
}

#define SET_ATOM_COMMON_CODE \
if(type == "bool") set_bool(name, *((bool*)ptr_value));\
else if(type == "uint") set_uint(name, *((uint*)ptr_value));\
else if(type == "int") set_int(name, *((int*)ptr_value));\
else if(type == "float") set_float(name, *((float*)ptr_value));\
else if(type == "double") set_double(name, *((double*)ptr_value));\
else if(type == "bvec2") set_bvec2(name, *((bvec2*)ptr_value));\
else if(type == "bvec3") set_bvec3(name, *((bvec3*)ptr_value));\
else if(type == "bvec4") set_bvec4(name, *((bvec4*)ptr_value));\
else if(type == "ivec2") set_ivec2(name, *((ivec2*)ptr_value));\
else if(type == "ivec3") set_ivec3(name, *((ivec3*)ptr_value));\
else if(type == "ivec4") set_ivec4(name, *((ivec4*)ptr_value));\
else if(type == "uvec2") set_uvec2(name, *((uvec2*)ptr_value));\
else if(type == "uvec3") set_uvec3(name, *((uvec3*)ptr_value));\
else if(type == "uvec4") set_uvec4(name, *((uvec4*)ptr_value));\
else if(type == "vec2") set_vec2(name, *((vec2*)ptr_value));\
else if(type == "vec3") set_vec3(name, *((vec3*)ptr_value));\
else if(type == "vec4") set_vec4(name, *((vec4*)ptr_value));\
else if(type == "dvec2") set_dvec2(name, *((dvec2*)ptr_value));\
else if(type == "dvec3") set_dvec3(name, *((dvec3*)ptr_value));\
else if(type == "dvec4") set_dvec4(name, *((dvec4*)ptr_value));\
else if(type == "mat2") set_mat2(name, *((mat2*)ptr_value));\
else if(type == "mat3") set_mat3(name, *((mat3*)ptr_value));\
else if(type == "mat4") set_mat4(name, *((mat4*)ptr_value));

void Uniform::set_atom(const string& name, void* ptr_value)
{
	if(uniform_map.count(name) == 0)
	{
		throw glass::KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}

	string type = uniform_map[name].type;

	SET_ATOM_COMMON_CODE
	else if(type == "sampler2D") set_sampler2D(name, *((sampler2D*)ptr_value));
	else if(type == "samplerCube") set_samplerCube(name, *((samplerCube*)ptr_value));
	else throw glass::TypeError("Uniform variable " + name + " is not an atom variable.");
}

void UBO::set_atom(const string& name, void* ptr_value)
{
	if(!(userData<UBO_Instance>()->block.contains(name)))
	{
		throw glass::KeyError(name + " is not a member of uniform block " + userData<UBO_Instance>()->block.name);
	}

	string type = userData<UBO_Instance>()->block[name].type;
	
	SET_ATOM_COMMON_CODE
	else throw glass::TypeError("Uniform block member " + name + " is not an atom variable.");
}

#define SET_PREPARE(TYPENAME) \
if(uniform_map.count(name) == 0)\
{\
	throw glass::KeyError("Uniform variable " + name + " is not defined in GLSL code!");\
}\
if(uniform_map[name].type != #TYPENAME)\
{\
	throw glass::TypeError(string("Cannot convert ") + #TYPENAME + " to " + uniform_map[name].type);\
}\
parent->use();

void Uniform::set_bool(const string& name, bool value)
{
	SET_PREPARE(bool);
	glUniform1i(uniform_map[name].location, (int)(value));
}

void Uniform::set_uint(const string& name, uint value)
{
	SET_PREPARE(uint);
	glUniform1ui(uniform_map[name].location, value);
}

void Uniform::set_int(const string& name, int value)
{
	SET_PREPARE(int);
	glUniform1i(uniform_map[name].location, value);
}

void Uniform::set_float(const string& name, float value)
{
	SET_PREPARE(float);
	glUniform1f(uniform_map[name].location, value);
}

void Uniform::set_double(const string& name, double value)
{
	SET_PREPARE(double);
	glUniform1f(uniform_map[name].location, (float)(value));
}

void Uniform::set_bvec2(const string& name, const bvec2& value)
{
	SET_PREPARE(bvec2);
	glUniform2i(uniform_map[name].location, (int)(value.x), (int)(value.y));
}

void Uniform::set_bvec3(const string& name, const bvec3& value)
{
	SET_PREPARE(bvec3);
	glUniform3i(uniform_map[name].location, (int)(value.x), (int)(value.y), (int)(value.z));
}

void Uniform::set_bvec4(const string& name, const bvec4& value)
{
	SET_PREPARE(bvec4);
	glUniform4i(uniform_map[name].location, (int)(value.x), (int)(value.y), (int)(value.z), (int)(value.w));
}

void Uniform::set_ivec2(const string& name, const ivec2& value)
{
	SET_PREPARE(ivec2);
	glUniform2i(uniform_map[name].location, value.x, value.y);
}

void Uniform::set_ivec3(const string& name, const ivec3& value)
{
	SET_PREPARE(ivec3);
	glUniform3i(uniform_map[name].location, value.x, value.y, value.z);
}

void Uniform::set_ivec4(const string& name, const ivec4& value)
{
	SET_PREPARE(ivec4);
	glUniform4i(uniform_map[name].location, value.x, value.y, value.z, value.w);
}

void Uniform::set_uvec2(const string& name, const uvec2& value)
{
	SET_PREPARE(uvec2);
	glUniform2ui(uniform_map[name].location, value.x, value.y);
}

void Uniform::set_uvec3(const string& name, const uvec3& value)
{
	SET_PREPARE(uvec3);
	glUniform3ui(uniform_map[name].location, value.x, value.y, value.z);
}

void Uniform::set_uvec4(const string& name, const uvec4& value)
{
	SET_PREPARE(uvec4);
	glUniform4ui(uniform_map[name].location, value.x, value.y, value.z, value.w);
}

void Uniform::set_vec2(const string& name, const vec2& value)
{
	SET_PREPARE(vec2);
	glUniform2f(uniform_map[name].location, value.x, value.y);
}

void Uniform::set_vec3(const string& name, const vec3& value)
{
	SET_PREPARE(vec3);
	glUniform3f(uniform_map[name].location, value.x, value.y, value.z);
}

void Uniform::set_vec4(const string& name, const vec4& value)
{
	SET_PREPARE(vec4);
	glUniform4f(uniform_map[name].location, value.x, value.y, value.z, value.w);
}

void Uniform::set_dvec2(const string& name, const dvec2& value)
{
	SET_PREPARE(dvec2);
	glUniform2f(uniform_map[name].location, (float)(value.x), (float)(value.y));
}

void Uniform::set_dvec3(const string& name, const dvec3& value)
{
	SET_PREPARE(dvec3);
	glUniform3f(uniform_map[name].location, (float)(value.x), (float)(value.y), (float)(value.z));
}

void Uniform::set_dvec4(const string& name, const dvec4& value)
{
	SET_PREPARE(dvec4);
	glUniform4f(uniform_map[name].location, (float)(value.x), (float)(value.y), (float)(value.z), (float)(value.w));
}

void Uniform::set_mat2(const string& name, mat2& value)
{
	SET_PREPARE(mat2);
	glUniformMatrix2fv(uniform_map[name].location, 1, GL_FALSE, value.data());
}

void Uniform::set_mat3(const string& name, mat3& value)
{
	SET_PREPARE(mat3);
	glUniformMatrix3fv(uniform_map[name].location, 1, GL_FALSE, value.data());
}

void Uniform::set_mat4(const string& name, mat4& value)
{
	SET_PREPARE(mat4);
	glUniformMatrix4fv(uniform_map[name].location, 1, GL_FALSE, value.data());
}

void Uniform::set_sampler2D(const string& name, sampler2D& value)
{
	if(uniform_map.count(name) == 0)
	{
		throw glass::KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "sampler2D")
	{
		throw glass::TypeError("Cannot use set_sampler2D to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}
	if(value.id() == 0)
	{
		return;
	}
	
	if(uniform_map[name].texture_unit == 0)
	{
		uniform_map[name].texture_unit = parent->available_texture_unit;
		parent->available_texture_unit++;
	}
	
	if(uniform_map[name].texture_unit >= sampler2D::max_units())
	{
		throw glass::RuntimeError("Used texture unit(" + str::str(uniform_map[name].texture_unit) + ") is over " + str::str(sampler2D::max_units() - 1) + " now.");
	}

	parent->use();
	glActiveTexture(GL_TEXTURE0 + uniform_map[name].texture_unit);
	value.bind();
	glUniform1i(uniform_map[name].location, uniform_map[name].texture_unit);
	glActiveTexture(GL_TEXTURE0);
	value.unbind();
}

void Uniform::set_sampler2D(const string& name, uint texture_id)
{
	if(uniform_map.count(name) == 0)
	{
		throw glass::KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "sampler2D")
	{
		throw glass::TypeError("Cannot use set_sampler2D to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}
	if(texture_id == 0)
	{
		return;
	}

	if(uniform_map[name].texture_unit == 0)
	{
		uniform_map[name].texture_unit = parent->available_texture_unit;
		parent->available_texture_unit++;
	}
	
	if(uniform_map[name].texture_unit >= sampler2D::max_units())
	{
		throw glass::RuntimeError("Used texture unit(" + str::str(uniform_map[name].texture_unit) + ") is over " + str::str(sampler2D::max_units() - 1) + " now.");
	}

	parent->use();
	glActiveTexture(GL_TEXTURE0 + uniform_map[name].texture_unit);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glUniform1i(uniform_map[name].location, uniform_map[name].texture_unit);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Uniform::set_samplerCube(const string& name, samplerCube& value)
{
	if(uniform_map.count(name) == 0)
	{
		throw glass::KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "samplerCube")
	{
		throw glass::TypeError("Cannot use set_samplerCube to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}
	if(value.id() == 0)
	{
		return;
	}

	if(uniform_map[name].texture_unit == 0)
	{
		uniform_map[name].texture_unit = parent->available_texture_unit;
		parent->available_texture_unit++;
	}
	
	if(uniform_map[name].texture_unit >= sampler2D::max_units())
	{
		throw glass::RuntimeError("Used texture unit(" + str::str(uniform_map[name].texture_unit) + ") is over " + str::str(sampler2D::max_units() - 1) + " now.");
	}
	
	parent->use();
	glActiveTexture(GL_TEXTURE0 + uniform_map[name].texture_unit);
	value.bind();
	glUniform1i(uniform_map[name].location, uniform_map[name].texture_unit);
	glActiveTexture(GL_TEXTURE0);
	value.unbind();
}

void Uniform::set_samplerCube(const string& name, uint texture_id)
{
	if(uniform_map.count(name) == 0)
	{
		throw glass::KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "samplerCube")
	{
		throw glass::TypeError("Cannot use set_samplerCube to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}
	if(texture_id == 0)
	{
		return;
	}

	if(uniform_map[name].texture_unit == 0)
	{
		uniform_map[name].texture_unit = parent->available_texture_unit;
		parent->available_texture_unit++;
	}
	
	if(uniform_map[name].texture_unit >= sampler2D::max_units())
	{
		throw glass::RuntimeError("Used texture unit(" + str::str(uniform_map[name].texture_unit) + ") is over " + str::str(sampler2D::max_units() - 1) + " now.");
	}

	parent->use();
	glActiveTexture(GL_TEXTURE0 + uniform_map[name].texture_unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
	glUniform1i(uniform_map[name].location, uniform_map[name].texture_unit);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

ostream& operator <<(ostream& out, const Uniform::Struct& structure)
{
	out << "struct " << structure.name << endl;
	out << "{" << endl;
	for(auto member = structure.members.begin(); member != structure.members.end(); member++)
	{
		out << "    " << member->type << " " << member->name << ";" << endl;
	}
	out << "};";

	return out;
}

Uniform::BlockMap::Reference Uniform::BlockMap::operator [](const std::string& name)
{
	return Uniform::BlockMap::Reference(parent, name);
}

Uniform::BlockMap::Reference::Reference(Uniform* _parent, const std::string& _name):
parent(_parent), name(_name)
{
	if(parent->uniform_block_map.count(_name) == 0)
	{
		throw glass::KeyError("Uniform Block " + _name + " is not defined in GLSL.");
	}
}

Uniform::Block* Uniform::BlockMap::Reference::instance()const
{
	return &(parent->uniform_block_map[name]);
}

void Uniform::BlockMap::Reference::bind(uint point)
{
	glUniformBlockBinding(parent->parent->id(), instance()->index, point);
	instance()->binding_point = point;
}

void Uniform::BlockMap::Reference::bind(UBO& ubo)
{
	// if(ubo.block != *this)
	// {
	// 	cout << "Warning: uniform block " << name << "'s structure is not same as " << ubo.block.name << endl;
	// }
	bind(ubo.bindingPoint());
}

void Uniform::BlockMap::Reference::unbind()
{
	bind(0);
}

bool Uniform::BlockMap::Reference::contains(const std::string& member_name)const
{
	return instance()->contains(member_name);
}

bool Uniform::BlockMap::Reference::empty()const
{
	return instance()->empty() || parent == NULL || parent->parent == NULL || parent->parent->id() == 0;
}

Uniform::Variable& Uniform::BlockMap::Reference::operator [](const std::string& member_name)
{
	return (*instance())[member_name];
}

const Uniform::Variable& Uniform::BlockMap::Reference::operator [](const std::string& member_name)const
{
	return (*instance())[member_name];
}

uint Uniform::BlockMap::Reference::size()const
{
	return instance()->size;
}

std::string Uniform::BlockMap::Reference::instance_name()const
{
	return instance()->instance_name;
}

int Uniform::BlockMap::Reference::index()const
{
	return instance()->index;
}

uint Uniform::BlockMap::Reference::binding_point()const
{
	return instance()->binding_point;
}
