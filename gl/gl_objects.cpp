#include "gl_objects.h"
#include "path.h"
#include "str.h"
#include "transform.hpp"
#include "model.h"
#include "image_process.h"

using namespace tran;

map<string, GLSL::Size> GLSL::built_in_types;
void GLSL::init()
{
	if(GLSL::built_in_types.empty())
	{
		GLSL::built_in_types["int"] = Size(sizeof(int), alignof(int), sizeof(int));
		GLSL::built_in_types["uint"] = Size(sizeof(uint), alignof(uint), sizeof(uint));
		GLSL::built_in_types["bool"] = Size(sizeof(bool), alignof(bool), sizeof(bool));
		GLSL::built_in_types["float"] = Size(sizeof(float), alignof(float), sizeof(float));
		GLSL::built_in_types["double"] = Size(sizeof(double), alignof(double), sizeof(double));

		GLSL::built_in_types["vec2"] = Size(sizeof(vec2), alignof(vec2), 2*sizeof(float));
		GLSL::built_in_types["vec3"] = Size(sizeof(vec3), alignof(vec3), 3*sizeof(float));
		GLSL::built_in_types["vec4"] = Size(sizeof(vec4), alignof(vec4), 4*sizeof(float));

		GLSL::built_in_types["ivec2"] = Size(sizeof(ivec2), alignof(ivec2), 2*sizeof(int));
		GLSL::built_in_types["ivec3"] = Size(sizeof(ivec3), alignof(ivec3), 3*sizeof(int));
		GLSL::built_in_types["ivec4"] = Size(sizeof(ivec4), alignof(ivec4), 4*sizeof(int));

		GLSL::built_in_types["uvec2"] = Size(sizeof(uvec2), alignof(uvec2), 2*sizeof(uint));
		GLSL::built_in_types["uvec3"] = Size(sizeof(uvec3), alignof(uvec3), 3*sizeof(uint));
		GLSL::built_in_types["uvec4"] = Size(sizeof(uvec4), alignof(uvec4), 4*sizeof(uint));

		GLSL::built_in_types["bvec2"] = Size(sizeof(bvec2), alignof(bvec2), 2*sizeof(bool));
		GLSL::built_in_types["bvec3"] = Size(sizeof(bvec3), alignof(bvec3), 3*sizeof(bool));
		GLSL::built_in_types["bvec4"] = Size(sizeof(bvec4), alignof(bvec4), 4*sizeof(bool));

		GLSL::built_in_types["dvec2"] = Size(sizeof(dvec2), alignof(dvec2), 2*sizeof(double));
		GLSL::built_in_types["dvec3"] = Size(sizeof(dvec3), alignof(dvec3), 3*sizeof(double));
		GLSL::built_in_types["dvec4"] = Size(sizeof(dvec4), alignof(dvec4), 4*sizeof(double));

		GLSL::built_in_types["mat2"] = Size(sizeof(mat2), alignof(mat2), 4*sizeof(float));
		GLSL::built_in_types["mat3"] = Size(sizeof(mat3), alignof(mat3), 9*sizeof(float));
		GLSL::built_in_types["mat4"] = Size(sizeof(mat4), alignof(mat4), 16*sizeof(float));
		GLSL::built_in_types["sampler2D"] = Size(sizeof(sampler2D), alignof(sampler2D), 0);
		GLSL::built_in_types["samplerCube"] = Size(sizeof(samplerCube), alignof(samplerCube), 0);
	}
}
GLSL::Constructor GLSL::constructor;

bool GLFW::is_glfw_init = false;
void GLFW::init()
{
	if(!GLFW::is_glfw_init)
	{
		glfwInit();
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAC, GL_TRUE);
	#endif
	    GLFW::is_glfw_init = true;
	}
}
bool GLFW::is_init()
{
	return GLFW::is_glfw_init;
}

bool GLAD::is_glad_init = false;
void GLAD::init()
{
	if(!GLAD::is_glad_init)
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			throw Exception("Failed to initialize GLAD");
		}
		GLAD::is_glad_init = true;
	}
}
bool GLAD::is_init()
{
	return GLAD::is_glad_init;
}

multiset<uint> VBO::existing_VBOs;
uint VBO::active_VBO = 0;

template<class DataType>
void multiset_pop(multiset<DataType>& mset, DataType value)
{
	auto it = mset.find(value);
	if(it != mset.end())
	{
		mset.erase(it);
	}
}

void VBO::init()
{
	if(_id == 0)
	{
		glGenBuffers(1, &_id);
	#ifdef _DEBUG
		cout << "constructing VBO " << _id << endl;
	#endif
		if(_id == 0)
		{
			throw MemoryError("Failed to create VBO!");
		}
		existing_VBOs.insert(_id);
	}
	else if(existing_VBOs.count(_id) == 0)
	{
		throw RuntimeError("VBO " + str::str(_id) + " has already been destructed.");
	}
}

VBO::VBO() {}

VBO::VBO(const VBO& vbo) :
_id(vbo._id),
_size(vbo._size)
{
	if(existing_VBOs.count(_id) > 0)
	{
		existing_VBOs.insert(_id);
	}
}

VBO::VBO(VBO&& vbo) :
_id(move(vbo._id)),
_size(move(vbo._size))
{
	vbo._id = 0;
	vbo._size = 0;
}

VBO& VBO::operator =(const VBO& vbo)
{
	if(this != &vbo)
	{
		_id = vbo._id;
		_size = vbo._size;

		if(existing_VBOs.count(_id) > 0)
		{
			existing_VBOs.insert(_id);
		}
	}
	return *this;
}

VBO& VBO::operator =(VBO&& vbo)
{
	if(this != &vbo)
	{
		_id = move(vbo._id);
		_size = move(vbo._size);
		vbo._id = 0;
		vbo._size = 0;
	}
	return *this;
}

bool VBO::is_bind()const
{
	return (existing_VBOs.count(_id) > 0 && active_VBO == _id);
}

VBO::~VBO()
{
	unbind();
	uint count = existing_VBOs.count(_id);
	if(count != 0)
	{
		multiset_pop(existing_VBOs, _id);
		if(count == 1)
		{
		#ifdef _DEBUG
			cout << "destructing VBO " << _id << endl;
		#endif
			glDeleteBuffers(1, &_id);
		}
	}
	_id = 0;
	_size = 0;
}

void VBO::clear()
{
	if(empty())
	{
		return;
	}

	unbind();
	existing_VBOs.erase(_id);
	#ifdef _DEBUG
		cout << "destructing VBO " << _id << endl;
	#endif
	glDeleteBuffers(1, &_id);
	_id = 0;
	_size = 0;
}

void VBO::bind()
{
	init();
	glBindBuffer(GL_ARRAY_BUFFER, _id);
	active_VBO = _id;
}

void VBO::unbind()const
{
	if(active_VBO == _id)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		active_VBO = 0;
	}
}

void VBO::bufferData(void* ptr_value, uint n_bytes)
{
	if(n_bytes == 0)
	{
		return;
	}

	bind();
	glBufferData(GL_ARRAY_BUFFER, n_bytes, ptr_value, GL_STATIC_DRAW);
	_size = n_bytes;
}

bool VBO::empty()const
{
	return (existing_VBOs.count(_id) == 0 || _size == 0);
}

uint VBO::size()const
{
	return _size;
}

uint VBO::id()const
{
	return _id;
}

void VBO::malloc(uint n_bytes)
{
	if(n_bytes == 0)
	{
		return;
	}

	bind();
	glBufferData(GL_ARRAY_BUFFER, n_bytes, NULL, GL_STATIC_DRAW);
	_size = n_bytes;
}

void* VBO::ptr()
{
	if(_size == 0)
	{
		return NULL;
	}
	
	bind();
	return glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
}

bool VBO::apply()
{
	bind();
	return glUnmapBuffer(GL_ARRAY_BUFFER);
}

VBO VBO::clone()
{
	VBO vbo;
	if(empty())
	{
		return vbo;
	}

	vbo.malloc(_size);
	bind();
	glBindBuffer(GL_COPY_WRITE_BUFFER, vbo._id);
	glCopyBufferSubData(GL_ARRAY_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, _size);
	return vbo;
}

VertexAttribPointer::VertexAttribPointer(uint _location, uint _vertex_size, uint _dtype, uint _should_normalize, uint _stride, uint _offset) : 
location(_location),
vertex_size(_vertex_size),
dtype(_dtype),
should_normalize(_should_normalize),
stride(_stride),
offset(_offset) {}

void VertexAttribPointer::interp(const VBO& _vbo)
{
	if(vertex_size == 0)
	{
		throw Exception("Empty attrib for VBO!");
	}

	vbo = _vbo;
	vbo.bind();
	glVertexAttribPointer(location, 
		                  vertex_size, 
		                  dtype, 
		                  should_normalize,
		                  stride,
		                  (void*)((byte*)NULL + offset));
	glEnableVertexAttribArray(location);
}

VertexAttribPointer& VertexAttribPointer::operator ()(uint _vertex_size, uint _dtype, uint _should_normalize, uint _stride, uint _offset)
{
	vertex_size = _vertex_size;
	dtype = _dtype;
	should_normalize = _should_normalize;
	stride = _stride;
	offset = _offset;
	return *this;
}

bool VertexAttribPointer::empty()const
{
	return (vertex_size == 0 || vbo.empty());
}

VertexAttribPointer& VAO::operator [](uint location)
{
	bind();
	if(!(map_VAP.count(location)))
	{
		map_VAP[location] = VertexAttribPointer(location, 0, GL_FLOAT, GL_FALSE, 0, 0);
	}
	return map_VAP[location];
}

multiset<uint> VAO::existing_VAOs;
uint VAO::active_VAO = 0;

void VAO::init()
{
	if(_id == 0)
	{
		glGenVertexArrays(1, &_id);
		if(_id == 0)
		{
			throw MemoryError("Failed to create VAO!");
		}
	#ifdef _DEBUG
		cout << "constructing VAO " << _id << endl;
	#endif
		existing_VAOs.insert(_id);
	}
	else if(existing_VAOs.count(_id) == 0)
	{
		throw RuntimeError("VAO " + str::str(_id) + " has been destructed.");
	}
}

VAO::VAO() {}

VAO::VAO(const VAO& vao) :
_id(vao._id), _ebo(vao._ebo), map_VAP(vao.map_VAP)
{
	if(existing_VAOs.count(_id) > 0)
	{
		existing_VAOs.insert(_id);
	}
}

VAO::VAO(VAO&& vao) :
_id(move(vao._id)), _ebo(move(vao._ebo)), map_VAP(move(vao.map_VAP))
{	
	vao._id = 0;
}

VAO& VAO::operator =(const VAO& vao)
{
	if(this != &vao)
	{
		_id = vao._id;
		_ebo = vao._ebo;
		map_VAP = vao.map_VAP;

		if(existing_VAOs.count(_id) > 0)
		{
			existing_VAOs.insert(_id);
		}
	}
	return *this;
}

VAO& VAO::operator =(VAO&& vao)
{
	if(this != &vao)
	{
		_id = move(vao._id);
		_ebo = move(vao._ebo);
		map_VAP = move(vao.map_VAP);
		
		vao._id = 0;
	}
	return *this;
}

VAO::~VAO()
{
	unbind();
	uint count = existing_VAOs.count(_id);
	if(existing_VAOs.count(_id) != 0)
	{
		multiset_pop(existing_VAOs, _id);
		if(count == 1)
		{
		#ifdef _DEBUG
			cout << "destructing VAO " << _id << endl;
		#endif
			glDeleteVertexArrays(1, &_id);
		}
	}
	_id = 0;
}

void VAO::bind()
{
	init();
	glBindVertexArray(_id);
	active_VAO = _id;
}

void VAO::unbind()const
{
	if(active_VAO == _id)
	{
		glBindVertexArray(0);
		active_VAO = 0;
	}
}

void VAO::addEBO(const EBO& __ebo)
{
	_ebo = __ebo;

	bind();
	_ebo.bind();
}

bool VAO::empty()const
{
	return (existing_VAOs.count(_id) == 0 || map_VAP.empty());
}

void VAO::draw()
{
	if(empty())
	{
		return;
	}

	bind();
	if(_ebo.empty())
	{
		throw RuntimeError("Unknown vertices number.\nPlease specify vertices number by using VAO::draw(uint n).\nOr add EBO to current VAO by using VAO::addEBO(EBO ebo).");
	}
	else
	{
		glDrawElements(GL_TRIANGLES, _ebo.size(), GL_UNSIGNED_INT, 0);
	}
}

void VAO::draw(uint n)
{
	if(empty() || n == 0)
	{
		return;
	}

	bind();
	if(_ebo.empty())
	{
		glDrawArrays(GL_TRIANGLES, 0, n);
	}
	else
	{
		glDrawElements(GL_TRIANGLES, std::min(n, _ebo.size()), GL_UNSIGNED_INT, 0);
	}
}

multiset<uint> EBO::existing_EBOs;
uint EBO::active_EBO;

void EBO::init()
{
	if(_id == 0)
	{
		glGenBuffers(1, &_id);
		if(_id == 0)
		{
			throw MemoryError("Failed to create EBO!");
		}
	#ifdef _DEBUG
		cout << "constructing EBO " << _id << endl;
	#endif
		existing_EBOs.insert(_id);
	}
	else if(existing_EBOs.count(_id) == 0)
	{
		throw RuntimeError("EBO " + str::str(_id) + " has already been destructed.");
	}
}

EBO::EBO() {}

EBO::EBO(const EBO& ebo) :
_id(ebo._id), _size(ebo._size)
{
	if(existing_EBOs.count(_id) > 0)
	{
		existing_EBOs.insert(_id);
	}
}

EBO::EBO(EBO&& ebo) :
_id(move(ebo._id)), _size(move(ebo._size))
{
	ebo._id = 0;
	ebo._size = 0;
}

EBO& EBO::operator =(const EBO& ebo)
{
	if(this != &ebo)
	{
		_id = ebo._id;
		_size = ebo._size;

		if(existing_EBOs.count(_id) > 0)
		{
			existing_EBOs.insert(_id);
		}
	}
	return *this;
}

EBO& EBO::operator =(EBO&& ebo)
{
	if(this != &ebo)
	{
		_id = move(ebo._id);
		_size = move(ebo._size);

		ebo._id = 0;
		ebo._size = 0;
	}
	return *this;
}

EBO::~EBO()
{
	unbind();
	uint count = existing_EBOs.count(_id);
	if(existing_EBOs.count(_id) != 0)
	{
		multiset_pop(existing_EBOs, _id);
		if(count == 1)
		{
		#ifdef _DEBUG
			cout << "destructing EBO " << _id << endl;
		#endif
			// cout << "destructing EBO " << _id << endl;
			glDeleteBuffers(1, &_id);
		}
	}
	_id = 0;
	_size = 0;
}

uint EBO::id()const
{
	return _id;
}

void EBO::bind()
{
	init();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
	active_EBO = _id;
}

void EBO::unbind()const
{
	if(active_EBO == _id)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		active_EBO = 0;
	}
}

void EBO::bufferData(void* ptr_value, uint n_bytes)
{
	clear();
	if(n_bytes == 0)
	{
		return;
	}

	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_bytes, ptr_value, GL_STATIC_DRAW);
	_size = n_bytes/sizeof(uint);
}

bool EBO::bufferData(const vector<uint>& indices)
{
	clear();
	if(indices.size() == 0)
	{
		return false;
	}
	if(indices.size() % 3 != 0)
	{
		throw RuntimeError("EBO's buffer data must can be divided by 3.");
	}

	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint), NULL, GL_STATIC_DRAW);
	void* ptr_start = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(ptr_start, (void*)(&(indices[0])), indices.size() * sizeof(uint));
	bool success = glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	unbind();

	_size = indices.size();

	return success;
}

bool EBO::bufferData(const vector<uvec3>& indices)
{
	clear();
	if(indices.size() == 0)
	{
		return false;
	}

	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * GLSL::built_in_types["uvec3"].glsl_size, NULL, GL_STATIC_DRAW);
	void* ptr_start = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	byte* ptr = (byte*)ptr_start;
	for(int i = 0; i < indices.size(); i++)
	{
		memcpy((void*)ptr, (void*)(&(indices[i])), GLSL::built_in_types["uvec3"].glsl_size);
		ptr += GLSL::built_in_types["uvec3"].glsl_size;
	}
	bool success = glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	unbind();

	_size = 3*indices.size();

	return success;
}

bool EBO::empty()const
{
	return (existing_EBOs.count(_id) == 0 || _size == 0);
}

uint EBO::size()const
{
	return _size;
}

bool EBO::is_bind()const
{
	return (existing_EBOs.count(_id) > 0 && active_EBO == _id);
}

void EBO::clear()
{
	if(empty())
	{
		return;
	}

	unbind();
	existing_EBOs.erase(_id);
	#ifdef _DEBUG
		cout << "destructing EBO " << _id << endl;
	#endif
	glDeleteBuffers(1, &_id);
	_id = 0;
	_size = 0;
}

void EBO::malloc(uint n_bytes)
{
	clear();
	if(n_bytes == 0)
	{
		return;
	}

	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_bytes, NULL, GL_STATIC_DRAW);
	_size = n_bytes/sizeof(uint);
}

void* EBO::ptr()
{
	if(empty())
	{
		return NULL;
	}

	bind();
	return glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
}

bool EBO::apply()
{
	bind();
	return glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

EBO EBO::clone()
{
	EBO ebo;
	if(empty())
	{
		return ebo;
	}

	ebo.malloc(_size * sizeof(uint));
	glBindBuffer(GL_COPY_WRITE_BUFFER, ebo._id);

	bind();
	glCopyBufferSubData(GL_ARRAY_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, _size * sizeof(uint));
	
	return ebo;
}

multiset<uint> RBO::existing_RBOs;
uint RBO::active_RBO;

void RBO::init()
{
	if(_id == 0)
	{
		glGenRenderbuffers(1, &_id);
		if(_id == 0)
		{
			throw RuntimeError("Failed to create RBO!");
		}
	#ifdef _DEBUG
		cout << "constructing RBO " << _id << endl;
	#endif
		existing_RBOs.insert(_id);
	}
	else if(existing_RBOs.count(_id) == 0)
	{
		throw RuntimeError("RBO " + str::str(_id) + " has already been destructed!");
	}
}

RBO::RBO(uint __width, uint __height, uint __dtype)
{
	malloc(__width, __height, __dtype);
}

RBO::RBO() {}

RBO::RBO(const RBO& rbo) :
_id(rbo._id), _width(rbo._width), _height(rbo._height), _dtype(rbo._dtype)
{
	if(existing_RBOs.count(_id) > 0)
	{
		existing_RBOs.insert(_id);
	}
}

RBO::RBO(RBO&& rbo) :
_id(move(rbo._id)), _width(move(rbo._width)), _height(move(rbo._height)), _dtype(move(rbo._dtype))
{
	rbo._id = 0;
}

RBO::~RBO()
{
	unbind();
	uint count = existing_RBOs.count(_id);
	if(count > 0)
	{
		multiset_pop(existing_RBOs, _id);
		if(count == 1)
		{
		#ifdef _DEBUG
			cout << "destructing RBO " << _id << endl;
		#endif
			glDeleteRenderbuffers(1, &_id);
		}
	}
	_id = 0;
}

RBO& RBO::operator =(const RBO& rbo)
{
	if(&rbo != this)
	{
		_id = rbo._id;
		_width = rbo._width;
		_height = rbo._height;
		_dtype = rbo._dtype;

		if(existing_RBOs.count(_id) > 0)
		{
			existing_RBOs.insert(_id);
		}
	}
	return *this;
}

RBO& RBO::operator =(RBO&& rbo)
{
	if(&rbo != this)
	{
		_id = move(rbo._id);
		_width = move(rbo._width);
		_height = move(rbo._height);
		_dtype = move(rbo._dtype);

		rbo._id = 0;
	}
	return *this;
}

uint RBO::id()const
{
	return _id;
}

uint RBO::width()const
{
	return _width;
}

uint RBO::height()const
{
	return _height;
}

uint RBO::dtype()const
{
	return _dtype;
}

void RBO::bind()
{
	init();
	glBindRenderbuffer(GL_RENDERBUFFER, _id);
	active_RBO = _id;
}

void RBO::unbind()
{
	if(active_RBO == _id)
	{
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		active_RBO = 0;
	}
}

void RBO::clear()
{
	unbind();
	if(existing_RBOs.count(_id) > 0)
	{
		existing_RBOs.erase(_id);
	#ifdef _DEBUG
		cout << "destructing RBO " << _id << endl;
	#endif
		glDeleteRenderbuffers(1, &_id);
	}
	_id = 0;
	_width = 0;
	_height = 0;
	_dtype = GL_DEPTH24_STENCIL8;
}

void RBO::malloc(uint __width, uint __height, uint __dtype)
{
	if(__width == _width && __height == _height && __dtype == _dtype)
	{
		return;
	}

	clear();
	bind();
	glRenderbufferStorage(GL_RENDERBUFFER, __dtype, __width, __height);
	_width = __width;
	_height = __height;
	_dtype = __dtype;
}

multiset<uint> FBO::existing_FBOs;
uint FBO::active_FBO;

void FBO::init()
{
	if(_id == 0)
	{
		glGenFramebuffers(1, &_id);
		if(_id == 0)
		{
			throw RuntimeError("Failed to create FBO!");
		}
	#ifdef _DEBUG
		cout << "constructing FBO " << _id << endl;
	#endif
		existing_FBOs.insert(_id);
	}
	else if(existing_FBOs.count(_id) == 0)
	{
		throw RuntimeError("FBO " + str::str(_id) + " has already been destructed.");
	}
}

FBO::FBO(uint __screen_width, uint __screen_height, uint __buffer_width, uint __buffer_height) :
_screen_width(__screen_width), _screen_height(__screen_height), _buffer_width(__buffer_width), _buffer_height(__buffer_height)
{}

FBO::FBO(const FBO& fbo) :
_id(fbo._id),
_screen_width(fbo._screen_width),
_screen_height(fbo._screen_height),
_buffer_width(fbo._buffer_width),
_buffer_height(fbo._buffer_height),
_color_RBO(fbo._color_RBO),
_depth_RBO(fbo._depth_RBO),
_stencil_RBO(fbo._stencil_RBO),
_color_sampler(fbo._color_sampler),
_depth_sampler(fbo._depth_sampler),
_stencil_sampler(fbo._stencil_sampler),
is_color_RBO_enabled(fbo.is_color_RBO_enabled),
is_depth_RBO_enabled(fbo.is_depth_RBO_enabled),
is_stencil_RBO_enabled(fbo.is_stencil_RBO_enabled),
is_color_sampler_enabled(fbo.is_color_sampler_enabled),
is_depth_sampler_enabled(fbo.is_depth_sampler_enabled),
is_stencil_sampler_enabled(fbo.is_stencil_sampler_enabled),
is_applied(fbo.is_applied),
_auto_clear(fbo._auto_clear),
_auto_resize(fbo._auto_resize)
{
	if(existing_FBOs.count(_id))
	{
		existing_FBOs.insert(_id);
	}
}

FBO::FBO(FBO&& fbo) :
_id(move(fbo._id)),
_screen_width(move(fbo._screen_width)),
_screen_height(move(fbo._screen_height)),
_buffer_width(move(fbo._buffer_width)),
_buffer_height(move(fbo._buffer_height)),
_color_RBO(move(fbo._color_RBO)),
_depth_RBO(move(fbo._depth_RBO)),
_stencil_RBO(move(fbo._stencil_RBO)),
_color_sampler(move(fbo._color_sampler)),
_depth_sampler(move(fbo._depth_sampler)),
_stencil_sampler(move(fbo._stencil_sampler)),
is_color_RBO_enabled(move(fbo.is_color_RBO_enabled)),
is_depth_RBO_enabled(move(fbo.is_depth_RBO_enabled)),
is_stencil_RBO_enabled(move(fbo.is_stencil_RBO_enabled)),
is_color_sampler_enabled(move(fbo.is_color_sampler_enabled)),
is_depth_sampler_enabled(move(fbo.is_depth_sampler_enabled)),
is_stencil_sampler_enabled(move(fbo.is_stencil_sampler_enabled)),
is_applied(move(fbo.is_applied)),
_auto_clear(move(fbo._auto_clear)),
_auto_resize(move(fbo._auto_resize))
{
	fbo._id = 0;
}

FBO::~FBO()
{
	unbind();
	uint count = existing_FBOs.count(_id);
	if(count > 0)
	{
		multiset_pop(existing_FBOs, _id);
		if(count == 1)
		{
		#ifdef _DEBUG
			cout << "destructing FBO " << _id << endl;
		#endif
			glDeleteFramebuffers(1, &_id);
		}
	}
	_id = 0;
}

FBO& FBO::operator =(const FBO& fbo)
{
	if(this != &fbo)
	{
		_id = fbo._id;
		_screen_width = fbo._screen_width;
		_screen_height = fbo._screen_height;
		_buffer_width = fbo._buffer_width;
		_buffer_height = fbo._buffer_height;
		_color_RBO = fbo._color_RBO;
		_depth_RBO = fbo._depth_RBO;
		_stencil_RBO = fbo._stencil_RBO;
		_color_sampler = fbo._color_sampler;
		_depth_sampler = fbo._depth_sampler;
		_stencil_sampler = fbo._stencil_sampler;
		is_color_RBO_enabled = fbo.is_color_RBO_enabled;
		is_depth_RBO_enabled = fbo.is_depth_RBO_enabled;
		is_stencil_RBO_enabled = fbo.is_stencil_RBO_enabled;
		is_color_sampler_enabled = fbo.is_color_sampler_enabled;
		is_depth_sampler_enabled = fbo.is_depth_sampler_enabled;
		is_stencil_sampler_enabled = fbo.is_stencil_sampler_enabled;
		is_applied = fbo.is_applied;
		_auto_clear = fbo._auto_clear;
		_auto_resize = fbo._auto_resize;

		if(existing_FBOs.count(_id) > 0)
		{
			existing_FBOs.insert(_id);
		}
	}
	return *this;
}

FBO& FBO::operator =(FBO&& fbo)
{
	if(this != &fbo)
	{
		_id = move(fbo._id);
		_screen_width = move(fbo._screen_width);
		_screen_height = move(fbo._screen_height);
		_buffer_width = move(fbo._buffer_width);
		_buffer_height = move(fbo._buffer_height);
		_color_RBO = move(fbo._color_RBO);
		_depth_RBO = move(fbo._depth_RBO);
		_stencil_RBO = move(fbo._stencil_RBO);
		_color_sampler = move(fbo._color_sampler);
		_depth_sampler = move(fbo._depth_sampler);
		_stencil_sampler = move(fbo._stencil_sampler);
		is_color_RBO_enabled = move(fbo.is_color_RBO_enabled);
		is_depth_RBO_enabled = move(fbo.is_depth_RBO_enabled);
		is_stencil_RBO_enabled = move(fbo.is_stencil_RBO_enabled);
		is_color_sampler_enabled = move(fbo.is_color_sampler_enabled);
		is_depth_sampler_enabled = move(fbo.is_depth_sampler_enabled);
		is_stencil_sampler_enabled = move(fbo.is_stencil_sampler_enabled);
		is_applied = move(fbo.is_applied);
		_auto_clear = move(fbo._auto_clear);
		_auto_resize = move(fbo._auto_resize);

		fbo._id = 0;
	}
	return *this;
}

uint FBO::id()const
{
	return _id;
}

uint FBO::screen_width()const
{
	return _screen_width;
}

uint FBO::screen_height()const
{
	return _screen_height;
}

uint FBO::buffer_width()const
{
	return _buffer_width;
}

uint FBO::buffer_height()const
{
	return _buffer_height;
}

void FBO::screen_size(uint __screen_width, uint __screen_height)
{
	_screen_width = __screen_width;
	_screen_height = __screen_height;
}

void FBO::_bind()
{
	init();
	glBindFramebuffer(GL_FRAMEBUFFER, _id);
	active_FBO = _id;
}

void FBO::bind()
{
	apply();
	// glCullFace(GL_FRONT);
	if(_auto_resize)
	{
		glViewport(0, 0, _buffer_width, _buffer_height);
	}
	if(_auto_clear)
	{
		clear();
	}
}

void FBO::_unbind()
{
	if(active_FBO == _id)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		active_FBO = 0;
	}
}

void FBO::unbind()
{
	_unbind();
	// glCullFace(GL_BACK);
	if(_auto_resize)
	{
		glViewport(0, 0, _screen_width, _screen_height);
	}
	if(_auto_clear)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

void FBO::auto_clear(bool flag)
{
	_auto_clear = flag;
}

void FBO::auto_resize(bool flag)
{
	_auto_resize = flag;
}

uint FBO::status()
{
	if(_id == 0)
	{
		return 0;
	}

	bind();
	return glCheckFramebufferStatus(GL_FRAMEBUFFER);
}

bool FBO::completed()
{
	if(_id == 0)
	{
		return false;
	}

	bind();
	return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

void FBO::clear()
{
	if(_id == 0)
	{
		return;
	}

	uint value = 0;
	if(is_color_sampler_enabled || is_color_RBO_enabled)
	{
		value |= GL_COLOR_BUFFER_BIT;
	}

	if(is_depth_sampler_enabled || is_depth_RBO_enabled)
	{
		value |= GL_DEPTH_BUFFER_BIT;
	}

	if(is_stencil_sampler_enabled || is_stencil_RBO_enabled)
	{
		value |= GL_STENCIL_BUFFER_BIT;
	}

	if(value == 0)
	{
		return;
	}

	_bind();
	glClear(value);
}

void FBO::enable_color_sampler()
{
	if(is_color_sampler_enabled)
	{
		return;
	}

	if(is_applied)
	{
		throw RuntimeError("Canot enable new buffer of FBO after first bind.");
	}

	if(is_color_RBO_enabled)
	{
		cout << "Warning: Disable color RBO because enabled color sampler." << endl;
		is_color_RBO_enabled = false;
	}

	is_color_sampler_enabled = true;
}

void FBO::enable_depth_sampler()
{
	if(is_depth_sampler_enabled)
	{
		return;
	}

	if(is_applied)
	{
		throw RuntimeError("Canot enable new buffer of FBO after first bind.");
	}

	if(is_depth_RBO_enabled)
	{
		cout << "Warning: Disable depth RBO because enabled depth sampler." << endl;
		is_depth_RBO_enabled = false;
	}

	is_depth_sampler_enabled = true;
}

void FBO::enable_stencil_sampler()
{
	if(is_stencil_sampler_enabled)
	{
		return;
	}

	if(is_applied)
	{
		throw RuntimeError("Canot enable new buffer of FBO after first bind.");
	}

	if(is_stencil_RBO_enabled)
	{
		cout << "Warning: Disable stencil RBO because enabled stencil sampler." << endl;
		is_stencil_RBO_enabled = false;
	}

	is_stencil_sampler_enabled = true;
}

void FBO::enable_color_RBO()
{
	if(is_color_RBO_enabled)
	{
		return;
	}

	if(is_applied)
	{
		throw RuntimeError("Canot enable new buffer of FBO after first bind.");
	}

	if(is_color_sampler_enabled)
	{
		cout << "Warning: Disable color sampler because enabled color RBO." << endl;
		is_color_sampler_enabled = false;
	}

	is_color_RBO_enabled = true;
}

void FBO::enable_depth_RBO()
{
	if(is_depth_RBO_enabled)
	{
		return;
	}

	if(is_applied)
	{
		throw RuntimeError("Canot enable new buffer of FBO after first bind.");
	}

	if(is_depth_sampler_enabled)
	{
		cout << "Warning: Disable depth sampler because enabled depth RBO." << endl;
		is_depth_sampler_enabled = false;
	}

	is_depth_RBO_enabled = true;
}

void FBO::enable_stencil_RBO()
{
	if(is_stencil_RBO_enabled)
	{
		return;
	}

	if(is_applied)
	{
		throw RuntimeError("Canot enable new buffer of FBO after first bind.");
	}

	if(is_stencil_sampler_enabled)
	{
		cout << "Warning: Disable stencil sampler because enabled stencil RBO." << endl;
		is_stencil_sampler_enabled = false;
	}

	is_stencil_RBO_enabled = true;
}

void FBO::apply()
{
	if(is_applied == true)
	{
		return;
	}

	if(is_color_RBO_enabled == false &&
	   is_depth_RBO_enabled == false && 
	   is_stencil_RBO_enabled == false &&
	   is_color_sampler_enabled == false &&
	   is_depth_sampler_enabled == false &&
	   is_stencil_sampler_enabled == false)
	{
		throw RuntimeError("FBO is incompleted!");
	}

	// glEnable(GL_CULL_FACE);
	_bind();
	if(is_color_sampler_enabled)
	{
		_color_sampler.malloc(_buffer_width, _buffer_height, GL_RGB, GL_UNSIGNED_BYTE);
		_color_sampler.filter(GL_LINEAR);
		_color_sampler.border_color(vec3(0,0,0));
		_color_sampler.wrap(GL_CLAMP_TO_BORDER);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _color_sampler.id(), 0);
	}
	else if(is_color_RBO_enabled)
	{
		_color_RBO.malloc(_buffer_width, _buffer_height, GL_RGB5);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _color_RBO.id());
	}

	if(is_depth_sampler_enabled)
	{
		_depth_sampler.malloc(_buffer_width, _buffer_height, GL_DEPTH_COMPONENT, GL_FLOAT);
		_depth_sampler.filter(GL_LINEAR);
		_depth_sampler.border_color(vec3(1,1,1));
		_depth_sampler.wrap(GL_CLAMP_TO_BORDER);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth_sampler.id(), 0);
	}
	else if(is_depth_RBO_enabled)
	{
		_depth_RBO.malloc(_buffer_width, _buffer_height, GL_DEPTH_COMPONENT16);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depth_RBO.id());
	}

	if(is_stencil_sampler_enabled)
	{
		_stencil_sampler.malloc(_buffer_width, _buffer_height, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE);
		_stencil_sampler.filter(GL_LINEAR);
		_stencil_sampler.border_color(vec3(1,1,1));
		_stencil_sampler.wrap(GL_CLAMP_TO_BORDER);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, _stencil_sampler.id(), 0);
	}
	else if(is_stencil_RBO_enabled)
	{
		_stencil_RBO.malloc(_buffer_width, _buffer_height, GL_STENCIL_INDEX8);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _stencil_RBO.id());
	}

	if(!is_color_sampler_enabled && !is_color_RBO_enabled)
	{
		glDrawBuffer(GL_NONE);
	    glReadBuffer(GL_NONE);
	}
	
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		_unbind();
		throw RuntimeError("FBO is incompleted.");
	}

	is_applied = true;
}

sampler2D& FBO::color_sampler()
{
	if(!is_applied)
	{
		throw RuntimeError("Cannot access FBO's color sampler before first bind.");
	}
	if(!is_color_sampler_enabled)
	{
		throw RuntimeError("color sampler is not enabled.");
	}

	return _color_sampler;
}

sampler2D& FBO::depth_sampler()
{
	if(!is_applied)
	{
		throw RuntimeError("Cannot access FBO's depth sampler before first bind.");
	}
	if(!is_depth_sampler_enabled)
	{
		throw RuntimeError("depth sampler is not enabled.");
	}

	return _depth_sampler;
}

sampler2D& FBO::stencil_sampler()
{
	if(!is_applied)
	{
		throw RuntimeError("Cannot access FBO's stencil sampler before first bind.");
	}
	if(!is_stencil_sampler_enabled)
	{
		throw RuntimeError("stencil sampler is not enabled.");
	}

	return _stencil_sampler;
}

RBO& FBO::color_RBO()
{
	if(!is_applied)
	{
		throw RuntimeError("Cannot access FBO's color RBO before first bind.");
	}
	if(!is_color_RBO_enabled)
	{
		throw RuntimeError("color RBO is not enabled.");
	}

	return _color_RBO;
}

RBO& FBO::depth_RBO()
{
	if(!is_applied)
	{
		throw RuntimeError("Cannot access FBO's depth RBO before first bind.");
	}
	if(!is_depth_RBO_enabled)
	{
		throw RuntimeError("depth RBO is not enabled.");
	}

	return _depth_RBO;
}

RBO& FBO::stencil_RBO()
{
	if(!is_applied)
	{
		throw RuntimeError("Cannot access FBO's stencil RBO before first bind.");
	}
	if(!is_stencil_RBO_enabled)
	{
		throw RuntimeError("stencil RBO is not enabled.");
	}

	return _stencil_RBO;
}

// Image FBO::image()
// {
//     glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
//     glReadBuffer(GL_COLOR_ATTACHMENT0);
//     glPixelStorei(GL_PACK_ALIGNMENT, 1);
// 	Image image(_screen_height, _screen_width, 3);
// 	glReadPixels(0, 0, _screen_width, _screen_height, color_buffer.sampler_buffer.format(), color_buffer.sampler_buffer.dtype(), image.data());
// 	return img::flip_y(image);
// }

multiset<uint> BaseShader::existing_BaseShaders;
void BaseShader::init()
{
	if(_id == 0)
	{
		_id = glCreateShader(_type);
		if(_id == 0)
		{
			throw RuntimeError("Failed to create BaseShader.");
		}
	#ifdef _DEBUG
		cout << "constructing BaseShader " << _id << endl;
	#endif
		existing_BaseShaders.insert(_id);
	}
	else if(existing_BaseShaders.count(_id) == 0)
	{
		throw RuntimeError("BaseShader " + str::str(_id) + " has already been destructed.");
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
	_id = 0;
}

BaseShader& BaseShader::operator =(const BaseShader& base_shader)
{
	if(this != &base_shader)
	{
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
	return _code;
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
	int pos_start = 0;
	int pos_end = 0;
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

		for(int i = pos_start; i < pos_end; i++)
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
		for(int i = pos_start; i < pos_end; i++)
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
	for(int i = 1; i <= n_lines; i++)
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
			if(pos_include_start == string::npos)
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
			throw SyntaxError(line_map[str::line_number(_code, pos_filename_start)] + "#include file name must be enveloped by <...> or \"...\"");
		}
		char end_char = _code[pos_filename_start] == '<' ? '>' : '\"';
		pos_filename_start++;
		str::skip_space(_code, pos_filename_start);
		pos_filename_end = _code.find(end_char, pos_filename_start);
		if(pos_filename_end == string::npos)
		{
			throw SyntaxError(line_map[str::line_number(_code, pos_filename_start)] + "#include file name must be enveloped by <...> or \"...\"");
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
				for(int i = include_line_end; i <= n_lines; i++)
				{
					line_map[i] = old_line_map[i - include_lines + 1];
				}
				for(int i = include_line_number; i < include_line_end; i++)
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
			throw SyntaxError(line_map[str::line_number(_code, pos_filename_start)] + "File \"" + filename + "\" is not exist.");
		}
	}
}

void BaseShader::format_error(string& info)
{
	int pos = 0;
	while(true)
	{
		int pos_error = info.find("ERROR: ", pos);
		if(pos_error == string::npos)
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
		throw IOError("File \"" + filename + "\" is not exist.");
	}

	_filename = path::relpath(filename);

	add_include_path(".");
	add_include_path(path::dirname(path::abspath(filename)));

	_code = path::cat(filename);
	replace_includes();

	char* value = new char[(_code.size()+1)*sizeof(char)];
	strcpy(value, _code.c_str());

	glShaderSource(_id, 1, &(value), NULL);
	delete value;
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
	    throw SyntaxError(info);
	}
}

Layout::Layout(){}

GLVector& Layout::operator ()(uint location)
{
	if(defined_layouts.count(location) == 0)
	{
		defined_layouts[location] = GLVector();
	}
	return defined_layouts[location];
}

const GLVector& Layout::operator ()(uint location)const
{
	if(defined_layouts.count(location) == 0)
	{
		throw KeyError("layout location " + str::str(location) + " is not defined");
	}
	return defined_layouts.at(location);
}

void Layout::order(const vector<uvec3>& order_mat)
{
	ebo.bufferData(order_mat);
}

void Layout::apply()
{
	if(is_applied || defined_layouts.empty())
	{
		return;
	}

	uint common_length = defined_layouts.begin()->second.size();
	uint data_size = 0;
	for(auto it = defined_layouts.begin(); it != defined_layouts.end(); it++)
	{
		if(it->second.size() != common_length)
		{
			throw RuntimeError(string("Each layout must have the same length.\n") + 
				               "But layout(location = " + str::str(defined_layouts.begin()->first) + ") is of length " + str::str(common_length) + ",\n" + 
				               "    layout(location = " + str::str(it->first) + ") is of length " + str::str(it->second.size()) + ".");
		}
		data_size += common_length * GLSL::built_in_types[it->second.dtype()].glsl_size;
	}
	vbo.malloc(data_size);
	byte* ptr = (byte*)(vbo.ptr());

	for(auto it = defined_layouts.begin(); it != defined_layouts.end(); it++)
	{
		string dtype = it->second.dtype();
		if(dtype == "bool" || dtype == "int" || dtype == "uint" || dtype == "float" || dtype == "double")
		{
			uint size = common_length*GLSL::built_in_types[dtype].glsl_size;
			memcpy((void*)ptr, it->second.ptr(0), size);
			ptr += size;
		}
		else if(dtype == "dvec2" || dtype == "dvec3" || dtype == "dvec4" ||
			    dtype == "vec2"  || dtype == "vec3" || dtype == "vec4" ||
			    dtype == "uvec2" || dtype == "uvec3" || dtype == "uvec4" ||
			    dtype == "ivec2" || dtype == "ivec3" || dtype == "ivec4" ||
			    dtype == "bvec2" || dtype == "bvec3" || dtype == "bvec4")
		{
			for(int i = 0; i < common_length; i++)
			{
				memcpy((void*)ptr, it->second.ptr(i), GLSL::built_in_types[dtype].glsl_size);
				ptr += GLSL::built_in_types[dtype].glsl_size;
			}
		}
		else if(dtype == "mat2")
		{
			for(int i = 0; i < common_length; i++)
			{
				mat2* ptr_mat2 = it->second.ptr<mat2>(i);
				memcpy((void*)ptr, (void*)(&((*ptr_mat2)[0])), 2*sizeof(float));
				ptr += 2*sizeof(float);
				memcpy((void*)ptr, (void*)(&((*ptr_mat2)[1])), 2*sizeof(float));
				ptr += 2*sizeof(float);
			}
		}
		else if(dtype == "mat3")
		{
			for(int i = 0; i < common_length; i++)
			{
				mat3* ptr_mat3 = it->second.ptr<mat3>(i);
				memcpy((void*)ptr, (void*)(&((*ptr_mat3)[0])), 3*sizeof(float));
				ptr += 3*sizeof(float);
				memcpy((void*)ptr, (void*)(&((*ptr_mat3)[1])), 3*sizeof(float));
				ptr += 3*sizeof(float);
				memcpy((void*)ptr, (void*)(&((*ptr_mat3)[2])), 3*sizeof(float));
				ptr += 3*sizeof(float);
			}
		}
		else if(dtype == "mat4")
		{
			for(int i = 0; i < common_length; i++)
			{
				mat4* ptr_mat4 = it->second.ptr<mat4>(i);
				memcpy((void*)ptr, (void*)(&((*ptr_mat4)[0])), 4*sizeof(float));
				ptr += 4*sizeof(float);
				memcpy((void*)ptr, (void*)(&((*ptr_mat4)[1])), 4*sizeof(float));
				ptr += 4*sizeof(float);
				memcpy((void*)ptr, (void*)(&((*ptr_mat4)[2])), 4*sizeof(float));
				ptr += 4*sizeof(float);
				memcpy((void*)ptr, (void*)(&((*ptr_mat4)[3])), 4*sizeof(float));
				ptr += 4*sizeof(float);
			}
		}
	}
	vbo.apply();

	uint offset = 0;
	for(auto it = defined_layouts.begin(); it != defined_layouts.end(); it++)
	{
		string dtype = it->second.dtype();
		uint location = it->first;
		uint stripe = GLSL::built_in_types[dtype].glsl_size;
		if(dtype == "bool") vao[location](1, GL_BOOL, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "uint") vao[location](1, GL_UNSIGNED_INT, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "int") vao[location](1, GL_INT, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "float") vao[location](1, GL_FLOAT, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "double") vao[location](1, GL_DOUBLE, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "bvec2") vao[location](2, GL_BOOL, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "bvec3") vao[location](3, GL_BOOL, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "bvec4") vao[location](4, GL_BOOL, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "ivec2") vao[location](2, GL_INT, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "ivec3") vao[location](3, GL_INT, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "ivec4") vao[location](4, GL_INT, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "uvec2") vao[location](2, GL_UNSIGNED_INT, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "uvec3") vao[location](3, GL_UNSIGNED_INT, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "uvec4") vao[location](4, GL_UNSIGNED_INT, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "vec2") vao[location](2, GL_FLOAT, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "vec3") vao[location](3, GL_FLOAT, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "vec4") vao[location](4, GL_FLOAT, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "dvec2") vao[location](2, GL_DOUBLE, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "dvec3") vao[location](3, GL_DOUBLE, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "dvec4") vao[location](4, GL_DOUBLE, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "mat2") vao[location](4, GL_FLOAT, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "mat3") vao[location](9, GL_FLOAT, GL_FALSE, stripe, offset).interp(vbo);
		else if(dtype == "mat4") vao[location](16, GL_FLOAT, GL_FALSE, stripe, offset).interp(vbo);
		
		offset += common_length * GLSL::built_in_types[dtype].glsl_size;
	}
	if(!ebo.empty())
	{
		vao.addEBO(ebo);
	}
	is_applied = true;
}

void Layout::draw()
{
	apply();
	vao.draw();
}

bool Layout::empty()const
{
	return defined_layouts.empty();
}

Uniform::Variable::Variable(){}
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

bool Uniform::is_atom(const string& name)
{
	return (uniform_map.count(name) && uniform_map[name].atoms.size() == 1 && uniform_map[name].atoms[0] == name);
}

Uniform::Uniform(Shader* _parent) : parent(_parent) {}

Uniform::Uniform(Shader* _parent, const Uniform& uniform) :
parent(_parent),
original_uniform_map(uniform.original_uniform_map),
uniform_map(uniform.uniform_map),
defined_structs(uniform.defined_structs)
{}

Uniform::Uniform(Shader* _parent, Uniform&& uniform) :
parent(_parent),
original_uniform_map(move(uniform.original_uniform_map)),
uniform_map(move(uniform.uniform_map)),
defined_structs(move(uniform.defined_structs))
{}

Uniform& Uniform::operator =(const Uniform& uniform)
{
	if(&uniform == this)
	{
		return *this;
	}
	original_uniform_map = uniform.original_uniform_map;
	uniform_map = uniform.uniform_map;
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
	defined_structs = move(uniform.defined_structs);
	__parent_names = move(uniform.__parent_names);

	return *this;
}

Uniform::Reference Uniform::operator [](const string& name)
{
	if(!(uniform_map.count(name)))
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
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
	if(type.find("[") != string::npos)
	{
		if(defined_structs.count(basetype) && defined_structs[basetype].host_hash == 0)
		{
			throw RuntimeError("Must call Uniform::define<"+basetype+">(\""+basetype+"\") before assigning value.");
		}
		please_define(basetype);
	}
	else if(defined_structs.count(type))
	{
		for(auto member = defined_structs[type].members.begin(); member != defined_structs[type].members.end(); member++)
		{
			if(defined_structs.count(member->type) && defined_structs[member->type].host_hash == 0)
			{
				throw RuntimeError("Must call Uniform::define<"+member->type+">(\""+member->type+"\") before assigning value.");
			}
			please_define(member->type);
		}
	}
}

int Uniform::location(const string& name)
{
	return glGetUniformLocation(parent->id(), name.c_str());
}

void Uniform::set_atom(const string& name, void* ptr_value)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}

	string type = uniform_map[name].type;
	if(type == "bool")
	{
		set_bool(name, *((bool*)ptr_value));
	}
	else if(type == "uint")
	{
		set_uint(name, *((uint*)ptr_value));
	}
	else if(type == "int")
	{
		set_int(name, *((int*)ptr_value));
	}
	else if(type == "float")
	{
		set_float(name, *((float*)ptr_value));
	}
	else if(type == "double")
	{
		set_double(name, *((double*)ptr_value));
	}
	else if(type == "bvec2")
	{
		set_bvec2(name, *((vec2*)ptr_value));
	}
	else if(type == "bvec3")
	{
		set_bvec3(name, *((vec3*)ptr_value));
	}
	else if(type == "bvec4")
	{
		set_bvec4(name, *((vec4*)ptr_value));
	}
	else if(type == "ivec2")
	{
		set_ivec2(name, *((vec2*)ptr_value));
	}
	else if(type == "ivec3")
	{
		set_ivec3(name, *((vec3*)ptr_value));
	}
	else if(type == "ivec4")
	{
		set_ivec4(name, *((vec4*)ptr_value));
	}
	else if(type == "uvec2")
	{
		set_uvec2(name, *((vec2*)ptr_value));
	}
	else if(type == "uvec3")
	{
		set_uvec3(name, *((vec3*)ptr_value));
	}
	else if(type == "uvec4")
	{
		set_uvec4(name, *((vec4*)ptr_value));
	}
	else if(type == "vec2")
	{
		set_vec2(name, *((vec2*)ptr_value));
	}
	else if(type == "vec3")
	{
		set_vec3(name, *((vec3*)ptr_value));
	}
	else if(type == "vec4")
	{
		set_vec4(name, *((vec4*)ptr_value));
	}
	else if(type == "dvec2")
	{
		set_dvec2(name, *((vec2*)ptr_value));
	}
	else if(type == "dvec3")
	{
		set_dvec3(name, *((vec3*)ptr_value));
	}
	else if(type == "dvec4")
	{
		set_dvec4(name, *((vec4*)ptr_value));
	}
	else if(type == "mat2")
	{
		set_mat2(name, *((mat2*)ptr_value));
	}
	else if(type == "mat3")
	{
		set_mat3(name, *((mat3*)ptr_value));
	}
	else if(type == "mat4")
	{
		set_mat4(name, *((mat4*)ptr_value));
	}
	else if(type == "sampler2D")
	{
		set_sampler2D(name, *((sampler2D*)ptr_value));
	}
	else if(type == "samplerCube")
	{
		set_samplerCube(name, *((samplerCube*)ptr_value));
	}
	else
	{
		throw TypeError("Uniform variable " + name + " is not an atom variable.");
	}
}

void Uniform::set_bool(const string& name, bool value)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "bool")
	{
		throw TypeError("Cannot use set_bool to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}
	parent->use();
	glUniform1i(uniform_map[name].location, (int)(value));
}

void Uniform::set_uint(const string& name, uint value)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "uint")
	{
		throw TypeError("Cannot use set_uint to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}

	parent->use();
	glUniform1ui(uniform_map[name].location, value);
}

void Uniform::set_int(const string& name, int value)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "int")
	{
		throw TypeError("Cannot use set_int to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}

	parent->use();
	glUniform1i(uniform_map[name].location, value);
}

void Uniform::set_float(const string& name, float value)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "float")
	{
		throw TypeError("Cannot use set_float to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}

	parent->use();
	glUniform1f(uniform_map[name].location, value);
}

void Uniform::set_double(const string& name, double value)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "double")
	{
		throw TypeError("Cannot use set_double to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}

	parent->use();
	glUniform1f(uniform_map[name].location, (float)(value));
}

void Uniform::set_bvec2(const string& name, const bvec2& value)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "bvec2")
	{
		throw TypeError("Cannot use set_bvec2 to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}

	parent->use();
	glUniform2i(uniform_map[name].location, (int)(value.x), (int)(value.y));
}

void Uniform::set_bvec3(const string& name, const bvec3& value)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "bvec3")
	{
		throw TypeError("Cannot use set_bvec3 to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}

	parent->use();
	glUniform3i(uniform_map[name].location, (int)(value.x), (int)(value.y), (int)(value.z));
}

void Uniform::set_bvec4(const string& name, const bvec4& value)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "bvec4")
	{
		throw TypeError("Cannot use set_bvec4 to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}

	parent->use();
	glUniform4i(uniform_map[name].location, (int)(value.x), (int)(value.y), (int)(value.z), (int)(value.w));
}

void Uniform::set_ivec2(const string& name, const ivec2& value)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "ivec2")
	{
		throw TypeError("Cannot use set_ivec2 to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}

	parent->use();
	glUniform2i(uniform_map[name].location, value.x, value.y);
}

void Uniform::set_ivec3(const string& name, const ivec3& value)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "ivec3")
	{
		throw TypeError("Cannot use set_ivec3 to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}

	parent->use();
	glUniform3i(uniform_map[name].location, value.x, value.y, value.z);
}

void Uniform::set_ivec4(const string& name, const ivec4& value)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "ivec4")
	{
		throw TypeError("Cannot use set_ivec4 to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}

	parent->use();
	glUniform4i(uniform_map[name].location, value.x, value.y, value.z, value.w);
}

void Uniform::set_uvec2(const string& name, const uvec2& value)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "uvec2")
	{
		throw TypeError("Cannot use set_uvec2 to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}

	parent->use();
	glUniform2ui(uniform_map[name].location, value.x, value.y);
}

void Uniform::set_uvec3(const string& name, const uvec3& value)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "uvec3")
	{
		throw TypeError("Cannot use set_uvec3 to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}

	parent->use();
	glUniform3ui(uniform_map[name].location, value.x, value.y, value.z);
}

void Uniform::set_uvec4(const string& name, const uvec4& value)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "uvec4")
	{
		throw TypeError("Cannot use set_uvec4 to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}

	parent->use();
	glUniform4ui(uniform_map[name].location, value.x, value.y, value.z, value.w);
}

void Uniform::set_vec2(const string& name, const vec2& value)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "vec2")
	{
		throw TypeError("Cannot use set_vec2 to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}

	parent->use();
	glUniform2f(uniform_map[name].location, value.x, value.y);
}

void Uniform::set_vec3(const string& name, const vec3& value)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "vec3")
	{
		throw TypeError("Cannot use set_vec3 to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}

	parent->use();
	glUniform3f(uniform_map[name].location, value.x, value.y, value.z);
}

void Uniform::set_vec4(const string& name, const vec4& value)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "vec4")
	{
		throw TypeError("Cannot use set_vec4 to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}

	parent->use();
	glUniform4f(uniform_map[name].location, value.x, value.y, value.z, value.w);
}

void Uniform::set_dvec2(const string& name, const dvec2& value)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "vec2")
	{
		throw TypeError("Cannot use set_dvec2 to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}

	parent->use();
	glUniform2f(uniform_map[name].location, (float)(value.x), (float)(value.y));
}

void Uniform::set_dvec3(const string& name, const dvec3& value)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "dvec3")
	{
		throw TypeError("Cannot use set_dvec3 to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}

	parent->use();
	glUniform3f(uniform_map[name].location, (float)(value.x), (float)(value.y), (float)(value.z));
}

void Uniform::set_dvec4(const string& name, const dvec4& value)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "dvec4")
	{
		throw TypeError("Cannot use set_dvec4 to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}

	parent->use();
	glUniform4f(uniform_map[name].location, (float)(value.x), (float)(value.y), (float)(value.z), (float)(value.w));
}

void Uniform::set_mat2(const string& name, mat2 value)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "mat2")
	{
		throw TypeError("Cannot use set_mat2 to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}

	parent->use();
	glUniformMatrix2fv(uniform_map[name].location, 1, GL_FALSE, value.data());
}

void Uniform::set_mat3(const string& name, mat3 value)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "mat3")
	{
		throw TypeError("Cannot use set_mat3 to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}

	parent->use();
	glUniformMatrix3fv(uniform_map[name].location, 1, GL_FALSE, value.data());
}

void Uniform::set_mat4(const string& name, mat4 value)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "mat4")
	{
		throw TypeError("Cannot use set_mat4 to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}

	parent->use();
	glUniformMatrix4fv(uniform_map[name].location, 1, GL_FALSE, value.data());
}

void Uniform::set_sampler2D(const string& name, sampler2D& value)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "sampler2D")
	{
		throw TypeError("Cannot use set_sampler2D to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}
	if(uniform_map[name].texture_unit == 0 || value.id() == 0)
	{
		return;
	}
	
	parent->use();
	if(uniform_map[name].texture_unit >= 16)
	{
		cout << "Warning: used texture unit(" << uniform_map[name].texture_unit << ") is over 15 now.";
	}
	glActiveTexture(GL_TEXTURE0 + uniform_map[name].texture_unit);
	value.bind();
	glUniform1i(uniform_map[name].location, uniform_map[name].texture_unit);
	glActiveTexture(GL_TEXTURE0);
}

void Uniform::set_sampler2D(const string& name, uint texture_id)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "sampler2D")
	{
		throw TypeError("Cannot use set_sampler2D to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}
	if(uniform_map[name].texture_unit == 0 || texture_id == 0)
	{
		return;
	}

	parent->use();
	if(uniform_map[name].texture_unit >= 16)
	{
		cout << "Warning: used texture unit(" << uniform_map[name].texture_unit << ") is over 15 now.";
	}
	glActiveTexture(GL_TEXTURE0 + uniform_map[name].texture_unit);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glUniform1i(uniform_map[name].location, 0);
	glActiveTexture(GL_TEXTURE0);
}

void Uniform::set_samplerCube(const string& name, samplerCube& value)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "samplerCube")
	{
		throw TypeError("Cannot use set_samplerCube to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}
	if(uniform_map[name].texture_unit == 0 || value.id() == 0)
	{
		return;
	}
	
	parent->use();
	if(uniform_map[name].texture_unit >= 16)
	{
		cout << "Warning: used texture unit(" << uniform_map[name].texture_unit << ") is over 15 now.";
	}
	glActiveTexture(GL_TEXTURE0 + uniform_map[name].texture_unit);
	value.bind();
	glUniform1i(uniform_map[name].location, uniform_map[name].texture_unit);
	glActiveTexture(GL_TEXTURE0);
}

void Uniform::set_samplerCube(const string& name, uint texture_id)
{
	if(uniform_map.count(name) == 0)
	{
		throw KeyError("Uniform variable " + name + " is not defined in GLSL code!");
	}
	if(uniform_map[name].type != "samplerCube")
	{
		throw TypeError("Cannot use set_samplerCube to set " + uniform_map[name].type + " uniform variable " + name + "\'s value.");
	}
	if(uniform_map[name].texture_unit == 0 || texture_id == 0)
	{
		return;
	}
	
	parent->use();
	if(uniform_map[name].texture_unit >= 16)
	{
		cout << "Warning: used texture unit(" << uniform_map[name].texture_unit << ") is over 15 now.";
	}
	glActiveTexture(GL_TEXTURE0 + uniform_map[name].texture_unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
	glUniform1i(uniform_map[name].location, uniform_map[name].texture_unit);
	glActiveTexture(GL_TEXTURE0);
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

multiset<uint> Shader::existing_Shaders;
uint Shader::active_Shader = 0;

void Shader::init()
{
	if(_id == 0)
	{
		_id = glCreateProgram();
		if(_id == 0)
		{
			throw Exception("Failed to create Shader!");
		}
	#ifdef _DEBUG
		cout << "constructing Shader " << _id << endl;
	#endif
		existing_Shaders.insert(_id);
	}
	else if(existing_Shaders.count(_id) == 0)
	{
		throw RuntimeError("Shader " + str::str(_id) + " has already been destructed.");
	}
}

void Shader::find_layouts(const string& code, const std::set<uint>& comments_set)
{
	// layout (location = 2) in vec2 texture_coord;
	int pos = 0;
	while(true)
	{
		pos = code.find("layout", pos);
		if(pos == string::npos)
		{
			return;
		}
		if(comments_set.count(pos) == 1 || 
		   !(pos == 0 || (pos-1 >= 0          && (code[pos-1] == '\n' || code[pos-1] == '\t' || code[pos-1] == ' '))) || 
		   !(             pos+6 < code.size() && (code[pos+6] == '\n' || code[pos+6] == '\t' || code[pos+6] == ' ' || code[pos+6] == '(')))
		{
			pos += 6;
			continue;
		}
		pos = code.find("(", pos) + 1;
		string word = str::get_word(code, pos);
		if(word != "location")
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

Shader::Shader(const string& vertex_code, const string& fragment_code) :
uniform(this)
{
	vertex_shader.compile(vertex_code);
	fragment_shader.compile(fragment_code);
	link();
}

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

Shader::~Shader()
{
	unuse();
	uint count = existing_Shaders.count(_id);
	if(count > 0)
	{
		multiset_pop(existing_Shaders, _id);
		if(count == 1)
		{
		#ifdef _DEBUG
			cout << "destructing Shader " << _id << endl;
		#endif
			glDeleteProgram(_id);
		}
	}
	_id = 0;
}

uint Shader::id()const
{
	return _id;
}

void Shader::compile_vertex_code(const string& vertex_code)
{
	vertex_shader.compile(vertex_code);
}

void Shader::compile_fragment_code(const string& fragment_code)
{
	fragment_shader.compile(fragment_code);
}

void Shader::link()
{
	if(vertex_shader.empty())
	{
		throw RuntimeError("Please compile vertex shader before link!");
	}
	if(fragment_shader.empty())
	{
		throw RuntimeError("Please compile fragment shader before link!");
	}

	init();
	glAttachShader(_id, vertex_shader.id());
	glAttachShader(_id, fragment_shader.id());
	glLinkProgram(_id);

	int  success;
	char infoLog[8192];
	glGetProgramiv(_id, GL_LINK_STATUS, &success);
	if(!success)
	{
	    glGetProgramInfoLog(_id, 8192, NULL, infoLog);
	    throw Exception(infoLog);
	}

	find_layouts(vertex_shader.code(), vertex_shader.comments_set);

	uniform.find_structs(vertex_shader.code(), vertex_shader.comments_set);
	uniform.find_structs(fragment_shader.code(), fragment_shader.comments_set);

	uniform.find_uniforms(vertex_shader.code(), vertex_shader.comments_set);
	uniform.find_uniforms(fragment_shader.code(), fragment_shader.comments_set);

	uniform.refresh();

	linked = true;
}

void Uniform::find_uniforms(const string& code, const std::set<uint>& comments_set)
{
	int pos = 0;
	while(true)
	{
		pos = code.find("uniform", pos);
		if(pos == string::npos)
		{
			return;
		}
		if(comments_set.count(pos) == 1 ||
		   !(pos == 0 || (pos-1 >= 0          && (code[pos-1] == ' ' || code[pos-1] == '\n' || code[pos-1] == '\t'))) ||
		   !(             pos+7 < code.size() && (code[pos+7] == ' ' || code[pos+7] == '\n' || code[pos+7] == '\t')))
		{
			pos += 7;
			continue;
		}

		int pos_start = pos + 8;
		int pos_end = code.find(";", pos_start);
		string type = str::get_word(code, pos_start);
		vector<string> names = str::split(code.substr(pos_start, pos_end-pos_start), ",");
		for(auto it = names.begin(); it != names.end(); it++)
		{
			string name = str::format_var_name(*it);
			int pos_left_brace = name.find("[");
			if(pos_left_brace == string::npos)
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

void Uniform::resolve_one_uniform(const string& name)
{
	string type = uniform_map[name].type;
	uniform_map[name].align_size = align_size_of(type);
	uniform_map[name].size = size_of(type);

	int pos_left_brace = type.find("[");
	if(pos_left_brace == string::npos)
	{
		if(defined_structs.count(type) == 0)
		{
			uniform_map[name].location = glGetUniformLocation(parent->id(), name.c_str());
			if(type.find("sampler") != string::npos)
			{
				uniform_map[name].texture_unit = parent->available_texture_unit;
				parent->available_texture_unit++;
			}
			for(auto parent_name = __parent_names.begin(); parent_name != __parent_names.end(); parent_name++)
			{
				uniform_map[*parent_name].atoms.push_back(name);
			}
			return;
		}
		else
		{
			for(auto member = defined_structs[type].members.begin(); member != defined_structs[type].members.end(); member++)
			{
				string member_name = name + "." + member->name;
				uniform_map[member_name] = Uniform::Variable(member->type, member_name);
				uniform_map[member_name].size = member->size;
				uniform_map[member_name].padding_size = member->padding_size;
				__parent_names.push_back(member_name);
				resolve_one_uniform(member_name);
			}
		}
	}
	else
	{
		int pos_right_brace = type.find("]");
		int n = str::str2<int>(type.substr(pos_left_brace+1, pos_right_brace-pos_left_brace-1));
		type = type.substr(0, pos_left_brace);
		for(int i = 0; i < n; i++)
		{
			string element_name = name + "[" + str::str(i) + "]";
			uniform_map[element_name] = Uniform::Variable(type, element_name);
			uniform_map[element_name].size = size_of(type);
			uniform_map[element_name].padding_size = (i != n-1 ? 0 : uniform_map[name].padding_size);
			__parent_names.push_back(element_name);
			resolve_one_uniform(element_name);
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
			throw TypeError("Unknown uniform type: " + type);
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
			throw TypeError("Cannot convert host struct " + defined_structs[type].host_name + " to GLSL struct " + type + ":\nhost struct's align size should be larger than GLSL struct's.");
		}

		defined_structs[type].align_size = max(max_align_size, defined_structs[type].host_align_size);
		
		return defined_structs[type].align_size;
	}
	else
	{
		return align_size_of(str::base_type(type));
	}
}

uint Uniform::size_of(string type)
{
	if(uniform_map.count(type))
	{
		type = uniform_map[type].type;
	}

	if(type.find("[") == string::npos)
	{
		if(GLSL::built_in_types.count(type))
		{
			return GLSL::built_in_types[type].size;
		}
		if(!(defined_structs.count(type)))
		{
			throw TypeError("Unknown uniform type: " + type);
		}
		if(defined_structs[type].size != 0)
		{
			return defined_structs[type].size;
		}

		uint n_members = defined_structs[type].members.size();
		uint offset = 0, old_offset = 0;
		uint max_align_size = 0;
		for(int i = 0; i < n_members; i++)
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
			defined_structs[type].members[i].size = size_of(current_type);
			offset += defined_structs[type].members[i].size;
			old_offset = offset;
		}
		uint size = ceil((double)offset/(double)max_align_size) * max_align_size;
		if(defined_structs[type].host_size != 0 && size > defined_structs[type].host_size)
		{
			defined_structs[type].host_size = 0;
			throw TypeError("Cannot convert host struct " + defined_structs[type].host_name + " to GLSL struct " + type + ":\nhost struct's size should be larger than GLSL struct's.");
		}
		defined_structs[type].size = max(size, defined_structs[type].host_size);
		defined_structs[type].members[n_members-1].padding_size = defined_structs[type].size - offset;
		return defined_structs[type].size;
	}
	else
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

	host_name.clear();
	host_size = 0;
	host_align_size = 0;
	host_hash = 0;
}

void Uniform::find_structs(const string& code, const std::set<uint>& comments_set)
{
	Struct structure;
	int pos = 0;
	while(true)
	{
		pos = code.find("struct", pos);
		if(pos == string::npos)
		{
			return;
		}
		if(comments_set.count(pos) == 1 || 
		   !(pos == 0 || (pos-1 >= 0          && (code[pos-1] == '\n' || code[pos-1] == ' ' || code[pos-1] == '\t'))) ||
		   !(             pos+6 < code.size() && (code[pos+6] == '\n' || code[pos+6] == ' ' || code[pos+6] == '\t')))
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
				if(pos_left_brace == string::npos)
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
	for(auto it = defined_structs.begin(); it != defined_structs.end(); it++)
	{
		it->second.align_size = 0;
		it->second.size = 0;
	}
	analyse_structs();
	resolve_all_uniforms();
}

void Shader::use()const
{
	if(!linked)
	{
		throw Exception("Need to link Shader before using!");
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

bool Shader::is_using()const
{
	return (existing_Shaders.count(_id) > 0 && active_Shader == _id);
}

void Shader::draw(Layout& layout)
{
	for(auto it = layout_map.begin(); it != layout_map.end(); it++)
	{
		if(enable_warning && layout.defined_layouts.count(it->first) == 0)
		{
			cout << "Warning: layout(location = " + str::str(it->first) + ") in " + layout_map[it->first] + " is not passed." << endl;
		}
		if(layout(it->first).dtype() != layout_map[it->first])
		{
			throw TypeError("Error when setting layout(location = " + str::str(it->first) + ") in " + layout_map[it->first] + ":\nCannot convert " + layout(it->first).dtype() + " to " + layout_map[it->first] + ".");
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
	layout.draw();
}

void Shader::draw(Model& model)
{
	vector<string> keys = model.keys();
	if(!(model.layout.empty()))
	{
		if(uniform.contains("model") && uniform["model"].type() == "Model")
		{
			uniform["model.material"] = model.material;
		}
		draw(model.layout);
	}
	for(auto it = keys.begin(); it != keys.end(); it++)
	{
		draw(model[*it]);
	}
}

void Shader::warning_on()
{
	enable_warning = true;
}

void Shader::warning_off()
{
	enable_warning = false;
}